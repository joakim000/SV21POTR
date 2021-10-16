#include "sorting.h"


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

void sort_lib(uint32_t *num, uint32_t size) {
    qsort(num, size, sizeof(uint32_t), cmpAscend);
}
void sort_lib_d(uint32_t *num, uint32_t size) {
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

void sort_bubble(uint32_t *num, uint32_t size)
{
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

void sort_bubble_d(uint32_t *num, uint32_t size)
{
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


void sort_insertion(uint32_t *num, uint32_t size)
{
    for (int i = 1; i < size; i++)
        insert(i, num, size, 0);
}

void sort_insertion_d(uint32_t *num, uint32_t size)
{
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

void sort_selection(uint32_t *num, uint32_t size)
{
    for (int bound = 0; bound < size; bound++) {
        int sel = select(bound, num, size, 0);
        swap(&num[bound], &num[sel]);
    }
}

void sort_selection_d(uint32_t *num, uint32_t size)
{
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

void sort_shell(uint32_t *num, uint32_t size)
{
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

void sort_shell_d(uint32_t *num, uint32_t size)
{
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



void sort_merge(uint32_t *num, uint32_t size)
// void sort_merge(uint32_t *num, uint32_t size, uint32_t *random)  //DEBUG
{
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

void sort_merge_d(uint32_t *num, uint32_t size) {
   // implement me!
}



/*  Threaded merge sort:
    The splitting into sublists in merge sort should make it an 
    easy target for threading.
*/
struct tmerge_args {
    uint32_t* num;
    uint32_t size;
    uint32_t start;
};

void *tmerge_recurse(void *params) {
    // Get args
    struct tmerge_args *args = params;
    uint32_t* num = args->num;
    uint32_t size = args->size;
    uint32_t start = args->start;
    
    // Shouldn't happen
    assert( ("Zero array size", size > 0) );

    // Split into 2 virtual parts    
    int aSize = size / 2;
    int bSize = size / 2 + size % 2;
 
    pthread_t tId_a = -1;
    pthread_t tId_b = -1;
    pthread_t tId = -1;


    // Continue splitting    
    if (size > 2) {
        int err;
       
        struct tmerge_args args_a = {
            .num = num,
            .size = aSize,
            .start = start
        };
        struct tmerge_args args_b = {
            .num = num,
            .size = bSize,
            .start = start + aSize
        };

        err = 
            pthread_create(&tId, NULL, &tmerge_recurse, (void *)&args_a);
        assert( ("Thread creation failed.", err == 0) );

        // pthread_join(tId_a, NULL);
        pthread_join(tId, NULL);


        err = 
            pthread_create(&tId, NULL, &tmerge_recurse, (void *)&args_b);
        assert( ("Thread creation failed.", err == 0) );
          
        // pthread_join(tId_b, NULL);
        pthread_join(tId, NULL);
      
    }

    // Now we're on the way up. First ignore size 1 (by definition sorted). Then start comparing.
    if (size > 1) {
        uint32_t tmp[size];
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

void sort_tmerge(uint32_t *num, uint32_t size) {
// void sort_tmerge(uint32_t *num, uint32_t size, uint32_t *random)  //DEBUG
{
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

    pthread_t tId;
    struct tmerge_args args = {
            .num = num,
            .size = size,
            .start = 0
        };

    int err = 
        pthread_create(&tId, NULL, &tmerge_recurse, (void *)&args);
    assert( ("Thread creation failed.", err == 0) );

    pthread_join(tId, NULL);

    free(aTemp);
    free(bTemp);
}
}

void sort_tmerge_d(uint32_t *num, uint32_t size) {

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
void sort_quick(uint32_t *num, uint32_t size)
{
    // implement me!
}
void sort_quick_d(uint32_t *num, uint32_t size)
{
    // implement me!
}






/*  Radix sort:

*/
void sort_radix(uint32_t *num, uint32_t size) {

}

void sort_radix_d(uint32_t *num, uint32_t size) {

}






/*  Timsort:

*/
void sort_tim(uint32_t *num, uint32_t size) {

}

void sort_tim_d(uint32_t *num, uint32_t size) {

}


