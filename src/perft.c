#include "perft.h"
#include "move.h"
#include "move_gen.h"

#include <stdio.h>

size_t perft(const Position *position, size_t depth, bool print)
{
	if (depth == 0) {
		if (print) {
			printf("Nodes searched: 1\n");
		}
		return 1;
	}

	size_t nodes = 0;

	Move moves[MAX_NUM_PSEUDO_LEGAL_MOVES];
	size_t num_moves = gen_moves(moves, position);

	Position temp_position;

	for (size_t i = 0; i < num_moves; i++) {
		temp_position = *position;
		make_move(&temp_position, moves[i]);
		if (is_king_in_check(&temp_position,
				     BLACK ^ temp_position.side_to_move)) {
			continue;
		}
		size_t new_nodes = perft(&temp_position, depth - 1, false);
		if (print) {
			printf("%s: %ld\n", move_to_string(moves[i]),
			       new_nodes);
		}
		nodes += new_nodes;
	}

	if (print) {
		printf("Nodes searched: %ld\n", nodes);
	}

	return nodes;
}
