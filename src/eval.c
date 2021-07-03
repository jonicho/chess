#include "eval.h"

// piece values from https://www.chessprogramming.org/Simplified_Evaluation_Function
static const int piece_values[] = {
	[EMPTY] = 0,  [PAWN] = 100,  [KNIGHT] = 320, [BISHOP] = 330,
	[ROOK] = 500, [QUEEN] = 900, [KING] = 20000,
};

int eval_position(const Position *position)
{
	int eval = 0;
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			uint8_t square = RF(rank, file);
			uint8_t piece = position->squares[square];

			if (PIECE_COLOR(piece) == WHITE) {
				eval += piece_values[PIECE_TYPE(piece)];
			} else {
				eval -= piece_values[PIECE_TYPE(piece)];
			}
		}
	}
	return eval;
}

int eval_move(const Position *position, const Move *move)
{
	return piece_values[PIECE_TYPE(position->squares[move->dst])];
}
