#include "crc.h"

// Data structures 
prog_t* prog;
// crc_t* crc;  
// msg_t* msg;

// Set aliases to MSF as default    
int2bits_t int2bits = int2bitsMSF;
bits2int_t bits2int = bits2intMSF;
ints2bits_t ints2bits = ints2bitsMSF;
bits2ints_t bits2ints = bits2intsMSF;

void arrangeMsg(crc_t* crc, msg_t* msg) {
     // Arrange message bits and pad
    if ( crc->inputLSF )
        ints2bitsLSF(strlen(msg->msgStr), sizeof(uint8_t), (msg->msgStr), msg->msgBits, SPECIALWIDTH, msg->initPad);  // Special accomodation, cf. error.h
        // ints2bitsLSF(strlen(msg->msgStr), sizeof(uint8_t), (msg->msgStr), msg->msgBits, crc->n, crc->initBits);           // Normal
    else
        ints2bitsMSF(strlen(msg->msgStr), sizeof(uint8_t), (msg->msgStr), msg->msgBits, crc->n, msg->initPad); 

    if (crc->init > 0) {
        // Local initBits to crc width
        TOWIDTH(initBits);

        // Write initbits into front padding
        for (int i = 0; i < crc->n; i++) {
            msg->msgBits[i] = initBits[i];
        };
        // printf("\nmsgBits (initBits written to frontpad): "); i82p(msg->msgBits, msg->paddedBitLen, 0, 0, 1);


        // // Get actual initBits from seed
        // for (int i = 0; i < crc->n; i++) {
        //     initBits[i] ^= msg->msgBits[i];
        // };
        // // printf("\n  initBits:  "); i2p(&crc->initBits, COUNT_OF(crc->initBits), 0, 0, 1);
        // // Write actual initBits to padding
        // for (int i = strlen(msg->msgBits) - crc->n, j = 0; i < strlen(msg->msgBits); i++, j++) {
        //     msg->msgBits[i] = initBits[j];  
        // }

    }
}

void checksumMsg(size_t paddedBitLen, uint64_t checksum, size_t width, uint8_t csmsgBits[]) {
    // Convert checksum to array of bit values
    uint8_t tmp_csBits[sizeof(checksum) * BITSINBYTE];
    int2bits(sizeof(checksum), &checksum, tmp_csBits, false);

    // Handled as uint64, truncate to CRC width
    uint8_t csBits[width]; 
    bitSlice(sizeof(checksum) * BITSINBYTE - width, width, &tmp_csBits, 0, csBits);

    if (PROG.verbose) printf("Checksum:\t%#X\n", checksum);
    if (PROG.verbose) printBits("Checksum", csBits, width, 0);
    
    // Extra checking step: Convert checksum bits back to checksum, check that it matches provided checksum
    uint64_t checksumRecon = (uint64_t)bits2int(width, csBits);
    if (PROG.verbose)  printf("Checksum: %#X  Cs recon:%#X\n", checksum, checksumRecon);
    assert( ("Checksum error", checksumRecon == checksum) );

    // Replace padding in csmsgBits with bits from checksum
    for (int i = paddedBitLen - width, j = 0; j < width; i++, j++)
            csmsgBits[i] = csBits[j];
}


