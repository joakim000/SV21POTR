#include "devheaders.h"

/* Tuesday exercises
  
## Simple Bitwise.
2. a. Print an unsinged char as its binary representation.
   b. Print the binary representation of a number up to 64bits.
   Need to know how big the number is in bytes for B. sizeof() helps.
*/

/* Note: Test values will raise default warning:
        "warning: integer constant is so large that it is unsigned"
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


void main() {
    /* 2a */
    char2bin('a');

    /* 2b */
    big2bin(-9223372036854775808);
    ubig2bin(18446744073709551615);
  
}



