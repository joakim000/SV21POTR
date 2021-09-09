#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro


// return # of elements or -1 (error)
// union för att tillåta olika typer?

struct DynArr {
   double *p;
   int elements;
   int slots;
};

int daInit(struct DynArr* a, int slots);
int daCreate(struct DynArr* a, double values[], int len);
//int daAdd(struct DynArr* a, int index, double value);  //index -1 == end
//int daRemove(struct DynArr* a, int index);
//int daSnip(struct DynArr* a, int startIndex, int endIndex);
double daGet(struct DynArr* a, int index);
double* daGetRange(int start, int end);
int daCompact(int reserve);


int main(){
    struct DynArr test;
    printf("Init exit:%d\n", daInit(&test, 10));

    double v[] = {0, 1, 2, 3, 4};
    
    printf("\nCreate exit:%d\n", daCreate(&test, v, COUNT_OF(v)));
    
    printf("%f\n", daGet(&test, 2));

    return 0;
}


int daInit(struct DynArr* a, int slots) {
    //free(a.p);
    a->p = calloc(slots, sizeof(double)); // Add test successful alloc
    a->slots = slots;
    a->elements = 0;
    return 0;
}

int daCreate(struct DynArr* a, double values[], int len) {   
    printf("Create - slots:%d len:%d", a->slots, len);

    if (len > a->slots) {
        //Needs realloc
        return -1;
    }
    else {
        int i;
        for (i = 0; i < len; i++) {
            *(a->p+i) = values[i];
        }
        a->elements = len;
    }
    return 0;
}

double daGet(struct DynArr* a, int index) {
    return *(a->p + index);
}

//int daAdd(struct DynArr* a, int index, double value){}  //index -1 == end
//int daRemove(struct DynArr* a, int index){}
//int daSnip(struct DynArr* a, int startIndex, int endIndex){}

