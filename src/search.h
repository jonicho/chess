#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "position.h"
#include "move.h"

#include <stddef.h>

// Searches for the best move in a position and returns its evaluation.
int search_for_best_move(Position *position, Move *best_move);

#endif
