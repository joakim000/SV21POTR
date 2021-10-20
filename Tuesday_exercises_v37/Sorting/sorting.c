#include "sorting.h"
#include "sort_struct.h"


/* Tuesday exercises
    ## Sorting
    4. Sort an array of unsigned ints. Make two functions, one Ascending, one Descending.
       To get you started: sorting.c

*/


/*  Sorting exercise

    This file contains natural language descriptions of a 
	selection of well-known algorithms:
        a. Bubble    - done
		b. Insertion - done 
		c. Selection - done
        d. Shell     - done
		e. Merge     - done
        f. Quick
*/


/**
 * ISSUES
 * 
 */

// Flags
#define DEBUGCOPY false
#define DEBUGWRITE false
#define DEBUG false

// struct sort_args {
//     // Standard args
//     uint32_t* num;
//     uint32_t size;
    
//     uint16_t maxThreads; // For threaded sorts 
//     int shellSeq[]; // Gap sequence for shell sort
// };



/* Lib qsort
    Assumed error-free, use as comparison for functional tests
    and as performance reference.
*/
int cmpAscend (const void * a, const void * b) {  // From tutorialspoint
   return ( *(uint32_t*)a - *(uint32_t*)b );
}
int cmpDescend (const void * a, const void * b) {  // From tutorialspoint
   return ( *(uint32_t*)b - *(uint32_t*)a );
}

void sort_lib(void *params) {
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;
    qsort(num, size, sizeof(uint32_t), cmpAscend);
}
void sort_lib_d(void *params) {
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;
    qsort(num, size, sizeof(uint32_t), cmpDescend);
}




/*  Bubble sort:
    The algorithm starts at the beginning of the data set.
    It compares the first two elements, and if the first is greater than the second, it swapped them.
    It continues doing this for each pair of adjacent elements to the end of the data set.
    It then starts again with the first two elements, repeating until no swaps have occurred on the last pass.
*/

void swap(uint32_t *a, uint32_t *b)
{
    uint32_t tmp = *a;
    *a = *b;
    *b = tmp;
}


// void sort_bubble(void *params)
void sort_bubble(void *params) {
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    bool swapped; 
    do {
        swapped = false;
        for (int i = 0; i < size-1; i++) {
            if (num[i] > num[i + 1]) {
                swap(&num[i], &num[i + 1]);
                swapped = true;
            }
        }
    } while (swapped == true);
}

void sort_bubble_d(void *params) {
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;
    bool swapped; 
    
    do {
        swapped = false;
        for (int i = 0; i < size-1; i++) {
            if (num[i] < num[i + 1]) {
               swap(&num[i], &num[i + 1]);
                swapped = true;
            }
        }
    } while (swapped == true);
}




/*  Insertion sort:
 1. Suppose there exists a function called Insert designed to insert a value into a sorted sequence at the beginning of an array.
    It operates by beginning at the end of the sequence and shifting each element one place to the right until a suitable position is found for the new element.
    The function has the side effect of overwriting the value stored immediately after the sorted sequence in the array.
 2. To perform an insertion sort, begin at the left-most element of the array and invoke Insert to insert each element encountered into its correct position.
    The ordered sequence into which the element is inserted is stored at the beginning of the array in the set of indices already examined.
    Each insertion overwrites a single value: the value being inserted.
*/

void insert(int index, uint32_t *num, uint32_t size, int ascend0descend1 ) {
    int moveToIndex = index;
    //Find place
    if (ascend0descend1 == 1) 
        for (int i = index - 1; i >= 0; i--) 
            if (num[i] < num[index])
                moveToIndex--;
            else
                break;
    else 
        for (int i = index - 1; i >= 0; i--) 
            if (num[i] > num[index])
                moveToIndex--;
            else
                break;

    // Move elements up and insert value
    if (moveToIndex != index) {
        uint32_t tmp = num[index];
        for (int i = index; i >= moveToIndex; i--) 
            num[i] = num[i - 1];
        num[moveToIndex] = tmp;
    }
}

void sort_insertion(void *params)
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    for (int i = 1; i < size; i++)
        insert(i, num, size, 0);
}

void sort_insertion_d(void *params)
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    for (int i = 1; i < size; i++)
        insert(i, num, size, 1);
}





