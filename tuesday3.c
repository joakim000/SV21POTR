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

void bitwise() {
    unsigned char a = 'a';
    // long biggus = 9223372036854775806;


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


void stringBits(size_t const size, void const * const ptr, char out[]) {
    unsigned char *byte = (unsigned char*) ptr;
    unsigned char bit;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            bit = (byte[i] >> j) & 1;
            // bit ? out[j] = '1' : out[j] = '0';
            if (!bit)
                out[i * 8 + j] = '0';
            else
                out[i * 8 + j] = '1';
        }
    }
    out[size*8] = 0;
    strrev(out);
}

void beepbeepbeep() {
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
  
    // 3b. Reverse the bit order of unsinged char. ie 1000 0000 -> 0000 0001 
    unsigned char a = 'a';          // a   01100001   => 10000110
    char bits[sizeof(a) * 8];
    stringBits(sizeof(a), &a, bits);
    printf("%c   %s\n", a, bits);

    a = ~a;
    stringBits(sizeof(a), &a, bits);
    printf("%c   %s\n", a, bits);

                                
}




void main() {
    // primes();

    // bitwise();

    beepbeepbeep();

}


/*
Trial A
2 20000001 40000001 60000001 80000001 100000001 120000001 140000001 160000001 180000001
 1.121 seconds
Trial B
2 20000001 40000001 60000001 80000001 100000001 120000001 140000001 160000001 180000001
 1.378 seconds
Erastophanes:
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97
Total found: 100556393
 23.536 seconds
*/

