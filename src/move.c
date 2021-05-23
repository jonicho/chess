#include "move.h"

#include <stdlib.h>

void make_move(Board *board, Move *move)
{
	uint8_t moving_piece = board->squares[move->src];
	uint8_t captured_piece = board->squares[move->dst];

	// make move
	board->squares[move->dst] = moving_piece;
	board->squares[move->src] = EMPTY;

	// flip side to move
	board->side_to_move ^= BLACK;

	// en passant target square
	if (PIECE_TYPE(moving_piece) == PAWN &&
	    abs(move->dst - move->src) == 16) {
		// if a pawn just made a two-square move
		board->en_passant_target_square =
			(move->dst + move->src) / 2; // square "behind" pawn
	}

	// halfmove clock
	if (PIECE_TYPE(captured_piece) != EMPTY ||
	    PIECE_TYPE(moving_piece) == PAWN) {
		board->halfmove_clock = 0;
	} else {
		board->halfmove_clock++;
	}

	// fullmove counter
	if (board->side_to_move == WHITE) {
		// increment if black just made a move
		board->fullmove_counter++;
	}
}