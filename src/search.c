#include "search.h"

#include "move_gen.h"
#include "eval.h"
#include "table.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static pthread_t search_thread;
static bool is_search_thread_running = false;
static bool stop = false;

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

static int negamax_alpha_beta(const Position *position, Move *best_move,
			      size_t depth, int alpha, int beta, size_t *nodes)
{
	if (stop) {
		pthread_exit(NULL);
		fprintf(stderr, "thread did not exit\n");
		exit(1);
	}
	(*nodes)++;
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
		int eval = -negamax_alpha_beta(&temp_position, &dummy_move,
					       depth - 1, -beta, -alpha, nodes);

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
	table_put(position->hash, *best_move);
	return best_move_eval;
}

typedef struct SearchInfo {
	const Position *position;
	SearchResult *result;
} SearchInfo;

static void *search(void *arg)
{
	SearchInfo *info = (SearchInfo *)arg;

	size_t depth = 1;
	while (true) {
		Move best_move;
		int best_move_eval =
			negamax_alpha_beta(info->position, &best_move, depth,
					   -INT_MAX, INT_MAX,
					   &info->result->nodes);

		info->result->best_move = best_move;
		info->result->best_move_eval = best_move_eval;
		info->result->depth = depth;
		depth++;
		if (stop) {
			pthread_exit(NULL);
			fprintf(stderr, "thread did not exit\n");
			exit(1);
		}
	}
	return NULL;
}

void start_search(const Position *position, SearchResult *result)
{
	if (is_search_thread_running) {
		return;
	}
	is_search_thread_running = true;
	stop = false;
	table_clear();
	result->nodes = 0;
	SearchInfo *info = malloc(sizeof(SearchInfo));
	*info = (SearchInfo){ .position = position, .result = result };
	int err = pthread_create(&search_thread, NULL, search, info);
	if (err != 0) {
		fprintf(stderr, "error: could not create search thread: %s\n",
			strerror(err));
		exit(-1);
	}
}

void stop_search()
{
	if (!is_search_thread_running) {
		return;
	}
	stop = true;
	int err = pthread_join(search_thread, NULL);
	if (err != 0) {
		fprintf(stderr, "error: could not join search thread: %s\n",
			strerror(err));
		exit(-1);
	}
	is_search_thread_running = false;
}

void do_search(const Position *position, unsigned int seconds,
	       SearchResult *result)
{
	start_search(position, result);
	sleep(seconds);
	stop_search();
}

bool is_searching()
{
	return is_search_thread_running;
}