/*  Selection sort:
    The algorithm divides the input list into two parts: 
        a sorted sublist of items which is built up from left to right at the front (left) of the list
        and a sublist of the remaining unsorted items that occupy the rest of the list.
    Initially, the sorted sublist is empty and the unsorted sublist is the entire input list.
    The algorithm proceeds by finding the smallest (or largest, depending on sorting order) element in the unsorted sublist,
        exchanging (swapping) it with the leftmost unsorted element (putting it in sorted order),
        and moving the sublist boundaries one element to the right.
*/
int select(int bound, uint32_t *num, uint32_t size, int small0large1) {
    int r = bound;
    if (small0large1 == 1) 
        for (int i = bound + 1; i < size; i++) 
            r = num[i] > num[r] ? i : r;        // select largest value
    else
        for (int i = bound + 1; i < size; i++) 
            r = num[i] < num[r] ? i : r;        // select smallest value
    return r;
}

void sort_selection(void *params)
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    for (int bound = 0; bound < size; bound++) {
        int sel = select(bound, num, size, 0);
        swap(&num[bound], &num[sel]);
    }
}

void sort_selection_d(void *params)
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;
    
    for (int bound = 0; bound < size; bound++) {
        int sel = select(bound, num, size, 1);
        swap(&num[bound], &num[sel]);
    }
}




/*  Shellsort:
    Shellsort is an optimization of insertion sort that allows the exchange of items that are far apart.
    The idea is to arrange the list of elements so that, starting anywhere, taking every hth element produces a sorted list.
    Such a list is said to be h-sorted. 
    It can also be thought of as h interleaved lists, each individually sorted. 
    Beginning with large values of h allows elements to move long distances in the original list, reducing large amounts of disorder quickly,
    and leaving less work for smaller h-sort steps to do. 
    If the list is then k-sorted for some smaller integer k, then the list remains h-sorted.
    Following this idea for a decreasing sequence of h values ending in 1 is guaranteed to leave a sorted list in the end.

    gaps = [1750, 701, 301, 132, 57, 23, 10, 4, 1]  // Extended Ciura gap sequence
                                                    // https://oeis.org/A102549/internal 
*/
void gapInsert(int index, int gap, uint32_t *num, uint32_t size, int ascend0descend1 ) {
    int moveToIndex = index;
    //Find place
    if (ascend0descend1 == 1)   
        for (int i = index - gap; i >= 0; i -= gap)  // Regular insert
            if (num[i] < num[index])
                moveToIndex -= gap;
            else
                break;
    else 
        for (int i = index - gap; i >= 0; i -= gap) 
            if (num[i] > num[index])
                moveToIndex -= gap;
            else
                break;

    // Move elements up and insert value
    if (moveToIndex != index) {
        uint32_t tmp = num[index];
        for (int i = index; i >= moveToIndex; i--) 
            num[i] = num[i - 1];
        num[moveToIndex] = tmp;
    }
}

void sort_shell(void *params)
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};  // Extended Ciura gap sequence
    // int gaps[] = {4, 1};  
    // int gaps[] = {5, 3, 1};
    // int gaps[] = {3, 1};
    // int gaps[] = {1};

    for (int si = 0; si < COUNT_OF(gaps); si++) { // si: shell iterator
        for (int i = 1; i < size; i++)
            gapInsert(i, gaps[si], num, size, 0);
    }   
}

void sort_shell_d(void *params)
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    int gaps[] = {1750, 701, 301, 132, 57, 23, 10, 4, 1};  // Extended Ciura gap sequence

    for (int si = 0; si < COUNT_OF(gaps); si++) { // si: shell iterator
        for (int i = 1; i < size; i++)
            gapInsert(i, gaps[si], num, size, 1);
    }   
}





/*  Merge sort:
    Conceptually, a merge sort works as follows:
    Divide the unsorted list into n sublists, each containing one element (a list of one element is considered sorted).
    Repeatedly merge sublists to produce new sorted sublists until there is only one sublist remaining. This will be the sorted list.
    Note: Top-down vs. bottom-up implementation.
*/
uint32_t totalSize; // DEBUG
uint32_t* randomArray; // DEBUG

