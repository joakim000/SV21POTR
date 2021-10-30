

#include "crc.h"



int main(int argc, char* argv[] )
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

    /* cmd args */
    struct benchargs {
        bool zoo, enc, valid,               // Command
            printSteps, verbose, timing,    // Flags
            refIn, refOut;                  // Custom spec

        char* inFile[FILENAME_MAX];
        char* outFile[FILENAME_MAX];

        int crc_spec, n, g, init, xor;
    } ca;
    if (argc < 2) {
        // Print help if no args
        puts("Help:");
        exit(EXIT_SUCCESS);
    }
    argdef_t defs[] = {
        // Commands
        { .isFlag = true, .var = (bool*)&ca.zoo, .str = "zoo" },                     // zoo
        { .isFlag = true, .var = (bool*)&ca.enc, .str = "enc" },                     // encode
        { .isFlag = true, .var = (bool*)&ca.valid, .str = "val" },                     // validate

        // Flags
        { .isFlag = true, .var = (bool*)&ca.printSteps, .str = "-steps" },                   // print steps
        { .isFlag = true, .var = (bool*)&ca.verbose, .str = "-v" },                       // verbose
        { .isFlag = true, .var = (bool*)&ca.timing, .str = "-t" },                       // timing

        // Spec
        { .isInt = true,  .var = (int*)&ca.crc_spec, .str = "-s", .defaultString = 0 },    // spec X
        // Custom spec
        { .isInt = true,  .var = (int*)&ca.n, .str = "-n", .defaultInt = 8 },    // bit width
        { .isInt = true,  .var = (int*)&ca.g, .str = "-g", .defaultInt = 0x07 }, // generator polynomial
        { .isInt = true,  .var = (int*)&ca.init, .str = "-i", .defaultInt = 0 },    // init value
        { .isInt = true,  .var = (int*)&ca.xor, .str = "-x", .defaultInt = 0 },    // final xor
        { .isFlag = true, .var = (bool*)&ca.refIn, .str = "-ri" },                      // inRef
        { .isFlag = true, .var = (bool*)&ca.refOut, .str = "-ro" },                      // outRef

        { .isString = true, .var = (char*)&ca.inFile, .str = "-in", .defaultString = "input.txt" },    // output file
        { .isString = true, .var = (char*)&ca.outFile, .str = "-out", .defaultString = "output.txt" },    // output file
    };
    processArgs(argv, argc, defs, COUNT_OF(defs));
    // Set flags
    if (ca.printSteps) PROG.printSteps = true;
    if (ca.verbose) PROG.verbose = true;
    if (ca.timing) PROG.timing = true;
    /* end cmd args */

    // Show CRC inventory
    if (ca.zoo) {
        zooTour(zoo, COUNT_OF(zoo));
        exit(EXIT_SUCCESS);
    }

    /* Load CRC spec */
    crc_t new_crc;
    crc = &new_crc;
    loadSpec(zoo, ca.crc_spec, &new_crc, false); 
    

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
    expected = CRC.checkAB;  // Spec
    // expected = 0x54FB;  // Example
    // uint8_t message[] = {'A', 'B', 'C'};
    // expected = 0xD59;  // Example
    // uint8_t message[] = {'A', 'B', 'C', 'D'}; 
    // expected = 0x6531;  // Example
    // uint8_t message[] = {'A', 'B', 'C', 'D', 'E'}; 
    // expected = 0x3556;  // Example
    // uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    // expected = 0xB35;  // Example
    // uint8_t message[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; 
    // expected = 0x59E;  // Should match example implementation
    // uint8_t message[] = {'F', 'a', 'r', 'o', 'c', 'h'};  // Example impl: 0x2535, spec: 


    // Assignment requirements for message length
    messageLengthCheck(COUNT_OF(message));

    // Create working copy
    uint8_t msg[COUNT_OF(message)];
    for (int i = 0; i < COUNT_OF(message); i++)
        msg[i] = message[i];
    // memcpy(msg, message, sizeof(msg));

    // Store message size before padding to avoid errors arising from over-padding
    size_t originalMsgSize = sizeof(msg) * 8;

    // Prepare padding bits
    TOWIDTH(initBits);

    // Convert msg to array of bit values, add padding 
    // uint8_t msgBits[sizeof(msg) * 8 + CRC.n];       // Spec
    uint8_t msgBits[sizeof(msg) * 8 + PADSIZE];           // Flag toggle 

    if ( CRC.inputLSF )
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, CRC.n,  padBits);    // Spec
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, PADSIZE,  initBits);        // Flag toggle 
    else
        ints2bitsMSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, CRC.n, initBits);
    if (VERBOSE) printBits("Message", msgBits, COUNT_OF(msgBits), 0);

    if (crc->init > 0) {
        // Get actual initBits from seed
        for (int i = 0; i < crc->n; i++) {
            // printf("initBit:%d ^= msgBit:%d = %d", initBits[i], msgBits[i], initBits[i] ^ msgBits[i]);
            initBits[i] ^= msgBits[i];
            // printf("   new initBit:%d\n", initBits[i]);
        }

        printf("\n  initBits:  "); i2p(initBits, COUNT_OF(initBits), 0, 0, 1);
        printf("\n  crc msgBits:  "); i2p(msgBits, COUNT_OF(msgBits), crc->n*2, 0, 1);

        // Write actual initBits to padding
        printf("msgBits count:%d ", COUNT_OF(msgBits));
            i2p(msgBits, COUNT_OF(msgBits), 0, 0, 1);

        for (int i = COUNT_OF(msgBits) - crc->n, j = 0; i < COUNT_OF(msgBits); i++, j++) {
            // printf("initBit:%d ^= msgBit:%d = %d", initBits[i], msgBits[i], initBits[i] ^ msgBits[i]);
            msgBits[i] = initBits[j];  
            // printf("i: %d", i);
        }

        printf("\n  crc msgBits:  "); i2p(msgBits, COUNT_OF(msgBits), crc->n*2, 0, 1);
        printf("\n  msgBits:  ");    i2p(msgBits, COUNT_OF(msgBits), 0, 0, 1);

    }

    /* Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB */

    // Print original message before checksum
    if (PRINTMSG) {
        char msgStr[COUNT_OF(message) + 1];
        charArrayToString(msg, COUNT_OF(message), msgStr);
        printf("\nOriginal message:\n%s\n", msgStr);
    }

    // Get checksum
    // int32_t checksum;
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
