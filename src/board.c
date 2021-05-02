#include <stdlib.h>
#include <stdio.h>
#include "board.h"

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

	default:
		fprintf(stderr, "unknown piece-code: %d\n", pieceCode);
		exit(-1);
	}
}

void board_print(Board *board)
{
	putchar(' ');
	for (size_t file = 0; file < 8; file++) {
		putchar('A' + file);
	}
	putchar('\n');

	for (size_t rank = 0; rank < 8; rank++) {
		putchar('1' + rank);
		for (size_t file = 0; file < 8; file++) {
			printf(piece_code_to_string(
				board->squares[rank * 8 + file]));
		}
		putchar('\n');
	}
}
