#include "devheaders.h"
#include "sorting.h"

#define ELEMENTS 100    // max 259056


void generate_array(uint32_t *num, uint32_t size);
void print_array(uint32_t *num, uint32_t size);
void copy_array(uint32_t *num, uint32_t size, uint32_t *out);
void compare_array(uint32_t *num, uint32_t size, uint32_t *comp);
void compare_print_array(uint32_t *num, uint32_t size, uint32_t *comp);



void main(){
    srand(time(0));
    uint32_t random[ELEMENTS] = {};
    // uint32_t numbers[ELEMENTS] = {};

    generate_array(random, ELEMENTS);



    // uint32_t num[] = {23, 1, 2, 43, 3, 34, 65, 5 };
    // printf("\n34 in array: %d", inArray(34, num, COUNT_OF(num)));
    // printf("\n666 in array: %d", inArray(66, num, COUNT_OF(num)));
    // printf("\n0 in array: %d", inArray(0, num, COUNT_OF(num)));


    // uint32_t num[] = {1, 4, 5, 43, 3, 34, 45, 65 };
    // sort_merge(random, ELEMENTS);
    // printf("\n");
    // for (int i = 0; i < COUNT_OF(num); i++)
    //     printf("%d ", num[i]);

    uint32_t libNums[ELEMENTS];
    copy_array(random, ELEMENTS, libNums);
    sort_lib(libNums, ELEMENTS);
    // print_array(libNums, ELEMENTS);

    uint32_t mergeNums[ELEMENTS];
    copy_array(random, ELEMENTS, mergeNums);
    sort_merge(mergeNums, ELEMENTS, random);    
    // compare_print_array(mergeNums, ELEMENTS, libNums);
    compare_array(mergeNums, ELEMENTS, libNums);

}

void generate_array(uint32_t *num, uint32_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        num[i] = rand();
    }
}

void compare_array(uint32_t *num, uint32_t size, uint32_t *comp)
{
    int errors = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (num[i] != comp[i])
            errors++;
    }
    printf("\n%d errors in %d elements.\n", errors, size);
}

void compare_print_array(uint32_t *num, uint32_t size, uint32_t *comp)
{
    int errors = 0;
    printf("\n[");
    for (size_t i = 0; i < size; i++)
    {
        if (num[i] == comp[i])
            printf("%d", num[i]);
        else {
            printf("**%d**", num[i]);
            errors++;
        }

        if (i < size - 1)
            printf(", ");
        if (i != 0 && (i + 1) % 10 == 0 && i + 1 != size)
            printf("\n");
    }
    printf("]\n%d errors\n", errors);
}


void print_array(uint32_t *num, uint32_t size)
{
    printf("\n[");
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