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


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

int main(){

    double v[] = {0.123, 1, 2, 3, 4};

    double* ptr = v;

        printf("v: %x\n", v);
        printf("ptr: %x\n", ptr);
        printf("*v: %f\n", *v);
        printf("*ptr: %f\n", *ptr);
        printf("\n");

        printf("sizeof(v): %d\n", sizeof(v));
        printf("sizeof(ptr): %d\n", sizeof(ptr));
        printf("sizeof(*v): %d\n", sizeof(*v));
        printf("sizeof(*ptr): %d\n", sizeof(*ptr));
        printf("\n");
        
        printf("count(v): %d\n", COUNT_OF(v));
        printf("count(ptr): %d\n", COUNT_OF(ptr));

        

        
      
       


    return 0;
}

