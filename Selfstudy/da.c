#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

/* TODOs
// return # of elements or -1 (error)
// union för att tillåta olika typer?
// nytt försök med typedef
// va_list: Compact med/utan reserve
            Get/GetRange
            Remove/RemoveRange      
*/      

struct DynArr {
   double *p;
   int elements;
   int slots;
   int reserve;
};

int daInit(struct DynArr* a, int reserve);  //
int daCreate(struct DynArr* a, double values[], int len);
int daAdd(struct DynArr* a, int index, double value);  //index -1 == end
int daCompactRemove(struct DynArr* a, int index);
int daCompactRemoveRange(struct DynArr* a, int startIndex, int endIndex);
double daGet(struct DynArr* a, int index);
double* daGetRange(struct DynArr* a, int start, int end);
int daCompact(struct DynArr* a, int reserve);
int daCount(struct DynArr* a);
int daAlloc(struct DynArr* a);


//int daRemove(struct DynArr* a, int index);

int main(){
    struct DynArr test;
    printf("Init exit:%d\n", daInit(&test, 5));

    double v[] = {0, 1, 2, 3, 4, 5, 6};
    //double v[] = {0, 1, 2, 3};

    printf("\nCreate exit:%d\n", daCreate(&test, v, COUNT_OF(v)));
    
    //printf("%f\n", daGet(&test, 2));

    int i;
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
        //printf("%f ", daGet(&test, i));
    }
    printf("\n");

    printf("Add exit:%d\n", daAdd(&test, 2, 10.123));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");

    printf("Add exit:%d\n", daAdd(&test, -1, 11.123));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");

/*
    printf("CompactRemove exit:%d\n", daCompactRemove(&test, 2));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");
*/

    printf("CompactRemoveRange exit:%d\n", daCompactRemoveRange(&test, 2, 2));
    printf("Dump: ");
    for (i = 0; i < test.elements; i++){
        printf("%f ", *(test.p+i));
    }
    printf("\n");

    return 0;
}


int daInit(struct DynArr* a, int reserve) {
    free(a->p);
    a->p = calloc(reserve, sizeof(double));
    if(a->p == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    a->slots = reserve;
    a->elements = 0;
    a->reserve = reserve;
    return 0;
}

int daCreate(struct DynArr* a, double values[], int len) {   
    printf("Create - slots:%d len:%d\n", a->slots, len);

    if (len > a->slots) {
        //Needs realloc
        a->p = realloc(a->p, len + a->reserve * sizeof(double)); 
        if(a->p == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        }
        a->slots = len + a->reserve;
        printf("Realloc done, slots: %d\n", a->slots);
    } 
    
    int i;
    for (i = 0; i < len; i++) {
        *(a->p+i) = values[i];
    }
    a->elements = len;
    
    return 0;
}

double daGet(struct DynArr* a, int index) {
    return *(a->p + index);
}

int daAdd(struct DynArr* a, int index, double value) {  //index -1 == end
    int i;
    if (a->elements == a->slots) {
        //Needs realloc
        a->p = realloc(a->p, a->slots + 1 + a->reserve * sizeof(double)); 
        if(a->p == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        }
        a->slots = a->slots + 1 + a->reserve;
        printf("Realloc done, slots: %d\n", a->slots);
    }
    if (index == -1) { //Add to end
        *(a->p + a->elements) = value;
        a->elements++;
    } else if (index > a->elements - 1 || index < 0) {
        // Illegal insert
        return -1;
    } else {
        for (i = a->elements - 1; i >= index; i-- ) {
            *(a->p + i + 1) = *(a->p + i);
        }
        *(a->p + index) = value;
        a->elements++;
    }
    return 0;
}  

int daCompactRemove(struct DynArr* a, int index) {
    if (index >= a->elements){
        //Illegal remove
        return -1;
    }
    
    int i;
    for (i = index + 1; i < a->elements; i++ ) {
            *(a->p + i - 1) = *(a->p + i);
        }
    a->elements--;
    
    return 0;
}

int daCompactRemoveRange(struct DynArr* a, int startIndex, int endIndex) {
    if (startIndex >= a->elements ||
        endIndex >= a->elements ||
        startIndex > endIndex ) {
        //Illegal remove
        return -1;
    }

    int blockSize = endIndex - startIndex + 1;
    int i;
    for (i = startIndex + blockSize; i < a->elements; i++ ) {
            *(a->p + i - blockSize) = *(a->p + i);
        }
    a->elements -= blockSize;
    
    return 0;

}

int daCount(struct DynArr* a) {
    return a->elements;
}

int daAlloc(struct DynArr* a){
    return a->slots;
}


//int daRemove(struct DynArr* a, int index){}


