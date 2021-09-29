#include <assert.h>
#include <ctype.h>
#include <errno.h>
//#include <float.h>
//#include <limits.h>
#include <locale.h>
#include <math.h>
//#include <setjmp.h>
//#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

// Timing
#define TIMING(y, x) ((float)(x - y) / CLOCKS_PER_SEC)
clock_t timer_start; clock_t timer_end; float stopwatch; 

