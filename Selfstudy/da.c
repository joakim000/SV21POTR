#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro


// return # of elements or -1 (error)
// union för att tillåta olika typer?

/*
typedef struct DynArr {
   double *p;
   int elements;
   int slots;
} da;
*/

struct DynArr {
   double *p;
   int elements;
   int slots;
};



/*
//int daInit(da* a, int slots);
//int daCreate(da* a, double values[]);
int daAdd(da a, int index, double value);  //index -1 == end
int daRemove(da a, int index);
int daSnip(da a, int startIndex, int endIndex);
double daGet(da a, int index);
*/

int daInit(struct DynArr* a, int slots);
int daCreate(struct DynArr* a, double values[], int len);
//int daAdd(da a, int index, double value);  //index -1 == end
//int daRemove(da a, int index);
//int daSnip(da a, int startIndex, int endIndex);
double daGet(struct DynArr* a, int index);


int main(){

    
   //da test;
   struct DynArr test;


    printf("Init:%d\n", daInit(&test, 10));
    printf("test.slots:%d\n", test.slots);

/*
     *test.p = 1.23;
    printf("%x\n", test.p);
    printf("%f\n", *test.p);
    
     *(test.p+1) = 2.23;
    printf("%x\n", test.p+1);
    printf("%f\n", *(test.p+1));
*/

    double v[] = {0, 1, 2, 3, 4};
    printf("len:%d\n", COUNT_OF(v));
    


    //daCreate(test, v);
    printf("\nCreate:%d\n", daCreate(&test, v, COUNT_OF(v)));
    
    printf("%f\n", daGet(&test, 2));

    


    return 0;
}

//int daInit(da* a, int slots) {
int daInit(struct DynArr* a, int slots) {
    //free(a.p);
    a->p = calloc(slots, sizeof(double)); // Add test
    a->slots = slots;
    a->elements = 0;

    printf("arg slots:%d\n", slots);
    printf("a.slots:%d\n", a->slots);

    return 0;
}



//int daCreate(da* a, double values[]) {
int daCreate(struct DynArr* a, double values[], int len) {   
    //int len = COUNT_OF(values);
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

//int daAdd(da a, int index, double value){}  //index -1 == end
//int daRemove(da a, int index){}
//int daSnip(da a, int startIndex, int endIndex){}

