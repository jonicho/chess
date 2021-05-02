#ifndef _FEN_H_
#define _FEN_H_

#include "board.h"

// Converts a fen string to a Board.
// Returns NULL if the string is not a valid fen string.
Board *fen_to_board(char *fen);

#endif
