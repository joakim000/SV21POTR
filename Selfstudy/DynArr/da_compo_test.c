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

// Small scale tests
#define TESTSIZE      100
#define REPS_RM        10
#define REPS_GET       50
#define REPS_INS       10
#define REPS_ADD       10
#define JUMP           10
#define BY              1
#define STARTINDEX      0


// Define helpers implemented in da_test.h
void genRand();
void init();
void rm_message(char s[]);
void get_message(char s[]);
void compact_message(char s[]);
void cleanup();       
void extraInfo();

int i;
da test;
DA_TYPE v[TESTSIZE];
clock_t start; clock_t end; float stopwatch;
time_t t;
float totalTime;
int startIndex;
unsigned long randRm[REPS_RM];
unsigned long randGet[REPS_GET];


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
// Timestamp
    time_t t = time(NULL); struct tm *tm = localtime(&t); char s[64]; assert(strftime(s, sizeof(s), "%c", tm)); printf("Test started %s\n", s);  

    // genRand();

    // unit_Compact();
    // unit_buildLookup();
    // unit_Get();
    // unit_SparseGet();
}


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



#include "da_test.h"

