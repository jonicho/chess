#include "position.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int8_t PAWN_CAPTURES[2] = { 9, 11 };
const int8_t KNIGHT_MOVES[8] = { 8, 19, 21, 12, -8, -19, -21, -12 };
const int8_t BISHOP_DIRECTIONS[4] = { 9, 11, -9, -11 };
const int8_t ROOK_DIRECTIONS[4] = { 10, 1, -10, -1 };
const int8_t QUEEN_DIRECTIONS[8] = { 9, 10, 11, 1, -9, -10, -11, -1 };
const int8_t KING_MOVES[8] = { 9, 10, 11, 1, -9, -10, -11, -1 };

void position_init(Position *position)
{
	memset(position, 0, sizeof(Position));
	for (size_t i = 0; i < 20; i++) {
		position->squares[i] = OFF_BOARD;
	}
	for (size_t i = 0; i < 10; i++) {
		position->squares[(i + 2) * 10] = OFF_BOARD;
		position->squares[(i + 2) * 10 + 9] = OFF_BOARD;
	}
	for (size_t i = 0; i < 20; i++) {
		position->squares[i + 100] = OFF_BOARD;
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

void position_print(const Position *position)
{
	for (int rank = 7; rank >= 0; rank--) {
		putchar('1' + rank);
		for (int file = 0; file < 8; file++) {
			putchar(' ');
			fputs(piece_code_to_string(
				      position->squares[RF(rank, file)]),
			      stdout);
		}
		putchar('\n');
	}
	putchar(' ');
	for (int file = 0; file < 8; file++) {
		putchar(' ');
		putchar('a' + file);
	}
	putchar('\n');
}

void position_print_debug(const Position *position)
{
	for (int rank = 11; rank >= 0; rank--) {
		if (rank >= 2 && rank <= 9) {
			putchar('1' + rank - 2);
		} else {
			putchar(' ');
		}

		for (int file = 0; file < 10; file++) {
			putchar(' ');
			fputs(piece_code_to_string(
				      position->squares[rank * 10 + file]),
			      stdout);
		}
		putchar('\n');
	}
	putchar(' ');
	putchar(' ');
	putchar(' ');
	for (int file = 0; file < 8; file++) {
		putchar(' ');
		putchar('a' + file);
	}
	putchar('\n');
}

bool is_square_threatened(const Position *position, uint8_t threatened_side,
			  uint8_t threatened_square)
{
	// pawn
	{
		int8_t dir = threatened_side == WHITE ? 1 : -1;
		for (size_t i = 0; i < sizeof(PAWN_CAPTURES); i++) {
			uint8_t square =
				threatened_square + (dir * PAWN_CAPTURES[i]);
			uint8_t piece = position->squares[square];
			if (piece == OFF_BOARD) {
				continue;
			}
			if (piece == (PAWN | (BLACK ^ threatened_side))) {
				return true;
			}
		}
	}

	// knight
	for (size_t i = 0; i < sizeof(KNIGHT_MOVES); i++) {
		uint8_t square = threatened_square + KNIGHT_MOVES[i];
		uint8_t piece = position->squares[square];
		if (piece == OFF_BOARD) {
			continue;
		}
		if (piece == (KNIGHT | (BLACK ^ threatened_side))) {
			return true;
		}
	}

	// bishop / diagonal queen
	for (size_t i = 0; i < sizeof(BISHOP_DIRECTIONS); i++) {
		uint8_t square = threatened_square;
		while (1) {
			square += BISHOP_DIRECTIONS[i];
			uint8_t piece = position->squares[square];
			if (piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(piece) != EMPTY) {
				if ((PIECE_TYPE(piece) == BISHOP ||
				     PIECE_TYPE(piece) == QUEEN) &&
				    PIECE_COLOR(piece) != threatened_side) {
					return true;
				}
				break;
			}
		}
	}

	// rook / horizontal and vertical queen
	for (size_t i = 0; i < sizeof(ROOK_DIRECTIONS); i++) {
		uint8_t square = threatened_square;
		while (1) {
			square += ROOK_DIRECTIONS[i];
			uint8_t piece = position->squares[square];
			if (piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(piece) != EMPTY) {
				if ((PIECE_TYPE(piece) == ROOK ||
				     PIECE_TYPE(piece) == QUEEN) &&
				    PIECE_COLOR(piece) != threatened_side) {
					return true;
				}
				break;
			}
		}
	}

	// king
	for (size_t i = 0; i < sizeof(KING_MOVES); i++) {
		uint8_t square = threatened_square + KING_MOVES[i];
		uint8_t piece = position->squares[square];
		if (piece != OFF_BOARD && PIECE_TYPE(piece) == KING &&
		    PIECE_COLOR(piece) != threatened_side) {
			return true;
		}
	}
	return false;
}

bool is_king_in_check(const Position *position, uint8_t side)
{
	uint8_t king_square = 255;
	for (int rank = 7; rank >= 0 && king_square == 255; rank--) {
		for (int file = 0; file < 8 && king_square == 255; file++) {
			uint8_t square = RF(rank, file);
			if (position->squares[square] == (KING | side)) {
				king_square = square;
			}
		}
	}
	if (king_square == 255) {
		// if the king was not found it is not in check
		return false;
	}

	return is_square_threatened(position, side, king_square);
}
