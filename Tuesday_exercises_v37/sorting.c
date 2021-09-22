#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "sorting.h"

#define ELEMENTS 100

void generate_array(uint32_t *num, uint8_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        num[i] = rand();
    }
}

void print_array(uint32_t *num, uint8_t size)
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

void copy_array(uint32_t *num, uint8_t size, uint32_t *out) {
    for (int i = 0; i < size; i++) 
        out[i] = num[i];
}


int main(void)
{

    //uint32_t == unsigned int
    //uint8_t == unsigned char
    srand(time(0));
    uint32_t random[ELEMENTS] = {};
    uint32_t numbers[ELEMENTS] = {};

    generate_array(random, ELEMENTS);
    // printf("Randomized array: \n");
    // print_array(random, ELEMENTS);



    /* Bubble ok */
    copy_array(random, ELEMENTS, numbers);
    printf("Randomized array (copy):\n");
    print_array(numbers, ELEMENTS);

    printf("Bubble sort: \n"); //Low to High
    sort_bubble(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);
 
    // printf("Bubble sort (descending): \n"); //High to Low
    // sort_bubble_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);
    



    /* Insertion ok */
    copy_array(random, ELEMENTS, numbers);
    printf("Randomized array (copy):\n");
    print_array(numbers, ELEMENTS);
    
    printf("Insertion sort: \n"); //Low to High
    sort_insertion(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);
 
    // printf("Insertion sort (descending): \n"); //High to Low
    // sort_insertion_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);




    /* Selection ok */
    copy_array(random, ELEMENTS, numbers);
    printf("Randomized array (copy):\n");
    print_array(numbers, ELEMENTS);
   
    printf("Selection sort: \n"); //Low to High
    sort_selection(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);
 
    // printf("Selection sort (descending): \n"); //High to Low
    // sort_selection_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);




    /* Merge */
    // copy_array(random, ELEMENTS, numbers);
    // printf("Randomized array (copy):\n");
    // print_array(numbers, ELEMENTS);
    
    // printf("Merge sort: \n"); //Low to High
    // sort_merge(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);
 
    // printf("Merge sort (descending): \n"); //High to Low
    // sort_merge_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    /* Shell */
    // copy_array(random, ELEMENTS, numbers);
    // printf("Randomized array (copy):\n");
    // print_array(numbers, ELEMENTS);
    
    // printf("Shell sort: \n"); //Low to High
    // sort_shell(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);
 
    // printf("Shell sort (descending): \n"); //High to Low
    // sort_shell_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);



    /* Quick */
    // copy_array(random, ELEMENTS, numbers);
    // printf("Randomized array (copy):\n");
    // print_array(numbers, ELEMENTS);
    
    // printf("Quick sort: \n"); //Low to High
    // sort_quick(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);
 
    // printf("Quick sort (descending): \n"); //High to Low
    // sort_quick_d(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);


    return 0;
}