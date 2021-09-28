#include "devheaders.h"
#include "sorting.h"


void main(){

    uint32_t num[] = {23, 1, 2, 43, 3, 34, 65, 5 };
    
    sort_merge(num, COUNT_OF(num));

    printf("\n");
    for (int i = 0; i < COUNT_OF(num); i++)
        printf("%d ", num[i]);


    // printf("selected: %d", sel);
}

