#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>



#define DA_TYPE char
#include "da_def.h"

int main(){
    // Timestamp
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%c", tm));
    printf("Test started %s\n", s);
   
    da test;

    // printf("Int size:%d  Bool size:%d \n", sizeof(int), sizeof(bool));

    printf("Start init\n");
    printf("Init exit:%d\n", daInit(&test, 5, 1.5, _double));

    // DA_TYPE v[] = {"dong", "kong", "long", "song", "a", 'b', 'c', 'd', 'e'};
    DA_TYPE v[] = {'d', 'k', 'l', 's', 'a', 'b', 'c', 'd', 'e', 'q', 'p', 'r', 's', 't', '!', '*', '?'};
    // DA_TYPE v[] = {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, x, y, z};
    //double v[] = {0, 1, 2, 3};

    printf("\nCreate exit:%d\n", daCreate(&test, v, COUNT_OF(v)));
    
    //printf("%f\n", daGet(&test, 2));

    int i;
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%c ", *(test.p+i));
        //printf("%f ", daGet(&test, i));
    }
    printf("\n");

    // printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    // printf("Dump: ");
    // for (i = 0; i < test.elements; i++){
    //     printf("%f ", *(test.p+i));
    // }
    // printf("\n");

    printf("Add exit:%d\n", daAdd(&test, -1, 'k'));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%c ", *(test.p+i));
    }
    printf("\n");

/* Remove test 1 */
    //printf("Remove exit:%d\n", daCompactRemove(&test, 0, 0));
    printf("SparseRemove exit:%d\n", daSparseRemove(&test, 0, 2));
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");

    for (i = 0; i < test.elements; i++){
        //printf("%s:", *(test.p+i));
        printf("%c i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d   ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.slots; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n");

/* Remove test 2 */
    printf("SparseRemove exit:%d\n", daSparseRemove(&test, 0, 2));
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");

    for (i = 0; i < test.elements; i++){
        printf("%c i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
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
    printf("SparseRemove exit:%d\n", daSparseRemove(&test, 6, 7));
    printf("Add exit:%d\n", daAdd(&test, -1, 'e'));
    //printf("Add exit:%d\n", daAdd(&test, 4, 10.5));
    printf("Add exit:%d\n", daAdd(&test, 6, 'd'));
    printf("Set exit:%d\n", daSparseSet(&test, 8, 'm'));

    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");
    
    for (i = 0; i < test.elements; i++){
        printf("%c i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d    ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.elements; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n");


/* Compacting test 1 */
   /*  printf("Compact exit:%d\n", daCompact(&test));
    
    printf("Vacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
     printf("\n");
    
    for (i = 0; i < test.elements; i++){
        printf("%c i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

    printf("Total:%d First:%d Last:%d    ", test.vacantTotal, test.vacantFirst, test.vacantLast);
    for (i = 0; i < test.elements; i++){
        printf("%d ", daVacs(&test, i));
    }
    printf("\n"); */

    daClear(&test);
    return 0;
}

