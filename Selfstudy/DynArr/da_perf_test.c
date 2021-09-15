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


// Performance testing
#define TESTSIZE   5000000
#define REPS_RM      10000
#define REPS_GET   40000000
#define REPS_INS      100
#define REPS_ADD      100
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


void main(){
    
// Timestamp
    time_t t = time(NULL); struct tm *tm = localtime(&t); char s[64]; assert(strftime(s, sizeof(s), "%c", tm)); printf("Test started %s\n", s);  

    genRand();

    combined1_rnd();
    combined2_rnd();
    
    // combined1();
    // combined2();

}

#include "da_test.c"