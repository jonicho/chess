#include "search.h"

#include "move_gen.h"
#include "eval.h"

#include <limits.h>

static int search_for_best_move_depth(const Position *position, Move *best_move,
				      size_t depth)
{
	Move moves[MAX_NUM_PSEUDO_LEGAL_MOVES];
	size_t num_moves = gen_moves(moves, position);
	size_t best_move_index = -1;
	int best_move_eval =
		position->side_to_move == WHITE ? INT_MIN : INT_MAX;
	for (size_t i = 0; i < num_moves; i++) {
		Position temp_position = *position;
		make_move(&temp_position, moves[i]);
		if (is_king_in_check(&temp_position,
				     BLACK ^ temp_position.side_to_move)) {
			continue;
		}
		int eval;
		if (depth > 1) {
			Move dummy_move;
			eval = search_for_best_move_depth(
				&temp_position, &dummy_move, depth - 1);
		} else {
			eval = eval_position(&temp_position);
		}
		if ((position->side_to_move == WHITE &&
		     eval > best_move_eval) ||
		    (position->side_to_move == BLACK &&
		     eval < best_move_eval)) {
			best_move_eval = eval;
			best_move_index = i;
		}
	}
	*best_move = moves[best_move_index];
	return best_move_eval;
}

int search_for_best_move(const Position *position, Move *best_move)
{
	return search_for_best_move_depth(position, best_move, 5);
}
