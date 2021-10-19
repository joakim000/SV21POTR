/* Sorts for sorting exercise bench -           
        edit this file to add or remove sorts
*/

#include <stdint.h>
#include "options.h"
#define SORTS_H

typedef struct sort_holder {
    char* name;
    char* print_name;
    bool default_run;
    // void (*sort_ptr)(uint32_t*, uint32_t size); // ptr to sort function
    void (*sort_ptr)(void* args); // ptr to sort function
    bool run;
} sort;

// Declare sort functions implemented in sorting.c
void sort_lib(void *params);
void sort_bubble(void *params);
void sort_insertion(void *params);
void sort_selection(void *params);  
void sort_shell(void *params);
void sort_merge(void *params);
void sort_tmerge(void *params);
void sort_quick(void *params);
void sort_radix(void *params);

// Descending variants
void sort_lib_d(void *params);
void sort_bubble_d(void *params);
void sort_insertion_d(void *params);
void sort_selection_d(void *params);
void sort_shell_d(void *params);
void sort_merge_d(void *params);
void sort_tmerge_d(void *params);
void sort_quick_d(void *params);
void sort_radix_d(void *params);


// Init sorts, using default run setting from options.h
#define NUMBER_OF_SORTS 9

sort sorts_a[NUMBER_OF_SORTS] = {
    {.name="lib", .print_name="Lib qsort", .default_run=true, .sort_ptr = sort_lib},
    {.name="bub", .print_name="Bubble sort", .default_run=BUBBLE, .sort_ptr = sort_bubble},
    {.name="ins", .print_name="Insertion sort", .default_run=INSERTION, .sort_ptr = sort_insertion},
    {.name="sel", .print_name="Selection sort", .default_run=SELECTION, .sort_ptr = sort_selection},
    {.name="she", .print_name="Shell sort", .default_run=SHELL, .sort_ptr = sort_shell},
    {.name="mer", .print_name="Merge sort", .default_run=MERGE, .sort_ptr = sort_merge},
    {.name="tme", .print_name="Merge sort (threaded)", .default_run=TMERGE, .sort_ptr = sort_tmerge},
    {.name="qui", .print_name="Quick sort", .default_run=QUICK, .sort_ptr = sort_quick},
    {.name="rad", .print_name="Radix sort", .default_run=RADIX, .sort_ptr = sort_radix}
}; 

sort sorts_d[NUMBER_OF_SORTS] = {
    {.name="lib", .print_name="Lib qsort", .default_run=true, .sort_ptr = sort_lib_d},
    {.name="bub", .print_name="Bubble sort", .default_run=BUBBLE, .sort_ptr = sort_bubble_d},
    {.name="ins", .print_name="Insertion sort", .default_run=INSERTION, .sort_ptr = sort_insertion_d},
    {.name="sel", .print_name="Selection sort", .default_run=SELECTION, .sort_ptr = sort_selection_d},
    {.name="she", .print_name="Shell sort", .default_run=SHELL, .sort_ptr = sort_shell_d},
    {.name="mer", .print_name="Merge sort", .default_run=MERGE, .sort_ptr = sort_merge_d},
    {.name="tme", .print_name="Merge sort (threaded)", .default_run=TMERGE, .sort_ptr = sort_tmerge_d},
    {.name="qui", .print_name="Quick sort", .default_run=QUICK, .sort_ptr = sort_quick_d},
    {.name="rad", .print_name="Radix sort", .default_run=RADIX, .sort_ptr = sort_radix_d}
}; 



struct sort_args {
    // Standard args
    uint32_t* num;
    uint32_t size;
    
    uint16_t maxThreads; // For threaded sorts 
    int shellSeq[]; // Gap sequence for shell sort
};
