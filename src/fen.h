#ifndef _FEN_H_
#define _FEN_H_

#include "position.h"

// Converts a fen string to a Position.
// Returns NULL if the string is not a valid fen string.
Position *fen_to_position(const char *fen);

#define FEN_STARTING_POSITION                                                  \
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#endif
