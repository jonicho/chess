#include "test.h"

#include "board.h"
#include "fen.h"
#include "perft.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct TestData {
	char *position_fen;
	size_t depth;
	size_t expected_nodes;
} TestData;

// test data from https://www.chessprogramming.org/Perft_Results
TestData test_data_list[] = {
	// inital position
	{ FEN_STARTING_POSITION, 0, 1 },
	{ FEN_STARTING_POSITION, 1, 20 },
	{ FEN_STARTING_POSITION, 2, 400 },
	{ FEN_STARTING_POSITION, 3, 8902 },
	{ FEN_STARTING_POSITION, 4, 197281 },
	{ FEN_STARTING_POSITION, 5, 4865609 },

	// position 2
	{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
	  1, 48 },
	{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
	  2, 2039 },
	{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
	  3, 97862 },
	{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
	  4, 4085603 },

	// position 3
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1, 14 },
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2, 191 },
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3, 2812 },
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4, 43238 },
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5, 674624 },
	{ "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6, 11030083 },

	// position 4
	{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1,
	  6 },
	{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2,
	  264 },
	{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3,
	  9467 },
	{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4,
	  422333 },
	{ "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5,
	  15833292 },

	// position 5
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1, 44 },
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2,
	  1486 },
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3,
	  62379 },
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4,
	  2103487 },
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5,
	  89941194 },

	// position 6
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  0, 1 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  1, 46 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  2, 2079 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  3, 89890 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  4, 3894594 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  5, 164075551 },
};

#define NUM_TEST_DATA (sizeof(test_data_list) / sizeof(TestData))

bool test_position(const TestData *test_data)
{
	Board *board = fen_to_board(test_data->position_fen);
	if (board == NULL) {
		printf("test: Failed to parse fen string: %s\n",
		       test_data->position_fen);
		return false;
	}

	size_t actual_nodes = perft(board, test_data->depth, false);
	if (actual_nodes != test_data->expected_nodes) {
		printf("test: Expected %ld nodes, but got %ld nodes at depth %ld with position %s\n",
		       test_data->expected_nodes, actual_nodes,
		       test_data->depth, test_data->position_fen);
		return false;
	}
	return true;
}

int test()
{
	size_t num_passed = 0;

	for (size_t i = 0; i < NUM_TEST_DATA; i++) {
		if (test_position(test_data_list + i)) {
			num_passed++;
		}
	}

	printf("test: %ld out of %ld tests passed (%.2f%%)\n", num_passed,
	       NUM_TEST_DATA, (double)num_passed / (double)NUM_TEST_DATA * 100);

	if (num_passed != NUM_TEST_DATA) {
		printf("test: There were test failures. See above for details.\n");
		return -1;
	}
	return 0;
}
