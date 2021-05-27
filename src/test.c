#include "test.h"

#include "board.h"
#include "fen.h"
#include "perft.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

bool test_position(char *position_fen, size_t depth, size_t expected_nodes)
{
	Board *board = fen_to_board(position_fen);
	if (board == NULL) {
		printf("test: Failed to parse fen string: %s\n", position_fen);
		return false;
	}

	size_t actual_nodes = perft(board, depth, false);
	if (actual_nodes != expected_nodes) {
		printf("test: Expected %ld nodes, but got %ld nodes at depth %ld with position %s\n",
		       expected_nodes, actual_nodes, depth, position_fen);
		return false;
	}
	return true;
}

int test()
{
	// test data from https://www.chessprogramming.org/Perft_Results

	bool all_pass = true;

	// inital position
	all_pass &= test_position(FEN_STARTING_POSITION, 0, 1);
	all_pass &= test_position(FEN_STARTING_POSITION, 1, 20);
	all_pass &= test_position(FEN_STARTING_POSITION, 2, 400);
	all_pass &= test_position(FEN_STARTING_POSITION, 3, 8902);
	all_pass &= test_position(FEN_STARTING_POSITION, 4, 197281);
	all_pass &= test_position(FEN_STARTING_POSITION, 5, 4865609);

	// position 2
	all_pass &= test_position(
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		1, 48);
	all_pass &= test_position(
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		2, 2039);
	all_pass &= test_position(
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		3, 97862);
	all_pass &= test_position(
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		4, 4085603);

	// position 3
	all_pass &= test_position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
				  1, 14);
	all_pass &= test_position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
				  2, 191);
	all_pass &= test_position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
				  3, 2812);
	all_pass &= test_position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
				  4, 43238);
	all_pass &= test_position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
				  5, 674624);
	all_pass &= test_position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
				  6, 11030083);

	// position 4
	all_pass &= test_position(
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		1, 6);
	all_pass &= test_position(
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		2, 264);
	all_pass &= test_position(
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		3, 9467);
	all_pass &= test_position(
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		4, 422333);
	all_pass &= test_position(
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
		5, 15833292);

	// position 5
	all_pass &= test_position(
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1,
		44);
	all_pass &= test_position(
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2,
		1486);
	all_pass &= test_position(
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3,
		62379);
	all_pass &= test_position(
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4,
		2103487);
	all_pass &= test_position(
		"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5,
		89941194);

	// position 6
	all_pass &= test_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		0, 1);
	all_pass &= test_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		1, 46);
	all_pass &= test_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		2, 2079);
	all_pass &= test_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		3, 89890);
	all_pass &= test_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		4, 3894594);
	all_pass &= test_position(
		"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
		5, 164075551);

	if (all_pass) {
		printf("test: All tests passed!\n");
		return 0;
	} else {
		printf("test: There were test failures. See above for details.\n");
		return -1;
	}
}
