#include "search.h"

#include "move_gen.h"
#include "eval.h"

#include <limits.h>

static void sort_moves(const Position *position, Move *moves, size_t num_moves)
{
	if (num_moves <= 1) {
		return;
	}

	int evals[MAX_MOVES];
	for (size_t i = 0; i < num_moves; i++) {
		evals[i] = eval_move(position, moves + i);
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

static int minimax_alpha_beta(const Position *position, Move *best_move,
			      size_t depth, int alpha, int beta)
{
	if (depth == 0) {
		return eval_position(position);
	}

	Move moves[MAX_MOVES];
	size_t num_moves = gen_moves(moves, position);
	sort_moves(position, moves, num_moves);
	size_t best_move_index = -1;
	int best_move_eval;
	if (position->side_to_move == WHITE) {
		best_move_eval = INT_MIN;
		for (size_t i = 0; i < num_moves; i++) {
			Position temp_position = *position;
			make_move(&temp_position, moves[i]);
			if (is_king_in_check(
				    &temp_position,
				    BLACK ^ temp_position.side_to_move)) {
				continue;
			}
			Move dummy_move;
			int eval = minimax_alpha_beta(&temp_position,
						      &dummy_move, depth - 1,
						      best_move_eval, beta);

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
	} else {
		best_move_eval = INT_MAX;
		for (size_t i = 0; i < num_moves; i++) {
			Position temp_position = *position;
			make_move(&temp_position, moves[i]);
			if (is_king_in_check(
				    &temp_position,
				    BLACK ^ temp_position.side_to_move)) {
				continue;
			}
			Move dummy_move;
			int eval = minimax_alpha_beta(&temp_position,
						      &dummy_move, depth - 1,
						      alpha, best_move_eval);

			if (eval < best_move_eval) {
				best_move_eval = eval;
				best_move_index = i;
			}
			if (eval < beta) {
				beta = eval;
			}
			if (beta <= alpha) {
				break;
			}
		}
	}
	*best_move = moves[best_move_index];
	return best_move_eval;
}

int search_for_best_move(const Position *position, Move *best_move)
{
	return minimax_alpha_beta(position, best_move, 8, INT_MIN, INT_MAX);
}
