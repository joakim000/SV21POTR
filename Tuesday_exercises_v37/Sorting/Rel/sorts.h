/* Sorts for sorting exercise run -           
        edit this file to add or remove sorts
*/

#include <stdint.h>

// Declare sort functions implemented in sorting.c
void sort_lib(uint32_t *num, uint32_t size);
void sort_bubble(uint32_t *num, uint32_t size);
void sort_insertion(uint32_t *num, uint32_t size);
void sort_selection(uint32_t *num, uint32_t size);
void sort_shell(uint32_t *num, uint32_t size);
void sort_merge(uint32_t *num, uint32_t size);
void sort_quick(uint32_t *num, uint32_t size);

    // Descending variants
void sort_lib_d(uint32_t *num, uint32_t size);
void sort_bubble_d(uint32_t *num, uint32_t size);
void sort_insertion_d(uint32_t *num, uint32_t size);
void sort_selection_d(uint32_t *num, uint32_t size);
void sort_shell_d(uint32_t *num, uint32_t size);
void sort_merge_d(uint32_t *num, uint32_t size);
void sort_quick_d(uint32_t *num, uint32_t size);


// Init sorts, using default run setting from options.h
#define NUMBER_OF_SORTS 6
sort sorts[NUMBER_OF_SORTS] = {
    {.name="bub", .print_name="Bubble sort", .default_run=BUBBLE, .sort_ptr = sort_bubble},
    {.name="ins", .print_name="Insertion sort", .default_run=INSERTION, .sort_ptr = sort_insertion},
    {.name="sel", .print_name="Selection sort", .default_run=SELECTION, .sort_ptr = sort_selection},
    {.name="she", .print_name="Shell sort", .default_run=SHELL, .sort_ptr = sort_shell},
    {.name="mer", .print_name="Merge sort", .default_run=MERGE, .sort_ptr = sort_merge},
    {.name="qui", .print_name="Quick sort", .default_run=QUICK, .sort_ptr = sort_quick}
};

