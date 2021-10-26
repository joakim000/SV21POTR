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

// #define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// #define PAD 15
    // Hex    Dec    LSF                MSF               
    // 0xC599 50585  1001100110100011   1100010110011001                
    //                                  1100010110011001

// #define POLYNOMIAL 0x39
// #define PAD 8
#define POLYNOMIAL 0xB
#define PAD 3



int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize );
void messageLengthCheck(size_t len);
void printBits(char label[], uint8_t bits[], size_t size);
bool validate(uint8_t message[], size_t msgSize, int16_t checksum, size_t padSize, uint8_t genBits[], size_t genSize);
void validPrint(uint8_t msg[], size_t msgSize, bool valid);


int main(void)
{
    // Expected checksum value for testing checksum calculation. Skips check when set to 0.
    uint16_t expected = 0; 

    // Define inputs
    uint16_t gen = POLYNOMIAL;
    uint8_t padSize = PAD;

    uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'}; // 15 zeros have been appended to the message
    expected = 0xB35;
    // uint8_t message[] = {'A', 'B'}; // 15 zeros have been appended to the message
    // expected = 0x54FB;

    /* Test CRC-3 Wikipedia and assignment example) 
       Message + padSize: 11010011101100 000   gen: 1011 
    */
    // uint8_t msgBits[] = {1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0};
    // uint8_t message[3];
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
    ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, NULL);
    printBits("Message", msgBits, COUNT_OF(msgBits));

    // uint8_t testmsg[COUNT_OF(msgBits) / sizeof(uint8_t) * 8];
    // bits2intsMSFTest(COUNT_OF(msgBits), sizeof(uint8_t), msgBits, testmsg );
    // char teststr[COUNT_OF(testmsg) + 1];
    // charArrayToString(testmsg, COUNT_OF(testmsg), teststr);
    // puts("test of general bits2ints");
    // puts(teststr);

    // Convert generator polynomial to array of bit values
    uint8_t genBits[sizeof(gen) * 8];
    int2bits(sizeof(gen), &gen, genBits);
    printBits("Generator", genBits, COUNT_OF(genBits));

    // Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB
    // Hex    Dec    LSF                MSF               
    // 0xB35   2869  1010110011010000   0000101100110101               
    // 0x54FB 21755  1101111100101010   0101010011111011    

    // Get checksum
    int16_t checkSum = getRem(msgBits, COUNT_OF(msgBits), genBits, COUNT_OF(genBits), padSize);
    printf("Checksum: 0x%x\n", checkSum);

    // Compare to expected checksum
    if (expected) {
        uint8_t expectedBits[sizeof(expected) * 8];
        int2bits(sizeof(expected), &expected, expectedBits);
        printBits("Expected", expectedBits, COUNT_OF(expectedBits));
        printf("Expected: 0x%x\n", expected);
        if (checkSum != expected) 
            printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
    }

    // Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements.
    // Validate the messsage.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"
    bool valid;

    // Test: Validate original message
    valid = validate(message, COUNT_OF(message), checkSum, padSize, genBits, COUNT_OF(genBits));   
    validPrint(message, COUNT_OF(message), valid);
  
    // Validate changed message
    message[1] = 'a';
    valid = validate(message, COUNT_OF(message), checkSum, padSize, genBits, COUNT_OF(genBits));   
    validPrint(message, COUNT_OF(message), valid);

    
    return 0;
}

int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize ) {
    for (int i = 0; i < msgSize - padSize; i++) 
        if (msgBits[i]) 
            for (int j = 0, k = i; j < genSize; j++, k++) 
                msgBits[k] = msgBits[k] ^ genBits[j];
    if (VERBOSE) { puts("Message post calculation"); i2p(msgBits, msgSize, 0, 2);  }

    uint8_t remBits[padSize];
    bitSlice(-1, padSize, msgBits, msgSize, remBits);
    printBits("Remainder", remBits, COUNT_OF(remBits));

    uint16_t rem = (uint16_t)bits2int(COUNT_OF(remBits), remBits);
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

void printBits(char label[], uint8_t bits[], size_t size) {
    if (VERBOSE) { 
        if (size > 40) {
            printf("%10s bits (%d):\n", label, size);
            i2p(bits, size, 0, 2); 
        }
        else {
            printf("%10s bits (%d): ", label, size);
            i2p(bits, size, 0, 1);
        } 
    }
}

bool validate(uint8_t message[], size_t msgSize, int16_t checksum, size_t padSize, uint8_t genBits[], size_t genSize) {
    // Create working copy
    uint8_t msg[msgSize];
    for (int i = 0; i < msgSize; i++)
        msg[i] = message[i];

    // Convert checksum to array of bit values
    uint8_t checksumBitsRaw[sizeof(checksum) * 8];
    int2bits(sizeof(checksum), &checksum, checksumBitsRaw);

    // Crop to padding size
    uint8_t checksumBits[padSize];
    bitSlice(-1, padSize, checksumBitsRaw, COUNT_OF(checksumBitsRaw), checksumBits);
    printBits("Cs recon", checksumBits, COUNT_OF(checksumBits));
    
    // Extra checking step: Convert checksum bits back to checksum, check that it matches provided checksum
    uint16_t checksumRecon = (uint16_t)bits2int(COUNT_OF(checksumBits), checksumBits);
    if (VERBOSE) printf("Rem recon: 0x%x\n", checksumBits);
    assert( ("Checksum error", checksumRecon == checksum) );

    // Convert msg to array of bit values, add remainder bits as padding
    uint8_t msgBits[sizeof(msg) * 8 + padSize];
    ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    printBits("Checksummed message", msgBits, COUNT_OF(msgBits));

    int16_t rem = getRem(msgBits, COUNT_OF(msgBits), genBits, genSize, padSize);
    if (VERBOSE) printf("Remainder: 0x%x\n", rem);

    if (!rem) 
        return true;
    else
        return false;
}

void validPrint(uint8_t msg[], size_t msgSize, bool valid) {
    char msgStr[msgSize + 1];
    charArrayToString(msg, msgSize, msgStr);
    printf("\nMessage to validate:\n%s\n", msgStr);
    if (valid) 
        printf("\e[1;32m%s\e[m\n", "The data is OK"); // green
    else
        printf("\e[1;31m%s\e[m\n", "The data is not OK"); // red
}