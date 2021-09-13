#include <assert.h>
#include <ctype.h>
#include <errno.h>
//#include <float.h>
//#include <limits.h>
//#include <locale.h>
#include <math.h>
//#include <setjmp.h>
//#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

// #define RND_CEIL 10000

int i;
//  int j;
time_t t;
unsigned long long tot = 0;

unsigned long long rndCeil = 100;
unsigned long long avr[10]; 
void main(){
    srand((unsigned) time(&t));  
    
    printf("Randman: %llu\n", RAND_MAX);
    int a[100]; 
    for (i = 0; i < COUNT_OF(avr); i++) {

        tot = 0;
        for (int j = 0; j < COUNT_OF(a); j++) {
            // a[j] = ((rand() << 16) | rand()) % rndCeil;
            a[j] = ((rand() << 16) | rand()) ;
            tot += a[j];
        }


        // for (i = 0; i < COUNT_OF(a); i++) {
        //     printf("%d ", a[i]);
        // }
        avr[i] =  tot/COUNT_OF(a);   
        // printf("\nCeil: %llu   Avr:%llu ", rndCeil, avr[i]);
        printf("\nAvr:%llu ", avr[i]);
        // rndCeil *= 10;
        rndCeil = rndCeil * 10;

    }

    printf("\n");
    

}

