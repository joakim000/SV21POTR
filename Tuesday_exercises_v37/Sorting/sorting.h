#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro



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
