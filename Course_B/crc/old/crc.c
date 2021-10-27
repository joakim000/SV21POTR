#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "binutils.h"
#include "crc.h"


int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize ) {
    for (int i = 0; i < msgSize - padSize; i++) 
        if (msgBits[i]) 
            for (int j = 0, k = i; j < genSize; j++, k++) 
                msgBits[k] = msgBits[k] ^ genBits[j];
    if (VERBOSE) { puts("Message post calculation"); i2p(msgBits, msgSize, 0, 2);  }

    uint8_t remBits[padSize];
    bitSlice(-1, padSize, msgBits, msgSize, remBits);
    if (VERBOSE) printBits("Remainder", remBits, COUNT_OF(remBits));

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
        if (size > 40) {
            printf("%10s bits (%d):\n", label, size);
            i2p(bits, size, 0, 2); 
        }
        else {
            printf("%10s bits (%d): ", label, size);
            i2p(bits, size, 0, 1);
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
    if (VERBOSE) printBits("Cs recon", checksumBits, COUNT_OF(checksumBits));
    
    // Extra checking step: Convert checksum bits back to checksum, check that it matches provided checksum
    uint16_t checksumRecon = (uint16_t)bits2int(COUNT_OF(checksumBits), checksumBits);
    if (VERBOSE) printf("Cs recon: 0x%x\n", checksumBits);
    assert( ("Checksum error", checksumRecon == checksum) );

    // Convert msg to array of bit values, add remainder bits as padding
    uint8_t msgBits[sizeof(msg) * 8 + padSize];
    if (MSGLSF)
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    if (VERBOSE) printBits("Checksummed message", msgBits, COUNT_OF(msgBits));

    int16_t rem = getRem(msgBits, COUNT_OF(msgBits), genBits, genSize, padSize);
    if (VERBOSE) printf("Remainder: 0x%x\n", rem);

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