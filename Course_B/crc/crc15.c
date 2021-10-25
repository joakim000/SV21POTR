#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "binutils.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// 12x^3 + 5x^2 + 9x + 9



int main(void)
{
    uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // 15 zeros have been appended to the message
    
    // Convert message to array of bit values
    uint8_t messageBits[sizeof(message) * 8];
    ints2bitsLSF(sizeof(message), sizeof(message[0]), &message, messageBits);

    // Convert polynomial to array of bit values
    uint16_t poly = POLYNOMIAL;
    uint8_t polyBits[sizeof(poly) * 8];
    int2bitsLSF(sizeof(poly), &poly, polyBits);

    // Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB
    


    // Checksum the message. I.e replace the zeros with the CRC accroding to the requirements.

    message[1] = 'a';

    // Validate the message.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"

    return 0;
}
