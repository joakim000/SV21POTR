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


// #define TESTSIZE 50000000
// // #define     REPS 45000000
// #define     REPS 100
// #define       BY      10


// Performance testing
#define TESTSIZE   5000000
#define REPS_RM      10000
#define REPS_GET   40000000
#define REPS_INS      100
#define REPS_ADD      100
#define JUMP           10
#define BY              1
#define STARTINDEX      0

// int REPS_RM     = 100;
// int REPS_GET    = 100;
// int REPS_INS    = 100;
// int REPS_ADD    = 100;
// int JUMP        = 10;
// int BY          = 10;

// Feature testing
// #define TESTSIZE 100
// int REPS        = 10;
// int JUMP        = 1;
// int BY          = 1;

int i;
da test;
DA_TYPE v[TESTSIZE];
clock_t start; clock_t end; float stopwatch;
time_t t;
float totalTime;
int startIndex;
unsigned long randRm[REPS_RM];
unsigned long randGet[REPS_GET];

void genRand() {
   int j;
   long ceil = TESTSIZE;
   for (j = 0; j < COUNT_OF(randRm); j++) {
        randRm[j] = ((rand() << 16) | rand()) % ceil;
        ceil -= BY;
    }
    ceil = TESTSIZE-REPS_RM*BY;
    for (j = 0; j < COUNT_OF(randGet); j++) {
        randGet[j] = ((rand() << 16) | rand()) % ceil;
    }
    
}