uint64_t getRem(crc_t* crc, msg_t* msg) {
    // gBits to actual bit width
    size_t gBits_size = crc->n + 1; // Generator is 1 bit wider than CRC  
    uint8_t gBits[gBits_size];
    for (int i = gBits_size - 1, j = COUNT_OF(crc->gBits) - 1; i >= 0; i--, j--)
        gBits[i] = crc->gBits[j];
    
    // int i; i = crc->init > 0 ? crc->n : 0;  // Skip a CRC width if using initial > 0
    int i = 0;
    if (PROG.printSteps) {
        // Poly division with printing of steps
        printf("\n Before: "); i2pc(msg->msgBits, msg->paddedBitLen, 0, 1, 34, msg->originalBitLen, crc->n,  msg->originalBitLen, 0); 
        // for (; i < REMLOOPEND; i++)   // Special accomodation, cf. error.h
        for (; i < msg->originalBitLen + msg->initPad; i++)  // Standard loop ending condition
            if (msg->msgBits[i]) {
                if (PROG.printStepsGen)
                    i2pc(gBits, gBits_size, 0, 1, 33, 0, gBits_size, msg->originalBitLen-i, i+9); 
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msg->msgBits[k] ^= gBits[j];
                printf("  @ %3d: ", i); i2pc(msg->msgBits, msg->paddedBitLen, 0, 1, 36, i, gBits_size, msg->originalBitLen, 0);
            }   
        printf("  After: "); i2pc(msg->msgBits, msg->paddedBitLen, 0, 1, 35, msg->originalBitLen, crc->n,  msg->originalBitLen, 0); 
    } 
    else 
        // Poly division (without printing of steps)
        // for (; i < REMLOOPEND; i++)  // Special accomodation, cf. error.h
        for (; i < msg->originalBitLen + msg->initPad; i++)  // Standard loop ending condition
            if (msg->msgBits[i]) 
                for (int j = 0, k = i; j < gBits_size; j++, k++) 
                    msg->msgBits[k] ^= gBits[j];

    if (PROG.verbose) { puts("Message post calculation"); i2p(msg->msgBits, msg->paddedBitLen, 0, 0, 1);  }

    // Slice off the remBits from msgBits
    uint8_t remBits[crc->n];
    bitSlice(-1, crc->n, msg->msgBits, msg->paddedBitLen, remBits);
    if (PROG.verbose) printBits("Remainder", remBits, COUNT_OF(remBits), crc->n);

    // Final xoring of rem as required by some CRC specs
    if (crc->xor > 0) {
        TOWIDTH(xorBits);
        for (int i = 0; i < COUNT_OF(remBits); i++) 
            remBits[i] ^= xorBits[i];
    }

    // Convert remBits to int with choice of bit ordering
    uint64_t rem;
    if ( crc->resultLSF )
        rem = (uint64_t)bits2intLSF(COUNT_OF(remBits), remBits);
    else
        rem = (uint64_t)bits2intMSF(COUNT_OF(remBits), remBits);

    if (PROG.verbose) printf("Remainder: %#X\n", rem);
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

// bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t originalMsgSize, crc_t* crc, msg_t* msg) {
bool validate(crc_t* crc, msg_t* msg) {
    uint64_t rem = getRem(crc, msg );

    if (PROG.verbose) printf("Remainder: %#X\n", rem);
    return rem ? false : true;
}

void validPrint(uint8_t msg[], size_t msgSize, bool valid) {
    if (PRINTMSG) {
        // char msgStr[msgSize + 1];
        // charArrayToString(msg, msgSize, msgStr);
        if (msgSize < PRINTLIMIT)
            printf("Message to validate:\t%s\n", msg);
        else
            printf("Message to validate:\t[%d characters]\n", msgSize);
    }
    if (valid) 
        printf("\e[1;32m%s\e[m\n", "The data is OK"); // green
    else
        printf("\e[1;31m%s\e[m\n", "The data is not OK"); // red
}

void loadDef(crcdef_t zoo[], size_t index, crc_t* crc) {
        // 0 n   1 Gen    2 IL1  3 Init  4 Nondir. 5 RefIn 6 RefOut 7 XorOut   8 Residue 9 Check      10 "AB"
        strcpy((crc)->description, zoo[index].name);
        crc->n =         zoo[index].specs[0];
        crc->g =         zoo[index].specs[1];
        crc->il1 =       zoo[index].specs[2];
        crc->init =      zoo[index].specs[3];
        crc->nondirect = zoo[index].specs[4];
        crc->inputLSF =  zoo[index].specs[5];
        crc->resultLSF = zoo[index].specs[6];
        crc->xor =       zoo[index].specs[7];
        crc->residue =   zoo[index].specs[8];
        crc->check =     zoo[index].specs[9];
        crc->checkAB =   zoo[index].specs[10];

    // Convert generator polynomial to array of bit values  
    int2bitsMSF(sizeof(crc->g), &crc->g, crc->gBits, true );            // uint8_t gBits[32];
    crc->gBits[COUNT_OF(crc->gBits) - crc->n - 1] = crc->il1;             // Set implicit leading 1

    // Convert init seed and final xor to array of bit values  
    int2bitsMSF(sizeof(crc->init), &crc->init, crc->initBits, 0 );    // uint8_t padbits[32];
    int2bitsMSF(sizeof(crc->xor), &crc->xor, crc->xorBits, 0 );             // uint8_t xorbits[32];
}

void loadSpec(crcdef_t zoo[], size_t index, crc_t* crc, bool table) {
    loadDef(zoo, index, crc);  
    
    if (table) {
        printf("%16s   ", crc->description);
        printf("%#10X   %#10X   ",  crc->g, crc->init);
        printf("%#10X   %5d   %6d   ", crc->xor, crc->inputLSF, crc->resultLSF);
    }
    else {
        printf("%s   ", crc->description);
        printf("Poly:%#X   Init:%#X   ",  crc->g, crc->init);
        printf("XorOut:%#X   RefIn:%d   RefOut:%d   ", crc->xor, crc->inputLSF, crc->resultLSF);
    }    

    // Check value-test for this spec
      // Disable printSteps when testing
    uint8_t tmp_printSteps = PROG.printSteps; PROG.printSteps = SELFTESTSTEPS;
    if (table) PROG.printSteps = false;
    // PROG.printSteps = true;

    // Prepare standard check message
    char message[] = "123456789";
    int8_t initPad = (crc->init > 0) ? crc->n : 0;  
    msg_t test_msg = {
        .msgStr =           message,
        .len =              strlen(message),
        .initPad =          initPad,
        .originalBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE,
        .paddedBitLen =     strlen(message) * sizeof(uint8_t) * BITSINBYTE + SPECIALWIDTH + initPad,     // Special
        // .paddedBitLen =   strlen(message) * sizeof(uint8_t) * BITSINBYTE + crc->n + frontPad,       // Normal
    };
    uint8_t test_msgBits[test_msg.paddedBitLen];
    for I2(COUNT_OF(test_msgBits)) test_msgBits[i] = 0;     
    test_msg.msgBits = test_msgBits;

    arrangeMsg(crc, &test_msg);
    // uint64_t test_res = getRem(test_msg.msgBits, test_msg.paddedBitLen, test_msg.originalBitLen, crc);
    uint64_t test_res = getRem(crc, &test_msg);

    if (test_res == crc->check)
        if (table)
            // printf("\e[1;32mPassed\e[m\n");
            printf("\e[1;32mPassed\e[m %#X = %#X\n", test_res, crc->check);
        else
            printf("\n\e[1;32mPassed check value-test for %s;\e[m matching %#X\n", crc->description, crc->check);
    else 
        if (table)
            // printf("\e[1;31mFailed\e[m\n");
            printf("\e[1;31mFailed\e[m %#X != %#X\n", test_res, crc->check);
        else
            printf("\n\e[1;31mFailed check value-test for %s;\e[m result %#X != check %#X\n", crc->description, test_res, crc->check);
    
    if (VERBOSELOAD && !table) { 
        printf("     gBits: "); i2p(&crc->gBits, COUNT_OF(crc->gBits), crc->n+1, 0, 1);
        // if (VERBOSE || expected) printBits("Generator",  crc->gBits, COUNT_OF( crc->gBits ), crc->gBits_size);
        printf("  initBits:  "); i2p(&crc->initBits, COUNT_OF(crc->initBits), crc->n, 0, 1);
        printf("   xorBits:  "); i2p(&crc->xorBits, COUNT_OF(crc->xorBits), crc->n, 0, 1);
    }
    PROG.printSteps = tmp_printSteps; // Reset printSteps flag
}

void zooTour(crcdef_t zoo[], size_t zoo_size) {
    printf("%5s   %16s   %10s   %10s   %10s   %5s   %6s   %6s\n", "Index", "Spec", "Poly", "Init", "XorOut", "RefIn", "RefOut", "Check value");
    for (int i = 0; i < zoo_size; i++) {
        crc_t zooItem;
        printf("%5d   ", i);
        loadSpec(zoo, i, &zooItem, true);
    }
}


//  uint64_t encode(char* msg, int crcIndex) {
//  }
//  bool validate(char* msg, int crcIndex, uint64_t checksum) {
//  }

static short allocCheck(void* p) {
    if(p == NULL ) {
        if (DA_DEBUG) fprintf(stderr, "Unable to allocate memory.\n");
        return 1;
    }
    else 
        return 0;

}