#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "jlibc/binutils.h"
#include "jlibc/cmdargs.h"
#define DA_TYPE uint8_t
#include "jlibc/da.h"

// #define MATCH_EXAMPLES
#include "errors.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)
#define TOWIDTH(x)  uint8_t x[CRC.n]; bitSlice(COUNT_OF(CRC.x) - CRC.n, CRC.n, &CRC.x, 0, x);


// Flags
#define VERBOSE       false
#define VERBOSELOAD   false
#define PRINTMSG      true
#define PRINTSTEPS    false
#define PRINTSTEPSGEN true

#define SELFTEST      true
#define SELFTESTSTEPS true



// Program fields
typedef struct prog_s {
    // Flags
    uint8_t verbose;
    uint8_t printMsg;
    uint8_t printSteps;
    uint8_t printStepsGen;
    uint8_t selfTest;
    uint8_t timing;

    uint8_t testMsg[9];
} prog_t;

// CRC fields
typedef struct crc_s {
    char description[64];
    // Definition
    uint32_t n;          // Width of CRC (8, 16 or 32 bit)
    uint32_t g;          // Generator polynomial
    uint8_t il1;         // implicit_leading_1
    uint32_t init;    // Initial CRC value
    uint32_t xor;        // Final XOR value
    uint8_t inputLSF;    // Input reflected
    uint8_t resultLSF;   // Result reflected
    uint32_t check;      // Expected result from 123456789
    uint32_t checkAB;    // Expected result from "AB"

    // Work
    uint8_t gBits[33];
    size_t gBits_size;      // n+1;
    uint8_t initBits[32];
    uint8_t xorBits[32];

    // uint8_t padbitsRaw[32];
    // size_t padbitsRaw_size; 
    // uint8_t gbitsRaw[32];
    // size_t gbitsRaw_size;
    // uint8_t xorbitsRaw[32];
    // size_t xorbitsRaw_size; 
} crc_t;
// Compact format 
typedef struct crcdef_s {
        char name[100];
        uint32_t specs[9];
} crcdef_t;

// Message fields
typedef struct msg_s {
    // uint8_t* origmsg;       // Original message
    char* msgStr;              // Message as string
    size_t origMsg_size;   
    
    da msg;                    // Working copy
    da msgBits;
    da msg_cs;                  // Checksummed message
    da msgBits_cs;
    
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
    uint8_t resBits[32];
   
    // Validation
    int32_t  rem;
    uint8_t remBits[32];
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

extern prog_t* prog;
extern crc_t* crc;  
extern msg_t* msg;
extern expect_t* expect;
// Macros for readability and keystroke-saving
#define PROG (*prog)
#define  CRC (*crc)
#define  MSG (*msg)
#define EXPECT (*expect)


/**
  @brief
  @return  
*/
void checksumMsg(uint8_t message[], size_t msgSize, int32_t checksum, size_t padSize, uint8_t msgBits[]);

/**
  @brief
  @return  
*/
int32_t getRem(uint8_t msgBits[], size_t msgSize, size_t originalMsgSize );

/**
  @brief
  @return  
*/
void messageLengthCheck(size_t len);

/**
  @brief
  @return  
*/
// bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t padSize, uint8_t genBits[], size_t genSize, size_t originalMsgSize);
bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t originalMsgSize);


/**
  @brief
  @return  
*/
void validPrint(uint8_t msg[], size_t msgSize, bool valid);

/**
  @brief
  @return  
*/
void loadDef(crcdef_t zoo[], size_t index, crc_t* out);
// void loadDef(struct crc_def zoo[], size_t index, crc_t* out);

/**
  @brief
  @return  
*/
void loadSpec(crcdef_t zoo[], size_t index, crc_t* out, bool compact);

/**
  @brief Print CRC inventory
  @return  
*/
void zooTour(crcdef_t zoo[], size_t zoo_size);