#include <assert.h>
#include <limits.h>
// #include <math.h>
// #include <stdatomic.h>  // Not available on Win
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>

// Threading
#include <pthread.h>
// #include <unistd.h>  //Header file for sleep()
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 16384
#endif
#define STACK_MIN PTHREAD_STACK_MIN

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro




