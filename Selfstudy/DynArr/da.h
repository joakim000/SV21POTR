/*  C exercise:
    Dynamic array implementation
    with optional sparse arrays

    210916 Joakim O

    da_compo_test.c     Ocular component tests
    da_perf_test.c      Performance tests
    /unity              Unit tests
    /perf               Selected performance logs


/* Errors
    0 OK
    1 Memory allocation
    2 Index out of range
*/

#include "devheaders.h"

#ifndef DA_TYPE
#define DA_TYPE double
#endif

typedef struct DynArr {
   DA_TYPE *p;
   short error;
   long elements;
   long slots;
   float growthFactor;
   unsigned short compactFreq;
   
   // Sparse
   bool *vacant;  
   long vacantTotal;
   long vacantFirst;
   long vacantLast;

   unsigned short* lookup;
   bool lookupCurrent;

} da;

// Init array
// Return: exit code
int daInit(da* a, int initAllocation, float growthFactor);  

// Init array, return struct
// Return: struct
// Check da.error
da daInit2(int initAllocation, float growthFactor);  

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
// Return: value
// Args: da* a, int index, datatype &target
// Return: exit code
DA_TYPE daGet(da* a, int index);
DA_TYPE daSparseGet(da* a, int index);

// Set value
// Args: da* a, int index, datatype value 
// Return: exit code
int daSet(da* a, int index, DA_TYPE value);
int daSparseSet(da* a, int index, DA_TYPE value);

// Compact sparse allocation
// Args: da* a
int daCompact(da* a);

// Return element count
// Args: da* a
int daCount(da* a);

// Return allocated slots
// Args: da* a 
int daAlloc(da* a);

/* Helpers */
static int daVacs(da* a, int index);                   // Vacancy counter
static int daRealloc(da* a, int extraSlots);           // Realloc routine
static short allocCheck(void *p);

/* Declarations end */


int daInit(da* a, int initAllocation, float growthFactor) {   
    /* Main init */
    a->slots = initAllocation;
    a->growthFactor = growthFactor;
    a->elements = 0;

    a->p = calloc(a->slots, sizeof(DA_TYPE));
    if (allocCheck(a->p)) return allocCheck(a->p); 

    /* Init vars for sparse functionality */
    a->compactFreq = 512;
    a->vacantTotal = 0;

    a->vacant = calloc(initAllocation, sizeof(bool)); 
    if (allocCheck(a->vacant)) return allocCheck(a->vacant);  
    for (int i = 0; i < initAllocation; i++)
        *(a->vacant + i) = false;
    
    // Experimental lookup optimization 
    a->lookupCurrent = false;

    a->lookup = calloc(initAllocation, sizeof(unsigned short)); 
    if (allocCheck(a->lookup)) return allocCheck(a->lookup);  
    for (int i = 0; i < initAllocation; i++)
        *(a->lookup + i) = 0;
        
    return 0;
}

static short allocCheck(void* p) {
    if(p == NULL ) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return 1;
    }
    else 
        return 0;

}

da daInit2(int initAllocation, float growthFactor) {   
    da r;
    r.error = 0;
    
    /* Main init */
    r.slots = initAllocation;
    r.growthFactor = growthFactor;
    r.elements = 0;

    r.p = calloc(r.slots, sizeof(DA_TYPE));
    r.error = allocCheck(r.p);
   
    /* Init vars for sparse functionality */
    r.compactFreq = 512;
    r.vacantTotal = 0;

    r.vacant = calloc(initAllocation, sizeof(bool)); 
    r.error = allocCheck(r.vacant);
    for (int i = 0; i < initAllocation; i++)
        *(r.vacant + i) = false;
    
    // Experimental lookup optimization 
    r.lookupCurrent = false;

    r.lookup = calloc(initAllocation, sizeof(unsigned short)); 
    r.error = allocCheck(r.lookup);
    for (int i = 0; i < initAllocation; i++)
        *(r.lookup + i) = 0;

    return r;
}

int daClear(da* a){
    free(a->p);
    free(a->vacant);
    free(a->lookup);
    return 0;
}

int daCreate(da* a, DA_TYPE values[], int len) {   
    // printf("Create - slots:%d len:%d\n", a->slots, len);

    if (len > a->slots) {
        //Needs realloc
        int extraSlots = len - a->slots;
        daRealloc(a, extraSlots);
    } 
    
    for (int i = 0; i < len; i++) {
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

static int daRealloc(da* a, int extraSlots){
    int newSize = (int)((a->slots * a->growthFactor) + extraSlots);
    a->p = realloc(a->p, newSize * sizeof(DA_TYPE)); 
    a->error = allocCheck(a->p);
    int oldSize = a->slots;
    a->slots = newSize;
    
    a->vacant = realloc(a->vacant, a->slots * sizeof(bool)); 
    a->error = allocCheck(a->p);

    // Init vacancy for added slots
    for (int i = oldSize; i < newSize; i++)
        *(a->vacant + i) = false;

    printf("\nRealloc done. Elements:%ld  Slots:%ld\n", a->elements, a->slots); // Print to log

    return 0;
}


int daAdd(da* a, int index, DA_TYPE value) {  //index -1 == end
    if (a->elements == a->slots) {
        //Needs realloc
        daRealloc(a, 0);
    }

    if (index == -1) { //Add to end
        *(a->p + a->elements + a->vacantTotal) = value;
        a->elements++;
    } else if (index > a->elements - 1 || index < 0) {
        // Illegal insert
        a->error = 2;
        printf("\nInsert out of range:%d", index);  //Print to log 
        return 2;
    } else {
        // Insert
        if (a->vacantTotal == 0) {
            // Flytta alla element ett steg upp, start vid index
            for (int i = a->elements - 1; i >= index; i-- ) {
                *(a->p + i + 1) = *(a->p + i);
            }

            // Sätt in värdet på angivet index        
            *(a->p + index) = value;
        }
        // Sparse insert
        else {    
            // correction for virtual insert index
            int insVac = daVacs(a, index); 
            
            // Check for vacant space
            if (insVac > daVacs(a, index - 1)){
                // Find slotindex before virtual insert index
                int si = index + insVac - 1;
                // This slotindex should be vacant
                assert(*(a->vacant + si));
                // Set value
                *(a->p + si) = value;
                // No longer vacant
                *(a->vacant + si) = false;
                a->vacantTotal--;
                // if si was lastIndex, no longer
                if (si == a->vacantLast)
                    a->vacantLast--; 

            } else {
                // Slotindex for virtual insert index 
                // printf("sparse, no vac on slotindex before this virtual index's slotindex");

                int si = index + insVac;
                // Flytta alla element ett steg upp, start vid index
                for (int i = a->elements + a->vacantTotal - 1; i >= si; i-- ) {
                    *(a->p + i + 1) = *(a->p + i);
                }
                 // Sätt in värdet på angivet virtuellt index  
                // printf("\ninput at index:%d slot:%d", index, si);
                *(a->p + si) = value;

                // Flytta upp vacant-flaggor
                if (si <= a->vacantLast) {
                    // printf("\nindex:%d  <= vacantLast:%d", index, a->vacantLast);
                    for (int i = a->vacantLast; i >= index; i-- ) {
                         *(a->vacant + i + 1) = *(a->vacant + i);
                    }
                    a->vacantLast++;
                }
                // if (index <= a->vacantLast)
                //     a->vacantLast++;
                if (si <= a->vacantFirst) {
                //  printf("\nindex:%d  <= vacantFirst:%d", index, a->vacantFirst);

                    a->vacantFirst++;
                }
                    


            }
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
        a->error = 2;
        return 2;
    }

    // Move elements blocksize steps in negative direction
    int blockSize = endIndex - startIndex + 1;
    for (int i = startIndex + blockSize; i < a->elements; i++ ) {
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
    if (a->vacantTotal > 0) {
        int dataIter = 0;
        // int haveData[a->elements]; // Funkar fint upp till 518087 element, kraschar med fler. Max för VLA?
        int* haveData = calloc(a->elements, sizeof(int));
        a->error = allocCheck(haveData);
        if (a->error) return a->error;

        for (int i=0; i < a->elements + a->vacantTotal; i++) {
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


static int daVacs(da* a, int index) {
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

// Time expensive, is there a way without the inner loop?
int buildLookup(da* a) {  
    if (a->vacantTotal > 0) {
    unsigned short v = 0;
        for (int i = a->vacantFirst; i <= a->elements + a->vacantTotal; i++) {
            // i <= a->vacantLast;
            if (*(a->vacant + i)) {
                v++;
            }
            // Work forward to find correction for virtual index rather than slot index
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

// int buildLookupDebug(da* a) {  
//     if (a->vacantTotal > 0) {
//         for (int i = a->vacantFirst; i <= a->elements + a->vacantTotal; i++) 
//             *(a->lookupDebug + i) = daVacs(a, i);
//     }
//     return 0;
// }

int daSparseRemove(da* a, int startIndex, int endIndex){
     if (startIndex >= a->elements ||
        endIndex >= a->elements ||
        startIndex > endIndex ) {
        //Illegal remove
        return -1;
    }
    int i;

    // if (a->vacantTotal > a->compactFreq)
    //      printf("\nRemove calling compact\n");
    //      daCompact(a);

    // case of block remove 
    if (startIndex != endIndex) goto block;

    single:;
    int vacSingle = daVacs(a, startIndex);
    // int vacSingle = *(a->lookup + startIndex);

    // corrected index
    int ci = startIndex + vacSingle;
    
    // set vacant
    *(a->vacant + ci) = true;
  
    // Set vacantLast to endIndex + vacs before endIndex  
    if (a->vacantTotal == 0) {
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
    //     *(a->lookup + i) = *(a->lookup + i) + 1;

    goto end;


    block:;
    const int blockSize = endIndex - startIndex + 1;
    /* Find vac-counts for block */ 
    int* blockVacs;
    blockVacs = calloc(blockSize, sizeof(int)); 
    a->error = allocCheck(blockVacs);
    if (a->error) return a->error;

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


    end:
    return 0;    
}


