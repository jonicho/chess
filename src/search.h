#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "position.h"
#include "move.h"

#include <stddef.h>

typedef struct SearchResult {
	Move best_move;
	int best_move_eval;
	size_t depth;
	size_t nodes;
} SearchResult;

// Searches for the best move in a position for the specified amount of seconds.
void do_search(const Position *position, unsigned int seconds,
	       SearchResult *result);

#endif
