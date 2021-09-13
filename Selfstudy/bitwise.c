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

void main(){
    int i;

    int x = 32;
    int y;
    for (i = 0; i < 33; i++){
        y = x | i;
        printf("32 | %d = %d\n", i, y);
    }
    
    int a[x];

   
}

