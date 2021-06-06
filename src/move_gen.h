#ifndef _MOVE_GEN_H_
#define _MOVE_GEN_H_

#include "move.h"
#include "board.h"

#include <stddef.h>

// Generates all pseudo legal moves.
// Returns the number of moves generated.
size_t gen_moves(Move *moves, const Board *board);

#endif
