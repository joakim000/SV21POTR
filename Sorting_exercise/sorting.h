#include <stdint.h>
#include <time.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

// Timing
#define TIMING(y, x) ((float)(x - y) / CLOCKS_PER_SEC)
clock_t timer_start; clock_t timer_end; 

void sort_lib(uint32_t *num, uint32_t size);
void sort_bubble(uint32_t *num, uint32_t size);
void sort_bubble_d(uint32_t *num, uint32_t size);
void sort_insertion(uint32_t *num, uint32_t size);
void sort_insertion_d(uint32_t *num, uint32_t size);
void sort_selection(uint32_t *num, uint32_t size);
void sort_selection_d(uint32_t *num, uint32_t size);
void sort_shell(uint32_t *num, uint32_t size);
void sort_merge(uint32_t *num, uint32_t size);
void sort_quick(uint32_t *num, uint32_t size);



