#ifndef _EVAL_H_
#define _EVAL_H_

#include "position.h"
#include "move.h"

// Evaluates a position in the point of view of the current side to move.
int eval_position(const Position *position);

// Evaluates a move.
// The higher the return value the better the move.
int eval_move(const Position *position, const Move move);

#endif
