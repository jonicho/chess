#ifndef _MOVE_GEN_H_
#define _MOVE_GEN_H_

#include "move.h"
#include "position.h"

#include <stddef.h>

// Generates all pseudo legal moves.
// Returns the number of moves generated.
size_t gen_moves(Move *moves, const Position *position);

// Generates all legal moves.
// Returns the number of moves generated.
size_t gen_legal_moves(Move *moves, const Position *position);

#endif
