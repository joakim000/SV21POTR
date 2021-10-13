#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define GENRAND(x) (((rand() << 16) | rand()) % x)
#elif __linux__
#define GENRAND(x) (rand() % x) 
#else
#error "Unsupported platform"
#endif

// Timing
#define TIMING(y, x) ((float)(x - y) / CLOCKS_PER_SEC)
clock_t timer_start; clock_t timer_end; 

// Helper functions
int checkArg(int argc, char *argv[], char arg[]);
void generate_array(uint32_t *num, uint32_t size, uint32_t run_len, uint32_t rnd_max);
void print_array(uint32_t *num, uint32_t size, uint32_t tmax);
void copy_array(uint32_t *num, uint32_t size, uint32_t *out, bool prt, uint32_t tmax);
void compare_array(uint32_t *num, uint32_t size, uint32_t *comp);

// sorting.c, Sort functions
void sort_lib(uint32_t *num, uint32_t size);
void sort_lib_d(uint32_t *num, uint32_t size);
void sort_bubble(uint32_t *num, uint32_t size);
void sort_bubble_d(uint32_t *num, uint32_t size);
void sort_insertion(uint32_t *num, uint32_t size);
void sort_insertion_d(uint32_t *num, uint32_t size);
void sort_selection(uint32_t *num, uint32_t size);
void sort_selection_d(uint32_t *num, uint32_t size);
void sort_shell(uint32_t *num, uint32_t size);
void sort_shell_d(uint32_t *num, uint32_t size);
void sort_merge(uint32_t *num, uint32_t size);
void sort_merge_d(uint32_t *num, uint32_t size);
void sort_quick(uint32_t *num, uint32_t size);
void sort_quick_d(uint32_t *num, uint32_t size);

enum sort_algos{bub, ins, sel, she, mer, qui};