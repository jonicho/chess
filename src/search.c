#include "search.h"

#include "move_gen.h"
#include "eval.h"
#include "table.h"

#include <limits.h>

#define SEARCH_DEPTH 9

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
			      size_t depth, int alpha, int beta)
{
	if (depth == 0) {
		return eval_position(position);
	}

	Move moves[MAX_MOVES];
	size_t num_moves = gen_moves(moves, position);
	sort_moves(position, moves, num_moves);
	size_t best_move_index = -1;
	int best_move_eval = INT_MIN;
	for (size_t i = 0; i < num_moves; i++) {
		Position temp_position = *position;
		make_move(&temp_position, moves[i]);
		if (is_king_in_check(&temp_position,
				     BLACK ^ temp_position.side_to_move)) {
			continue;
		}
		Move dummy_move;
		int eval = -negamax_alpha_beta(&temp_position, &dummy_move,
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
	*best_move = moves[best_move_index];
	table_put(position->hash, *best_move);
	return best_move_eval;
}

int search_for_best_move(const Position *position, Move *best_move)
{
	table_clear();
	for (size_t i = 0; i < SEARCH_DEPTH - 1; i++) {
		negamax_alpha_beta(position, best_move, i, -INT_MAX, INT_MAX);
	}
	return negamax_alpha_beta(position, best_move, SEARCH_DEPTH, -INT_MAX,
				  INT_MAX);
}
