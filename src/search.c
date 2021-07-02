#include "search.h"

#include "move_gen.h"
#include "eval.h"

#include <limits.h>

static int minimax_alpha_beta(const Position *position, Move *best_move,
			      size_t depth, int alpha, int beta)
{
	if (depth == 0) {
		return eval_position(position);
	}

	Move moves[MAX_NUM_PSEUDO_LEGAL_MOVES];
	size_t num_moves = gen_moves(moves, position);
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
