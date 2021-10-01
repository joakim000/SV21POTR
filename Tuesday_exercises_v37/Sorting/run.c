/*  Sorting exercise

    Main file: sorting.c

    This is a companion running script with some helper functions,
    prettyprint tables, functionality test and basic timing.
    stdlib qsort used as reference.
*/

#include "sorting.h"

// Flags
#define PRT false // print random tables
#define PT false  // print result tables
#define CT true // qsort comparison testing
#define T true // timing

#define    BUBBLE false
#define INSERTION false
#define SELECTION false
#define     SHELL false
#define     MERGE true
#define     QUICK false

#define RND_MAX INT16_MAX
// #define RND_MAX UINT32_MAX

// Sort size
#define ELEMENTS 100000000

// Declarations
void generate_random_array(uint32_t *num, uint32_t size);
void generate_mixed_array(uint32_t *num, uint32_t size);
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

    generate_random_array(random, ELEMENTS);
    // generate_mixed_array(random, ELEMENTS);
    // print_array(random, ELEMENTS);

    /* Lib */
    copy_array(random, ELEMENTS, compare);
    printf("\nLib qsort: \n"); 
    timer_start = clock();
        sort_lib(compare, ELEMENTS);
    timer_end = clock();
    if (PT) print_array(compare, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));


    /* Bubble */
    if (BUBBLE) {
        copy_array(random, ELEMENTS, numbers);
        printf("\nBubble sort: \n"); 
        timer_start = clock();
            sort_bubble(numbers, ELEMENTS);
        timer_end = clock();
        if (PT) print_array(numbers, ELEMENTS);
        if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
        if (CT) compare_array(numbers, ELEMENTS, compare);
    }

    /* Insertion */
    if (INSERTION) {
        copy_array(random, ELEMENTS, numbers);
        printf("\nInsertion sort: \n"); //Low to High
        timer_start = clock();
            sort_insertion(numbers, ELEMENTS);
        timer_end = clock();
        if (PT) print_array(numbers, ELEMENTS);
        if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
        if (CT) compare_array(numbers, ELEMENTS, compare);
    }

    /* Selection */
    if (SELECTION) {
        copy_array(random, ELEMENTS, numbers);
        printf("\nSelection sort: \n"); //Low to High
        timer_start = clock();
            sort_selection(numbers, ELEMENTS);
        timer_end = clock();
        if (PT) print_array(numbers, ELEMENTS);
        if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
        if (CT) compare_array(numbers, ELEMENTS, compare);
    }
   
    /* Shell */
    if (SHELL) {
        copy_array(random, ELEMENTS, numbers);
        printf("\nShell sort: \n"); //Low to High
        timer_start = clock();
            sort_shell(numbers, ELEMENTS);
        timer_end = clock();
        if (PT) print_array(numbers, ELEMENTS);
        if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
        if (CT) compare_array(numbers, ELEMENTS, compare);
    }
   
    /* Merge */
    if (MERGE) {
        copy_array(random, ELEMENTS, numbers);
        printf("\nMerge sort: \n"); //Low to High
        timer_start = clock();
            sort_merge(numbers, ELEMENTS);
        timer_end = clock();
        if (PT) print_array(numbers, ELEMENTS);
        if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
        if (CT) compare_array(numbers, ELEMENTS, compare);
    }

    /* Quick */
    if (QUICK) {
        copy_array(random, ELEMENTS, numbers);
        printf("\nQuick sort: \n"); //Low to High
        timer_start = clock();
            sort_quick(numbers, ELEMENTS);
        timer_end = clock();
        if (PT) print_array(numbers, ELEMENTS);
        if (T) _printf_p("%d elements in %5.3f seconds.\n", ELEMENTS, TIMING(timer_start, timer_end));
        if (CT) compare_array(numbers, ELEMENTS, compare);
    }

    free(random);
    free(numbers);
    free(compare);

    return 0;
}

/* Helper functions */
void generate_array_old(uint32_t *num, uint32_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        num[i] = rand();
    }
}

void generate_random_array(uint32_t *num, uint32_t size)
{
    for (size_t i = 0; i < size; i++) 
        num[i] = ((rand() << 16) | rand()) % RND_MAX;
}

void generate_mixed_array(uint32_t *num, uint32_t size)
{
    int runSize = 1001;  // Must be odd number
    for (size_t i = 0; i < size; ) 
        if (i % 2) {
            uint32_t runStart = num[i - 1];
            for (int runEnd = i + runSize; i < runEnd && i < size; i++)
                num[i] = runStart + i;
        }
        else {
            for (int runEnd = i + runSize; i < runEnd && i < size; i++) 
                num[i] = ((rand() << 16) | rand()) % RND_MAX;
        }
}

void print_array(uint32_t *num, uint32_t size)
{
    int orders = (int)log10(RND_MAX) + 1;
    int space = 2;
    char fmt[10];
    sprintf(fmt, "%%%dd", orders+space);

    for (size_t i = 0; i < size; i++)
    {
        printf(fmt, num[i]);
        if (i < size - 1)
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("\n");
}

void copy_array(uint32_t *num, uint32_t size, uint32_t *out) {
    for (int i = 0; i < size; i++) 
        out[i] = num[i];
    if (PRT) printf("\nRandomized array (copy):\n");
    if (PRT) print_array(out, ELEMENTS);
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
