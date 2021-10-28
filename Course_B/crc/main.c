#include "crc.h"

// Make errors to match assignment examples
// #define MATCH_EXAMPLES
#if defined(MATCH_EXAMPLES)
#define PADSIZE padSize + 1             // Overpad by 1 bit
#define REMLOOPEND msgSize - padSize    // Allow processing to continue after message end, 1 step / overpadding bit.
#else
#define PADSIZE padSize                 // Normal padding
#define REMLOOPEND orginalMsgSize       // Always stop processing at message end. This mitigates errors from overpadding.
#endif

#define DA_TYPE uint8_t
// #include "crc_zoo.h"

// extern prog_t* prog;
// extern crc_t* crc;  
// extern msg_t* msg;
// extern expect_t* expect;

int main(void)
{
    #include "crc_zoo.c"

    // crc_t new_crc = {
    //     .n = 15,
    //     .g = 0xC599,
    //     .il1 = false,
    //     .initial = 0,
    //     .xor = 0,
    //     .inputLSF = true,
    //     .resultLSF = false
    // };
    // crc = &crc15can;

    puts("printing stuff from crc struct inside getRem");
    crc_t new_crc;
    loadDef(zoo, 10, &new_crc);
    crc = &new_crc;

    printf("Desc: %s\n", (*crc).description);
    printf("n:%d  g:%X  xor:%X\n", (*crc).n, (*crc).g, (*crc).xor);
    i2p(&(*crc).gbits, 32, (*crc).gbits_size, 0, 3);
    i2p(&(*crc).padbits, 32, (*crc).n, 0, 3);
    i2p(&(*crc).xorbits, 32, (*crc).n, 0, 3);


    puts("crc struct test done, exiting");
    // exit(EXIT_SUCCESS);

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
        .expected = 0x48B1,  // Correct
        // expected = 0x54FB,  // Example
    };
    expect = &new_expect;
    uint8_t val[]  = {'A', 'B'};
    daCreate(&(*expect).msg, val, COUNT_OF(val));
    uint8_t boll = daGet(&(*expect).msg, 2);
    daAdd(&(*expect).msg, 'f');
    char troll[] = "troll";
    // memcpy(struct->array, some_array, sizeof(struct->array));

    // Expected checksum value for testing checksum calculation. Skips check when set to 0.
    uint32_t expected = 1; 

    // Define inputs
    // uint32_t gen = POLYNOMIAL;
    uint8_t padSize = PAD;

    // uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\n'};
    // uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    uint8_t message[] = {'H', 'e', 'l', 'l', 'o'};
    // expected = 0xB35;
    // expected = EXPECTED;
    // uint8_t message[] = {'A', 'B'}; // 15 zeros have been appended to the message
    // expected = 0x48B1;  // Correct
    // expected = 0x54FB;  // Example

    // uint8_t message[] = {'@'};
    // expected = 0x0;  //CRC-8

    // uint8_t message[] = {'@', '@'};
    // expected = 0xC7; //CRC-8 0x07
    // expected = 0x4508;   //CRC16 0x1021
    // expected = 0xD6;  //CRC-3 0xB

    // uint8_t message[] = {'A', 'A'};
    // expected = 0xC0;  //CRC-8 0x07
    // expected = 0xDD;  //CRC-3 0xB

    // uint8_t message[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    expected = 0x2CF;  

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
    // msg.msg = 
    uint8_t msg[COUNT_OF(message)];
    for (int i = 0; i < COUNT_OF(message); i++)
        msg[i] = message[i];

    // Store message size before padding to avoid errors arising from over-padding
    size_t originalMsgSize = sizeof(msg) * 8;

    // Prepare padding bits
    uint32_t initial = INITIAL;
    uint8_t padBitsRaw[sizeof(initial) * 8];
    int2bits(sizeof(initial), &initial, padBitsRaw, false );
    if (VERBOSE) printBits("Pad raw", padBitsRaw, COUNT_OF(padBitsRaw), padSize);
    // Crop to size
    uint8_t padBits[padSize];
    for (int i = COUNT_OF(padBits) - 1, j = COUNT_OF(padBitsRaw) - 1; i >= 0; i--, j--)
        padBits[i] = padBitsRaw[j];
    if (VERBOSE) printBits("Pad", padBits, COUNT_OF(padBits), padSize);

    // Convert msg to array of bit values, add padding 
    uint8_t msgBits[sizeof(msg) * 8 + padSize];
    // uint8_t msgBits[sizeof(msg) * 8 + padSize +1]; // CRC-15 CAN'T
    if (MSGLSF)
        ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize,  padBits);
        // ints2bitsLSF(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize +1,  NULL);
    else
        ints2bits(sizeof(msg), sizeof(msg[0]), &msg, msgBits, padSize, padBits);
    if (VERBOSE) printBits("Message", msgBits, COUNT_OF(msgBits), 0);





    // Convert generator polynomial to array of bit values
    // uint8_t genBitsRaw[sizeof(gen) * 8 + IMPLICIT_LEADING_1];
    // int2bits(sizeof(gen), &gen, genBitsRaw, IMPLICIT_LEADING_1);
    // // Crop to size
    // uint8_t genBits[padSize + 1];
    // for (int i = COUNT_OF(genBits) - 1, j = COUNT_OF(genBitsRaw) - 1; i >= 0; i--, j--)
    //     genBits[i] = genBitsRaw[j];
    // // bitSlice(-1, padSize, genBitsRaw, COUNT_OF(genBitsRaw), genBits);
    // if (IMPLICIT_LEADING_1)
    //     genBits[0] = 1;
    // printf("Generator: 0x%X\n", gen);
    // if (VERBOSE) printBits("GeneratorRaw", genBitsRaw, COUNT_OF(genBitsRaw), padSize+1);
    // if (VERBOSE || expected) printBits("Generator", genBits, COUNT_OF(genBits), padSize+1);

    printf("Generator: 0x%X\n", (*crc).g);
    if (VERBOSE) printBits("GeneratorRaw", (*crc).gbits, sizeof((*crc).g) * 8, 0);
    if (VERBOSE || expected) printBits("Generator",  (*crc).gbits, sizeof((*crc).g) * 8, (*crc).gbits_size);





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
    // int32_t checksum = getRem(msgBits, COUNT_OF(msgBits), (*crc).gbits, (*crc).gbits_size, (*crc).n, originalMsgSize);
    int32_t checksum = getRem_new(msgBits, COUNT_OF(msgBits), originalMsgSize);
    printf("Checksum: 0x%X\n", checksum);

    // Compare to expected checksum
    if (expected) {
        uint8_t checksumBits[sizeof(checksum) * 8];
        int2bits(sizeof(checksum), &checksum, checksumBits, false);
        printBits("Checksum", checksumBits, COUNT_OF(checksumBits), padSize);   

        // uint8_t boll = daGet(&(*expect).msg, 2);
        // uint8_t new_expectedBits[sizeof(expected) * 8];
        // (*expect).expectedbits = new_expectedBits;
        // int2bits(sizeof(expected), &expected, (*expect).expectedbits, false);
        
        uint8_t expectedBits[sizeof(expected) * 8];
        int2bits(sizeof(expected), &expected, expectedBits, false);
        printf("Expected: 0x%X\n", expected);
        printBits("Expected", expectedBits, COUNT_OF(expectedBits), padSize);
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
    valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), padSize, (*crc).gbits, (*crc).gbits_size, originalMsgSize);
    validPrint(message, COUNT_OF(message), valid);
  
    // Changed message
    message[1] = 'a';
    // checksumMsg(message, COUNT_OF(message), checksum, padSize, msgBitsCS);
    // valid = validate(msgBitsCS, COUNT_OF(msgBitsCS), padSize, genBits, COUNT_OF(genBits), originalMsgSize);
    // validPrint(message, COUNT_OF(message), valid);

    return 0;
}
