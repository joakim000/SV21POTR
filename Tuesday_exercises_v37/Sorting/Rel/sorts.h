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
void sort_tmerge(uint32_t *num, uint32_t size);
void sort_quick(uint32_t *num, uint32_t size);
void sort_radix(uint32_t *num, uint32_t size);

// Descending variants
void sort_lib_d(uint32_t *num, uint32_t size);
void sort_bubble_d(uint32_t *num, uint32_t size);
void sort_insertion_d(uint32_t *num, uint32_t size);
void sort_selection_d(uint32_t *num, uint32_t size);
void sort_shell_d(uint32_t *num, uint32_t size);
void sort_merge_d(uint32_t *num, uint32_t size);
void sort_tmerge_d(uint32_t *num, uint32_t size);
void sort_quick_d(uint32_t *num, uint32_t size);
void sort_radix_d(uint32_t *num, uint32_t size);


// Init sorts, using default run setting from options.h
#define NUMBER_OF_SORTS 9

sort sorts_a[NUMBER_OF_SORTS] = {
    {.name="lib", .print_name="Lib qsort", .default_run=true, .sort_ptr = sort_lib},
    {.name="bub", .print_name="Bubble sort", .default_run=BUBBLE, .sort_ptr = sort_bubble},
    {.name="ins", .print_name="Insertion sort", .default_run=INSERTION, .sort_ptr = sort_insertion},
    {.name="sel", .print_name="Selection sort", .default_run=SELECTION, .sort_ptr = sort_selection},
    {.name="she", .print_name="Shell sort", .default_run=SHELL, .sort_ptr = sort_shell},
    {.name="mer", .print_name="Merge sort", .default_run=MERGE, .sort_ptr = sort_merge},
    {.name="qui", .print_name="Quick sort", .default_run=QUICK, .sort_ptr = sort_quick},
    {.name="tme", .print_name="Merge sort (threaded)", .default_run=TMERGE, .sort_ptr = sort_tmerge},
    {.name="rad", .print_name="Radix sort", .default_run=RADIX, .sort_ptr = sort_radix}
}; 

sort sorts_d[NUMBER_OF_SORTS] = {
    {.name="lib", .print_name="Lib qsort", .default_run=true, .sort_ptr = sort_lib_d},
    {.name="bub", .print_name="Bubble sort", .default_run=BUBBLE, .sort_ptr = sort_bubble_d},
    {.name="ins", .print_name="Insertion sort", .default_run=INSERTION, .sort_ptr = sort_insertion_d},
    {.name="sel", .print_name="Selection sort", .default_run=SELECTION, .sort_ptr = sort_selection_d},
    {.name="she", .print_name="Shell sort", .default_run=SHELL, .sort_ptr = sort_shell_d},
    {.name="mer", .print_name="Merge sort", .default_run=MERGE, .sort_ptr = sort_merge_d},
    {.name="qui", .print_name="Quick sort", .default_run=QUICK, .sort_ptr = sort_quick_d},
    {.name="tme", .print_name="Merge sort (threaded)", .default_run=TMERGE, .sort_ptr = sort_tmerge_d},
    {.name="rad", .print_name="Radix sort", .default_run=RADIX, .sort_ptr = sort_radix_d}
}; 


