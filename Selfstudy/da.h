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

   bool *vacant;  // Use bit field :1 if possible
   int vacantTotal;
   int vacantFirst;
   int vacantLast;

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
int daRemove(da* a, int startIndex, int endIndex);
int daSparseRemove(da* a, int startIndex, int endIndex);

// Get value
// Args: da* a, int index
// Return: union
// Args: da* a, int index, datatype &target
// Return: exit code
double daGet(da* a, int index);
double daSparseGet(da* a, int index);

// Set value - NOT IMPLEMENTED
// Args: da* a, int index, datatype value 
// Return: exit code
int daSet(da* a, int index, double value);

// Get range - NOT IMPLEMENTED
// Args: da* a, int start, int end 
// Return: union?
double* daGetRange(da* a, int start, int end);

// Compact allocation, set reserve
// Args: da* a, int reserve
int daCompact(da* a);
//int daCompact(da* a, int reserve);

// Return element count
// Args: da* a
int daCount(da* a);

// Return allocated slots
// Args: da* a 
int daAlloc(da* a);

/* Helpers */
int daVacs(da* a, int index);   // Vacancy counter
int daRealloc(da* a);           // Realloc routine

/* Declarations end */


int daInit(da* a, int initSlots, float growthFactor) {
    //free(a->p);

    // Init vars for sparse functionality
    a->vacantTotal = 0;
    a->vacant = calloc(initSlots, sizeof(bool)); 
    if(a->vacant == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    int i;
    for (i = 0; i < initSlots; i++)
        *(a->vacant + i) = false;
    // End sparse vars
    
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

        a->vacant = realloc(a->vacant, a->slots * sizeof(bool)); 
        if(a->vacant == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        }         
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
        
        a->vacant = realloc(a->vacant, a->slots * sizeof(bool)); 
        if(a->vacant == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        } 
        printf("Realloc done, slots: %d\n", a->slots);
    }
    if (index == -1) { //Add to end
        *(a->p + a->elements) = value;
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


int daRemove(da* a, int startIndex, int endIndex) {
    if (startIndex >= a->elements ||
        endIndex >= a->elements ||
        startIndex > endIndex ) {
        //Illegal remove
        return -1;
    }

    // Move elements blocksize steps in negative direction
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

/* Functions for sparse functionality */

int daCompact(da* a){
    int i;
    bool findVac = true;
    int nextVacToFill; 

    for (i = 0; i < a->elements + a->vacantTotal; i++) {
        if (*(a->vacant + i)) {
            if (findVac) {
            // Looking for vacancy and found one    
                nextVacToFill = i;
                findVac = false;
                continue;
            } else {
            // Looking for data but found vacancy    
                continue;
            } 
        } else {
            if (findVac) {
            // Looking for vacancy but found data   
                continue;
            } else {
            // Looking for data and found some
                *(a->p + nextVacToFill) = *(a->p + i);
                // This slot now vacant
                *(a->vacant + i) = true;
                // Look for next vacancy
                findVac = true;
                // Start looking after previous vacancy
                i = nextVacToFill; 
            }
        }
    }
    // Reset vacancy data
    for (i = 0; i < a->slots; i++)
        *(a->vacant + i) = false;
    a->vacantTotal = 0;

    return 0;
}

int daVacs(da* a, int index) {
    //Count vacancies
    int v = 0;
    int i;
    if (a->vacantTotal == 0 || index < a->vacantFirst)
        v == 0;
    else {
        //for (i = a->vacantFirst; i <= a->vacantLast && i <= index && v < a->vacantTotal; i++){
        // Mindre optimerad, utan first/last
        for (i = 0; i <= index + v && v < a->vacantTotal; i++){    
            if (*(a->vacant + i))
                v++;
        }
    }
    //printf("vac counter: return v:%d", v);
    return v;
}

double daSparseGet(da* a, int index) {
    int v = daVacs(a, index);

    //printf("daGet: vacs:%d  getVacs:%d\n", w, v);
    printf("daGet:  getVacs:%d  ", v);

    return *(a->p + index + v);
}

int daSparseRemove(da* a, int startIndex, int endIndex){
     if (startIndex >= a->elements ||
        endIndex >= a->elements ||
        startIndex > endIndex ) {
        //Illegal remove
        return -1;
    }

    const int blockSize = endIndex - startIndex + 1;
    int i;
    //for (i = startIndex + blockSize; i < a->elements; i++ ) {
    //        *(a->p + i - blockSize) = *(a->p + i);
    //    }
    
    // Find vac-counts for block 
    // int foundVacs = kompensation under räkningen?
    // Optimering: först kolla för 0 på vacs för endIndex 
    int vacEnd = daVacs(a, endIndex);
    
    int vacStart = daVacs(a, startIndex);
    int blockVacs[1000]; // FIGURE OUT C99 VLAs ///
    blockVacs[0] = vacStart;
   
    for (i = 1; i < blockSize; i++){
        if (*(a->vacant + i + startIndex))
            blockVacs[i] = ++vacStart;
        else
            blockVacs[i] = vacStart;
    }

    printf("\nblockVacs:\n");
    for (i = 0; i < blockSize; i++)
        printf("%d ", blockVacs[i]);
    printf("\n");

    for (i = 1; i < blockSize; i++){
            blockVacs[i] = daVacs(a, startIndex+i);

    }
    
    
    for (i = 0; i < blockSize; i++)
        printf("%d ", blockVacs[i]);
    printf("\n");
    
    
    // Save uncorrected startIndex for updating vacantFirst
    int uncorrStartIndex = startIndex;

    // Set vacant on vac-corrected indices
    startIndex += blockVacs[0];
    endIndex += blockVacs[blockSize - 1];
    int blockIter = 0;


    // for (i = startIndex; i <= endIndex; i=i+1+blockVacs[blockIter]) {
    for (i = startIndex; i <= endIndex; i++) {
        *(a->vacant + i + blockVacs[blockIter]) = true;
        blockIter++;
    }
    
    a->elements -= blockSize;
    if (a->vacantTotal == 0){
        a->vacantFirst = startIndex;
        a->vacantLast = endIndex;
    } else {
        // printf("\nstartIndex:%d vacantFirst:%d\n", startIndex, a->vacantFirst);  
        if (uncorrStartIndex < a->vacantFirst)
            a->vacantFirst = uncorrStartIndex;
        // printf("\nstartIndex:%d vacantFirst:%d\n", startIndex, a->vacantFirst);  
        // printf("\nendIndex:%d vacantLast:%d\n", endIndex, a->vacantLast);    
        if (endIndex > a->vacantLast)
            a->vacantLast = endIndex;
        // printf("\nendIndex:%d vacantLast:%d\n", endIndex, a->vacantLast);  
    }
    a->vacantTotal += blockSize;

    return 0;    
}


