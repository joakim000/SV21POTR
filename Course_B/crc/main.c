

#include "crc.h"



int main(void)
{
    /* Setup */
    #include "crc_zoo.c" // Unconventional use of #include, just a convenient way to put the CRC definitions in a separate file.
    // Program settings and variables
    prog_t new_prog = {
        .verbose = VERBOSE,
        .printMsg = PRINTMSG,
        .printSteps = PRINTSTEPS,
        .printStepsGen = PRINTSTEPSGEN,
        .selfTest = SELFTEST,
        // .testkMsg = TESTMSG
    };
    prog = &new_prog;    

    /* Load CRC spec */
    crc_t new_crc;
    loadDef(zoo, 10, &new_crc);
    crc = &new_crc;
    printf("CRC spec: %s\n", CRC.description);
    printf("n:%d   g:0x%X   init:0x%X   ", CRC.n, CRC.g, CRC.init);
    printf("xor:0x%X   inRef:%d   outRef:%d\n", CRC.xor, CRC.inputLSF, CRC.resultLSF);
    printf("     gBits: "); i2p(&CRC.gBits, COUNT_OF(CRC.gBits), CRC.gBits_size, 0, 1);
    // if (VERBOSE || expected) printBits("Generator",  CRC.gBits, COUNT_OF( CRC.gBits ), CRC.gBits_size);
    printf("  initBits:  "); i2p(&CRC.initBits, COUNT_OF(CRC.initBits), CRC.n, 0, 1);
    printf("   xorBits:  "); i2p(&CRC.xorBits, COUNT_OF(CRC.xorBits), CRC.n, 0, 1);
    
    // Self test for this spec
    uint8_t test_msg[9];
    uint8_t test_msgBits[9 * sizeof(uint8_t) * 8 + CRC.n];
    for (int i = 0; i < 9; i++) test_msg[i] = i + 1; // Std check input: {1, 2, 3, 4, 5, 6, 7, 8, 9}
    if ( CRC.inputLSF )
        ints2bitsLSF(sizeof(test_msg), sizeof(test_msg[0]), &test_msg, test_msgBits, CRC.n,  CRC.initBits); 
    else
        ints2bitsMSF(sizeof(test_msg), sizeof(test_msg[0]), &test_msg, test_msgBits, CRC.n,  CRC.initBits); 
    uint8_t tmp_printSteps = PROG.printSteps; PROG.printSteps = 0;    
    uint8_t tmp_printStepsGen = PROG.printStepsGen; PROG.printStepsGen = 0;
    
    uint32_t test_res = getRem(test_msgBits, COUNT_OF(test_msgBits), 9 * sizeof(uint8_t) * 8);
    if (test_res == CRC.check)
        printf("\e[1;32mPassed self-test for %s;\e[m result 0x%X == check 0x%X", CRC.description, test_res, CRC.check);
    else 
        printf("\e[1;31mFailed self-test for %s;\e[m result 0x%X != check 0x%X", CRC.description, test_res, CRC.check);
    PROG.printSteps = tmp_printSteps; PROG.printStepsGen = tmp_printSteps;
    // End self test


    msg_t new_msg = {
        // .resbits = &new_msg_resbits,
        // .rembits = {0, 1, 2}
    };
    msg = &new_msg;
    
    expect_t new_expect = {
        .desc = "CRC-15 CAN; AB",
        .msgStr = "AB",
        // .message = {'s', 'p'},
        .msg = daInit(10), 
        .expected = 0x48B1,  // Spec
        // expected = 0x54FB,  // Example
    };
    expect = &new_expect;
    uint8_t val[]  = {'A', 'B'};
    daCreate(&(*expect).msg, val, COUNT_OF(val));
    uint8_t boll = daGet(&(*expect).msg, 2);
    daAdd(&(*expect).msg, 'f');
    // memcpy(struct->array, some_array, sizeof(struct->array));

    // Expected checksum value for testing checksum calculation. Skips check when set to 0.
    uint32_t expected = 1; 

    uint8_t message[] = {'A', 'B'}; 
    // expected = 0x48B1;  // Spec
    // expected = 0x54FB;  // Example
    // uint8_t message[] = {'A', 'B', 'C'};
    // expected = 0xD59;  // Example
    // uint8_t message[] = {'A', 'B', 'C', 'D'}; 
    // expected = 0x6531;  // Example
    // uint8_t message[] = {'A', 'B', 'C', 'D', 'E'}; 
    // expected = 0x3556;  // Example
    // uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    // expected = 0xB35;  // Example
    // uint8_t message[] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; // 0x59E
    // expected = 0x59E;  // Should match example implementation
    // uint8_t message[] = {'F', 'a', 'r', 'o', 'c', 'h'};  // Example impl: 0x2535, spec: 


    // Assignment requirements for message length
    messageLengthCheck(COUNT_OF(message));

    // Create working copy
    uint8_t msg[COUNT_OF(message)];
    for (int i = 0; i < COUNT_OF(message); i++)
        msg[i] = message[i];

    // Store message size before padding to avoid errors arising from over-padding
    size_t originalMsgSize = sizeof(msg) * 8;

    // Prepare padding bits
    // uint32_t initial = INITIAL;
    uint32_t init = CRC.init;
    uint8_t padBitsRaw[sizeof(init) * 8];
    int2bits(sizeof(init), &init, padBitsRaw, false );
    if (VERBOSE) printBits("Pad raw", padBitsRaw, COUNT_OF(padBitsRaw), 0);
    // Crop to size
    uint8_t padBits[CRC.n];
    for (int i = COUNT_OF(padBits) - 1, j = COUNT_OF(padBitsRaw) - 1; i >= 0; i--, j--)
        padBits[i] = padBitsRaw[j];
    if (VERBOSE) printBits("Pad", padBits, COUNT_OF(padBits), CRC.n);

    // Convert msg to array of bit values, add padding 
    // uint8_t msgBits[sizeof(msg) * 8 + CRC.n];       // Spec
    // uint8_t msgBits[sizeof(msg) * 8 + CRC.n +1];    // CRC-15/CANT
    uint8_t msgBits[sizeof(msg) * 8 + PADSIZE];           // Flag toggle 


    if ( CRC.inputLSF )
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, CRC.n,  padBits);    // Spec
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, CRC.n +1,  NULL);    // CRC-15/CANT
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, PADSIZE,  padBits);        // Flag toggle 
    else
        ints2bitsMSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, CRC.n, padBits);
    if (VERBOSE) printBits("Message", msgBits, COUNT_OF(msgBits), 0);


    /* Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB */

    // Print original message before checksum
    if (PRINTMSG) {
        char msgStr[COUNT_OF(message) + 1];
        charArrayToString(msg, COUNT_OF(message), msgStr);
        printf("\nOriginal message:\n%s\n", msgStr);
    }

    // Get checksum
    // int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), CRC.gBits, CRC.gBits_size, CRC.n, originalMsgSize);
    int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), originalMsgSize);
    printf("Checksum: 0x%X\n", checksum);

    // Compare to expected checksum
    if (expected) {
        uint8_t checksumBits[sizeof(checksum) * 8];
        int2bits(sizeof(checksum), &checksum, checksumBits, false);
        if (PROG.verbose) printBits("Checksum", checksumBits, COUNT_OF(checksumBits), CRC.n);   

        uint8_t expectedBits[sizeof(expected) * 8];
        int2bits(sizeof(expected), &expected, expectedBits, false);
        printf("Expected: 0x%X\n", expected);
        if (PROG.verbose) printBits("Expected", expectedBits, COUNT_OF(expectedBits), CRC.n);
        if (checksum != expected) 
            printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
    }

    /* Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements. */
    uint8_t msgBitsCS[sizeof(msg) * 8 + CRC.n];
    checksumMsg(message, COUNT_OF(message), checksum, CRC.n, msgBitsCS);

    /* Validate the messsage.
       If the remainder is zero print "The data is OK\n"; otherwise print "The data is not OK\n" */
    bool valid;
    // valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), CRC.n, CRC.gBits, CRC.gBits_size, originalMsgSize);
    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), originalMsgSize);
    // bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t originalMsgSize);
    
    validPrint(message, COUNT_OF(message), valid);
  
    // Changed message
    message[1] = 'a';
    // checksumMsg(message, COUNT_OF(message), checksum, CRC.n, msgBitsCS);
    // valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), CRC.n, genBits, COUNT_OF(genBits), originalMsgSize);
    // validPrint(message, COUNT_OF(message), valid);

    return 0;
}
