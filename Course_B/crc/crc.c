#include "crc.h"

// Data structures 
prog_t* prog;
crc_t* crc;  
msg_t* msg;
expect_t* expect;

// Set aliases to MSF as default    
int2bits_t int2bits = int2bitsMSF;
bits2int_t bits2int = bits2intMSF;
ints2bits_t ints2bits = ints2bitsMSF;
bits2ints_t bits2ints = bits2intsMSF;

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
    if (VERBOSE) printBits("Checksum", checksumBits, COUNT_OF(checksumBits), padSize);
    
    // Extra checking step: Convert checksum bits back to checksum, check that it matches provided checksum
    uint32_t checksumRecon = (uint32_t)bits2int(COUNT_OF(checksumBits), checksumBits);
    if (VERBOSE) printf("Cs recon: 0x%x\n", checksumRecon);
    assert( ("Checksum error", checksumRecon == checksum) );

    // Convert msg to array of bit values, add remainder bits as padding
    // uint8_t msgBits[msgSize * 8 + padSize];
    if ( (*crc).inputLSF )
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    if (VERBOSE) printBits("Checksummed message", msgBits, sizeof(msg) * 8 + padSize, 0);
}

int32_t getRem(uint8_t msgBits[], size_t msgSize, size_t originalMsgSize ) {
    size_t n = (*crc).n;
    size_t gbits_size = (*crc).gbits_size;

    printBits("gbits från *crc", (*crc).gbits, COUNT_OF((*crc).gbits), 0);
    uint8_t gbits[gbits_size];
    for (int i = gbits_size - 1, j = COUNT_OF((*crc).gbits) - 1; i >= 0; i--, j--)
        gbits[i] = (*crc).gbits[j];
    
    if (PRINTSTEPS) {
        printf("\n Before: "); i2pc(msgBits, msgSize, 0, 1, 34, originalMsgSize, n,  originalMsgSize, 0); 
        // for (int i = 0; i < msgSize - padSize; i++) 
        for (int i = 0; i < REMLOOPEND; i++) 
        // for (int i = 0; i < originalMsgSize; i++) 
            if (msgBits[i]) {
                if (PRINTSTEPSGEN)
                    i2pc(gbits, gbits_size, 0, 1, 33, 0, gbits_size, originalMsgSize-i, i+9); 
                for (int j = 0, k = i; j < gbits_size; j++, k++) 
                    msgBits[k] = msgBits[k] ^ gbits[j];
                printf("  @ %3d: ", i); i2pc(msgBits, msgSize, 0, 1, 36, i, gbits_size, originalMsgSize, 0);
            }   
        printf("  After: "); i2pc(msgBits, msgSize, 0, 1, 35, originalMsgSize, n,  originalMsgSize, 0); 
        // printf("  After: "); i2pc(msgBits, msgSize, 0, 1, 35, originalMsgSize, n,  -1, 0); // no space
    } 
    else 
        for (int i = 0; i < originalMsgSize; i++)  
            if (msgBits[i]) 
                for (int j = 0, k = i; j < gbits_size; j++, k++) 
                    msgBits[k] = msgBits[k] ^ gbits[j];
        

    if (VERBOSE) { puts("Message post calculation"); i2p(msgBits, msgSize, 0, 2, 0);  }

    uint8_t remBits[n];
    bitSlice(-1, n, msgBits, msgSize, remBits);
    if (VERBOSE) printBits("Remainder", remBits, COUNT_OF(remBits), n);

    uint32_t rem;
    if ( (*crc).resultLSF )
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
    // int32_t rem = getRem(msgBits, msgBitsCount, genBits, genSize, padSize, originalMsgSize);
    // if (VERBOSE) printf("Remainder: 0x%x\n", rem);
//     if (!rem) 
//         return true;
//     else
//         return false;
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

void loadDef(struct crc_def zoo[], size_t index, crc_t* out) {
    strcpy((out)->description, zoo[index].name);
    out->n = zoo[index].specs[0];
    out->checkAB = zoo[index].specs[1];
    out->check = zoo[index].specs[2];
    out->g = zoo[index].specs[3];
    out->initial = zoo[index].specs[4];
    out->inputLSF = zoo[index].specs[5];
    out->resultLSF = zoo[index].specs[6];
    out->xor = zoo[index].specs[7];
    out->il1 = zoo[index].specs[8];

    // Convert generator polynomial to array of bit values  
    int2bitsMSF(sizeof(out->g), &out->g, out->gbits, true );            // uint8_t gbits[32];
    out->gbits_size = out->n + 1;      
    printBits("gbits before il1", out->gbits, COUNT_OF((*crc).gbits), 0);          
    out->gbits[COUNT_OF((*crc).gbits) - out->n - 1] = out->il1;             // Set implicit leading 1
    printf("\nout->il1:%d  ind COUNT_OF((*crc).gbits) - out->n:%d\n", out->il1,  COUNT_OF((*crc).gbits) - out->n);
    printBits(" gbits after il1", out->gbits, COUNT_OF((*crc).gbits), 0);          


    int2bitsMSF(sizeof(out->initial), &out->initial, out->initbits, 0 );    // uint8_t padbits[32];
    // out->padbits_size = out->n; // size_t padbits_size;                  // = n 
    int2bitsMSF(sizeof(out->xor), &out->xor, out->xorbits, 0 );             // uint8_t xorbits[32];
    // out->xorbits_size = out->n; // size_t xorbits_size;                  // = n 
    
    // uint8_t gbitsRaw[32];
    // size_t gbitsRaw_size;
    // uint8_t padbitsRaw[32];
    // size_t padbitsRaw_size; 
    // uint8_t xorbitsRaw[32];
    // size_t xorbitsRaw_size; 
}