void init() {  
    int i;
    totalTime = 0;
    srand((unsigned) time(&t));   
    for (i = 0; i < TESTSIZE; i++) { v[i] = i; }
    daInit(&test, TESTSIZE, 1.5);
    if(daCreate(&test, v, COUNT_OF(v)) != 0) printf("Create error\n");
    printf("\nElements before:%d", test.elements);
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

void cleanup() {
    printf("\n                                      Combined time: %4.3f\n\n", totalTime);
    daClear(&test);
}


void extraInfo() {
    // int i;
    printf("\nVacant: ");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
    printf("\n");
    for (i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");
}

// unsigned long long r(long ceil) {
//     return ((rand() << 16) | rand()) % ceil;
// }



/* Combined test 1 Random */
void combined1_rnd() {
    init();
    start = clock();
    for (i = 0; i < REPS_RM; i++) {
        daRemove(&test, randRm[i], randRm[i]+BY-1);
    }
    end = clock(); 
    rm_message("Random Remove");
    
    start = clock();
    for (i = 0; i < REPS_GET; i++) daGet(&test, randGet[i]);
    end = clock(); 
    get_message("Random Get");
    
    cleanup();
}

/* Combined test 2 Random */
void combined2_rnd() {
    init();
    start = clock();
    for (i = 0; i < REPS_RM; i++) {
        daSparseRemove(&test, randRm[i], randRm[i]+BY-1);
    }
    end = clock();
    rm_message("Random Sparse remove");
    
    // start = clock();
    // for (i = 0; i < REPS_GET; i++) daSparseGet(&test, randGet[i]);
    // end = clock(); 
    // get_message("Sparse get");

    start = clock();
    daCompact(&test);
    end = clock();    
    compact_message("Compact");

    start = clock();
    for (i = 0; i < REPS_GET; i++) daSparseGet(&test, randGet[i]);
    end = clock(); 
    get_message("Random Sparse get");


    cleanup();
}

void combined1() {
    init();
    startIndex = STARTINDEX;
    start = clock();
    for (i = 0; i < REPS_RM; i++) {
        daRemove(&test, startIndex, startIndex+BY-1);
        startIndex += JUMP;
    }
    end = clock(); 
    rm_message("Remove");
    
    start = clock();
    for (i = 0; i < REPS_GET; i++) daGet(&test, i);
    end = clock(); 
    get_message("Get");
    
    cleanup();
}



void combined2() {
    init();
    startIndex = STARTINDEX;
    start = clock();
    for (i = 0; i < REPS_RM; i++) {
        daSparseRemove(&test, startIndex, startIndex+BY-1);
        startIndex += JUMP;
    }
    end = clock();
    rm_message("Sparse remove");
    
    // start = clock();
    // for (i = 0; i < REPS_GET; i++) daSparseGet(&test, i);
    // end = clock(); 
    // get_message("Sparse get");

    start = clock();
    daCompact(&test);
    end = clock();    
    compact_message("Compact");

    start = clock();
    for (i = 0; i < REPS_GET; i++) daSparseGet(&test, i);
    end = clock(); 
    get_message("Sparse get");

    cleanup();
}

void unit_buildLookup() {
    int startstep = 0;
    int steps = 50;
    bool failed = false;
    
    init();

    // start = clock();
    // buildLookup(&test);
    // end = clock();
    // get_message("Build lookup: ");

    start = clock();
    for (int i = 0; i < REPS_RM; i++) {
        daSparseRemove(&test, randRm[i], randRm[i]+BY-1);
    }
    end = clock();
    rm_message("Sparse remove");

    // for (i = 0; i < test.slots; i++){
    //     printf("%d ", *(test.vacant + i));
    // }
    // printf("\n");
    
    // start = clock();
    // buildLookup(&test);
    // end = clock();
    // get_message("Build lookup: ");

    // start = clock();
    // buildLookupDebug(&test);
    // end = clock();
    // get_message("Build lookup debug: ");

    printf("\nVacs:\n");
    for (int i = startstep; i < startstep + steps; i++){
    // for (int i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d daVacs:%d lookup:%u lookupDebug:%u\n", 
        daSparseGet(&test, i), i, i+daVacs(&test, i), daVacs(&test, i), test.lookup[i], test.lookupDebug[i] );
    }
    printf("\n");

}

void unit_Compact() {
    init();
    start = clock();
    for (int i = 0; i < REPS_RM; i++) {
        daSparseRemove(&test, randRm[i], randRm[i]+BY-1);
    }
    // daSparseRemove(&test, 1, 3);
    // daSparseRemove(&test, 7, 7);
    // daSparseRemove(&test, 9, 9);
    // daSparseRemove(&test, 12, 13);

    end = clock();
    rm_message("Sparse remove");
    for (i = 0; i < test.slots; i++){
        printf("%d ", *(test.vacant + i));
    }
    printf("\n");
    
    for (int i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");


    start = clock();
    daCompact(&test);
    end = clock();    
    compact_message("Compact");
    for (i = 0; i < test.slots; i++) {
        printf("%d ", *(test.vacant + i));
    }
    printf("\n");
    
    for (int i = 0; i < test.elements; i++){
        printf("%4.3f i:%d slot:%d\n", daSparseGet(&test, i), i, i+daVacs(&test, i));
    }
    printf("\n");

}

void unit_Get() {
    init();
    printf("\n");
    int start = 0;
    int steps = 1000000;
    bool failed = false;

    // for (int i = start; i < start+steps; i++) {
    //     printf("%4.3f ", v[i]);
    // }
    // printf("\n");
    
    // for (int i = start; i < start+steps; i++) {
    //     printf("%4.3f ", daGet(&test, i));
    // }

    for (int i = start; i < start+steps; i++) {
        if (v[i] != daGet(&test, i)) {
            printf("\nError on step %d, %f != %f", i, v[i], daGet(&test, i));
            failed = true;
        }
    }
    if (!failed) printf("\ndaGet passed.\n");

}

void unit_SparseGet() {
    init();
    printf("\n");
    int start = 0;
    int steps = 1000000;
    bool failed = false;

    // for (int i = start; i < start+steps; i++) {
    //     printf("%4.3f ", v[i]);
    // }
    // printf("\n");
    
    // for (int i = start; i < start+steps; i++) {
    //     printf("%4.3f ", daGet(&test, i));
    // }

    for (int i = start; i < start+steps; i++) {
        if (v[i] != daSparseGet(&test, i)) {
            printf("\nError on step %d, %f != %f", i, v[i], daSparseGet(&test, i));
            failed = true;
        }
    }
    if (!failed) printf("\ndaSparseGet passed with no vacants.\n");


    daSparseRemove(&test, 2, 3); //Expected: index 1 = 1.0, index 2 = 4.0
    printf("\nExpected: index 1 = 1.0, index 2 = 4.0. Index 1:%f   index 2:%f\n", daSparseGet(&test, 1), daSparseGet(&test, 2));
    printf("\nNonsparse get to compare. Index 1:%f   index 2:%f\n", daGet(&test, 1), daGet(&test, 2));

}

void main(){
    // int i;
    
// Timestamp
    time_t t = time(NULL); struct tm *tm = localtime(&t); char s[64]; assert(strftime(s, sizeof(s), "%c", tm)); printf("Test started %s\n", s);  

    genRand();

    combined1_rnd();
    combined2_rnd();
    
    // unit_Compact();
    // unit_buildLookup();

    // combined1();
    // combined2();

    // unit_Get();
    // unit_SparseGet();

/* Remove test 1 */
    // init();
    // start = clock();
    // daRemove(&test, 0, REPS);
    // end = clock(); 
    // cleanup();

/* Remove test 2 */
    // init();
    // start = clock();
    // daSparseRemove(&test, 0, REPS);
    // end = clock(); 
    // cleanup();

// /* Remove test 3 */
//     init();
//     startIndex = 0;
//     start = clock();
//     for (i = 0; i < REPS; i++)
//         daRemove(&test, startIndex, BY);
//         startIndex += JUMP;
//     end = clock(); 
//     // extraInfo();
//     message("Remove");
//     cleanup();

// /* Remove test 4 */
//     init();
//     startIndex = 0;
//     start = clock();
//     for (i = 0; i < REPS; i++)
//         daSparseRemove(&test, startIndex, BY);
//         startIndex += JUMP;
//     end = clock();
//     // extraInfo();
//     message("Sparse remove");
    
//     start = clock();
//     daCompact(&test);
//     end = clock();    
//     message("Compact");
    
//     cleanup();

/* Get test 1 */
    // init();
    // daRemove(&test, 100, 200);
    // daRemove(&test, 40000000, 40001000);
    // start = clock();
    // for (i = 0; i < REPS; i+=1)
        // daGet(&test, i);
        // daGet(&test, rand() % TESTSIZE);
    // end = clock();
    // extraInfo();
    // cleanup("Get");

/* Get test 2 */
    // init();
    // daSparseRemove(&test, 100, 200);
    // daSparseRemove(&test, 40000000, 40001000);
    // start = clock();
    // for (i = 0; i < REPS; i+=1)
        // daSparseGet(&test, i);
        // daSparseGet(&test, rand() % TESTSIZE);
    // end = clock();
    // extraInfo();
    // cleanup("Sparse get (not sparse)");

/* Add test 1 */
    // printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    // printf("Dump: ");
    // for (i = 0; i < test.elements; i++){
    //     printf("%f ", *(test.p+i));
    // }
    // printf("\n");




}
