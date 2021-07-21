#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "position.h"
#include "move.h"

#include <stddef.h>
#include <pthread.h>

#define CHECKMATE_EVAL 1000000000

typedef struct Search {
	const Position *position;
	pthread_t thread;
	bool running;
	bool stop_requested;
	size_t nodes;
	Move best_move;
	int best_move_eval;
	size_t depth;
} Search;

void search_init(Search *search, const Position *position);

void search_start(Search *search);

void search_stop(Search *search);

#endif
