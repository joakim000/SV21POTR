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
    int i;

    int v[] = {0, 1, 2, 3, 4};
    int *p = v;
    p = calloc(10, sizeof(int));

    for (i = 0; i < COUNT_OF(v); i++) 
        printf("%d ", v[i]);
    printf("\n");


    printf("%x", p);

    printf("\n");

    printf("%d", *(p+10) );

    printf("\n");

    for (i = 0; i < COUNT_OF(v); i++) 
        printf("%d ", *p+i);
    printf("\n");


     for (i = 0; i < 20; i++) 
        printf("%d ", *(p+i));
    printf("\n");


    for (i = 0; i < 15; i++) 
        printf("%d ", *p++);
    printf("\n");

    printf("%d ", *(p-3));
 
    return 0;
}

