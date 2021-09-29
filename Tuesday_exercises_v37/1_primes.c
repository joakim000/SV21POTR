#include "devheaders.h"

/* 
    Three primality checking functions 
    with test function (function + performance) 

    Performance differs significantly, as does 
    performance gains with optimization (-O0 vs. -O3).
    C.f example output in comment below. 

*/

// For storing results
int* trial_A_primes; int* trial_B_primes; int* erasto_primes;

/* Primality 1: Trial division
    A. Dela varje kandidat med varje heltal mindre än kandidatens kvadratrot. Hittas
    ingen division utan rest är kandidaten ett primtal.

    B. Som A men varje primtal istället för varje heltal.
*/

void trial_A(int findCount) {
    int* primes;
    primes = calloc(findCount, sizeof(int));
    assert(primes != NULL);
    int found = 0;
    bool prime;
    int candidate = 2;
    timer_start = clock();
    while (found < findCount) {
        prime = true;
        int root = (int)sqrt(candidate);
        for (int i = 2; i <= root; i++) {
            if (i != candidate && candidate % i == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            primes[0+found] = candidate;
            found++;
        }    
        candidate++;
    }
    timer_end = clock();
    printf("Trial A:\n");
    printf("Found %d", findCount);
    printf(" in %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);

    trial_A_primes = calloc(found, sizeof(int));
    assert( ("Memory allocation failed.", trial_A_primes != NULL) );
    for (int i = 0; i < found; i++) {      
        *(trial_A_primes + i) = primes[i];
    }
    free(primes);
}


void trial_B(int findCount) {
    int* primes;
    primes = calloc(findCount, sizeof(int));
    assert(primes != NULL);
    primes[0] = 2;
    int found = 1;
    bool prime;
    int candidate = 3;
    timer_start = clock();
    while (found < findCount) {
        prime = true;
        int root = (int)sqrt(candidate);
        for (int i = 0; i < found && primes[i] <= root; i++) {
            if (primes[i] != candidate && candidate % primes[i] == 0) {
                prime = false;
                break;
            }   
        }
        if (prime) {
            primes[0+found] = candidate;
            found++;
        }    
        candidate++;
    }
    timer_end = clock();
    printf("Trial B:\n");
    printf("Found %d", findCount);
    printf(" in %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);
    trial_B_primes = calloc(found, sizeof(int));
    assert( ("Memory allocation failed.", trial_B_primes != NULL) );
    for (int i = 0; i < found; i++) {      
        *(trial_B_primes + i) = primes[i];
    }
    free(primes);
}


/* Primality 2: Erastophanes såll
    Gör en lista: 2 till n. 
    Låt alla vara omarkerade.
    Upprepa:
        Markera första omarkerade tal som primtal.
        Markera alla multipler av detta tal som komposittal.
    Till: Alla primtal mindre än kvadratroten av n markerats. 
    Alla omarkerade tal är primtal.
*/

void erastophanes(int size) {
    typedef struct sieveNode {
        bool prime;
        bool comp;
    } hole;
    
    // Init
    int limit = (int)sqrt(size);
    hole* s;
    s = calloc(size, sizeof(hole));
    assert(s != NULL);
    for (int i = 2; i < size; i++) {
        s[i].prime = false;
        s[i].comp = false;
    }
    // Init unused indexes to safe values
    for (int i = 0; i < 2; i++) {
        s[i].prime = false;
        s[i].comp = true;
    }

    // Sieve    
    timer_start = clock();
    for (int i = 2; i < size; i++) {
        if ( !(s[i].prime || s[i].comp) ) { 
            s[i].prime = true;

        if (i < limit)
            for (int j = i + i; j < size; j += i) 
                s[j].comp = true;    
        }
    }
    timer_end = clock();

    // Print
    printf("Erastophanes:\n");
    int count = 0;
    for (int i = 2; i < size; i++) {
        if (s[i].prime) {
            count++;
        }
    }
    printf("Found %d", count);
    printf(" in %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);
    
    erasto_primes = calloc(count, sizeof(int));
    assert( ("Memory allocation failed.", erasto_primes != NULL) );
    // int j = 0;
    for (int i = 2, j = 0; i < size; i++) {
        if (s[i].prime) {
            *(erasto_primes + j) = i;
            j++;
        }
    }
    free(s);
}

/* Test function for primality checkers 
    Shows approximate run times
    Checks for mutual consistency

    Example output (-O0):
    ---
    Trial A:
    Found 250000 in 1.990 seconds
    Trial B:
    Found 800000 in 1.997 seconds
    Erastophanes:
    Found 12902018 in 3.368 seconds

    Trial A: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    Trial B: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
     Erasto: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    Compared 250000 numbers.
    ---
    Example output (-O3):
    ---
    Trial A:
    Found 250000 in 1.977 seconds
    Trial B:
    Found 800000 in 1.982 seconds
    Erastophanes:
    Found 12902018 in 1.942 seconds

    Trial A: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    Trial B: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
     Erasto: 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    Compared 250000 numbers.
    ---
*/


void primes() {
    trial_A(250000);
    trial_B(800000);    
    erastophanes(235000000);
    // erastophanes(142000000);
    
    int showLength = 20;
    int compareLength = 250000;
    printf("\nTrial A: ");
    for (int i = 0; i < showLength; i++) 
       printf("%d ", *(trial_A_primes + i)); 
    printf("\nTrial B: ");
    for (int i = 0; i < showLength; i++) 
       printf("%d ", *(trial_B_primes + i)); 
    printf("\n Erasto: ");
    for (int i = 0; i < showLength; i++) 
       printf("%d ", *(erasto_primes + i));  

    for (int i = 0; i < compareLength; i++) {
        if (! ( erasto_primes[i] == trial_A_primes[i] && erasto_primes[i] == trial_B_primes[i] )) {
            printf("\nInconsistency at index:%d \n");
            break;
        }
    } 
    printf("\nCompared %d numbers.", compareLength);

    free(erasto_primes);
    free(trial_A_primes);
    free(trial_B_primes);
}

void main() {
    /* 1 */
    primes();

}
