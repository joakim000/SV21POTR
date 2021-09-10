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

//Args: double, double, int, int
int vatest(double d1,...){
    double dbl1;
    double dbl2;
    int i1;
    int i2;
    
    va_list args;
    va_start(args, 3);

    dbl1 = d1;
    dbl2 = va_arg(args, double);
    i1 = va_arg(args, int);
    i2 = va_arg(args, int);

    printf("%f %f %d %d\n", dbl1, dbl2, i1, i2);

    va_end(args);
}


int main(){

    vatest(1.1, 2.1, 3, 4);


    return 0;
}

