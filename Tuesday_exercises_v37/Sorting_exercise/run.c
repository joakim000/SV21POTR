/*  Sorting exercise

    Main file: sorting.c

    This is a companion running script with some helper functions,
    prettyprint tables, functionality test and basic timing.
    stdlib qsort used as reference.
*/

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorting.h"

// Timing
#define TIMING(y, x) ((float)(x - y) / CLOCKS_PER_SEC)
clock_t timer_start; clock_t timer_end; 

// Flags
#define PRT false // print random tables
#define PT true  // print result tables
#define CT true // qsort comparison testing
#define T true // timing

// Sort size
#define ELEMENTS 50    

// Declarations
void generate_array(uint32_t *num, uint32_t size);
void print_array(uint32_t *num, uint32_t size);
void copy_array(uint32_t *num, uint32_t size, uint32_t *out);
void compare_array(uint32_t *num, uint32_t size, uint32_t *comp);

int main(void)
{
    srand(time(0));

    uint32_t* random = calloc(ELEMENTS, sizeof(uint32_t));
    assert( ("Memory allocation failed.", random != NULL) );
    uint32_t* numbers = calloc(ELEMENTS, sizeof(uint32_t));
    assert( ("Memory allocation failed.", numbers != NULL) );
    uint32_t* compare = calloc(ELEMENTS, sizeof(uint32_t));
    assert( ("Memory allocation failed.", compare != NULL) );

    generate_array(random, ELEMENTS);
    // printf("Randomized array: \n");
    // print_array(random, ELEMENTS);

    /* Lib */
    copy_array(random, ELEMENTS, compare);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(compare, ELEMENTS);
    
    printf("\nLib qsort: \n"); 
    timer_start = clock();
        sort_lib(compare, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(compare, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));


    /* Bubble */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);

    printf("\nBubble sort: \n"); 
    timer_start = clock();
        sort_bubble(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);


    /* Insertion */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("\nInsertion sort: \n"); //Low to High
    timer_start = clock();
        sort_insertion(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
  

    /* Selection */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
   
    printf("\nSelection sort: \n"); //Low to High
    timer_start = clock();
        sort_selection(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 
   
    /* Shell */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("\nShell sort: \n"); //Low to High
    timer_start = clock();
        sort_shell(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 
   
    /* Merge */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("\nMerge sort: \n"); //Low to High
    timer_start = clock();
        sort_merge(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 

    /* Quick */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);

    printf("\nQuick sort: \n"); //Low to High
    timer_start = clock();
        sort_quick(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 

    return 0;
}

/* Helper functions */
void generate_array(uint32_t *num, uint32_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        num[i] = rand();
    }
}

void print_array(uint32_t *num, uint32_t size)
{
    printf("[");
    for (size_t i = 0; i < size; i++)
    {
        printf("%d", num[i]);
        if (i < size - 1)
            printf(", ");
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("]\n");
}

void copy_array(uint32_t *num, uint32_t size, uint32_t *out) {
    for (int i = 0; i < size; i++) 
        out[i] = num[i];
}

void compare_array(uint32_t *num, uint32_t size, uint32_t *comp)
{
    int errors = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (num[i] != comp[i])
            errors++;
    }
    printf("%d errors in %d elements.\n", errors, size);
}
