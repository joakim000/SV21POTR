#include "devheaders.h"

/*
		a. Bubble / Exchange
		b. Insertion
		c. Selection
		d. Merge
		e. Shell
        f. Quick
*/

/*  The algorithm starts at the beginning of the data set.
    It compares the first two elements, and if the first is greater than the second, it swaps them.
    It continues doing this for each pair of adjacent elements to the end of the data set.
    It then starts again with the first two elements, repeating until no swaps have occurred on the last pass.
*/
void swap(uint32_t *a, uint32_t *b)
{
    uint32_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void sort_bubble(uint32_t *num, uint8_t size)
{
    bool swaps; 
    do {
        swaps = false;
        for (int i = 0; i < size; i++) {
            if (num[i] > num[i + 1]) {
                swap(&num[i], &num[i + 1]);
                swaps = true;
            }
        }
    } while (swaps == true);
}

void sort_bubble_d(uint32_t *num, uint8_t size)
{
    bool swaps; 
    do {
        swaps = false;
        for (int i = 0; i < size-1; i++) {
            if (num[i] < num[i + 1]) {
               swap(&num[i], &num[i + 1]);
                swaps = true;
            }
        }
    } while (swaps == true);
}

/*
 1. Suppose there exists a function called Insert designed to insert a value into a sorted sequence at the beginning of an array.
    It operates by beginning at the end of the sequence and shifting each element one place to the right until a suitable position is found for the new element.
    The function has the side effect of overwriting the value stored immediately after the sorted sequence in the array.
 2. To perform an insertion sort, begin at the left-most element of the array and invoke Insert to insert each element encountered into its correct position.
    The ordered sequence into which the element is inserted is stored at the beginning of the array in the set of indices already examined.
    Each insertion overwrites a single value: the value being inserted.
*/
void sort_insertion(uint32_t *num, uint8_t size)
{
    //implement me!
}

void sort_insertion_d(uint32_t *num, uint8_t size)
{
    //implement me!
}







/*
    The algorithm divides the input list into two parts: 
        a sorted sublist of items which is built up from left to right at the front (left) of the list
        and a sublist of the remaining unsorted items that occupy the rest of the list.
    Initially, the sorted sublist is empty and the unsorted sublist is the entire input list.
    The algorithm proceeds by finding the smallest (or largest, depending on sorting order) element in the unsorted sublist,
        exchanging (swapping) it with the leftmost unsorted element (putting it in sorted order),
        and moving the sublist boundaries one element to the right.
*/
void sort_selection(uint32_t *num, uint8_t size)
{
    int bound = 0;


}





/*
    Conceptually, a merge sort works as follows:
    Divide the unsorted list into n sublists, each containing one element (a list of one element is considered sorted).
    Repeatedly merge sublists to produce new sorted sublists until there is only one sublist remaining. This will be the sorted list.
    Note: Top-down vs. bottom-up implementation.
*/
void sort_merge(uint32_t *num, uint8_t size)
{
    //implement me!
}









/*
    Shellsort is an optimization of insertion sort that allows the exchange of items that are far apart.
    The idea is to arrange the list of elements so that, starting anywhere, taking every hth element produces a sorted list.
    Such a list is said to be h-sorted. 
    It can also be thought of as h interleaved lists, each individually sorted. 
    Beginning with large values of h allows elements to move long distances in the original list, reducing large amounts of disorder quickly,
    and leaving less work for smaller h-sort steps to do. 
    If the list is then k-sorted for some smaller integer k, then the list remains h-sorted.
    Following this idea for a decreasing sequence of h values ending in 1 is guaranteed to leave a sorted list in the end.
*/
void sort_shell(uint32_t *num, uint8_t size)
{
    //implement me!
}









/*
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
void sort_quick(uint32_t *num, uint8_t size)
{
    //implement me!
}




