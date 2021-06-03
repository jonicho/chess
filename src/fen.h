#ifndef _FEN_H_
#define _FEN_H_

#include "board.h"

// Converts a fen string to a Board.
// Returns NULL if the string is not a valid fen string.
Board *fen_to_board(const char *fen);

#define FEN_STARTING_POSITION                                                  \
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#endif
