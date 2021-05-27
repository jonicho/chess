#include "board.h"

#include <stdlib.h>
#include <stdio.h>

void board_init(Board *board)
{
	memset(board, 0, sizeof(Board));
	for (size_t i = 0; i < 20; i++) {
		board->squares[i] = OFF_BOARD;
	}
	for (size_t i = 0; i < 10; i++) {
		board->squares[(i + 2) * 10] = OFF_BOARD;
		board->squares[(i + 2) * 10 + 9] = OFF_BOARD;
	}
	for (size_t i = 0; i < 20; i++) {
		board->squares[i + 100] = OFF_BOARD;
	}
}

char *piece_code_to_string(uint8_t pieceCode)
{
	switch (pieceCode) {
	case EMPTY:
		return "-";
	case WHITE_PAWN:
		return "\u2659";
	case WHITE_KNIGHT:
		return "\u2658";
	case WHITE_BISHOP:
		return "\u2657";
	case WHITE_ROOK:
		return "\u2656";
	case WHITE_QUEEN:
		return "\u2655";
	case WHITE_KING:
		return "\u2654";
	case BLACK_PAWN:
		return "\u265F";
	case BLACK_KNIGHT:
		return "\u265E";
	case BLACK_BISHOP:
		return "\u265D";
	case BLACK_ROOK:
		return "\u265C";
	case BLACK_QUEEN:
		return "\u265B";
	case BLACK_KING:
		return "\u265A";
	case OFF_BOARD:
		return "O";

	default:
		fprintf(stderr, "unknown piece-code: %d\n", pieceCode);
		exit(-1);
	}
}

void board_print(Board *board)
{
	putchar(' ');
	for (int file = 0; file < 8; file++) {
		putchar(' ');
		putchar('A' + file);
	}
	putchar('\n');

	for (int rank = 7; rank >= 0; rank--) {
		putchar('1' + rank);
		for (int file = 0; file < 8; file++) {
			putchar(' ');
			printf(piece_code_to_string(
				board->squares[RF(rank, file)]));
		}
		putchar('\n');
	}
}

void board_print_debug(Board *board)
{
	putchar(' ');
	putchar(' ');
	putchar(' ');
	for (int file = 0; file < 8; file++) {
		putchar(' ');
		putchar('A' + file);
	}
	putchar('\n');

	for (int rank = 11; rank >= 0; rank--) {
		if (rank >= 2 && rank <= 9) {
			putchar('1' + rank - 2);
		} else {
			putchar(' ');
		}

		for (int file = 0; file < 10; file++) {
			putchar(' ');
			printf(piece_code_to_string(
				board->squares[rank * 10 + file]));
		}
		putchar('\n');
	}
}
