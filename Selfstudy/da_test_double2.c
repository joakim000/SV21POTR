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
#include "da_def.h"


#define TESTSIZE 1800000
#define     REPS  1491000
#define       BY      1

int i;
da test;
DA_TYPE v[TESTSIZE];
clock_t start;
clock_t end;
    
void init() {  
    int i;
    for (i = 0; i < TESTSIZE; i++) v[i] = i;
    daInit(&test, TESTSIZE, 1.5);
    if(daCreate(&test, v, COUNT_OF(v)) != 0) printf("Create error\n");
    printf("\nElements before:%d", test.elements);
}

void cleanup(char s[]) {
    printf("\nElements after:%d", test.elements);
    //printf("\nTotal:%d First:%d Last:%d", test.vacantTotal, test.vacantFirst, test.vacantLast);
    printf("\n%s %d of %d (by %d)\n    Clocks:%ld  Secs:%4.3f\n", s, REPS*BY, TESTSIZE, BY, end-start, (float)(end - start) / CLOCKS_PER_SEC);    
    daClear(&test);
}

void extraInfo() {
    int i;
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

int main(){
    int i;
// Timestamp
    time_t t = time(NULL); struct tm *tm = localtime(&t); char s[64]; assert(strftime(s, sizeof(s), "%c", tm)); printf("Test started %s\n", s);  

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

/* Remove test 3 */
    // init();
    // start = clock();
    // for (i = 0; i < REPS; i+=1)
    //     daRemove(&test, 0, BY-1);
    // end = clock(); 
    // // extraInfo();
    // cleanup("Remove");

/* Remove test 4 */
    // init();
    // start = clock();
    // for (i = 0; i < REPS; i+=1)
    //     daSparseRemove(&test, 0, BY-1);
    // end = clock();
    // // extraInfo();
    // cleanup("Sparse remove");

/* Get test 1 */
    init();
    start = clock();
    for (i = 0; i < REPS; i+=1)
        daGet(&test, i);
    end = clock();
    // extraInfo();
    cleanup("Get");

   init();
    start = clock();
    for (i = 0; i < REPS; i+=1)
        daSparseGet(&test, i);
    end = clock();
    // extraInfo();
    cleanup("Sparse get (not sparse)");

/* Add test 1 */
    // printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    // printf("Dump: ");
    // for (i = 0; i < test.elements; i++){
    //     printf("%f ", *(test.p+i));
    // }
    // printf("\n");




    return 0;
}

