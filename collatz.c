#include <collatz.h>

int cache_wrapper(char* cache_method, __uint64_t candidate) {

    int step_count = collatz_r(candidate, cache_method);

    return step_count;
}

unsigned collatz_r(__uint64_t candidate, char* cache_method) {
    if(candidate == 1) {
        return 0;
    }

    if(strcmp(cache_method, "NONE") != 0) {
        if(cache_has(candidate)) {
            return cache_value_for(candidate);
        }
    }


    __uint64_t new_candidate;
    if(candidate % 2 == 0) {
        new_candidate = candidate / 2;
    } 
    else {
        new_candidate = 3 * candidate + 1;
    } 

    if(new_candidate == 1) return 1;

    unsigned count = 1 + collatz_r(new_candidate, cache_method);

    if(strcmp(cache_method, "NONE") != 0) {
        cache_insert(candidate, count, cache_method);
    }

    return count;
}