#include <stdbool.h>
#include <stdint.h>

#ifndef CACHE_H
#define CACHE_H

struct cache_node {
    __uint64_t value;
    int last_used;
    int frequency;
    int step_count;
};

extern struct cache_node collatz_cache[];

// The following are global variables that are used for cache functionality
extern int cache_accesses;
extern int least_recent_index;
extern int least_frequent_index;
extern int current_max;
extern int cache_hits;
// extern struct cache_node cache[75000];
extern int CACHE_SIZE;

bool cache_has(__uint64_t candidate);

int cache_value_for(__uint64_t candidate);

void cache_insert(__uint64_t candidate, int step_count, char* cache_method);

#endif