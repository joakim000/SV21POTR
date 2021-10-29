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

// #define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// #define PAD 15
#define MSGLSF false
#define CRCLSF false
    // Hex    Dec    LSF                MSF               
    // 0xC599 50585  1001100110100011   1100010110011001                
    //                                  1100010110011001

#define INITIAL 0x0

#define IMPLICIT_LEADING_1 1
// #define PAD 3
// #define POLYNOMIAL 0xB

// #define PAD 8
// #define POLYNOMIAL 0x07  // i1 0111      = 0x17
// #define POLYNOMIAL 0x39  // i1 0011 1001 = 0x139
// #define POLYNOMIAL 0xD5  // i1 1101 0101 = 0x1D5
// #define POLYNOMIAL 0x107 //  1 0000 0111
// #define POLYNOMIAL 0x1D

#define PAD 16
#define POLYNOMIAL 0x8005
// #define POLYNOMIAL 0x1021


// #define PAD 32
// #define POLYNOMIAL 0x04C11DB7
// #define POLYNOMIAL 0x814141AB
// #define POLYNOMIAL 0xAF

void checksumMsg(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t msgBits[]);
int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize, size_t orginalMsgSize );
void messageLengthCheck(size_t len);
bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t padSize, uint8_t genBits[], size_t genSize, size_t originalMsgSize);
void validPrint(uint8_t msg[], size_t msgSize, bool valid);


typedef struct prog_s {
    // Flags
    bool verbose;
    bool printmsg;
    bool printsteps;
} prog_t;
typedef struct crc_s {
    // Definition
    uint8_t n;           // Width of CRC (8, 16 or 32 bit)
    uint32_t g;          // Generator polynomial
    bool implicit_leading_1;
    uint32_t initial;    // Initial CRC value
    uint32_t finalxor;   // Final XOR value
    bool inputLSF;       // Input reflected
    bool resultLSF;      // Result reflected
    // Work
    uint8_t* gbits;
    size_t gbits_size;
    uint8_t* gbitsRaw;
    size_t gbitsRaw_size;
    uint8_t* padbits;
    size_t padbits_size; // = n
    uint8_t* padbitsRaw;
    size_t padbitsRaw_size; 
    uint8_t* xorbits;
    size_t xorbits_size; // = n
    uint8_t* xorbitsRaw;
    size_t xorbitsRaw_size; 
} crc_t;
typedef struct msg_s {
    uint8_t* msg;
    size_t  msg_size;
    uint8_t* msgbits;
    size_t  msgbits_size; 
    char* msgstr;
    // CRC result    
    int32_t  crc;
    uint8_t* crcbits;
    size_t crcbits_size;
    uint8_t* crcbitsRaw;
    size_t crcbitsRaw_size;
    // Validation
    int32_t  rem;
    uint8_t* rembits;
    size_t rembits_size;
    bool valid;
    // Calculation testing
    int32_t expected;
    uint8_t* expectedbits;
    size_t expectedbits_size;
} msg_t;

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
    // uint8_t message[] = {'A', 'B'}; // 15 zeros have been appended to the message
    // expected = 0x54FB;

    // uint8_t message[] = {'@'};
    // expected = 0x0;  //CRC-8

    uint8_t message[] = {'@', '@'};
    // expected = 0xC7; //CRC-8 0x07
    expected = 0x4508;   //CRC16 0x1021
    // expected = 0xD6;  //CRC-3 0xB

    // uint8_t message[] = {'A', 'A'};
    // expected = 0xC0;  //CRC-8 0x07
    // expected = 0xDD;  //CRC-3 0xB


    // Wikipedia 2
    // uint8_t message[] = {'W'};
    // expected = 0xA2;  //CRC-8 0x107 MSF   1 0000 0111
    // expected = 0x98;  //CRC-8 0x107 LSF   1 1100 0001
    


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

    // Store message size before padding to avoid errors arising from over-padding
    size_t originalMsgSize = sizeof(msg) * 8;

    // Prepare padding bits
    uint32_t initial = INITIAL;
    uint8_t padBitsRaw[sizeof(initial) * 8];
    int2bits(sizeof(initial), &initial, padBitsRaw, false );
    if (VERBOSE) printBits("Pad raw", padBitsRaw, COUNT_OF(padBitsRaw));
    // Crop to size
    uint8_t padBits[padSize];
    for (int i = COUNT_OF(padBits) - 1, j = COUNT_OF(padBitsRaw) - 1; i >= 0; i--, j--)
        padBits[i] = padBitsRaw[j];
    if (VERBOSE) printBits("Pad", padBits, COUNT_OF(padBits));

    // Convert msg to array of bit values, add padding 
    uint8_t msgBits[sizeof(msg) * 8 + padSize];
    // uint8_t msgBits[sizeof(msg) * 8 + padSize +1]; // CRC-15 CAN'T
    if (MSGLSF)
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize,  padBits);
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize +1,  NULL);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, padBits);
    if (VERBOSE) printBits("Message", msgBits, COUNT_OF(msgBits));

     // Convert generator polynomial to array of bit values
    uint8_t genBitsRaw[sizeof(gen) * 8 + IMPLICIT_LEADING_1];
    int2bits(sizeof(gen), &gen, genBitsRaw, IMPLICIT_LEADING_1);
    // Crop to size
    uint8_t genBits[padSize + 1];

    for (int i = COUNT_OF(genBits) - 1, j = COUNT_OF(genBitsRaw) - 1; i >= 0; i--, j--)
        genBits[i] = genBitsRaw[j];
    // bitSlice(-1, padSize, genBitsRaw, COUNT_OF(genBitsRaw), genBits);
    if (IMPLICIT_LEADING_1)
        genBits[0] = 1;


    printf("Generator: 0x%X\n", gen);
    if (VERBOSE) printBits("GeneratorRaw", genBitsRaw, COUNT_OF(genBitsRaw));
    if (VERBOSE || expected) printBits("Generator", genBits, COUNT_OF(genBits));

    // Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB
    // Hex     LSF                MSF               
    // 0xB35   1010110011010000   0000101100110101               
    // 0x54FB  1101111100101010   0101010011111011    

    // Print original message before checksum
    if (PRINTMSG) {
        char msgStr[COUNT_OF(message) + 1];
        charArrayToString(msg, COUNT_OF(message), msgStr);
        printf("\nOriginal message:\n%s", msgStr);
    }
    // Get checksum
    int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), genBits, COUNT_OF(genBits), padSize, originalMsgSize);
    printf("Checksum: 0x%X\n", checksum);

    // Compare to expected checksum
    if (expected) {
        uint8_t checksumBits[sizeof(checksum) * 8];
        int2bits(sizeof(checksum), &checksum, checksumBits, false);
        printBits("Checksum", checksumBits, COUNT_OF(checksumBits));

        uint8_t expectedBits[sizeof(expected) * 8];
        int2bits(sizeof(expected), &expected, expectedBits, false);
        printf("Expected: 0x%X\n", expected);
        printBits("Expected", expectedBits, COUNT_OF(expectedBits));
        if (checksum != expected) 
            printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
    }

    // Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements.
    uint8_t msgBitsCS[sizeof(msg) * 8 + padSize];
    checksumMsg(message, COUNT_OF(message), checksum, padSize, msgBitsCS);

    // Validate the messsage.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"
    bool valid;
    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), padSize, genBits, COUNT_OF(genBits), originalMsgSize);
    validPrint(message, COUNT_OF(message), valid);
  
    // Changed message
    message[1] = 'a';
    checksumMsg(message, COUNT_OF(message), checksum, padSize, msgBitsCS);
    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), padSize, genBits, COUNT_OF(genBits), originalMsgSize);
    validPrint(message, COUNT_OF(message), valid);

    return 0;
}

