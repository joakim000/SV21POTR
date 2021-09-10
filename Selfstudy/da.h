#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

#define CHAR 1
#define INT 2
#define LONG 3
#define FLOAT 4
#define DOUBLE 5
#define BOOL 6

/* TODOs
// return # of elements or -1 (error)
// union för att tillåta olika typer?
// nytt försök med typedef
   
*/      

typedef struct DynArr {
   double *p;

   char *c; 
   int *d;
   long *l;
   float *f;
   double *dbl;
   bool *b;

   int elements;
   int slots;
   int reserve;
   int type;
} da;

// Init array
// Args: da* a, int reserve, int datatype
// Return: exit code
int daInit(da* a, int reserve, int datatype);  

// Load starting values
// Args: da* a, datatype values[], int len
// Return: exit code
int daCreate(da* a, double values[], int len);

// Insert or add (index -1)
// Args: da* a, int index, datatype value
// Return: exit code  
int daAdd(da* a,...);  

// Remove element or range
// Args: da* a, int startIndex, int endIndex
// Return: exit code
int daCompactRemove(da* a, int startIndex, int endIndex);

// Get value
// Args: da* a, int index
// Return: union
// Args: da* a, int index, datatype &target
// Return: exit code
double daGet(da* a, int index);

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





int daInit(da* a, int reserve, int datatype) {
    free(a->p);
    switch (datatype){
        case CHAR:
            a->c = calloc(reserve, sizeof(char)); break;
        case INT:
            a->d = calloc(reserve, sizeof(int)); break;
        case LONG:
            a->l = calloc(reserve, sizeof(long)); break;
        case FLOAT:
            a->f = calloc(reserve, sizeof(float)); break;
        case DOUBLE:
            a->dbl = calloc(reserve, sizeof(double)); break;
        case BOOL:
            a->b = calloc(reserve, sizeof(bool)); break;
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
    a->type = datatype;
    return 0;
}

int daCreate(da* a, double values[], int len) {   
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

double daGet(da* a, int index) {
    return *(a->p + index);
}

int daAdd(da* a,...) {  //index -1 == end
    int index;
    double value;

    int x;
    
    typeof (*x) y;

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
        int i;
        for (i = a->elements - 1; i >= index; i-- ) {
            *(a->p + i + 1) = *(a->p + i);
        }
        *(a->p + index) = value;
        a->elements++;
    }
    return 0;
}  


int daCompactRemove(da* a, int startIndex, int endIndex) {
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

int daCount(da* a) {
    return a->elements;
}

int daAlloc(da* a){
    return a->slots;
}


int daRemove(da* a, int index){}


