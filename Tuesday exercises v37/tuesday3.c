#include "devheaders.h"

/* Tuesday exercises

 ## Prime numbers.
1. a. Printout the first 100 primes.
   b. Printout every tenth prime of the first 1000 primes.
   
## Simple Bitwise.
2. a. Print an unsinged char as its binary representation.
   b. Print the binary representation of a number up to 64bits.
   Need to know how big the number is in bytes for B. sizeof() helps.

## Reversing
3. a. Reverse a string with a maximum of 213 letters.
   b. Reverse the bit order of unsinged char. ie 1000 0000 -> 0000 0001

## Sorting
4. Sort an array of unsigned ints. Make two functions, one Ascending, one Descending.
   To get you started: sorting.c

*/


/* Test function for primality checkers 
    Shows approximate run times
    Checks for mutual consistency

    Example output:
    ---
    Trial A:
    Found 250000 in 1.977 seconds
    Trial B:
    Found 800000 in 1.982 seconds
    Erastophanes:
    Found 12902018 in 1.942 seconds

    2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71
    Compared 250000 numbers.
    ---
*/
int* trial_A_primes; int* trial_B_primes; int* erasto_primes;
void trial_A(int findCount), trial_B(int findCount), erastophanes(int size);

void primes() {
    trial_A(250000);
    trial_B(800000);    
    erastophanes(235000000);
    
    int showLength = 20;
    int compareLength = 250000;
    printf("\n");
    for (int i = 0; i < showLength; i++) 
       printf("%d ", *(erasto_primes + i));  
    printf("\n");
    for (int i = 0; i < showLength; i++) 
       printf("%d ", *(trial_A_primes + i)); 
    printf("\n");
    for (int i = 0; i < showLength; i++) 
       printf("%d ", *(trial_B_primes + i)); 

    for (int i = 0; i < compareLength; i++) {
        if (! ( erasto_primes[i] == trial_A_primes[i] && erasto_primes[i] == trial_B_primes[i] )) 
            printf("\nInconsistency at index:%d \n");
    } 
    printf("\nCompared %d numbers.", compareLength);

    free(erasto_primes);
    free(trial_A_primes);
    free(trial_B_primes);
}


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
    start = clock();
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
    end = clock();
    printf("Trial A:\n");
    printf("Found %d", findCount);
    printf(" in %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);

    trial_A_primes = calloc(found, sizeof(int));
    assert(trial_A_primes != NULL);
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
    start = clock();
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
    end = clock();
    printf("Trial B:\n");
    printf("Found %d", findCount);
    printf(" in %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);
    trial_B_primes = calloc(found, sizeof(int));
    assert(trial_B_primes != NULL);
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
    start = clock();
    for (int i = 2; i < size; i++) {
        if ( !(s[i].prime || s[i].comp) ) { 
            s[i].prime = true;

        if (i < limit)
            for (int j = i + i; j < size; j += i) 
                s[j].comp = true;    
        }
    }
    end = clock();

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
    assert(erasto_primes != NULL);
    int j = 0;
    for (int i = 2; i < size; i++) {
        if (s[i].prime) {
            *(erasto_primes + j) = i;
            j++;
        }
    }
    free(s);
}

/*
## Simple Bitwise.
2. a. Print an unsinged char as its binary representation.
   b. Print the binary representation of a number up to 64bits.
   Need to know how big the number is in bytes for B. sizeof() helps.
*/

void char2bin(unsigned char c) {
    int recons = 0;
    for (int i = 7; i >= 0; i--) {
        int res = (int)c & (int)pow(2,i);
        if (res) {
            printf("%d", 1);
            recons += (int)pow(2,i);
        }
        else {
            printf("%d", 0);
        }
    }
    printf("\n%3d %3c   reconstruct: %3d %3c\n", c, c, recons, recons);
}

void big2bin(long long biggus) {
    // -9223372036854775808 9223372036854775807

    printf("\nSize: %3d bytes\n", sizeof(biggus));
    long long recons = 0;
    for (int i = 63; i >= 0; i--) {
        long long res = biggus & (unsigned long long)pow(2,i);
        if (res) {
            printf("%d", 1);
            recons += (long long)pow(2,i);
        }
        else {
            printf("%d", 0);
        }
    }
    printf("\n%lld   reconstruct: %lld\n", biggus, recons);
}

void ubig2bin(unsigned long long biggus) {
    // 18446744073709551615

    printf("\nSize: %3d bytes\n", sizeof(biggus));
    unsigned long long recons = 0;
    for (int i = 63; i >= 0; i--) {
        unsigned long long res = biggus & (unsigned long long)pow(2,i);
        if (res) {
            printf("%d", 1);
            recons += (unsigned long long)pow(2,i);
        }
        else {
            printf("%d", 0);
        }
    }
    printf("\n%llu   reconstruct: %llu\n", biggus, recons);
}

/*
## Reversing
3. a. Reverse a string with a maximum of 213 letters.
   b. Reverse the bit order of unsinged char. ie 1000 0000 -> 0000 0001
*/
void my_strrev(char str[]) {  
    char tmp[strlen(str)];
    strcpy(tmp, str);

    // Reverse
    int j = 0;
    for (int i = strlen(str)-1; i >= 0; i--) {
        str[j] = tmp[i];
        j++;
    }
} 

void reverseString() {
    // 3a. Reverse a string with a maximum of 213 letters.
    char str213[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThe";
    char str214[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogTheQ";
    if (strlen(str213) <= 213)
        strrev(str213);
    if (strlen(str214) <= 213)
        strrev(str214);
    printf("\nLib reverse:\n213: %s\n214: %s", str213, str214);
    

    char str213_2[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThe";
    char str214_2[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogTheQ";
    if (strlen(str213_2) <= 213)
        strrev(str213_2);
    if (strlen(str214_2) <= 213)
        strrev(str214_2);
    printf("\nMy reverse:\n213: %s\n214: %s\n\n", str213_2, str214_2);
  
}

void reverseChar(unsigned char c) {
    // 3b. Reverse the bit order of unsinged char. ie 1000 0000 -> 0000 0001 
    bool bits[8];
    int recons = 0;
    for (int i = 7; i >= 0; i--) {
        int res = (int)c & (int)pow(2,i);
        if (res) {
            printf("%d", 1);
            recons += (int)pow(2,i);
            bits[i] = true;
        }
        else {
            printf("%d", 0);
            bits[i] = false;
        }
    }
    printf("\n%3d %3c   reconstruct: %3d %3c\n", c, c, recons, recons);

    // Reverse
    int rev = 0;
    for (int i = 0; i < 8; i++) {
        if (bits[i]) {
            printf("%d", 1);
        }
        else {
            printf("%d", 0);
        }
    }
    int j = 0;
    for (int i = 7; i >= 0; i--) {
        if (bits[i]) {
            rev += (int)pow(2,j);
        }
        j++;
    }
    printf("\n%3d %3c   reverse: %3d %3c", c, c, rev, rev);
}



void main() {
    /* 1 */
    // primes();

    /* 2a */
    // char2bin('a');

    /* 2b */
    big2bin(-9223372036854775808);
    ubig2bin(18446744073709551615);

    /* 3a */
    // reverseString();
    
    /* 3b */
    // reverseChar('a');

}


