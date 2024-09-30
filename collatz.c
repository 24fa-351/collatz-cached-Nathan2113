#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

unsigned collatz_r(__uint64_t candidate, char* cache_method);

int cache_wrapper(char* cache_method, __uint64_t candidate);

bool cache_has(__uint64_t candidate);

int cache_value_for(__uint64_t candidate);

void cache_insert(__uint64_t candidate, int step_count, char* cache_method);



// // LRU cache - when memory is full, the least recently used (oldest) item is replaced in the cache
// struct lru_node {
//     __uint64_t value;
//     int last_used;
//     int step_count;
// };

// // LFU cache - when memory is full, the least frequently used item is replaced in the cache
// struct lfu_node {
//     __uint64_t value;
//     int frequency;
//     int step_count;
// };

// TESTING if i can put both cache type functionalities into the same struct
struct cache_node {
    __uint64_t value;
    int last_used;
    int frequency;
    int step_count;
};

// Since most functionality for the cache is the same for both LRU and LFU (besides insert), we can make it global in order
// to pass into functions more easily
struct cache_node cache[75000];

// Global variable to keep track of how many times a cache value is used
int cache_hits = 0;

// Global variable to keep track of how many times the cache is accessesd
int cache_accesses = 0;

// CHANGE THIS
// Global constant used for cache size (cache size is allocated on line 45)
int CACHE_SIZE = 75000;

// Global variable that holds the current index in the cache so the program doesn't have to iterate through the entire cache every time
int current_max = 1;


int main(int argc, char* argv[]) {
    if(argc < 5) {
        printf("[-] Must pass the number of values to test (N), the smallest value to test (MIN), the largest value to test (MAX) and the caching method (LRU, LFU)\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100 LRU\n");
        return 0;
    } // END if(argc < 3)

    if(atoi(argv[1]) < 1 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1) {
        printf("[-] Must pass a value greater than 0 for all integer inputs\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100 LRU\n");
        return 0;
    } // END if(atoi(argv[1]) < 1 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1)

    // Initializing the string for what cache method to be used, then checking if it's valid
    char* cache_method = argv[4];

    // Converting command line argument for cache method to all uppercase
    for(int i = 0; i < sizeof(cache_method); i++) {
        cache_method[i] = toupper(cache_method[i]);
    } // END for(int i = 0; i < sizeof(cache_method); i++)

    if(strcmp(cache_method, "LRU") != 0 && strcmp(cache_method, "LFU") != 0 && strcmp(cache_method, "NONE") != 0) {
        printf("[-] Invalid cache method...Use \"NONE\", \"LRU\", or \"LFU\"\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100 LRU\n");

        return 0;
    }

    // //Initializing cache (either LRU or LFU) depending on command line arguments
    // if(strcmp(cache_method, "LRU")) {
    //     struct lru_node cache[10000];
    // }
    // else {
    //     struct lfu_node cache[10000];
    // }


    srand(time(NULL));

    FILE* fptr;
    fptr = fopen("results.csv", "w");

    int step_count = 0;
    int accesses = 0; // Counts how many times that cache is checked
    __uint64_t min = atoi(argv[2]);
    __uint64_t max = atoi(argv[3]);

    // for(int i = 0; i < 1000; i++) {
    //     cache[i].value = 3;
    //     cache[i].step_count = 7;
    // }

    fprintf(fptr, "Collatz Candidate, Step Count\n");


    for(int i = 0; i < atoi(argv[1]); i++) {
        __uint64_t collatz_candidate = rand() % (max - min + 1) + min;

        // step_count = collatz_r(collatz_candidate);
        step_count = cache_wrapper(cache_method, collatz_candidate);

        // printf("The step count of %lu is %u\n\n", collatz_candidate, step_count);

        // printf("Step count for %lu: %u\n", collatz_candidate, step_count);
        fprintf(fptr, "%lu,%u\n", collatz_candidate, step_count);
    }

    if(strcmp(cache_method, "NONE") != 0) {
        printf("The total amount of times the cache was accessed is %u\n", cache_accesses);
        printf("The total amount of times a cache value was used is %u\n", cache_hits);
        printf("The total amount of misses is %u\n", cache_accesses - cache_hits);
        printf("The percentage of cache hits is %.6f\n", ((float)cache_hits / (float)cache_accesses));
    }


    return 0;
}

int cache_wrapper(char* cache_method, __uint64_t candidate) {

    int step_count = collatz_r(candidate, cache_method);

    return step_count;
}

