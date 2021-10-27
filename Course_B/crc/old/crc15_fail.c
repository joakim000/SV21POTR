#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "binutils.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

// Flags
#define VERBOSE true
#define PRINTMSG true
#define PRINTSTEPS true

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
#define PAD 15
#define MSGLSF false
    // Hex    Dec    LSF                MSF               
    // 0xC599 50585  1001100110100011   1100010110011001                
    //                                  1100010110011001

// #define PAD 3
// #define POLYNOMIAL 0xB

// #define PAD 8
// #define POLYNOMIAL 0x07
// #define POLYNOMIAL 0x39
// #define POLYNOMIAL 0xD5
// #define POLYNOMIAL 0x107 // 100000111



// #define PAD 16
// #define POLYNOMIAL 0x8005
// #define POLYNOMIAL 0x8005

// #define PAD 32
// #define POLYNOMIAL 0x04C11DB7
// #define POLYNOMIAL 0x814141AB

void checksumMsg(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t msgBitsCS[], size_t msgBitsCSSize);
int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize );
void messageLengthCheck(size_t len);
// bool validate(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t genBits[], size_t genSize);
bool validate(uint8_t msgBits[], size_t msgBitsSize, size_t padSize, uint8_t genBits[], size_t genSize);
void validPrint(uint8_t msg[], size_t msgSize, bool valid);


int main(void)
{
    // Expected checksum value for testing checksum calculation. Skips check when set to 0.
    uint32_t expected = 1; 

    // Define inputs
    uint32_t gen = POLYNOMIAL;
    uint8_t padSize = PAD;

    // uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\n'};
    // uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    // expected = 0xB35;
    // expected = EXPECTED;
    uint8_t message[] = {'A', 'B', 'C'}; // 15 zeros have been appended to the message
    // expected = 0x54FB;

    // uint8_t message[] = {'@'};
    // expected = 0x0;  //CRC-8
    // uint8_t message[] = {'@', '@'};
    // expected = 0xC7; //CRC-8 0x07
    // expected = 0xD6;  //CRC-3 0xB
    // uint8_t message[] = {'A', 'A'};
    // expected = 0xC0;  //CRC-8 0x07
    // expected = 0xDD;  //CRC-3 0xB

    // Wikipedia 2
    // uint8_t message[] = {'W'};
    // expected = 0xA2;  //CRC-8 0x107 MSF   10100010
    // expected = 0x98;  //CRC-8 0x107 LSF   10011000
    


    /* Test CRC-3 Wikipedia and assignment example) 
       Message + padSize: 11010011101100 000   gen: 1011 
    */
    // uint8_t msgBits[] = {1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0};
    // uint8_t genBits[] = {1, 0, 1, 1};
    // padSize = 3;
    // expected = 0x4;

    // Set convert functions to use MSF or LSF
    setMSF();

    // Assignment requirements for message length
    messageLengthCheck(COUNT_OF(message));

    // Create working copy
    uint8_t msg[COUNT_OF(message)];
    for (int i = 0; i < COUNT_OF(message); i++)
        msg[i] = message[i];

    // Convert msg to array of bit values, add padding zeroes
    uint8_t msgBits[sizeof(msg) * 8 + padSize];
    if (MSGLSF)
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, NULL);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, NULL);
    if (VERBOSE) printBits("Message", msgBits, COUNT_OF(msgBits));

     // Convert generator polynomial to array of bit values
    uint8_t genBitsRaw[sizeof(gen) * 8];
    int2bits(sizeof(gen), &gen, genBitsRaw);
    // Crop to size
    uint8_t genBits[padSize + 1];

    for (int i = COUNT_OF(genBits) - 1, j = COUNT_OF(genBitsRaw) - 1; i >= 0; i--, j--)
        genBits[i] = genBitsRaw[j];
    // for (int i = 0; i < COUNT_OF(genBits); i++)
    //     genBits[i] = genBitsRaw[i];
    // bitSlice(-1, padSize, genBitsRaw, COUNT_OF(genBitsRaw), genBits);
    
    if (VERBOSE || expected) {
        printf("Generator: 0x%X\n", gen);
        printBits("GeneratorRaw", genBitsRaw, COUNT_OF(genBitsRaw));
        printBits("Generator", genBits, COUNT_OF(genBits));
    }
    // Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB
    // Hex     LSF                MSF               
    // 0xB35   1010110011010000   0000101100110101               
    // 0x54FB  1101111100101010   0101010011111011    

    // Print original message before checksum
    if (PRINTMSG) {
        char msgStr[COUNT_OF(message) + 1];
        charArrayToString(msg, COUNT_OF(message), msgStr);
        printf("\nOriginal message:\n%s\n", msgStr);
    }
    // Get checksum
    int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), genBits, COUNT_OF(genBits), padSize);
    printf("Checksum: 0x%X\n", checksum);

    // Compare to expected checksum
    if (expected) {
        uint8_t checksumBits[sizeof(checksum) * 8];
        int2bits(sizeof(checksum), &checksum, checksumBits);
        printBits("Checksum", checksumBits, COUNT_OF(checksumBits));

        uint8_t expectedBits[sizeof(expected) * 8];
        int2bits(sizeof(expected), &expected, expectedBits);
        printf("Expected: 0x%X\n", expected);
        printBits("Expected", expectedBits, COUNT_OF(expectedBits));
        if (checksum != expected) 
            printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
    }

    // Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements.
    uint8_t msgBitsCS[COUNT_OF(message) * 8 + padSize];
        
    checksumMsg(message, COUNT_OF(message), checksum, padSize, msgBitsCS, COUNT_OF(msgBitsCS));

    // Validate the messsage.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"
    bool valid;

    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), padSize, genBits, COUNT_OF(genBits));   
    validPrint(message, COUNT_OF(message), valid);
  
    // Changed message
    message[1] = 'a';
    checksumMsg(message, COUNT_OF(message), checksum, padSize, msgBitsCS, COUNT_OF(msgBitsCS));
    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), padSize, genBits, COUNT_OF(genBits));   
    validPrint(message, COUNT_OF(message), valid);
    
    return 0;
}

