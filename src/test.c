#include "test.h"

#include "position.h"
#include "move.h"
#include "fen.h"
#include "perft.h"
#include "move_gen.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct TestData {
	char *position_fen;
	size_t depth;
	size_t expected_nodes;
} TestData;

TestData test_data_list[] = {
	// first 6 test positions from https://www.chessprogramming.org/Perft_Results

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

	// from https://gist.github.com/peterellisjones/8c46c28141c162d1d8a0f0badbc9cff9
	{ "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2", 1, 8 },
	{ "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 5 3", 1, 8 },
	{ "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2", 1, 19 },
	{ "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2",
	  1, 5 },
	{ "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2", 1,
	  44 },
	{ "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9", 1, 39 },
	{ "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4", 1, 9 },
	{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3,
	  62379 },
	{ "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
	  3, 89890 },
	{ "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6, 1134888 },
	{ "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", 6, 1015133 },
	{ "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6, 1440467 },
	{ "5k2/8/8/8/8/8/8/4K2R w K - 0 1", 6, 661072 },
	{ "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 6, 803711 },
	{ "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4, 1274206 },
	{ "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4, 1720476 },
	{ "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 6, 3821001 },
	{ "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 5, 1004658 },
	{ "4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 6, 217342 },
	{ "8/P1k5/K7/8/8/8/8/8 w - - 0 1", 6, 92683 },
	{ "K1k5/8/P7/8/8/8/8/8 w - - 0 1", 6, 2217 },
	{ "8/k1P5/8/1K6/8/8/8/8 w - - 0 1", 7, 567584 },
	{ "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4, 23527 }
};

#define NUM_TEST_DATA (sizeof(test_data_list) / sizeof(TestData))

bool test_position(const TestData *test_data)
{
	Position *position = fen_to_position(test_data->position_fen);
	if (position == NULL) {
		printf("test: Failed to parse fen string: %s\n",
		       test_data->position_fen);
		return false;
	}

	size_t actual_nodes = perft(position, test_data->depth, false);
	if (actual_nodes != test_data->expected_nodes) {
		printf("test: Expected %ld nodes, but got %ld nodes at depth %ld with position %s\n",
		       test_data->expected_nodes, actual_nodes,
		       test_data->depth, test_data->position_fen);
		return false;
	}
	return true;
}

bool test_captures_only(const char *position_fen)
{
	const Position *position = fen_to_position(position_fen);
	if (position == NULL) {
		printf("test: Failed to parse fen string: %s\n", position_fen);
		return false;
	}
	Move all_moves[MAX_MOVES];
	size_t num_all_moves = gen_legal_moves(all_moves, position, false);
	Move captures[MAX_MOVES];
	size_t num_captures = gen_legal_moves(captures, position, true);
	// check that all captures are in all_moves
	for (size_t i = 0; i < num_captures; i++) {
		bool found = false;
		for (size_t j = 0; j < num_all_moves; j++) {
			if (MOVE_EQ(all_moves[j], captures[i])) {
				found = true;
				break;
			}
		}
		if (!found) {
			printf("test: Capture %s is not in all_moves for position %s\n",
			       move_to_string(captures[i]), position_fen);
			return false;
		}
	}
	// check that all captures are in captures and all non-captures are not in captures
	for (size_t i = 0; i < num_all_moves; i++) {
		bool found = false;
		for (size_t j = 0; j < num_captures; j++) {
			if (MOVE_EQ(all_moves[i], captures[j])) {
				found = true;
				break;
			}
		}
		bool is_capture =
			PIECE_TYPE(position->squares[all_moves[i].dst]) !=
				EMPTY ||
			(PIECE_TYPE(position->squares[all_moves[i].src]) ==
				 PAWN &&
			 all_moves[i].dst ==
				 position->en_passant_target_square);
		if (found != is_capture) {
			printf("test: %s is%s in captures but is%s a capture for position %s\n",
			       move_to_string(all_moves[i]),
			       found ? "" : " not", is_capture ? "" : " not",
			       position_fen);
			return false;
		}
	}
	return true;
}

int test()
{
	size_t num_tests = 0;
	size_t num_passed = 0;

	for (size_t i = 0; i < NUM_TEST_DATA; i++) {
		num_tests++;
		if (test_position(test_data_list + i)) {
			num_passed++;
		}

		num_tests++;
		if (test_captures_only(test_data_list[i].position_fen)) {
			num_passed++;
		}
	}

	printf("test: %ld out of %ld tests passed (%.2f%%)\n", num_passed,
	       num_tests, (double)num_passed / (double)num_tests * 100);

	if (num_passed != num_tests) {
		printf("test: There were test failures. See above for details.\n");
		return -1;
	}
	return 0;
}
