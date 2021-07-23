#include "eval.h"

// piece values from https://www.chessprogramming.org/Simplified_Evaluation_Function
static const int piece_values[] = {
	[EMPTY | WHITE] = 0,	 [PAWN | WHITE] = 100,
	[KNIGHT | WHITE] = 320,	 [BISHOP | WHITE] = 330,
	[ROOK | WHITE] = 500,	 [QUEEN | WHITE] = 900,
	[KING | WHITE] = 20000,	 [EMPTY | BLACK] = 0,
	[PAWN | BLACK] = -100,	 [KNIGHT | BLACK] = -320,
	[BISHOP | BLACK] = -330, [ROOK | BLACK] = -500,
	[QUEEN | BLACK] = -900,	 [KING | BLACK] = -20000,
};

int eval_position(const Position *position)
{
	int eval = 0;
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			uint8_t square = RF(rank, file);
			uint8_t piece = position->squares[square];
			eval += piece_values[piece];
		}
	}
	return position->side_to_move == WHITE ? eval : -eval;
}

int eval_move(const Position *position, const Move move)
{
	return piece_values[PIECE_TYPE(position->squares[move.dst])];
}
