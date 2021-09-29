#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "sorting.h"

#define ELEMENTS 32    // max 259056
#define PRT false // print random tables
#define PT true // print tables
#define T true // timing

void generate_array(uint32_t *num, uint32_t size);
void print_array(uint32_t *num, uint32_t size);
void copy_array(uint32_t *num, uint32_t size, uint32_t *out);

int main(void)
{
    
    // printf("hej");
    //uint32_t == unsigned int
    //uint32_t == unsigned char
    srand(time(0));
    uint32_t random[ELEMENTS] = {};
    uint32_t numbers[ELEMENTS] = {};

    generate_array(random, ELEMENTS);
    // printf("Randomized array: \n");
    // print_array(random, ELEMENTS);



    /* Bubble ok */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);

    printf("Bubble sort: \n"); //Low to High
    start = clock();
        sort_bubble(numbers, ELEMENTS);
    end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(start, end));
 
    // printf("Bubble sort (descending): \n"); //High to Low
    // sort_bubble_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);
    



    /* Insertion ok */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("Insertion sort: \n"); //Low to High
    start = clock();
        sort_insertion(numbers, ELEMENTS);
    end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(start, end));
 
    // printf("Insertion sort (descending): \n"); //High to Low
    // sort_insertion_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);




    /* Selection ok */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
   
    printf("Selection sort: \n"); //Low to High
    start = clock();
        sort_selection(numbers, ELEMENTS);
    end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(start, end));
 
    // printf("Selection sort (descending): \n"); //High to Low
    // sort_selection_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    /* Shell */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("Shell sort: \n"); //Low to High
    start = clock();
        sort_shell(numbers, ELEMENTS);
    end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(start, end));
 
    // printf("Shell sort (descending): \n"); //High to Low
    // sort_shell_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);




    /* Merge */
    copy_array(random, ELEMENTS, numbers);
    if (PRT) printf("Randomized array (copy):\n");
    if (PRT) print_array(numbers, ELEMENTS);
    
    printf("Merge sort: \n"); //Low to High
    start = clock();
        sort_merge(numbers, ELEMENTS);
    end = clock();
    if (PT) print_array(numbers, ELEMENTS);
    if (T) _printf_p("%d elements in %5.3f seconds\n", ELEMENTS, TIMING(start, end));
 
    // printf("Merge sort (descending): \n"); //High to Low
    // sort_merge_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    



    /* Quick */
    // copy_array(random, ELEMENTS, numbers);
    // printf("Randomized array (copy):\n");
    // if (PT) print_array(numbers, ELEMENTS);
    
    // printf("Quick sort: \n"); //Low to High
    // sort_quick(numbers, ELEMENTS);
    // if (PT) print_array(numbers, ELEMENTS);
 
    // printf("Quick sort (descending): \n"); //High to Low
    // sort_quick_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);


    return 0;
}

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
    printf("]\n\n");
}

void copy_array(uint32_t *num, uint32_t size, uint32_t *out) {
    for (int i = 0; i < size; i++) 
        out[i] = num[i];
}