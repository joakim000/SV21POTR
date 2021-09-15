#include "devheaders.h"

#define DA_TYPE double
#include "da.h"


// Performance testing
#define TESTSIZE   120000
#define REPS_RM      20000
#define REPS_GET   40000
#define REPS_INS      10000
#define REPS_ADD      10000
#define JUMP           10
#define BY              1
#define STARTINDEX      0

// Define helpers implemented in da_test.h
void genRand();
void init();
void rm_message(char s[]);
void get_message(char s[]);
void compact_message(char s[]);
void add_message(char s[]);
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
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);
    
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
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);

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
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);
    
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
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);
    
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

void add() {
    init();
    // printf("Init successful\n");
    startIndex = 10;
    int jump = 0;

    daRealloc(&test, (int)TESTSIZE); 

    // start = clock();
    // for (int i = 0; i < REPS_ADD; i++) {
    //     daAdd(&test, -1, (DA_TYPE)i);
    // }
    // end = clock();
    // add_message("Add");
    // printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);

     start = clock();
    for (i = 0; i < REPS_RM; i++) {
        daSparseRemove(&test, startIndex, startIndex+BY-1);
        startIndex += jump;
    }
    end = clock();
    rm_message("Sparse remove");
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);

    
    start = clock();
    for (int i = 0; i < REPS_INS; i++) {
        daAdd(&test, startIndex, (DA_TYPE)i);
        startIndex += jump;
    }
    end = clock();
    add_message("Insert");
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);

    start = clock();
    for (int i = 0; i < REPS_INS; i++) {
        daAdd(&test, randRm[i], (DA_TYPE)i);
    }
    end = clock();
    add_message("Random Insert");
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);


    // start = clock();
    // daCompact(&test);
    // end = clock();    
    // compact_message("Compact");

    cleanup();
}



void main(){
    
// Timestamp
    time_t t = time(NULL); struct tm *tm = localtime(&t); char s[64]; assert(strftime(s, sizeof(s), "%c", tm)); printf("Test started %s\n", s);  

    genRand();

    add();

    //combined1_rnd();
    // combined2_rnd();
    
    // combined1();
    // combined2();

}

#include "da_test.c"