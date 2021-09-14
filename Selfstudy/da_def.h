#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

//Google's apparantly clever array size macro
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

#ifndef DA_TYPE
#define DA_TYPE double
#endif

typedef struct DynArr {
   DA_TYPE *p;
   long elements;
   long slots;
   float growthFactor;
   unsigned short compactFreq;
   unsigned short* lookup;
   unsigned short* lookupDebug;
   bool lookupCurrent;

   // Sparse
   bool *vacant;  
   long vacantTotal;
   long vacantFirst;
   long vacantLast;
} da;

// Init array
// Args: da* a, int reserve, int datatype
// Return: exit code
int daInit(da* a, int initSlots, float growthFactor);  

// Clear allocations when done
// Return: exit code
int daClear(da* a);

// Load starting values
// Args: da* a, datatype values[], int len
// Return: exit code
int daCreate(da* a, DA_TYPE values[], int len);

// Insert or add (index -1)
// Args: da* a, int index, DA_TYPE value
// Return: exit code  
int daAdd(da* a, int index, DA_TYPE value);  

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
DA_TYPE daGet(da* a, int index);
DA_TYPE daSparseGet(da* a, int index);

// Set value
// Args: da* a, int index, datatype value 
// Return: exit code
int daSet(da* a, int index, DA_TYPE value);
int daSparseSet(da* a, int index, DA_TYPE value);

// Get range - NOT IMPLEMENTED
// Args: da* a, int start, int end 
// Return: union?
DA_TYPE* daGetRange(da* a, int start, int end);

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
int daVacs(da* a, int index);                   // Vacancy counter
int daRealloc(da* a, int extraSlots);           // Realloc routine

/* Declarations end */


