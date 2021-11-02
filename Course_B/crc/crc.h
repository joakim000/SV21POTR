#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "jlibc/binutils.h"
#include "jlibc/cmdargs.h"
#define DA_TYPE uint8_t
#include "jlibc/da.h"

// #define MATCH_EXAMPLES
#include "errors.h"

// Magic numbers
#define BITSINBYTE 8
#define MAX_MESSAGE_ARGLENGTH 0x400  // Limited by stack size
#define MAX_MESSAGE_READLENGTH 0x120000  // Limited by stack size  
#define PRINTLIMIT 0x40



// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define PRINTERR(x) fprintf(stderr, "\n\e[1;31m%s\e[m", x);
#define EACH (size_t i = 0; i < size; i++)
#define I2(x) (int i = 0; i < x; i++) 
#define TOWIDTH(x)  uint8_t x[crc->n]; bitSlice(COUNT_OF(crc->x) - crc->n, crc->n, &crc->x, 0, x);
// #define STR2CHARS(x, y, z) uint8_t y[strlen(x)+z]; for (int i=0;i<strlen(x);i++)y[i]=x[i];
// #define STR2CHARS(x, y) uint8_t y[strlen(x)]; for (int i=0;i<strlen(x);i++)y[i]=x[i];
// #define STR2ARR(x, y) uint8_t y[strlen((char*)x)]; strcpy(y,(char*)x);
#define STR2ARR(x, y) uint8_t y[strlen((char*)x)]; for I2(strlen((char*)x)) y[i]=x[i];
#define CROPSTR(x, y) char y[strlen((char*)x)+1]; strcpy(y,(char*)x);

// Timing
#define TIMING(y, x) ((double)(x - y) / CLOCKS_PER_SEC)
// clock_t timer_start; clock_t timer_end; 

// Flags
#define VERBOSE       false
#define VERBOSELOAD   false
#define PRINTMSG      true
#define PRINTSTEPS    false
#define PRINTSTEPSGEN true
#define SELFTEST      true
#define SELFTESTSTEPS false

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
    uint32_t n;          // Bit width 
    uint32_t g;          // Generator polynomial
    uint8_t il1;         // implicit_leading_1
    uint32_t init;       // Initial CRC value (seed)
    uint8_t  nondirect;  // Init seed can be direct or nondirect
    uint8_t inputLSF;    // Input reflected
    uint8_t resultLSF;   // Result reflected
    uint32_t xor;        // Final XOR value
    uint32_t residue;    // Given as spec on some sites, not sure how to use it yet
    uint32_t check;      // Expected result from "123456789"
    uint32_t checkAB;    // Expected result from "AB"

    // Work
    uint8_t gBits[33];
    uint8_t initBits[32];
    uint8_t xorBits[32];
} crc_t;

// CRC definition, serialized specs 
typedef struct crcdef_s {
        char name[0x80];
        uint32_t specs[11];
} crcdef_t;

// Message fields
typedef struct msg_s {
    char* msgStr;              // Message as string 
    size_t len;
    uint8_t* msgBits;       //  
    size_t originalBitLen;    // 
    size_t paddedBitLen;    // 
    
    // CRC checksum    
    uint32_t res;
   
    // Message validation
    uint8_t* csmsgBits;       // 
    uint32_t rem;
    bool valid;

    // Calculation testing
    uint32_t expected;
} msg_t;

// Global access data structures
extern prog_t* prog;
// extern crc_t* crc;  
// extern msg_t* msg;

// Macros for readability and keystroke-saving
#define PROG (*prog)
// #define  CRC (*crc)
// #define  MSG (*msg)


/**
  @brief Arrange bits in message according to CRC spec
  @return  
*/
void arrangeMsg(crc_t* crc, msg_t* msg);

/**
  @brief Set checksum bits in message
  @return  
*/
void checksumMsg(size_t paddedBitLen, uint32_t checksum, size_t width, uint8_t csmsgBits[]);

/**
  @brief Get the remainder, aka the result, aka the checksum
  @return  
*/
uint32_t getRem(uint8_t msgBits[], size_t msgSize, size_t originalMsgSize, crc_t* crc );

/**
  @brief  Assignment requirement
  @return  
*/
void messageLengthCheck(size_t len);

/**
  @brief Validate message
  @return  
*/
bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t originalMsgSize, crc_t* crc);


/**
  @brief Print results of validation
  @return  
*/
void validPrint(uint8_t msg[], size_t msgSize, bool valid);

/**
  @brief Load CRC definition
  @return  
*/
void loadDef(crcdef_t zoo[], size_t index, crc_t* out);

/**
  @brief Wrapper for loadDef, prints info and performs value check test when loading 
  @return  
*/
void loadSpec(crcdef_t zoo[], size_t index, crc_t* out, bool compact);

/**
  @brief Print CRC inventory
  @return  
*/
void zooTour(crcdef_t zoo[], size_t zoo_size);


#define HELPTEXT1 "\
 Commands\n\
  zoo       Where all the CRCs live\n\
  enc       Encode a message\n\
  val       Validate a message\n\
  \n\
 Input\n\
  -s        CRC specificaton (visit zoo for indexed list)\n\
  -m        Message\n\
  -c        Checksum for validation\n\
  -in       Input file; message for encode, or checksummed for validation (format below)\n\
  -out      Output file for encode, format: [0xABC]Lorem ipsum dolor sit amet\n\
  \n\
 Flags\n\
  -sbs     Print tablulated calculations (with colours!)\n\
  -t       Timing: simple benchmark\n\
  -v       Verbose: lots of strange debugging text\n\
\n\
Edit crc_zoo.c to add more / custom specifications\n\
\n\
Examples:  crc enc -s 33 -in message.txt -out output.txt\n\
           crc val -s 33 -in message.txt -c 0xABC\n\n"

/**
  @brief Allocation status check
  @return  0: ok  1: NULL
*/
static short allocCheck(void* p);