int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize ) {
    if (PRINTSTEPS) {
        printf(" Before: "); i2p(msgBits, msgSize, 0, 1); 
        for (int i = 0; i < msgSize - padSize; i++) 
            if (msgBits[i]) {
                for (int j = 0, k = i; j < genSize; j++, k++) 
                    msgBits[k] = msgBits[k] ^ genBits[j];
                printf("Step %2d: ", i); i2p(msgBits, msgSize, 0, 1);   
            }   
        printf("  After: "); i2p(msgBits, msgSize, 0, 1);
    }
    else 
        for (int i = 0; i < msgSize - padSize; i++) 
            if (msgBits[i]) 
                for (int j = 0, k = i; j < genSize; j++, k++) 
                    msgBits[k] = msgBits[k] ^ genBits[j];

    if (VERBOSE) { puts("Message post calculation"); i2p(msgBits, msgSize, 0, 2);  }

    uint8_t remBits[padSize];
    bitSlice(-1, padSize, msgBits, msgSize, remBits);
    if (VERBOSE) printBits("Remainder", remBits, COUNT_OF(remBits));

    uint32_t rem = (uint32_t)bits2int(COUNT_OF(remBits), remBits);
    if (VERBOSE) printf("Remainder: 0x%x\n", rem);
    
    return rem;
}

void messageLengthCheck(size_t len) { 
    if (len < 1) {
        fprintf(stderr, "Message subceeds 1 characters (%d), exiting.", len);
        exit(EXIT_FAILURE);
    }
    if (len > 14) {
        fprintf(stderr, "Message exceeds 14 characters (%d), exiting.", len);
        exit(EXIT_FAILURE);
    }
}



void checksumMsg(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t msgBitsCS[], size_t msgBitsCSSize) {
    // Create working copy
    // uint8_t msg[msgSize];
    // for (int i = 0; i < msgSize; i++)
    //     msg[i] = message[i];

    // Convert checksum to array of bit values
    uint8_t checksumBitsRaw[sizeof(checksum) * 8];
    int2bits(sizeof(checksum), &checksum, checksumBitsRaw);

    // Crop to padding size
    uint8_t checksumBits[padSize];

    for (int i = COUNT_OF(checksumBits) - 1, j = COUNT_OF(checksumBitsRaw) - 1; i >= 0; i--, j--)
        checksumBits[i] = checksumBitsRaw[j];
    // bitSlice(-1, padSize, checksumBitsRaw, COUNT_OF(checksumBitsRaw), checksumBits);

    if (VERBOSE) printf("Checksum: 0x%x\n", checksum);
    if (VERBOSE) printBits("Checksum", checksumBits, COUNT_OF(checksumBits));
    
    // Extra checking step: Convert checksum bits back to checksum, check that it matches provided checksum
    uint32_t checksumRecon = (uint32_t)bits2int(COUNT_OF(checksumBits), checksumBits);
    if (VERBOSE) printf("Cs recon: 0x%x\n", checksumRecon);
    assert( ("Checksum error", checksumRecon == checksum) );

    // Convert msg to array of bit values, add remainder bits as padding
    // uint8_t msgBits[sizeof(message) * 8 + padSize];
    if (MSGLSF)
        ints2bitsLSF(msgSize, sizeof(message[0]), &message, msgBitsCS, padSize, checksumBits);
    else
        ints2bits(msgSize, sizeof(message[0]), &message, msgBitsCS, padSize, checksumBits);
    if (VERBOSE) printBits("Checksummed message", msgBitsCS, msgBitsCSSize);

}

bool validate(uint8_t msgBits[], size_t msgBitsSize, size_t padSize, uint8_t genBits[], size_t genSize) {
    int32_t rem = getRem(msgBits, msgBitsSize, genBits, genSize, padSize);
    // if (VERBOSE) printf("Remainder: 0x%x\n", rem);
    if (!rem) 
        return true;
    else
        return false;
}

void validPrint(uint8_t msg[], size_t msgSize, bool valid) {
    if (PRINTMSG) {
        char msgStr[msgSize + 1];
        charArrayToString(msg, msgSize, msgStr);
        printf("\nMessage to validate:\n%s\n", msgStr);
    }
    if (valid) 
        printf("\e[1;32m%s\e[m\n", "The data is OK"); // green
    else
        printf("\e[1;31m%s\e[m\n", "The data is not OK"); // red
}