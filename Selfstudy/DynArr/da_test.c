/*
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>
// #include <windows.h>

#define DA_TYPE double

#include "da.h"

#ifndef TESTSIZE
#define TESTSIZE      100
#endif
#ifndef REPS_RM
#define REPS_RM        10
#endif
#ifndef REPS_GET
#define REPS_GET       50
#endif
#ifndef REPS_INS
#define REPS_INS       10
#endif
#ifndef REPS_ADD
#define REPS_ADD       10
#endif
#ifndef REPS_JUMP
#define JUMP           10
#endif
#ifndef BY
#define BY              1
#endif
#ifndef STARTINDEX
#define STARTINDEX      0
#endif


da test;
DA_TYPE v[TESTSIZE];
clock_t start; clock_t end; float stopwatch;
time_t t;
float totalTime;
int startIndex;
unsigned long randRm[REPS_RM];
unsigned long randGet[REPS_GET];

*/


void init() {  
    totalTime = 0;
    srand((unsigned) time(&t));   
    for (int i = 0; i < TESTSIZE; i++) { v[i] = i; }
    daInit(&test, TESTSIZE, 1.5);
    if(daCreate(&test, v, COUNT_OF(v)) != 0) printf("Create error\n");
    printf("\nElements before:%d", test.elements);
}

void cleanup() {
    printf("\n                                      Combined time: %4.3f\n\n", totalTime);
    daClear(&test);
}

void genRand() {
   int j;
   long ceil = TESTSIZE;
   int size;
   size = COUNT_OF(randRm) < TESTSIZE ? COUNT_OF(randRm) : TESTSIZE;
   for (int j = 0; j < size; j++) {
        randRm[j] = ((rand() << 16) | rand()) % ceil;
        ceil -= BY;
    }

    ceil = TESTSIZE-REPS_RM*BY;
    ceil = 50000;
    size = COUNT_OF(randGet) < TESTSIZE ? COUNT_OF(randGet) : TESTSIZE;
    for (j = 0; j < size; j++) {
        randGet[j] = ((rand() << 16) | rand()) % ceil;
    }

    ceil = TESTSIZE-REPS_RM*BY;
    ceil = 50000;
    size = COUNT_OF(randIns) < TESTSIZE ? COUNT_OF(randIns) : TESTSIZE;
    for (j = 0; j < size; j++) {
        randIns[j] = ((rand() << 16) | rand()) % ceil;
    }
    
}

void rm_message(char s[]) {
    printf("\n Elements after:%d", test.elements);
    printf("\nVacants    Total:%d First:%d Last:%d", test.vacantTotal, test.vacantFirst, test.vacantLast);
    // printf("\n"); for (i = 0; i < test.slots; i++) printf("%d ", *(test.vacant + i));
    stopwatch = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\n%s %d of %d (by %d)\n    Secs:%4.3f\n", s, REPS_RM*BY, TESTSIZE, BY, stopwatch);    
    totalTime += stopwatch;
}

void get_message(char s[]) {
    stopwatch = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\n%s %d of %d \n    Secs:%4.3f\n", s, REPS_GET, TESTSIZE-REPS_RM+BY-1, stopwatch);    
    totalTime += stopwatch;
}

void compact_message(char s[]) {
    printf("\nVacants    Total:%d First:%d Last:%d", test.vacantTotal, test.vacantFirst, test.vacantLast);
    // printf("\n"); for (i = 0; i < test.slots; i++) printf("%d ", *(test.vacant + i));
    stopwatch = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\n%s %d of %d\n    Secs:%4.3f\n", s, REPS_RM*BY, TESTSIZE, stopwatch);    
    totalTime += stopwatch;
}

void add_message(char s[]) {
    printf("\n Elements after:%d", test.elements);
    printf("\nVacants    Total:%d First:%d Last:%d", test.vacantTotal, test.vacantFirst, test.vacantLast);
    // printf("\n"); for (i = 0; i < test.slots; i++) printf("%d ", *(test.vacant + i));
    stopwatch = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\n%s %d of %d (Jump:%d)\n    Secs:%4.3f\n", s, REPS_ADD, TESTSIZE, 0, stopwatch);    
    totalTime += stopwatch;
}

void extraInfo() {
    printf("\nVacant: ");
    for (int i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
    printf("\n");
    for (int i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");
}

// unsigned long long r(long ceil) {
//     return ((rand() << 16) | rand()) % ceil;
// }


