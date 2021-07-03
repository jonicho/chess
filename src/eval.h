#ifndef _EVAL_H_
#define _EVAL_H_

#include "position.h"
#include "move.h"

// Evaluates a position.
// A positive number is good for white, a negative number is good for black.
int eval_position(const Position *position);

// Evaluates a move.
// The higher the return value the better the move.
int eval_move(const Position *position, const Move *move);

#endif
