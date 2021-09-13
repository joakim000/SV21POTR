#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>
// #include <windows.h>

#define DA_TYPE int
#include "da_def.h"


#define TESTSIZE 100000
#define     PART 30000

int main(){
    int i;
    clock_t start;
    clock_t end;
    
    // Timestamp
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%c", tm));
    printf("Test started %s\n", s);  

    DA_TYPE v[TESTSIZE];
    for (i = 0; i < TESTSIZE; i++) v[i] = i;
   
/* Remove test 1 */
    da test1;
    daInit(&test1, TESTSIZE, 1.5);
    if(daCreate(&test1, v, COUNT_OF(v)) != 0) printf("Create error\n");
    printf("\nElements:%d", test1.elements);
    start = clock();
    daRemove(&test1, 0, PART);
    end = clock();
    printf("\nElements:%d", test1.elements);
    printf("\nTotal:%d First:%d Last:%d   ", test1.vacantTotal, test1.vacantFirst, test1.vacantLast);
    printf("\nRemove %d of %d\n    Clocks:%ld  Secs:%f\n", PART, TESTSIZE, end-start, (float)(end - start) / CLOCKS_PER_SEC);    
    daClear(&test1);

/* Remove test 2 */
    da test2;
    daInit(&test2, TESTSIZE, 1.5);
    if(daCreate(&test1, v, COUNT_OF(v)) != 0) printf("Create error\n");
    printf("\nElements:%d", test2.elements);
    start = clock();
    daSparseRemove(&test2, 0, PART);
    end = clock();
    printf("\nElements:%d", test2.elements);
    printf("\nTotal:%d First:%d Last:%d   ", test2.vacantTotal, test2.vacantFirst, test2.vacantLast);
    printf("\nSparse remove %d of %d\n    Clocks:%ld  Secs:%f\n", PART, TESTSIZE, end-start, (float)(end - start) / CLOCKS_PER_SEC);    
    daClear(&test2);

/* Remove test 3 */
    da test3;
    daInit(&test3, TESTSIZE, 1.5);
    daCreate(&test3, v, COUNT_OF(v));
    printf("\nElements:%d", test3.elements);
    start = clock();
    for (i = 0; i < PART; i+=1)
        daRemove(&test3, i, i);
    end = clock();
    printf("\nElements:%d", test3.elements);
    printf("\nTotal:%d First:%d Last:%d   ", test3.vacantTotal, test3.vacantFirst, test3.vacantLast);
    printf("\nRemove 1by1 %d of %d\n    Clocks:%ld  Secs:%f\n", PART, TESTSIZE, end-start, (float)(end - start) / CLOCKS_PER_SEC);  
    daClear(&test3);

/* Remove test 4 */
    da test4;
    daInit(&test4, TESTSIZE, 1.5);
    daCreate(&test4, v, COUNT_OF(v));
    printf("\nElements:%d", test4.elements);
    start = clock();
    for (i = 0; i < PART; i+=1)
        daSparseRemove(&test4, i, i);
    end = clock();
    printf("\nElements:%d", test4.elements);
    printf("\nTotal:%d First:%d Last:%d   ", test4.vacantTotal, test4.vacantFirst, test4.vacantLast);
    printf("\nSparse remove 1by1 %d of %d\n    Clocks:%ld  Secs:%f\n", PART, TESTSIZE, end-start, (float)(end - start) / CLOCKS_PER_SEC);  
    daClear(&test4);


/* Remove test 3 */
    // da test3;
    // daInit(&test3, TESTSIZE, 1.5);
    // daCreate(&test3, v, COUNT_OF(v));
    // printf("\nElements:%d", test3.elements);
    // start = clock();
    // daSparseRemove(&test3, 0, TESTSIZE/2-1);
    // daCompact(&test3);
    // end = clock();
    // printf("\nElements:%d", test3.elements);
    // printf("\nTotal:%d First:%d Last:%d   ", test3.vacantTotal, test3.vacantFirst, test3.vacantLast);
    // printf("\nSparse remove + compact %d of %d\n    Clocks:%ld  Secs:%f\n", TESTSIZE/2, TESTSIZE, end-start, (float)(end - start) / CLOCKS_PER_SEC);  
    // daClear(&test3);

/* Add test 1 */
    // printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    // printf("Dump: ");
    // for (i = 0; i < test.elements; i++){
    //     printf("%f ", *(test.p+i));
    // }
    // printf("\n");


    return 0;
}

