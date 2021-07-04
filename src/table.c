#include "table.h"

#include <stdlib.h>
#include <stdio.h>

static Table *table;

void table_init()
{
	table = malloc(sizeof(Table));
	for (size_t i = 0; i < TABLE_NUM_BUCKETS; i++) {
		table->buckets[i].hash = 0;
		table->buckets[i].next = NULL;
	}
	table->num_entries = 0;
}

static void free_list(Entry *entry)
{
	if (entry == NULL) {
		return;
	}
	free_list(entry->next);
	free(entry);
}

void table_clear()
{
	for (size_t i = 0; i < TABLE_NUM_BUCKETS; i++) {
		free_list(table->buckets[i].next);
		table->buckets[i].hash = 0;
		table->buckets[i].next = NULL;
	}
	table->num_entries = 0;
}

void table_put(uint64_t hash, Move best_move)
{
	if (table->buckets[hash % TABLE_NUM_BUCKETS].hash == hash) {
		table->buckets[hash % TABLE_NUM_BUCKETS].best_move = best_move;
		return;
	}
	if (table->buckets[hash % TABLE_NUM_BUCKETS].hash == 0) {
		table->num_entries++;
		table->buckets[hash % TABLE_NUM_BUCKETS].hash = hash;
		table->buckets[hash % TABLE_NUM_BUCKETS].best_move = best_move;
		return;
	}

	Entry **entry_ptr = &table->buckets[hash % TABLE_NUM_BUCKETS].next;
	while (*entry_ptr != NULL && (*entry_ptr)->hash != hash) {
		entry_ptr = &(*entry_ptr)->next;
	}
	if ((*entry_ptr) != NULL) {
		(*entry_ptr)->best_move = best_move;
		return;
	}

	Entry *new_entry = malloc(sizeof(Entry));
	new_entry->hash = hash;
	new_entry->best_move = best_move;
	new_entry->next = NULL;
	*entry_ptr = new_entry;

	table->num_entries++;
}

Move *table_get_best_move(uint64_t hash)
{
	if (table->buckets[hash % TABLE_NUM_BUCKETS].hash == hash) {
		return &table->buckets[hash % TABLE_NUM_BUCKETS].best_move;
	}
	if (table->buckets[hash % TABLE_NUM_BUCKETS].hash == 0) {
		return NULL;
	}

	Entry *entry = table->buckets[hash % TABLE_NUM_BUCKETS].next;
	while (entry != NULL && entry->hash != hash) {
		entry = entry->next;
	}
	if (entry == NULL) {
		return NULL;
	}
	return &entry->best_move;
}

void table_print_debug()
{
	size_t used_buckets = 0;
	size_t max_bucket_size = 0;
	for (size_t i = 0; i < TABLE_NUM_BUCKETS; i++) {
		if (table->buckets[i].hash == 0) {
			continue;
		}

		used_buckets++;

		size_t bucket_size = 1;
		Entry *entry = table->buckets[i].next;
		while (entry != NULL) {
			bucket_size++;
			entry = entry->next;
		}
		if (bucket_size > max_bucket_size) {
			max_bucket_size = bucket_size;
		}
	}

	printf("table: total entries: %ld, used buckets: %ld, entries in list: %.02f%%, bucket usage: %.02f%%, overall usage: %.02f%%, max bucket size: %ld\n",
	       table->num_entries, used_buckets,
	       (float)(table->num_entries - used_buckets) / table->num_entries *
		       100,
	       (float)used_buckets / TABLE_NUM_BUCKETS * 100,
	       (float)table->num_entries / TABLE_NUM_BUCKETS * 100,
	       max_bucket_size);
}
