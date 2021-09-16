#include "devheaders.h"

#define DA_TYPE double
#include "da.h"


// Performance testing
#define TESTSIZE   75000
#define REPS_RM    25000
#define REPS_GET   20000
#define REPS_INS      25000
#define REPS_ADD      25000
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
unsigned long randIns[REPS_INS];


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
    startIndex = 40000;
    int jump = 1;

    daRealloc(&test, (int)TESTSIZE); 

    /* Add to end */
    // start = clock();
    // for (int i = 0; i < REPS_ADD; i++) {
    //     daAdd(&test, -1, (DA_TYPE)i);
    // }
    // end = clock();
    // add_message("Add");
    // printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);

    /* Nonsparse insert */
    // start = clock();
    // for (int i = 0; i < REPS_INS; i++) {
    //     daAdd(&test, startIndex, (DA_TYPE)i);
    //     startIndex += jump;
    // }
    // end = clock();
    // add_message("Nonsparse Linear Insert start@ 40k         ");
    // printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);
   
    
    /* Nonsparse random insert */
    // start = clock();
    // for (int i = 0; i < REPS_INS; i++) {
    //     daAdd(&test, randIns[i], (DA_TYPE)i);
    // }
    // end = clock();
    // add_message("Nonsparse Random Insert");
    // printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);
    

    /* Removal for sparse testing */
    start = clock();
    for (i = 0; i < REPS_RM; i++) {
        daSparseRemove(&test, randRm[i], randRm[i]+BY-1);
        // startIndex += jump;
    }
    end = clock();
    rm_message("Random Sparse remove");
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);
    

    /* Linear sparse insert */
    // startIndex = 10000;
    // printf("First insert at index:%d slot:%d", startIndex, startIndex+daVacs(&test, startIndex));
    // start = clock();
    // for (i = 0; i < REPS_INS; i++) {
    //     daAdd(&test, startIndex, (DA_TYPE)i);
    //     // daAdd(&test, randGet[i], (DA_TYPE)i);
    //     startIndex +=1;
    // }
    // end = clock();
    //   printf("               Last insert at index:%d slot:%d", startIndex, startIndex+daVacs(&test, startIndex));
    // add_message("Linear Sparse Insert on Random Sparse Remove, start @ 10000");
    // printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);

    // goto end;
    /* Random sparse insert */
    printf("First insert at index:%d slot:%d", randGet[0], randGet[0]+daVacs(&test, randGet[0]));
    start = clock();
    for (i = 0; i < REPS_INS; i++) {
        daAdd(&test, randGet[i], (DA_TYPE)i);
    }
    end = clock();
      printf("               Last insert at index:%d slot:%d", randGet[REPS_INS-1], randGet[REPS_INS-1]+daVacs(&test, randGet[REPS_INS-1]));
    add_message("Random Sparse Insert on Random Sparse Remove");
    printf("\nElements:%ld  Slots:%ld\n", test.elements, test.slots);


    // start = clock();
    // daCompact(&test);
    // end = clock();    
    // compact_message("Compact");

    end:
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