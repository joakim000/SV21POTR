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
  
*/      

typedef enum datatypes { 
_char,
_int,
_long,
_float,
_double,
_bool
} datatype; 

typedef struct DynArr {
   void *p;

   char *a_char; 
   int *a_int;
   long *a_long;
   float *a_float;
   double *a_double;
   bool *a_bool;

   int elements;
   int slots;
   int reserve;
   datatype type;
   unsigned long long typesize;
} da;

// Init array
// Args: da* a, int reserve, int datatype
// Return: exit code
int daInit(da* a, int reserve, datatype type);  

// Load starting values
// Args: da* a, datatype values[], int len
// Return: exit code
int daCreate(da* a, double values[], int len);

// Insert or add (index -1)
// Args: da* a, int index, datatype value
// Return: exit code  
int daAdd(da* a, int index, void *value);  
//int daAdd(da* a,...);  

// Remove element or range
// Args: da* a, int startIndex, int endIndex
// Return: exit code
int daCompactRemove(da* a, int startIndex, int endIndex);

// Get value
// Args: da* a, int index
// Return: union
// Args: da* a, int index, datatype &target
// Return: exit code
int daGet(da* a, int index, void *out);

// Set value
// Args: da* a, int index, datatype value 
// Return: exit code
int daSet(da* a, int index, double value);

// Get range 
// Args: da* a, int start, int end 
// Return: union?
double* daGetRange(da* a, int start, int end);

// Compact allocation, set reserve
// Args: da* a, int reserve
int daCompact(da* a, int reserve);

// Return element count
// Args: da* a
int daCount(da* a);

// Return allocated slots
// Args: da* a 
int daAlloc(da* a);

// Quick remove - amortized O(1)
// Args: da* a, int index
int daRemove(da* a, int index);

// Quick remove - amortized O(1)
// Args: da* a, int index
int daRemove(da* a, int index);

int daRealloc(da* a, int requested);



int daInit(da* a, int reserve, datatype type) {
    //free(a->p);
    switch (type){
        case _char:
            a->a_char = calloc(reserve, sizeof(char)); break;
        case _int:
            a->a_int = calloc(reserve, sizeof(int)); break;
        case _long:
            a->a_long = calloc(reserve, sizeof(long)); break;
        case _float:
            a->a_float = calloc(reserve, sizeof(float)); break;
        case _double:
            a->typesize = sizeof(double);
            // a->p = (double*)a->p;
            a->a_double = calloc(reserve, a->typesize); break;
        case _bool:
            a->a_bool = calloc(reserve, sizeof(bool)); break;
        default:
            printf("\nUnknown datatype.\n");
            //fprintf(stderr, "\nUnknown datatype.\n");
            return -1;
    }
    if(a->p == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    a->slots = reserve;
    a->elements = 0;
    a->reserve = reserve;
    a->type = type;
    return 0;
}

/* int daCreate(da* a, double values[], int len) {   
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
} */

int daGet(da* a, int index, void *out) {
     double* dp;
     switch (a->type){
        case _double:
            //a->p = (double*)a->p;
            //double *r = *(double*)(a->p + index); 
            //a->p = (double*)a->dbl;
            // a->p = (double*)(a->p + index);
            out = (a->a_double + index);
            return 0;
        default:
            printf("\nUnknown datatype.\n");
            //fprintf(stderr, "\nUnknown datatype.\n");
            return -1;
     }
    //return *(a->p + index);
}

int daRealloc(da* a, int requested) {
    switch (a->type){
        case _double:
           a->a_double = realloc(a->a_double, requested + a->reserve * a->typesize); 
            if(a->p == NULL ) {
                 fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
            }
            break;
    }
    a->slots = requested + a->reserve;
    printf("Realloc done, slots: %d\n", a->slots);
    return 0;
}


//int daAdd(da* a,...) {
int daAdd(da* a, int index, void *value) {    
    
    if (a->elements == a->slots) {
        //Needs realloc
        daRealloc(a, a->slots + 1);
        // a->p = realloc(a->p, a->slots + 1 + a->reserve * a->typesize); 
        // if(a->p == NULL ) {
        //     fprintf(stderr, "Unable to allocate memory.\n");
        //     return -1;
        // }
        // a->slots = a->slots + 1 + a->reserve;
        // printf("Realloc done, slots: %d\n", a->slots);
    }
    if (index > a->elements - 1 || index < -1) {
        // Illegal insert
        return -1;
    }
    if (index == -1) { //Add to end
        switch (a->type) {
            case _double:
                *(a->a_double + a->elements) = (double)value;
                break;

        }
    } else {
        int i;
        switch (a->type) {
            case _double:
                for (i = a->elements - 1; i >= index; i-- ) {
                    *(a->a_double + i + 1) = *(a->a_double + i);
                }
                *(a->a_double + index) = *(double*)value;
                break;
        }
        
        
    }
    a->elements++;
    return 0;
}  


/* int daCompactRemove(da* a, int startIndex, int endIndex) {
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
} */

int daCount(da* a) {
    return a->elements;
}

int daAlloc(da* a){
    return a->slots;
}


int daRemove(da* a, int index){}