int daInit(da* a, int initSlots, float growthFactor) {
    
    // Init vars for sparse functionality
    a->compactFreq = 512;
    a->vacantTotal = 0;
    a->vacant = calloc(initSlots, sizeof(bool)); 
    if(a->vacant == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    for (int i = 0; i < initSlots; i++)
        *(a->vacant + i) = false;
    a->lookup = calloc(initSlots, sizeof(unsigned short)); 
    if(a->lookup == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    for (int i = 0; i < initSlots; i++)
        *(a->lookup + i) = 0;

    a->lookupDebug = calloc(initSlots, sizeof(unsigned short)); 
    if(a->lookupDebug == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    for (int i = 0; i < initSlots; i++)
        *(a->lookupDebug + i) = 0;
    
    a->lookupCurrent = false;
    // End sparse vars
    
    /* Main init */
    a->p = calloc(initSlots, sizeof(DA_TYPE));
    if(a->p == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 
    a->slots = initSlots;
    a->elements = 0;
    a->growthFactor = growthFactor;
    return 0;
}

int daClear(da* a){
    free(a->p);
    free(a->vacant);
    return 0;
}

int daCreate(da* a, DA_TYPE values[], int len) {   
    // printf("Create - slots:%d len:%d\n", a->slots, len);

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

DA_TYPE daGet(da* a, int index) {
    return *(a->p + index);
}

int daSet(da* a, int index, DA_TYPE value) {
    *(a->p + index) = value;
    return 0;
}

int daRealloc(da* a, int extraSlots){
    int newSize = (int)((a->slots * a->growthFactor) + extraSlots);
    a->p = realloc(a->p, newSize * sizeof(DA_TYPE)); 
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
    // Init added slots
    int i;
    for (i = oldSize; i < newSize; i++)
        *(a->vacant + i) = false;

    // printf("Realloc function done, slots: %d\n", a->slots);
}


int daAdd(da* a, int index, DA_TYPE value) {  //index -1 == end
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
        bool regularInsert = true;
        
        // Sparse insert
        if (a->vacantTotal > 0) {    
            // correction for virtual insert index
            int iVac = daVacs(a, index); 
            
            // Check for vacant space
            if (iVac != daVacs(a, index - 1)){
                // Find slotindex before virtual insert index
                int si = index + iVac - 1;
                // This slotindex should be vacant
                assert(*(a->vacant + si));
                // Set value
                *(a->p + si) = value;
                // No longer vacant
                *(a->vacant + si) = false;
                // Don't do regular insert
                regularInsert = false;
            } else {
                // For now; if there's no vacant space we'll 
                // compact the array and do regular insert, else
                // we'd have to move a lot of values in vacant.
                daCompact(a);
            }
        }    

        // Regular insert
        if (regularInsert){
            // Flytta alla element ett steg upp, start vid index
            for (i = a->elements - 1; i >= index; i-- ) {
                *(a->p + i + 1) = *(a->p + i);
            }

            // Sätt in värdet på angivet index        
            *(a->p + index) = value;
        }

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

int daCompact_old(da* a){
    int i;
    bool findVac = true;
    int nextVacToFill; 

    // for (i = 0; i < a->elements + a->vacantTotal; i++) {
    for (i = a->vacantFirst; i < a->elements + a->vacantTotal && i < a->vacantLast; i++) {
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
                // Move data
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

int daCompact(da* a){
    if (a->vacantTotal > 0) {
        int dataIter = 0;
        // int haveData[a->elements]; // Funkar fint upp till 518087 element, kraschar med fler. Max för VLA?
        int* haveData = calloc(a->elements, sizeof(int));
        if(haveData == NULL ) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        } 

        for (int i=0; i < a->elements+a->vacantTotal; i++) {
            if (*(a->vacant + i) == false) {
                haveData[dataIter] = i;
                dataIter++;
            }
            else
                *(a->vacant + i) = false;  // Reset vacancy data
        }

        // printf("\nHavedata: (%d) ", a->elements);
        // for (int i=0; i < a->elements; i++)
            // printf("%d ", haveData[i]);


        for (int i=0; i < a->elements; i++)
            *(a->p + i) = *(a->p + haveData[i]);
        
        a->vacantTotal = 0; // Reset vacancy data
        free(haveData);
    }
    return 0;
}


int daVacs(da* a, int index) {
    //Count vacancies
    int v = 0;
    int i;
    if (!(a->vacantTotal == 0 || index < a->vacantFirst)) {
        // OBS sök till (i <= index + v) för att hitta ev. vakanser mellan slotindex och virtuellt index
        for (i = a->vacantFirst;  i <= index + v && v < a->vacantTotal; i++) {
        // for (i = a->vacantFirst; i <= a->vacantLast && i <= index + v && v < a->vacantTotal; i++) {
        //for (i = 0; i <= index + v && v < a->vacantTotal; i++) {    // Mindre optimerad, utan first/last
            if (*(a->vacant + i))
                v++;
        }
    }
    return v;
}

DA_TYPE daSparseGet(da* a, int index) {
    int v = daVacs(a, index);
    //printf("Vacs:%d  ", v);

    return *(a->p + index + v);
}

int daSparseSet(da* a, int index, DA_TYPE value) {
    int v = daVacs(a, index);
    *(a->p + index + v) = value;
    
    return 0;
}

int buildLookup(da* a) {  
    if (a->vacantTotal > 0) {
    unsigned short v = 0;
        for (int i = a->vacantFirst; i <= a->elements + a->vacantTotal; i++) {
            // i <= a->vacantLast;
            if (*(a->vacant + i)) {
                v++;
            }
            int w = v;
            int extra = 0;
            for (int j = i+1; j <= i + w; j++) {
                if (*(a->vacant + j)) {
                    w++;
                    extra++;
                }
            }
            *(a->lookup + i) = v + extra;
        }
    }
    a->lookupCurrent = true;
    return 0;
}

int buildLookupDebug(da* a) {  
    if (a->vacantTotal > 0) {
        for (int i = a->vacantFirst; i <= a->elements + a->vacantTotal; i++) 
            *(a->lookupDebug + i) = daVacs(a, i);
    }
    return 0;
}

int daSparseRemove(da* a, int startIndex, int endIndex){
     if (startIndex >= a->elements ||
        endIndex >= a->elements ||
        startIndex > endIndex ) {
        //Illegal remove
        return -1;
    }
    int i;

    // printf("vacantTotal:%d", a->vacantTotal);
    
    // if (a->vacantTotal > a->compactFreq)
    //      printf("\nRemove calling compact\n");
    //      daCompact(a);

    // case of single 
    if (startIndex == endIndex) goto single;

    const int blockSize = endIndex - startIndex + 1;
    /* Find vac-counts for block */ 
    int* blockVacs;
    blockVacs = calloc(blockSize, sizeof(int)); 
    if(blockVacs == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return -1;
    } 

    // Optimering: först kolla för 0 på vacs för endIndex 
    int vacEnd = daVacs(a, endIndex);
    if (vacEnd == 0){
        for (i = 0; i < blockSize; i++)
            blockVacs[i] = 0;
    } else {
        int vacStart = daVacs(a, startIndex);
        blockVacs[0] = vacStart;
    
        // Based on vacancy compensation for startIndex, calc others
        for (i = 1; i < blockSize; i++){
            if (*(a->vacant + i + startIndex + vacStart))
                blockVacs[i] = ++vacStart;
            else
                blockVacs[i] = vacStart;
        }
        
        // printf("\nblockVacs:\n");
        // for (i = 0; i < blockSize; i++)
        //     printf("%d ", blockVacs[i]);
        // printf("\n");

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
        if (startIndex < a->vacantFirst)
            a->vacantFirst = startIndex;
        if (endIndex > a->vacantLast)
            a->vacantLast = endIndex;
    }
    // Update totals
    a->elements -= blockSize;
    a->vacantTotal += blockSize;

    free(blockVacs);
    goto end;

    single:;
    int vacSingle = daVacs(a, startIndex);
    // int vacSingle = *(a->lookup + startIndex);

    // corrected index
    int ci = startIndex + vacSingle;
    
    // set vacant
    *(a->vacant + ci) = true;
  
    // Set vacantLast to endIndex + vacs before endIndex  
    if (a->vacantTotal == 0){
        // First sparseRemove after init or compact
        a->vacantFirst = startIndex;
        a->vacantLast = ci;
    } else {
        if (startIndex < a->vacantFirst)
            a->vacantFirst = startIndex;
        if (ci > a->vacantLast)
            a->vacantLast = ci;
    }
    // Update totals
    a->elements -= 1;
    a->vacantTotal += 1;

    // Update lookup
    // for (int i = ci; i < a->elements + a->vacantTotal; i++)
        // *(a->lookup + i) = *(a->lookup + i) + 1;

    end:
    return 0;    
}


