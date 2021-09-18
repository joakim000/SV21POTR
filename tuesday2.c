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

/* Primality 1: Trial division
    A. Dela varje kandidat med varje heltal mindre än kandidatens kvadratrot. Hittas
    ingen division utan rest är kandidaten ett primtal.

    B. Som A men varje primtal istället för varje heltal.

    First 60 primes:
    2	3	5	7	11	13	17	19	23	29	31	37	41	43	47	53	59	61	67	71
    73	79	83	89	97	101	103	107	109	113	127	131	137	139	149	151	157	163	167	173
    179	181	191	193	197	199	211	223	227	229	233	239	241	251	257	263	269	271	277	281
*/
int* trial;
void trial_A(int findCount) {
    int* primes;
    primes = calloc(findCount, sizeof(int));
    assert(primes != NULL);
    int found = 0;
    bool prime;
    int candidate = 2;
    start = clock();
    printf("trial: innan while");
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
    for (int i = 0; i < found; i += findCount/10) {
        printf("%d ", primes[i]);
    }
    printf("\n %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);

    trial = calloc(found, sizeof(int));
    for (int i = 0; i < found; i++) {      
        *(trial + i) = primes[i];
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
        float root = sqrt(candidate);
        for (int i = 0; i < found && (float)primes[i] <= root; i++) {
            if (primes[i] != candidate && candidate % primes[i] == 0)
                prime = false;
                break;
        }
        if (prime) {
            primes[0+found] = candidate;
            found++;
        }    
        candidate++;
    }
    end = clock();
    printf("Trial B:\n");
    // for (int i = 0; i < found; i += findCount/10) {
    for (int i = 19999990; i < 20000040; i++) {
        printf("%d ", primes[i]); 
    }
    printf("\n %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);
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
int* erasto;
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
            if (i > 2000000 && i < 2000100)
            printf("%d ", i);
        }
    }
    printf("\nTotal found: %d", count);
    printf("\n %5.3f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);
    
    erasto = calloc(count, sizeof(int));
    int j = 0;
    for (int i = 2; i < size; i++) {
        if (s[i].prime) {
            *(erasto + j) = i;
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




int main() {
    erastophanes(200000000);
    


    
    trial_A(11000000);


    for (int i = 0; i < 20; i++) 
       printf("%d ", *(erasto + i)); 
    printf("\n");
    for (int i = 0; i < 20; i++) 
       printf("%d ", *(trial + i)); 


    // trial_B(11000000);
    // erastophanes(2050000000);
    

    return 0;
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