void checksumMsg(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t msgBits[]) {
     // Create working copy
    uint8_t msg[msgSize];
    for (int i = 0; i < msgSize; i++)
        msg[i] = message[i];
    
    // Convert checksum to array of bit values
    uint8_t checksumBitsRaw[sizeof(checksum) * 8];
    int2bits(sizeof(checksum), &checksum, checksumBitsRaw, false);

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
    // uint8_t msgBits[msgSize * 8 + padSize];
    if (MSGLSF)
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    if (VERBOSE) printBits("Checksummed message", msgBits, sizeof(msg) * 8 + padSize);
}

int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize, size_t orginalMsgSize ) {
    if (PRINTSTEPS) {
        printf("\n Before: "); i2pc(msgBits, msgSize, 0, 1, 34, orginalMsgSize, padSize,  orginalMsgSize); //i2p(msgBits, msgSize, 0, 1); 
        for (int i = 0; i < msgSize - padSize; i++) 
        // for (int i = 0; i < orginalMsgSize; i++) 
            if (msgBits[i]) {
                printf("  @ %3d: ", i); i2pc(msgBits, msgSize, 0, 1, 36, i, genSize, orginalMsgSize);
                for (int j = 0, k = i; j < genSize; j++, k++) 
                    msgBits[k] = msgBits[k] ^ genBits[j];
            }   
        printf("  After: "); i2pc(msgBits, msgSize, 0, 1, 35, orginalMsgSize, padSize,  orginalMsgSize); //i2p(msgBits, msgSize, 0, 1); 
    }
    else 
        for (int i = 0; i < orginalMsgSize; i++)  
            if (msgBits[i]) 
                for (int j = 0, k = i; j < genSize; j++, k++) 
                    msgBits[k] = msgBits[k] ^ genBits[j];

    if (VERBOSE) { puts("Message post calculation"); i2p(msgBits, msgSize, 0, 2);  }

    uint8_t remBits[padSize];
    bitSlice(-1, padSize, msgBits, msgSize, remBits);
    if (VERBOSE) printBits("Remainder", remBits, COUNT_OF(remBits));

    uint32_t rem;
    if (CRCLSF)
        rem = (uint32_t)bits2intLSF(COUNT_OF(remBits), remBits);
    else
        rem = (uint32_t)bits2int(COUNT_OF(remBits), remBits);

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

bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t padSize, uint8_t genBits[], size_t genSize, size_t originalMsgSize) {
    int32_t rem = getRem(msgBits, msgBitsCount, genBits, genSize, padSize, originalMsgSize);
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
        printf("Message to validate:\n%s\n", msgStr);
    }
    if (valid) 
        printf("\e[1;32m%s\e[m\n", "The data is OK"); // green
    else
        printf("\e[1;31m%s\e[m\n", "The data is not OK"); // red
}