#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "position.h"
#include "move.h"

#include <stddef.h>
#include <pthread.h>

#define CHECKMATE_EVAL 1000000000

typedef struct SearchResult {
	Move best_move;
	int best_move_eval;
	size_t depth;
	size_t nodes;
} SearchResult;

// Starts a search for the best move.
pthread_t start_search(const Position *position, SearchResult *result);

// Stops a running search.
void stop_search(pthread_t thread);

// Searches for the best move in a position for the specified amount of seconds.
void do_search(const Position *position, unsigned int seconds,
	       SearchResult *result);

#endif
