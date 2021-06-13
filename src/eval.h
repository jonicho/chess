#ifndef _EVAL_H_
#define _EVAL_H_

#include "position.h"

// Evaluates a position.
// A positive number is good for white, a negative number is good for black.
int eval_position(const Position *position);

#endif
