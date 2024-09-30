#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <collatz.h>
#include <cache.h>

// Global variable to keep track of how many times a cache value is used
int cache_hits = 0;

// Global variable to keep track of how many times the cache is accessesd
int cache_accesses = 0;

// Global variable that holds the current index in the cache so the program doesn't have to iterate through the entire cache every time
int current_max = 1;

int main(int argc, char* argv[]) {
    if(argc < 5) {
        printf("[-] Must pass the number of values to test (N), the smallest value to test (MIN), the largest value to test (MAX) and the caching method (LRU, LFU)\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100 LRU\n");
        return 0;
    } 

    if(atoi(argv[1]) < 1 || atoi(argv[2]) < 1 || atoi(argv[3]) < 1) {
        printf("[-] Must pass a value greater than 0 for all integer inputs\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100 LRU\n");
        return 0;
    }

    // Initializing the string for what cache method to be used, then checking if it's valid
    char* cache_method = argv[4];

    // Converting command line argument for cache method to all uppercase
    for(int i = 0; i < sizeof(cache_method); i++) {
        cache_method[i] = toupper(cache_method[i]);
    } 

    if(strcmp(cache_method, "LRU") != 0 && strcmp(cache_method, "LFU") != 0 && strcmp(cache_method, "NONE") != 0) {
        printf("[-] Invalid cache method...Use \"NONE\", \"LRU\", or \"LFU\"\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100 LRU\n");

        return 0;
    }

    srand(time(NULL));

    FILE* fptr;
    fptr = fopen("results.csv", "w");

    int step_count = 0;
    __uint64_t min = atoi(argv[2]);
    __uint64_t max = atoi(argv[3]);


    fprintf(fptr, "Collatz Candidate, Step Count\n");


    for(int i = 0; i < atoi(argv[1]); i++) {
        __uint64_t collatz_candidate = rand() % (max - min + 1) + min;

        step_count = cache_wrapper(cache_method, collatz_candidate);

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