// DEBUG helper
bool inArray(uint32_t find, uint32_t *num, uint32_t size) {
    for (int i = 0; i < size; i++) {
       if (find == num[i]) 
          return true;
    }
    return false;
}

// For holding temp values 
uint32_t* a;
uint32_t* b;

void merge_recurse(uint32_t *num, uint32_t size, uint32_t start ) {
    // Shouldn't happen
    assert( ("Zero array size", size > 0) );

    // printf("call: size %4u start %4u \n", size, start);


    // Split into 2 virtual parts    
    int aSize = size / 2;
    int bSize = size / 2 + size % 2;
 
    // Continue splitting    
    if (size > 2) {
        merge_recurse(num, aSize, start);
        merge_recurse(num, bSize, start + aSize);
    }

    // Now we're on the way up. First ignore size 1 (by definition sorted). Then start comparing.
    if (size > 1) {
        // printf("process: size %4u start %4u \n", size, start);
        // uint32_t a[aSize], b[bSize];
        /* Copy values to be sorted for this call. */
        // Note: Cleverer implementations use back-and-forth copying to avoid this step.
        // Begin reading a-values at start index for this call
        for (int i = 0; i < aSize; i++) { 
            a[i] = num[start + i];
        }

        // Read b-values at count of a-values further along array 
        for (int i = 0; i < bSize; i++) { 
            b[i] = num[start + aSize + i];
        }

        /* Compare and write values into sort array. */
        int p = start; // For this call, start writing here. Later increment p when writing value.

        for (int i = 0, j = 0; i < aSize; i++) {
            // Case 1: have more a-values but out of b-values. Continue looping over and writing a-vals.
            // Note: Missing case 1 was source of intermittent errors depending on look of random array. 
            // Case 2: Already sorted (a-val < b-val), write and continue to next a-val
            if (j >= bSize || a[i] < b[j]) {
                num[p++] = a[i];
                continue;
            }
            
            // Case 3: b-val < a-val. Continue writing b-vals until current a-val is smaller or b-vals exhausted.
            do {
                num[p++] = b[j++];
            }  while (j < bSize && b[j] < a[i]);
            // After 1 or more b-vals, finally write current a-val. 
            num[p++] = a[i];
        }
    }
}



void sort_merge(void *params)
// void sort_merge(uint32_t *num, uint32_t size, uint32_t *random)  //DEBUG
{
    struct sort_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;

    // To hold temp values
    uint32_t* aTemp;
    aTemp = calloc(size / 2 + 2, sizeof(uint32_t));
    assert( ("Memory allocation failed.", aTemp != NULL) );
    a = aTemp;

    uint32_t* bTemp;
    bTemp = calloc(size / 2 + 2, sizeof(uint32_t));
    assert( ("Memory allocation failed.", bTemp != NULL) );
    b = bTemp;

    // totalSize = size; // DEBUG
    // randomArray = random; // DEBUG

    merge_recurse(num, size, 0);
    free(aTemp);
    free(bTemp);
}

void sort_merge_d(void *params) {
   // implement me!
}







/*  Threaded merge sort:
    The splitting into sublists in merge sort should make it an 
    easy target for threading.
*/

#define MAP false
#define STACKSIZEDEBUG false

struct tmerge_args {
    uint32_t* num;
    uint32_t size;
    uint32_t start;
    uint16_t depth; // Mapping
    char branch;    // Mapping
};

pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER;
uint16_t maxthreadCount = 12;

// uint16_t maxDetectedthreadCount = 0;  // Debug
// uint16_t maxDepth = 0;            // Debug
// uint16_t callCount = 0;           // Debug

// _Atomic long threadCount = ATOMIC_VAR_INIT(0L);
uint_fast16_t threadCount;

