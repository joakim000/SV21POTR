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

    if (PROG.verbose) printf("Checksum: 0x%x\n", checksum);
    if (PROG.verbose) printBits("Checksum", checksumBits, COUNT_OF(checksumBits), padSize);
    
    // Extra checking step: Convert checksum bits back to checksum, check that it matches provided checksum
    uint32_t checksumRecon = (uint32_t)bits2int(COUNT_OF(checksumBits), checksumBits);
    if (PROG.verbose) printf("Cs recon: 0x%x\n", checksumRecon);
    assert( ("Checksum error", checksumRecon == checksum) );

    // Convert msg to array of bit values, add remainder bits as padding
    // uint8_t msgBits[msgSize * 8 + padSize];
    if ( CRC.inputLSF )
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, checksumBits);
    if (PROG.verbose) printBits("Checksummed message", msgBits, sizeof(msg) * 8 + padSize, 0);
}

int32_t getRem(uint8_t msgBits[], size_t msgSize, size_t originalMsgSize ) {
    size_t gBits_size = CRC.gBits_size;

    // printBits("gBits from *crc", CRC.gBits, COUNT_OF(CRC.gBits), 0);
    uint8_t gBits[gBits_size];
    for (int i = gBits_size - 1, j = COUNT_OF(CRC.gBits) - 1; i >= 0; i--, j--)
        gBits[i] = CRC.gBits[j];
    
    if (PROG.printSteps) {
        printf("\n Before: "); i2pc(msgBits, msgSize, 0, 1, 34, originalMsgSize, CRC.n,  originalMsgSize, 0); 
        // for (int i = 0; i < msgSize - padSize; i++) 
        for (int i = 0; i < REMLOOPEND; i++) 
        // for (int i = 0; i < originalMsgSize; i++) 
            if (msgBits[i]) {
                if (PROG.printStepsGen)
                    i2pc(gBits, gBits_size, 0, 1, 33, 0, gBits_size, originalMsgSize-i, i+9); 
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msgBits[k] = msgBits[k] ^ gBits[j];
                printf("  @ %3d: ", i); i2pc(msgBits, msgSize, 0, 1, 36, i, gBits_size, originalMsgSize, 0);
            }   
        printf("  After: "); i2pc(msgBits, msgSize, 0, 1, 35, originalMsgSize, CRC.n,  originalMsgSize, 0); 
        // printf("  After: "); i2pc(msgBits, msgSize, 0, 1, 35, originalMsgSize, n,  -1, 0); // no space
    } 
    else 
        for (int i = 0; i < originalMsgSize; i++)  
            if (msgBits[i]) 
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msgBits[k] = msgBits[k] ^ gBits[j];
        

    if (PROG.verbose) { puts("Message post calculation"); i2p(msgBits, msgSize, 0, 0, 1);  }

    uint8_t remBits[CRC.n];
    bitSlice(-1, CRC.n, msgBits, msgSize, remBits);
    if (PROG.verbose) printBits("Remainder", remBits, COUNT_OF(remBits), CRC.n);

    uint32_t rem;
    if ( CRC.resultLSF )
        rem = (uint32_t)bits2intLSF(COUNT_OF(remBits), remBits);
    else
        rem = (uint32_t)bits2intMSF(COUNT_OF(remBits), remBits);

    if (PROG.verbose) printf("Remainder: 0x%x\n", rem);
    
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

bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t originalMsgSize) {
    int32_t rem = getRem(msgBits, msgBitsCount, originalMsgSize );

    if (PROG.verbose) printf("Remainder: 0x%x\n", rem);
    return rem ? false : true;
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
    out->init = zoo[index].specs[4];
    out->inputLSF = zoo[index].specs[5];
    out->resultLSF = zoo[index].specs[6];
    out->xor = zoo[index].specs[7];
    out->il1 = zoo[index].specs[8];

    // Convert generator polynomial to array of bit values  
    int2bitsMSF(sizeof(out->g), &out->g, out->gBits, true );            // uint8_t gBits[32];
    out->gBits_size = out->n + 1;      
    // printBits("gBits before il1", out->gBits, COUNT_OF(CRC.gBits), 0);          
    out->gBits[COUNT_OF(CRC.gBits) - out->n - 1] = out->il1;             // Set implicit leading 1
    // printf("\nout->il1:%d  ind COUNT_OF(CRC.gBits) - out->n:%d\n", out->il1,  COUNT_OF(CRC.gBits) - out->n);
    // printBits(" gBits after il1", out->gBits, COUNT_OF(CRC.gBits), 0);          


    int2bitsMSF(sizeof(out->init), &out->init, out->initBits, 0 );    // uint8_t padbits[32];
    // out->padbits_size = out->n; // size_t padbits_size;                  // = n 
    int2bitsMSF(sizeof(out->xor), &out->xor, out->xorBits, 0 );             // uint8_t xorbits[32];
    // out->xorbits_size = out->n; // size_t xorbits_size;                  // = n 
    
    // uint8_t gBitsRaw[32];
    // size_t gBitsRaw_size;
    // uint8_t padbitsRaw[32];
    // size_t padbitsRaw_size; 
    // uint8_t xorbitsRaw[32];
    // size_t xorbitsRaw_size; 
}