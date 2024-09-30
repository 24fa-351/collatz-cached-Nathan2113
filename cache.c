#include <cache.h>
#include <string.h>

int CACHE_SIZE = 75000;

struct cache_node collatz_cache[75000];

bool cache_has(__uint64_t candidate) {
    bool in_cache = false;
    int index = 0;

    while(!in_cache && index < current_max) {
        if(collatz_cache[index].value == candidate) {
            in_cache = true;
        }

        index++;
    }

    cache_accesses++;

    return in_cache;
}


int cache_value_for(__uint64_t candidate) {

    for(int index = 0; index < current_max; index++) {
        if(collatz_cache[index].value == candidate) {

            cache_hits++;
            collatz_cache[index].frequency++;

            return collatz_cache[index].step_count;
        }
    }

    return 0;
}


void cache_insert(__uint64_t candidate, int step_count, char* cache_method) {
    // For LRU, using cache_accesses (a global variable for how many times the cache has been checked)
    // will give us the value of the least recently used when replacing cache values

    // For LFU, we use a global variable inside the cache_node struct called "frequency" when replacing values
        for(int index = 0; index < current_max; index++) {
            if(collatz_cache[index].value == candidate) {
                break;
            }


            if(collatz_cache[index].value == 0) {
                // printf("Inserting Value...\n");
                collatz_cache[index].value = candidate;
                collatz_cache[index].frequency = 1;
                collatz_cache[index].step_count = step_count;
                collatz_cache[index].last_used = cache_accesses;

                if(current_max < CACHE_SIZE)
                    current_max++;

                return;
            }
        } 

    // This section of the cache_insert method is used once the cache has been filled, and an old cache value will be replaced
    // Checks if the cache_method used is LRU
    if(strcmp(cache_method, "LRU") == 0) {
        int least_recent_index = 0;

        for(int index = 0; index < current_max; index++) {
            if(collatz_cache[index].last_used < collatz_cache[least_recent_index].last_used) {
                least_recent_index = index;
            }
        }
        collatz_cache[least_recent_index].value = candidate;
        collatz_cache[least_recent_index].frequency = 1;
        collatz_cache[least_recent_index].step_count = step_count;
        collatz_cache[least_recent_index].last_used = cache_accesses;
    }


    // Check if the cache_method used is LFU
    if(strcmp(cache_method, "LFU") == 0) {
        int least_frequent_index = 0;

        for(int index = 0; index < current_max; index++) {
            if(collatz_cache[index].frequency < collatz_cache[least_frequent_index].frequency) {
                least_frequent_index = index;
            }
        }
        collatz_cache[least_frequent_index].value = candidate;
        collatz_cache[least_frequent_index].frequency = 1;
        collatz_cache[least_frequent_index].step_count = step_count;
        collatz_cache[least_frequent_index].last_used = cache_accesses;
    }

    return;
}