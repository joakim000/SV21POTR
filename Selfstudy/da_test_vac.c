#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "da.h"

int main(){
    da test;

    printf("Int size:%d  Bool size:%d \n", sizeof(int), sizeof(bool));

    printf("Start init\n");
    printf("Init exit:%d\n", daInit(&test, 5, 1.5));

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

    // printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    // printf("Dump: ");
    // for (i = 0; i < test.elements; i++){
    //     printf("%f ", *(test.p+i));
    // }
    // printf("\n");

    printf("Add exit:%d\n", daAdd(&test, -1, 7.123));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");

/* Remove test 1 */
    //printf("Remove exit:%d\n", daCompactRemove(&test, 2, 2));
    printf("SparseRemove exit:%d\n", daSparseRemove(&test, 5, 5));
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");

    for (i = 0; i < test.elements; i++){
        //printf("%4.3f:", *(test.p+i));
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d   ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.slots; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n");

/* Remove test 2 */
    printf("SparseRemove exit:%d\n", daSparseRemove(&test, 3, 3));
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");

    for (i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d   ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.slots; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n");

/* Remove test 3 */
    //printf("Remove exit:%d\n", daCompactRemove(&test, 2, 4));
    printf("SparseRemove exit:%d\n", daSparseRemove(&test, 1, 1));
    
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");
    
    for (i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d    ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.elements; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n");


/* Compacting test 1 */
     printf("Compact exit:%d\n", daCompact(&test));
    
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");
    
    for (i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d    ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.elements; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n");

    return 0;
}

