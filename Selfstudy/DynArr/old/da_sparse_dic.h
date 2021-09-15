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
   double *p;
   int elements;
   int slots;
   float growthFactor;

   //bool *vacant;
   int sparseTotal;
   int sparseFirst;
   int sparseLast;
   int* sparse;


} da;

// Init array
// Args: da* a, int reserve, int datatype
// Return: exit code
int daInit(da* a, int initSlots, float growthFactor);  //

// Load starting values
// Args: da* a, datatype values[], int len
// Return: exit code
int daCreate(da* a, double values[], int len);

// Insert or add (index -1)
// Args: da* a, int index, double value
// Return: exit code  
int daAdd(da* a, int index, double value);  

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
int daSparseRemove(da* a, int startIndex, int endIndex);





int daInit(da* a, int initSlots, float growthFactor) {
    //free(a->p);
    
    // Init vars for sparse functionality
    a->sparseTotal = 0;
    //a->sparseFirst;
    //a->sparseLast;
    a->sparse = calloc(initSlots, sizeof(int)); //För många!
    

    a->p = calloc(initSlots, sizeof(double));
    if(a->p == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    a->slots = initSlots;
    a->elements = 0;
    a->growthFactor = growthFactor;
    return 0;
}

int daCreate(da* a, double values[], int len) {   
    printf("Create - slots:%d len:%d\n", a->slots, len);

    if (len > a->slots) {
        //Needs realloc
        int newSize = (int)((a->slots + len) * a->growthFactor);
        a->p = realloc(a->p, newSize * sizeof(double)); 
        if(a->p == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        }
        a->slots = newSize;

        /* a->vacant = realloc(a->vacant, a->slots * sizeof(bool)); 
        if(a->vacant == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        } */
        
        printf("Realloc done, slots: %d\n", a->slots);
    } 
    
    int i;
    for (i = 0; i < len; i++) {
        *(a->p+i) = values[i];
        //*(a->vacant+i) = false;
    }
    a->elements = len;
    
    return 0;
}

double daGet(da* a, int index) {
    return *(a->p + index);
}

int daAdd(da* a, int index, double value) {  //index -1 == end
    int i;
    if (a->elements == a->slots) {
        //Needs realloc
        int newSize = (int)(a->slots * a->growthFactor);
        a->p = realloc(a->p, newSize * sizeof(double)); 
        if(a->p == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        }
        a->slots = newSize;
        
        /* a->vacant = realloc(a->vacant, a->slots * sizeof(bool)); 
        if(a->vacant == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        } */
        printf("Realloc done, slots: %d\n", a->slots);
    }
    if (index == -1) { //Add to end
        *(a->p + a->elements) = value;
        // Vi har nu ett fler element
        a->elements++;
    } else if (index > a->elements - 1 || index < 0) {
        // Illegal insert
        return -1;
    } else {
        // Insert
        // Flytta alla element ett steg upp, start vid index
        for (i = a->elements - 1; i >= index; i-- ) {
            *(a->p + i + 1) = *(a->p + i);
        }
        // Sätt in värdet på angivet index        
        *(a->p + index) = value;
        // Vi har nu ett fler element
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


int daSparseRemove(da* a, int startIndex, int endIndex){
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