void *tmerge_recurse(void *params) {
    // pthread_mutex_lock(&mtx2); callCount++; pthread_mutex_unlock(&mtx2);
    // callCount++;
     
    // int thisCall;
    // if (MAP) thisCall = callCount;
    
    // Get args
    struct tmerge_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;
    uint32_t start = args->start;
    // uint16_t depth = args->depth + 1;

      // Shouldn't happen
    assert( ("Zero array size", size > 0) );

    // if (DEBUG) {
    //     if (depth > maxDepth)
    //         maxDepth = depth;
    //     if (threadCount > maxDetectedthreadCount)
    //         maxDetectedthreadCount = threadCount;
    // }
    // // Mapping
    // if (MAP) {
    //     printf("call: size %4u    start %4u    depth %2u   branch %2c   threadCount %2d   call# %5d   tId %u\n", size, start, depth, args->branch, threadCount, thisCall, pthread_self()); 
    // }
      
    // Split into 2 virtual parts    
    int aSize = size / 2;
    int bSize = size / 2 + size % 2;

    // Continue splitting    
    if (size > 2) {
        struct tmerge_args args_a = {
            .num = num,
            .size = aSize,
            .start = start,
            // .depth = depth,   // Mapping
            // .branch = 'a'     // Mapping
        };
        struct tmerge_args args_b = {
            .num = num,
            .size = bSize,
            .start = start + aSize,
            // .depth = depth,   // Mapping
            // .branch = 'b'     // Mapping
        };

        // When below maxthreadCount create new recursing thread, else recurse in this thread
        if (threadCount <= maxthreadCount - 2 ) {
            pthread_t tId_a, tId_b;
            pthread_attr_t attr;
            assert( ("Attr init error.",
                pthread_attr_init(&attr) == 0) );  

            // Create new thread for branch a
            assert( ("Setstacksize error.", 
                pthread_attr_setstacksize(&attr, aSize * sizeof(uint32_t) + STACK_MIN) == 0) );
            // if (DEBUG) printf("Create thread a, reqested stacksize: %d  corrected stacksize: %d\n", aSize*sizeof(uint32_t), aSize * sizeof(uint32_t) + STACK_MIN);
            assert( ("Thread creation error.", 
                pthread_create(&tId_a, &attr, &tmerge_recurse, (void *)&args_a) == 0) );
            // if (DEBUG) printf("Created thread a, id: %u\n", tId_a);
            pthread_mutex_lock(&mtx1); threadCount++; pthread_mutex_unlock(&mtx1); 

            // Create new thread for branch b
            assert( ("Setstacksize error.", 
                pthread_attr_setstacksize(&attr, bSize * sizeof(uint32_t) + STACK_MIN) == 0) );
            // if (DEBUG) printf("Create thread b, reqested stacksize: %d  corrected stacksize: %d\n", bSize*sizeof(uint32_t), bSize * sizeof(uint32_t) + STACK_MIN);
            assert( ("Thread creation error.", 
                pthread_create(&tId_b, &attr, &tmerge_recurse, (void *)&args_b) == 0) );
            // if (DEBUG) printf("Created thread b, id: %u\n", tId_b);
            pthread_mutex_lock(&mtx1); threadCount++; pthread_mutex_unlock(&mtx1); 

            pthread_join(tId_a, NULL);
            pthread_mutex_lock(&mtx1); threadCount--; pthread_mutex_unlock(&mtx1); 

            pthread_join(tId_b, NULL);
            pthread_mutex_lock(&mtx1); threadCount--; pthread_mutex_unlock(&mtx1); 
        }
        else {
            tmerge_recurse((void *)&args_a);
            tmerge_recurse((void *)&args_b);
        }
    }
   
    // Now we're on the way up. First ignore size 1 (by definition sorted). Then start comparing.
    if (size > 1) {
        // if (MAP) {
        //     printf("\e[1;32mproc: size %4u    start %4u    depth %2u   branch %2c   threadCount %2d   call# %5d   tId %u\e[m\n", size, start, depth, args->branch,threadCount, thisCall, pthread_self()); 
        // }

        // Work arrays on thread stack
        uint32_t a[aSize], b[bSize];
        // uint16_t a[aSize], b[bSize];
        
        /* Copy values to be sorted for this call. */
        // Note: Cleverer implementations use back-and-forth copying to avoid this step.
        // Begin reading a-values at start index for this call
        for (int i = 0; i < aSize; i++) { 
            a[i] = num[start + i];
        }

        // Read b-values at count of a-values further along array 
        for (int i = 0; i < bSize; i++) { 
            b[i] = num[start + aSize + i];
        }

        /* Compare and write values into sort array. */
        int p = start; // For this call, start writing here. Later increment p when writing value.

        for (int i = 0, j = 0; i < aSize; i++) {
            // Case 1: have more a-values but out of b-values. Continue looping over and writing a-vals.
            // Note: Missing case 1 was source of intermittent errors depending on look of random array. 
            // Case 2: Already sorted (a-val < b-val), write and continue to next a-val
            if (j >= bSize || a[i] < b[j]) {
                num[p++] = a[i];
                continue;
            }
            
            // Case 3: b-val < a-val. Continue writing b-vals until current a-val is smaller or b-vals exhausted.
            do {
                num[p++] = b[j++];
            }  while (j < bSize && b[j] < a[i]);
            // After 1 or more b-vals, finally write current a-val. 
            num[p++] = a[i];
        }
    }
}

