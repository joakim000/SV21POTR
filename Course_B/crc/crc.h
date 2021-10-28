#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "jlibc/binutils.h"
#include "jlibc/cmdargs.h"
#include "jlibc/da.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

// Flags
#define VERBOSE false
#define PRINTMSG true
#define PRINTSTEPS true
#define PRINTSTEPSGEN true

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
#define PAD 15
#define MSGLSF true
#define CRCLSF false
    // Hex    Dec    LSF                MSF               
    // 0xC599 50585  1001100110100011   1100010110011001                
    //                                  1100010110011001

#define INITIAL 0x0

#define IMPLICIT_LEADING_1 0
// #define PAD 3
// #define POLYNOMIAL 0xB

// #define PAD 8
// #define POLYNOMIAL 0x07  // i1 0111      = 0x17
// #define POLYNOMIAL 0x39  // i1 0011 1001 = 0x139
// #define POLYNOMIAL 0xD5  // i1 1101 0101 = 0x1D5
// #define POLYNOMIAL 0x107 //  1 0000 0111
// #define POLYNOMIAL 0x1D

// #define PAD 16
// #define POLYNOMIAL 0x8005
// #define POLYNOMIAL 0x1021


// #define PAD 32
// #define POLYNOMIAL 0x04C11DB7
// #define POLYNOMIAL 0x814141AB
// #define POLYNOMIAL 0xAF





// Program fields
typedef struct prog_s {
    // Flags
    bool verbose;
    bool printmsg;
    bool printsteps;
} prog_t;

// CRC fields
typedef struct crc_s {
    char description[64];
    // Definition
    uint32_t n;          // Width of CRC (8, 16 or 32 bit)
    uint32_t g;          // Generator polynomial
    bool il1;            // implicit_leading_1
    uint32_t initial;    // Initial CRC value
    uint32_t xor;        // Final XOR value
    bool inputLSF;       // Input reflected
    bool resultLSF;      // Result reflected
    uint32_t check;      // Expected result from 123456789
    uint32_t checkAB;    // Expected result from "AB"


    // Work
    uint8_t gbits[32];
    // uint8_t* gbits;
    size_t gbits_size;      // n+1;
    uint8_t padbits[32];
    // size_t padbits_size; // = n
    uint8_t xorbits[32];
    // size_t xorbits_size; // = n

    // uint8_t padbitsRaw[32];
    // size_t padbitsRaw_size; 
    // uint8_t gbitsRaw[32];
    // size_t gbitsRaw_size;
    // uint8_t xorbitsRaw[32];
    // size_t xorbitsRaw_size; 
} crc_t;
// Compact format 
struct crc_def {
        char name[100];
        uint32_t specs[9];
};

// Message fields
typedef struct msg_s {
    // uint8_t* origmsg;       // Original message
    char* msgstr;              // Message as string
    size_t origmsg_size;   
    
    da msg;                    // Working copy
    da msgbits;
    da csmsg;                  // Checksummed message
    da csmsgbits;
    
    // uint8_t* msg;           // Working copy
    // size_t  msg_size;
    // uint8_t* msgbits;
    // size_t  msgbits_size; 
    // uint8_t* csmsg;         // Checksummed message
    // size_t  csmsg_size;
    // uint8_t* csmsgbits;
    // size_t  csmsgbits_size; 

    // CRC result    
    int32_t  res;
    uint8_t resbits[32];
    // size_t resbits_size;      // = n;
    // uint8_t resbitsRaw[32];
    // size_t resbitsRaw_size;

    // Validation
    int32_t  rem;
    uint8_t rembits[32];
    // size_t rembits_size;        // = n;
    bool valid;
    // // Calculation testing
    // int32_t expected;
    // uint8_t* expectedbits;
    // size_t expectedbits_size;
} msg_t;

// Testing fields
typedef struct expect_s {
    // Calculation testing
    char* desc;
    uint8_t* msgStr;
    da msg;
    int32_t expected;
    uint8_t expectedbits[32];
    size_t expectedbits_size;
} expect_t;


prog_t* prog;
crc_t* crc;  
msg_t* msg;
expect_t* expect;



/**
  @brief
  @return  
*/
void checksumMsg(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t msgBits[]);

/**
  @brief
  @return  
*/
int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize, size_t orginalMsgSize );
int32_t getRem_new(uint8_t msgBits[], size_t msgSize, size_t orginalMsgSize );

/**
  @brief
  @return  
*/
void messageLengthCheck(size_t len);

/**
  @brief
  @return  
*/
bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t padSize, uint8_t genBits[], size_t genSize, size_t originalMsgSize);

/**
  @brief
  @return  
*/
void validPrint(uint8_t msg[], size_t msgSize, bool valid);

/**
  @brief
  @return  
*/
void loadDef(struct crc_def zoo[], size_t index, crc_t* out);
