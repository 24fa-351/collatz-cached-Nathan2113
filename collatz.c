#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

unsigned collatz_r(__uint64_t candidate);

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("[-] Must pass the number of values to test (N), the smallest value to test (MIN) and the largest value to test (MAX)\n");
        printf("[-] EXAMPLE: ./collatz 15 1 100\n");
    } // END if(argc < 3)

    // THIS WILL NEED TO BE CHANGED LATER
    if(atoi(argv[1]) < 1) {
        printf("[-] Must pass a value greater than 0\n");
    }

    srand(time(NULL));

    FILE* fptr;
    fptr = fopen("results.csv", "w");

    // In order to get random number in a range
        // rand() % (max_number + 1 - minimum_number) + minimum_number

    // TESTING
    // printf("Min Value: %u\n", atoi(argv[2]));
    // printf("Max Value: %u\n", atoi(argv[3]));

    // printf("Value passed: %lu\n", collatz_candidate);

    int step_count = 0;
    __uint64_t min = atoi(argv[2]);
    __uint64_t max = atoi(argv[3]);

    fprintf(fptr, "Collatz Candidate, Step Count\n");

    for(int i = 0; i < atoi(argv[1]); i++) {
        __uint64_t collatz_candidate = rand() % (max - min + 1) + min;

        step_count = collatz_r(collatz_candidate);

        printf("Step count for %lu: %u\n", collatz_candidate, step_count);
        fprintf(fptr, "%lu,%u\n", collatz_candidate, step_count);
    }

    return 0;
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