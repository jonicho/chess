#include "search.h"

#include "move_gen.h"
#include "eval.h"
#include "table.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sort_moves(const Position *position, Move *moves, size_t num_moves)
{
	if (num_moves <= 1) {
		return;
	}

	int evals[MAX_MOVES];
	for (size_t i = 0; i < num_moves; i++) {
		evals[i] = eval_move(position, moves[i]);
	}

	// insertion sort
	for (size_t i = 1; i < num_moves - 1; i++) {
		size_t j = i;
		Move tmp_move = moves[i];
		int tmp_eval = evals[i];
		while (j > 0 && evals[j - 1] < tmp_eval) {
			evals[j] = evals[j - 1];
			moves[j] = moves[j - 1];
			j--;
		}
		evals[j] = tmp_eval;
		moves[j] = tmp_move;
	}
}

static int negamax_alpha_beta(Search *search, const Position *position,
			      int8_t depth, int alpha, int beta)
{
	if (search->stop_requested) {
		pthread_exit(NULL);
		fprintf(stderr, "thread did not exit\n");
		exit(1);
	}
	search->nodes++;
	if (position->halfmove_clock >= 100) {
		Move tmp[MAX_MOVES];
		if (gen_legal_moves(tmp, position, false) == 0 &&
		    is_king_in_check(position, position->side_to_move)) {
			return -CHECKMATE_EVAL - depth;
		} else {
			return 0;
		}
	}
	if (depth == 0) {
		return position->eval;
	}

	Move best_move = NO_MOVE;
	int best_move_eval = INT_MIN;
	bool no_more_moves = true;

	const Move table_best_move = table_get_best_move(position->hash);

	if (!IS_NO_MOVE(table_best_move)) {
		Position temp_position = *position;
		make_move(&temp_position, table_best_move);
		no_more_moves = false;
		int eval = -negamax_alpha_beta(search, &temp_position,
					       depth - 1, -beta, -alpha);

		if (eval > best_move_eval) {
			best_move_eval = eval;
			best_move = table_best_move;
		}
		if (eval > alpha) {
			alpha = eval;
		}
		if (beta <= alpha) {
			table_put(position->hash, table_best_move, depth);
			return best_move_eval;
		}
	}

	Move moves[MAX_MOVES];
	size_t num_moves = gen_moves(moves, position, false);
	sort_moves(position, moves, num_moves);
	for (size_t i = 0; i < num_moves; i++) {
		if (MOVE_EQ(moves[i], table_best_move)) {
			continue;
		}
		Position temp_position = *position;
		make_move(&temp_position, moves[i]);
		if (is_king_in_check(&temp_position,
				     BLACK ^ temp_position.side_to_move)) {
			continue;
		}
		no_more_moves = false;
		int eval = -negamax_alpha_beta(search, &temp_position,
					       depth - 1, -beta, -alpha);

		if (eval > best_move_eval) {
			best_move_eval = eval;
			best_move = moves[i];
		}
		if (eval > alpha) {
			alpha = eval;
		}
		if (beta <= alpha) {
			table_put(position->hash, best_move, depth);
			return best_move_eval;
		}
	}
	if (no_more_moves) {
		if (is_king_in_check(position, position->side_to_move)) {
			return -CHECKMATE_EVAL - depth;
		} else {
			return 0;
		}
	}
	table_put(position->hash, best_move, depth);
	return best_move_eval;
}

static void *search_thread(void *arg)
{
	Search *search = (Search *)arg;

	int8_t depth = 1;
	while (true) {
		if (search->stop_requested) {
			pthread_exit(NULL);
			fprintf(stderr, "thread did not exit\n");
			exit(-1);
		}
		int best_move_eval = negamax_alpha_beta(
			search, search->position, depth, -INT_MAX, INT_MAX);

		search->best_move = table_get_best_move(search->position->hash);
		search->best_move_eval = best_move_eval;
		search->depth = depth;
		search->callback(search);
		depth++;
	}
	return NULL;
}

void search_init(Search *search, const Position *position, Callback callback)
{
	*search = (Search){ 0 };
	search->position = position;
	search->callback = callback;
}

void search_start(Search *search)
{
	if (search_is_running(search) || search->stop_requested) {
		return;
	}
	table_clear();
	gettimeofday(&search->start_time, NULL);
	int err = pthread_create(&search->thread, NULL, search_thread, search);
	if (err != 0) {
		fprintf(stderr, "error: could not create search thread: %s\n",
			strerror(err));
		exit(-1);
	}
}

void search_stop(Search *search)
{
	if (!search_is_running(search)) {
		return;
	}
	search->stop_requested = true;
	int err = pthread_join(search->thread, NULL);
	if (err != 0) {
		fprintf(stderr, "error: could not join search thread: %s\n",
			strerror(err));
		exit(-1);
	}
	gettimeofday(&search->stop_time, NULL);
}

bool search_is_running(const Search *search)
{
	return search->start_time.tv_sec != 0 && search->stop_time.tv_sec == 0;
}

void search_eval_to_string(const Search *search, char *buffer,
			   size_t buffer_size)
{
	if (search->best_move_eval >= CHECKMATE_EVAL) {
		int moves_until_win = (-search->best_move_eval +
				       CHECKMATE_EVAL + (int)search->depth) /
					      2 +
				      1;
		snprintf(buffer, buffer_size, "%d (winning in %d move%s)",
			 search->best_move_eval, moves_until_win,
			 moves_until_win == 1 ? "" : "s");
	} else if (search->best_move_eval <= -CHECKMATE_EVAL) {
		int moves_until_lose = (search->best_move_eval +
					CHECKMATE_EVAL + (int)search->depth) /
				       2;
		snprintf(buffer, buffer_size, "%d (losing in %d move%s)",
			 search->best_move_eval, moves_until_lose,
			 moves_until_lose == 1 ? "" : "s");
	} else {
		snprintf(buffer, buffer_size, "%d", search->best_move_eval);
	}
}
