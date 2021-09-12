#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "da.h"

int main() {
    da test;
<<<<<<< HEAD
    
    
    printf("Init exit:%d\n", daInit(&test, 5, _double));
=======
    printf("Start init\n");
    printf("Init exit:%d\n", daInit(&test, 5, 1.5));
>>>>>>> sparse

    double v[] = {0, 1, 2, 3, 4, 5, 6};
    //double v[] = {0, 1, 2, 3};

    // printf("\nCreate exit:%d\n", daCreate(&test, v, COUNT_OF(v)));
    
    //printf("%f\n", daGet(&test, 2));

    int i;
    double out;
   /*  printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        //printf("%f ", *(test.p+i));
        daGet(&test, i, &out);
        printf("%f ", out);
    }
    printf("\n"); */

    double addVal = 10.123;
    printf("Add exit:%d\n", daAdd(&test, 2, &addVal));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        // printf("%f ", *(test.p+i));
        // printf("%f ", daGet(&test, i));
        daGet(&test, i, &out);
        printf("%f ", out);
    }
    printf("\n");

    addVal = 11.123;
    printf("Add exit:%d\n", daAdd(&test, -1, &addVal));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        // printf("%f ", *(test.p+i));
        // printf("%f ", daGet(&test, i));
        daGet(&test, i, &out);
        printf("%f ", out);
    }
    printf("\n");


    // printf("CompactRemove exit:%d\n", daCompactRemove(&test, 2, 2));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        // printf("%f ", *(test.p+i));
        // printf("%f ", daGet(&test, i));
    }
    printf("\n");

    // printf("CompactRemove exit:%d\n", daCompactRemove(&test, 2, 4));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        // printf("%f ", *(test.p+i));
        // printf("%f ", daGet(&test, i));
    }
    printf("\n");


    return 0;
}

