#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "sorting.h"


// Flags
#define PRT false // print random tables
#define PT false // print tables
#define CT true // comparison testing
#define T true // timing

// Sort size
#define ELEMENTS 240000    // max 259056 (stack)

// Declarations
void generate_array(uint32_t *num, uint32_t size);
void print_array(uint32_t *num, uint32_t size);
void copy_array(uint32_t *num, uint32_t size, uint32_t *out);
void compare_array(uint32_t *num, uint32_t size, uint32_t *comp);

int main(void)
{
    printf("This line not reached with ELEMENTS > 259056  (0x3f3f0)\n");

    srand(time(0));

    // uint32_t random[ELEMENTS] = {};
    // uint32_t numbers[ELEMENTS] = {};
    // uint32_t compare[ELEMENTS] = {};

    uint32_t* random;
    uint32_t* numbers;
    uint32_t* compare;
    random = calloc(ELEMENTS, sizeof(uint32_t));
    assert( ("Memory allocation failed.", random != NULL) );
    numbers = calloc(ELEMENTS, sizeof(uint32_t));
    assert( ("Memory allocation failed.", numbers != NULL) );
    compare = calloc(ELEMENTS, sizeof(uint32_t));
    assert( ("Memory allocation failed.", compare != NULL) );



    generate_array(random, ELEMENTS);
    // printf("Randomized array: \n");
    // print_array(random, ELEMENTS);

    /* Lib */
    copy_array(random, ELEMENTS, compare);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(compare, ELEMENTS);
    
    printf("\nLib qsort: \n"); //Low to High
    timer_start = clock();
        sort_lib(compare, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(compare, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));

     // printf("Lib sort (descending): \n"); //High to Low
    // sort_lib_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    /* Bubble */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);

    printf("\nBubble sort: \n"); //Low to High
    timer_start = clock();
        // sort_bubble(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);

    // printf("Bubble sort (descending): \n"); //High to Low
    // sort_bubble_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);
    



    /* Insertion */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("\nInsertion sort: \n"); //Low to High
    timer_start = clock();
        sort_insertion(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 
    // printf("Insertion sort (descending): \n"); //High to Low
    // sort_insertion_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);




    /* Selection */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
   
    printf("\nSelection sort: \n"); //Low to High
    timer_start = clock();
        // sort_selection(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 
    // printf("Selection sort (descending): \n"); //High to Low
    // sort_selection_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    /* Shell */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("\nShell sort: \n"); //Low to High
    timer_start = clock();
        sort_shell(numbers, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 
    // printf("Shell sort (descending): \n"); //High to Low
    // sort_shell_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);




    /* Merge */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("\nMerge sort: \n"); //Low to High
    timer_start = clock();
        sort_merge(numbers, ELEMENTS, random);
    timer_end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));
    if (CT) compare_array(numbers, ELEMENTS, compare);
 
    // printf("Merge sort (descending): \n"); //High to Low
    // sort_merge_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    



    /* Quick */
    // copy_array(random, ELEMENTS, numbers);
    // printf("Randomized array (copy):\n");
    // if (PT) print_array(numbers, ELEMENTS);
    
    // printf("\nQuick sort: \n"); //Low to High
    // sort_quick(numbers, ELEMENTS);
    // if (PT) print_array(numbers, ELEMENTS);
    // if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(timer_start, timer_end));
    // if (CT) compare_array(numbers, ELEMENTS, compare);
 
    // printf("Quick sort (descending): \n"); //High to Low
    // sort_quick_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);


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