#ifndef _TABLE_H_
#define _TABLE_H_

#include "move.h"

#include <stdint.h>
#include <stddef.h>

#define TABLE_NUM_BUCKETS (1 << 22)

typedef struct Entry {
	uint64_t hash;
	Move best_move;
	uint8_t depth;
	struct Entry *next;
} Entry;

typedef struct Table {
	Entry buckets[TABLE_NUM_BUCKETS];
	size_t num_entries;
} Table;

void table_init();
void table_clear();
void table_put(uint64_t hash, Move best_move, uint8_t depth);
Move *table_get_best_move(uint64_t hash);

void table_print_debug();

#endif
