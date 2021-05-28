#include "perft.h"
#include "move.h"
#include "move_gen.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t perft(Board *board, size_t depth, bool print)
{
	if (depth == 0) {
		if (print) {
			printf("Nodes searched: 1\n");
		}
		return 1;
	}

	size_t nodes = 0;

	Move moves[MAX_NUM_MOVES];
	size_t num_moves = gen_moves(moves, board);

	Board temp_board;

	for (size_t i = 0; i < num_moves; i++) {
		memcpy(&temp_board, board, sizeof(Board));
		make_move(&temp_board, moves + i);
		size_t new_nodes = perft(&temp_board, depth - 1, false);
		if (print) {
			printf("%s: %ld\n", move_to_string(moves + i),
			       new_nodes);
		}
		nodes += new_nodes;
	}

	if (print) {
		printf("Nodes searched: %ld\n", nodes);
	}

	return nodes;
}