void print_num(uintptr_t num){
    printf("%d\n", num);
}

//  printf("\e[1;32mproc: size %u    start %u    depth %u    maxproc %u   a: %u  b: %u\e[m\n" , s
// char* str[] consoleColor(int color, char* str[]) {
    // return sprintf(str, )
// }
    
void sort_tmerge(void *params) {
{
    struct sort_args *call_args = params;
    uint32_t* num = call_args->num;
    uint32_t size = call_args->size;
    maxthreadCount = call_args->maxThreads;

    printf("Call to tmerge, maxthreads: %d\n", maxthreadCount);

    int err;
    pthread_t tId;
    struct tmerge_args args = {
            .num = num,
            .size = size,
            .start = 0,
            // .depth = 0,     // Mapping
            // .branch = 'm'   // Mapping
        };

    pthread_attr_t attr;   
    assert( ("ptread_attr_init error.",
        pthread_attr_init(&attr) == 0) ); 
    assert( ("phtread_attr_setstacksize error.", 
        pthread_attr_setstacksize(&attr, size * sizeof(uint32_t) + STACK_MIN) == 0) );

    // if (DEBUG) printf("Create thread m, reqested stacksize: %d  corrected stacksize: %d\n", size*sizeof(uint32_t), size * sizeof(uint32_t) + STACK_MIN);
    assert( ("Thread creation failed.", 
        pthread_create(&tId, &attr, &tmerge_recurse, (void *)&args) == 0) );
    // atomic_store(&threadCount, threadCount + 1);
    //  if (DEBUG) printf("Created thread m, id: %u\n", tId);
    pthread_mutex_lock(&mtx1); threadCount++; pthread_mutex_unlock(&mtx1);
    
    pthread_join(tId, NULL);
    // atomic_store(&threadCount, threadCount - 1);
    pthread_mutex_lock(&mtx1); threadCount--; pthread_mutex_unlock(&mtx1); 

    // if (DEBUG) printf("\nMax depth: %d\n", maxDepth);
    // if (DEBUG) printf("\nMax detected threadCount: %d\n", maxDetectedthreadCount);

    }
}

void sort_tmerge_d(void *params) {

}





/*  Quicksort:
 1. If the range has less than two elements, return immediately as there is nothing to do. 
    Possibly for other very short lengths a special-purpose sorting method is applied and the remainder of these steps skipped.
 2. Otherwise pick a value, called a pivot, that occurs in the range (the precise manner of choosing depends on the partition routine, and can involve randomness).
 3. Partition the range: reorder its elements, while determining a point of division, 
    so that all elements with values less than the pivot come before the division,
    while all elements with values greater than the pivot come after it;
    elements that are equal to the pivot can go either way.
    Since at least one instance of the pivot is present, most partition routines ensure that the value that ends up at the point of division is equal to the pivot,
    and is now in its final position (but termination of quicksort does not depend on this, as long as sub-ranges strictly smaller than the original are produced).
 4. Recursively apply the quicksort to the sub-range up to the point of division and to the sub-range after it, 
    possibly excluding from both ranges the element equal to the pivot at the point of division.
    (If the partition produces a possibly larger sub-range near the boundary where all elements are known to be equal to the pivot, these can be excluded as well.)
*/
void sort_quick(void *params)
{
    // implement me!
}
void sort_quick_d(void *params)
{
    // implement me!
}






/*  Radix sort:

*/
void sort_radix(void *params) {

}

void sort_radix_d(void *params) {

}






/*  Timsort:

*/
void sort_tim(void *params) {

}

void sort_tim_d(void *params) {

}


