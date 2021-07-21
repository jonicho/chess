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
	const Move *best_move = table_get_best_move(position->hash);

	int evals[MAX_MOVES];
	for (size_t i = 0; i < num_moves; i++) {
		if (best_move != NULL && (moves + i)->src == best_move->src &&
		    (moves + i)->dst == best_move->dst &&
		    (moves + i)->promotion_piece ==
			    best_move->promotion_piece) {
			evals[i] = INT_MAX;
		} else {
			evals[i] = eval_move(position, moves + i);
		}
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
			      Move *best_move, size_t depth, int alpha,
			      int beta)
{
	if (search->stop_requested) {
		pthread_exit(NULL);
		fprintf(stderr, "thread did not exit\n");
		exit(1);
	}
	search->nodes++;
	if (depth == 0) {
		return eval_position(position);
	}

	Move moves[MAX_MOVES];
	size_t num_moves = gen_moves(moves, position);
	sort_moves(position, moves, num_moves);
	size_t best_move_index = -1;
	int best_move_eval = INT_MIN;
	bool no_more_moves = true;
	for (size_t i = 0; i < num_moves; i++) {
		Position temp_position = *position;
		make_move(&temp_position, moves[i]);
		if (is_king_in_check(&temp_position,
				     BLACK ^ temp_position.side_to_move)) {
			continue;
		}
		no_more_moves = false;
		Move dummy_move;
		int eval =
			-negamax_alpha_beta(search, &temp_position, &dummy_move,
					    depth - 1, -beta, -alpha);

		if (eval > best_move_eval) {
			best_move_eval = eval;
			best_move_index = i;
		}
		if (eval > alpha) {
			alpha = eval;
		}
		if (beta <= alpha) {
			break;
		}
	}
	if (no_more_moves) {
		if (is_king_in_check(position, position->side_to_move)) {
			return -CHECKMATE_EVAL - depth;
		} else {
			return eval_position(position);
		}
	}
	*best_move = moves[best_move_index];
	table_put(position->hash, *best_move, depth);
	return best_move_eval;
}

static void *search_thread(void *arg)
{
	Search *search = (Search *)arg;

	size_t depth = 1;
	while (true) {
		if (search->stop_requested) {
			pthread_exit(NULL);
			fprintf(stderr, "thread did not exit\n");
			exit(-1);
		}
		Move best_move;
		int best_move_eval =
			negamax_alpha_beta(search, search->position, &best_move,
					   depth, -INT_MAX, INT_MAX);

		search->best_move = best_move;
		search->best_move_eval = best_move_eval;
		search->depth = depth;
		depth++;
	}
	return NULL;
}

void search_init(Search *search, const Position *position)
{
	*search = (Search){ 0 };
	search->position = position;
}

void search_start(Search *search)
{
	if (search->running || search->stop_requested) {
		return;
	}
	table_clear();
	search->running = true;
	int err = pthread_create(&search->thread, NULL, search_thread, search);
	if (err != 0) {
		fprintf(stderr, "error: could not create search thread: %s\n",
			strerror(err));
		exit(-1);
	}
}

void search_stop(Search *search)
{
	if (!search->running) {
		return;
	}
	search->stop_requested = true;
	int err = pthread_join(search->thread, NULL);
	if (err != 0) {
		fprintf(stderr, "error: could not join search thread: %s\n",
			strerror(err));
		exit(-1);
	}
	search->running = false;
}
