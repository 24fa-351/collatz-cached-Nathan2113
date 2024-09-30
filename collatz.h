#include <stdint.h>

#ifndef COLLATZ_H
#define COLLATZ_H

unsigned collatz_r(__uint64_t candidate, char* cache_method);

int cache_wrapper(char* cache_method, __uint64_t candidate);

#endif