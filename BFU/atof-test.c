#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

int main(){

    char s[] = "1";
    char *sp = "1";

    printf("a %s\n", s);
    printf("b %f\n", atof(s));
    //printf("%f\n", atof(&s));
    //printf("d %s\n", sp);
    printf("f %f\n", atof(sp));
    //printf("g %f\n", atof(*sp));

    float f;
    sscanf(s, "%f", &f);
    printf("h %f\n", f);

    printf("i %f\n", strtof(s, NULL));

    return 0;
}
 
