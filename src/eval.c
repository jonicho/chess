#include "eval.h"

int eval_position(const Position *position)
{
	int eval = 0;
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			uint8_t square = RF(rank, file);
			uint8_t piece = position->squares[square];
			if (PIECE_TYPE(piece) == EMPTY) {
				continue;
			}

			if (PIECE_COLOR(piece) == WHITE) {
				eval++;
			} else {
				eval--;
			}
		}
	}
	return eval;
}
