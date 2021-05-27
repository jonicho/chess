#include "board.h"
#include "fen.h"
#include "perft.h"

#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	if (argc > 2 && (strcmp(argv[1], "perft") == 0)) {
		perft(fen_to_board(FEN_STARTING_POSITION), atol(argv[2]), true);
		exit(0);
	}
}
