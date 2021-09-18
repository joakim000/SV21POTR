#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

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

void sort_ascending(uint32_t *num, uint8_t size)
{
    //implement me!
}

void sort_descending(uint32_t *num, uint8_t size)
{
    //implement me!
}

int main(void)
{

    //uint32_t == unsigned int
    //uint8_t == unsigned char
    srand(time(0));
    uint32_t numbers[ELEMENTS] = {};

    generate_array(numbers, ELEMENTS);
    printf("Randomized array: \n");
    print_array(numbers, ELEMENTS);

    printf("Ascending array: \n"); //Low to High
    sort_ascending(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);

    printf("Descending array: \n"); //High to Low
    sort_descending(numbers, ELEMENTS);
    print_array(numbers, ELEMENTS);

    return 0;
}