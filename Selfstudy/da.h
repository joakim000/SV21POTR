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
int daRealloc(da* a, int extraSlots);           // Realloc routine

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
        int extraSlots = len - a->slots;
        daRealloc(a, extraSlots);
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

int daRealloc(da* a, int extraSlots){
    int newSize = (int)((a->slots * a->growthFactor) + extraSlots);
    a->p = realloc(a->p, newSize * sizeof(double)); 
    if(a->p == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    }
    int oldSize = a->slots;
    a->slots = newSize;
    
    a->vacant = realloc(a->vacant, a->slots * sizeof(bool)); 
    if(a->vacant == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    // Init added slots on vacant
    int i;
    for (i = oldSize; i < newSize; i++)
        *(a->vacant + i) = false;

    printf("Realloc function done, slots: %d\n", a->slots);
}


int daAdd(da* a, int index, double value) {  //index -1 == end
    int i;
    if (a->elements == a->slots) {
        //Needs realloc
        daRealloc(a, 0);
    }

    if (index == -1) { //Add to end
        *(a->p + a->elements + a->vacantTotal) = value;
        a->elements++;
    } else if (index > a->elements - 1 || index < 0) {
        // Illegal insert
        return -1;
    } else {
        // Insert

        // Cannot (yet) insert on sparse array
        daCompact(a);

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
                i = nextVacToFill; // will ++ at beginning of loop
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
        // OBS sök till (i <= index + v) för att hitta ev. vakanser mellan slotindex och virtuellt index
        for (i = a->vacantFirst; i <= a->vacantLast && i <= index + v && v < a->vacantTotal; i++){
        // Mindre optimerad, utan first/last
        //for (i = 0; i <= index + v && v < a->vacantTotal; i++){    
            if (*(a->vacant + i))
                v++;
        }
    }
    return v;
}

double daSparseGet(da* a, int index) {
    int v = daVacs(a, index);
    printf("Vacs:%d  ", v);

    return *(a->p + index + v);
}

/* Status daSparseRemove:
    compact array remove 1 beginning/middle/end - ok
    compact array remove 3 beginning/middle/end - ok
    remove 1 before vac - ok
    remove 3 before vac - ok
    remove 1 after vac - verkar ok nu, mer test behövs
    remove 3 after vac - verkar ok nu, mer test behövs
    remove 1 between vacs -
    remove 3 between vacs - 

*/ 

int daSparseRemove(da* a, int startIndex, int endIndex){
     if (startIndex >= a->elements ||
        endIndex >= a->elements ||
        startIndex > endIndex ) {
        //Illegal remove
        return -1;
    }

    const int blockSize = endIndex - startIndex + 1;
    int i;
   
    /* Find vac-counts for block */ 
    int blockVacs[1000]; // FIGURE OUT C99 VLAs /
    // Optimering: först kolla för 0 på vacs för endIndex 
    int vacEnd = daVacs(a, endIndex);
    if (vacEnd == 0){
        for (i = 0; i < blockSize; i++)
            blockVacs[i] = 0;
    } else {
        int vacStart = daVacs(a, startIndex);
        blockVacs[0] = vacStart;
    
        // Denna optimerade version har något problem
        // Ok nu?
        for (i = 1; i < blockSize; i++){
            if (*(a->vacant + i + startIndex + vacStart))
                blockVacs[i] = ++vacStart;
            else
                blockVacs[i] = vacStart;
        }
        printf("\nblockVacs:\n");
        for (i = 0; i < blockSize; i++)
            printf("%d ", blockVacs[i]);
        printf("\n");

        /* Slower option in case above should exhibit wierdness */
        // for (i = 1; i < blockSize; i++){
        //         blockVacs[i] = daVacs(a, startIndex+i);
        // }
            
        // for (i = 0; i < blockSize; i++)
        //     printf("%d ", blockVacs[i]);
        // printf("\n");
    } /* End find vac-counts */

    // Set vacant on vac-corrected slot indices
    int blockIter = 0;
    for (i = startIndex; i <= endIndex; i++) {
        *(a->vacant + i + blockVacs[blockIter]) = true;
        blockIter++;
    }
    
    // Set vacantLast to endIndex + vacs before endIndex
    endIndex += blockVacs[blockSize - 1];
    if (a->vacantTotal == 0){
        // First sparseRemove after init or compact
        a->vacantFirst = startIndex;
        a->vacantLast = endIndex;
    } else {
        // printf("\nstartIndex:%d vacantFirst:%d\n", startIndex, a->vacantFirst);  
        if (startIndex < a->vacantFirst)
            a->vacantFirst = startIndex;
        // printf("\nstartIndex:%d vacantFirst:%d\n", startIndex, a->vacantFirst);  
        // printf("\nendIndex:%d vacantLast:%d\n", endIndex, a->vacantLast);    
        if (endIndex > a->vacantLast)
            a->vacantLast = endIndex;
        // printf("\nendIndex:%d vacantLast:%d\n", endIndex, a->vacantLast);  
    }
    // Update totals
    a->elements -= blockSize;
    a->vacantTotal += blockSize;

    return 0;    
}


