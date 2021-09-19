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
    // print_array(numbers, ELEMENTS);

    /* Bubble */
    copy_array(random, ELEMENTS, numbers);
    printf("Bubble sort: \n"); //Low to High
    sort_bubble(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);
 
    printf("Bubble sort (descending): \n"); //High to Low
    sort_bubble_d(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);
    

   /* Insertion */
    copy_array(random, ELEMENTS, numbers);
    printf("Insertion sort: \n"); //Low to High
    sort_insertion(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);
 
    printf("Bubble sort (descending): \n"); //High to Low
    sort_insertion_d(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);





    // printf("Ascending array: \n"); //Low to High
    // sort_ascending(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);

    // printf("Descending array: \n"); //High to Low
    // sort_descending(numbers, ELEMENTS);
    // print_array(numbers, ELEMENTS);

    return 0;
}