#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "position.h"
#include "move.h"

#include <stddef.h>
#include <pthread.h>
#include <sys/time.h>

#define CHECKMATE_EVAL 1000000000

typedef struct Search Search;

typedef void (*Callback)(const Search *search);

struct Search {
	const Position *position;
	pthread_t thread;
	struct timeval start_time;
	struct timeval stop_time;
	bool stop_requested;
	size_t nodes;
	Move best_move;
	int best_move_eval;
	size_t depth;
	Callback callback;
};

void search_init(Search *search, const Position *position, Callback callback);

void search_start(Search *search);

void search_stop(Search *search);

bool search_is_running(const Search *search);

void search_eval_to_string(const Search *search, char *buffer,
			   size_t buffer_size);

#endif
