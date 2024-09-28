#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

unsigned collatz_r(__uint64_t candidate);

float cache_wrapper(char* cache_method, __uint64_t candidate);

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

    if(strcmp(cache_method, "LRU") != 0 && strcmp(cache_method, "LFU") != 0) {
        printf("[-] Invalid cache method...Use either \"LRU\" or \"LFU\"\n");
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

        // step_count = collatz_r(collatz_candidate);
        step_count = cache_wrapper(cache_method, collatz_candidate);

        printf("Step count for %lu: %u\n", collatz_candidate, step_count);
        fprintf(fptr, "%lu,%u\n", collatz_candidate, step_count);
    }

    return 0;
}

float cache_wrapper(char* cache_method, __uint64_t candidate) {
    // TESTING
    // if(strcmp(cache_method, "LRU")) {
    //     printf("You are using %s\n", cache_method);
    // }
    // else if(strcmp(cache_method, "LFU")) {
    //     printf("You are using %s\n", cache_method);
    // }

    int step_count = collatz_r(candidate);

    return step_count;
}

unsigned collatz_r(__uint64_t candidate) {
    if(candidate == 1) {
        return 0;
    }

    __uint64_t new_candidate;
    if(candidate % 2 == 0) {
        new_candidate = candidate / 2;
    } // END if(candidate % 2 == 0)
    else {
        new_candidate = 3 * candidate + 1;
    } // END else

    if(new_candidate == 1) return 1;

    unsigned count = 1 + collatz_r(new_candidate);

    return count;
}