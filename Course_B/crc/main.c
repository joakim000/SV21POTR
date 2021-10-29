

#include "crc.h"

int main(void)
{
    // Unconventional use of #include, just a convenient way to put
    // the CRC definitions in a separate file.
    #include "crc_zoo.c"
    
    puts("printing stuff from crc struct inside getRem");
    crc_t new_crc;
    loadDef(zoo, 10, &new_crc);
    crc = &new_crc;

    printf("Desc: %s\n", (*crc).description);
    printf("n:%d  g:%X  xor:%X\n", (*crc).n, (*crc).g, (*crc).xor);
    i2p(&(*crc).gbits, COUNT_OF((*crc).gbits), (*crc).gbits_size, 0, 3);
    i2p(&(*crc).initbits, COUNT_OF((*crc).initbits), (*crc).n, 0, 3);
    i2p(&(*crc).xorbits, COUNT_OF((*crc).xorbits), (*crc).n, 0, 3);


    // uint8_t new_msg_resbits[12];
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

    // uint8_t message[] = {'A', 'B'}; 
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
    uint8_t message[] = {'F', 'a', 'r', 'o', 'c', 'h'};  // Example impl: 0x2535, spec: 


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
    uint32_t initial = (*crc).initial;
    uint8_t padBitsRaw[sizeof(initial) * 8];
    int2bits(sizeof(initial), &initial, padBitsRaw, false );
    if (VERBOSE) printBits("Pad raw", padBitsRaw, COUNT_OF(padBitsRaw), 0);
    // Crop to size
    uint8_t padBits[(*crc).n];
    for (int i = COUNT_OF(padBits) - 1, j = COUNT_OF(padBitsRaw) - 1; i >= 0; i--, j--)
        padBits[i] = padBitsRaw[j];
    if (VERBOSE) printBits("Pad", padBits, COUNT_OF(padBits), (*crc).n);

    // Convert msg to array of bit values, add padding 
    // uint8_t msgBits[sizeof(msg) * 8 + (*crc).n];       // Spec
    // uint8_t msgBits[sizeof(msg) * 8 + (*crc).n +1];    // CRC-15/CANT
    uint8_t msgBits[sizeof(msg) * 8 + PADSIZE];           // Flag toggle 


    if ( (*crc).inputLSF )
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, (*crc).n,  padBits);    // Spec
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, (*crc).n +1,  NULL);    // CRC-15/CANT
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, PADSIZE,  padBits);        // Flag toggle 
    else
        ints2bitsMSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, (*crc).n, padBits);
    if (VERBOSE) printBits("Message", msgBits, COUNT_OF(msgBits), 0);

    // Print generator poly
    printf("Generator: 0x%X\n", (*crc).g);
    if (VERBOSE) printBits("GeneratorRaw", (*crc).gbits, sizeof((*crc).g) * 8, 0);
    if (VERBOSE || expected) printBits("Generator",  (*crc).gbits, COUNT_OF( (*crc).gbits ), (*crc).gbits_size);

    /* Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB */

    // Print original message before checksum
    if (PRINTMSG) {
        char msgStr[COUNT_OF(message) + 1];
        charArrayToString(msg, COUNT_OF(message), msgStr);
        printf("\nOriginal message:\n%s\n", msgStr);
    }

    // Get checksum
    // int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), (*crc).gbits, (*crc).gbits_size, (*crc).n, originalMsgSize);
    int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), originalMsgSize);
    printf("Checksum: 0x%X\n", checksum);

    // Compare to expected checksum
    if (expected) {
        uint8_t checksumBits[sizeof(checksum) * 8];
        int2bits(sizeof(checksum), &checksum, checksumBits, false);
        printBits("Checksum", checksumBits, COUNT_OF(checksumBits), (*crc).n);   

        uint8_t expectedBits[sizeof(expected) * 8];
        int2bits(sizeof(expected), &expected, expectedBits, false);
        printf("Expected: 0x%X\n", expected);
        printBits("Expected", expectedBits, COUNT_OF(expectedBits), (*crc).n);
        if (checksum != expected) 
            printf("\e[1;31m%s\e[m\n", "Checksum does not match expected value."); // red
    }

    /* Checksum the messsage. I.e replace the zeros with the CRC accroding to the requirements. */
    uint8_t msgBitsCS[sizeof(msg) * 8 + (*crc).n];
    checksumMsg(message, COUNT_OF(message), checksum, (*crc).n, msgBitsCS);

    /* Validate the messsage.
       If the remainder is zero print "The data is OK\n"; otherwise print "The data is not OK\n" */
    bool valid;
    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), (*crc).n, (*crc).gbits, (*crc).gbits_size, originalMsgSize);
    validPrint(message, COUNT_OF(message), valid);
  
    // Changed message
    message[1] = 'a';
    // checksumMsg(message, COUNT_OF(message), checksum, (*crc).n, msgBitsCS);
    // valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), (*crc).n, genBits, COUNT_OF(genBits), originalMsgSize);
    // validPrint(message, COUNT_OF(message), valid);

    return 0;
}
