#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "da.h"

int main(){
    da test;
    printf("Start init\n");
    printf("Init exit:%d\n", daInit(&test, 5));

    double v[] = {0, 1, 2, 3, 4, 5, 6};
    //double v[] = {0, 1, 2, 3};

    printf("\nCreate exit:%d\n", daCreate(&test, v, COUNT_OF(v)));
    
    //printf("%f\n", daGet(&test, 2));

    int i;
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
        //printf("%f ", daGet(&test, i));
    }
    printf("\n");

    printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");

    printf("Add exit:%d\n", daAdd(&test, -1, 11.123));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");


    printf("CompactRemove exit:%d\n", daCompactRemove(&test, 2, 2));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");

    printf("CompactRemove exit:%d\n", daCompactRemove(&test, 2, 4));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");


    return 0;
}