unsigned collatz_r(__uint64_t candidate, char* cache_method) {
    if(candidate == 1) {
        return 0;
    }

    // printf("The current value being tested: %lu\n", candidate);

    // printf("Here\n");

    if(strcmp(cache_method, "NONE") != 0) {
        if(cache_has(candidate)) {
            // printf("Returning value from cache...\n");
            return cache_value_for(candidate);
        }
    }


    __uint64_t new_candidate;
    if(candidate % 2 == 0) {
        new_candidate = candidate / 2;
    } // END if(candidate % 2 == 0)
    else {
        new_candidate = 3 * candidate + 1;
    } // END else

    if(new_candidate == 1) return 1;

    unsigned count = 1 + collatz_r(new_candidate, cache_method);

    if(strcmp(cache_method, "NONE") != 0) {
        cache_insert(candidate, count, cache_method);
    }


    return count;
}


bool cache_has(__uint64_t candidate) {
    bool in_cache = false;
    int index = 0;

    while(!in_cache && index < current_max) {
        if(cache[index].value == candidate) {
            // printf("Successfully found %lu\n", cache[index].value);
            in_cache = true;
        }

        // printf("Index: %u\n", index);
        index++;
    }

    cache_accesses++;
    // printf("Cache has been accessed %u times\n", cache_accesses);

    // printf("%u\n", in_cache);

    return in_cache;
}


int cache_value_for(__uint64_t candidate) {
    // printf("Cache hit...\n");
    // printf("Here\n");

    for(int index = 0; index < current_max; index++) {
        if(cache[index].value == candidate) {
            //TESTING
            // printf("Successfully found the cache value %lu, its step count is: %u\n", cache[index].value, cache[index].step_count);

            // printf("Fetching value from cache...\n");

            cache_hits++;
            cache[index].frequency++;

            //TESTING
            // printf("The frequency of %lu is %u\n\n", cache[index].value, cache[index].frequency);

            return cache[index].step_count;
        }
    }

    return 0;
}


void cache_insert(__uint64_t candidate, int step_count, char* cache_method) {
    // printf("Here\n");
    // printf("The size of the cache is %u\n", CACHE_SIZE);
    // printf("Candidate %lu\n", candidate);


    // For LRU, using cache_accesses (a global variable for how many times the cache has been checked)
    // will give us the value of the least recently used when replacing cache values

    // For LFU, we use a global variable inside the cache_node struct called "frequency" when replacing values

    // This for loop is used if the cache has not been filled

    // printf("Value being passed in: %lu\n", candidate);
        for(int index = 0; index < current_max; index++) {
            // if(cache[index].value == candidate) {
            //     return;
            // }

            // printf("Cache value currently in array %lu\n", cache[index].value);
            if(cache[index].value == candidate) {
                // printf("Breaking from the for loop\n");
                break;
            }

    

            if(cache[index].value == 0) {
                // printf("Inserting Value...\n");
                cache[index].value = candidate;
                cache[index].frequency = 1;
                cache[index].step_count = step_count;
                cache[index].last_used = cache_accesses;

                // printf("Index being replaced: %u\n", index);
                // printf("New cache value: %lu\n\n", cache[index].value);

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
            if(cache[index].last_used < cache[least_recent_index].last_used) {
                least_recent_index = index;
            }
        }

        // printf("Value %lu being replaced by %lu\n", cache[least_recent_index].value, candidate);
        // printf("%lu\'s recently used: %u\n\n", cache[least_recent_index].value, cache[least_recent_index].last_used);

        cache[least_recent_index].value = candidate;
        cache[least_recent_index].frequency = 1;
        cache[least_recent_index].step_count = step_count;
        cache[least_recent_index].last_used = cache_accesses;
    }


    // Check if the cache_method used is LFU
    if(strcmp(cache_method, "LFU") == 0) {
        int least_frequent_index = 0;

        for(int index = 0; index < current_max; index++) {
            if(cache[index].frequency < cache[least_frequent_index].frequency) {
                least_frequent_index = index;
            }
        }

        // printf("Value %lu being replaced by %lu\n", cache[least_frequent_index].value, candidate);
        // printf("%lu\'s frequency: %u\n\n", cache[least_frequent_index].value, cache[least_frequent_index].frequency);

        cache[least_frequent_index].value = candidate;
        cache[least_frequent_index].frequency = 1;
        cache[least_frequent_index].step_count = step_count;
        cache[least_frequent_index].last_used = cache_accesses;
    }

    return;
}