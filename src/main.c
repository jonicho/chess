#include "board.h"
#include "fen.h"
#include "perft.h"
#include "test.h"

#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	if (argc > 2 && (strcmp(argv[1], "perft") == 0)) {
		perft(fen_to_board(argc > 3 ? argv[3] : FEN_STARTING_POSITION),
		      atol(argv[2]), true);
		exit(0);
	} else if (argc > 1 && (strcmp(argv[1], "test") == 0)) {
		int test_result = test();
		exit(test_result);
	}
	exit(-1);
}
