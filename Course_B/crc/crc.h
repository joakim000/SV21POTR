
#include "jlibc/binutils.h"
#include "jlibc/cmdargs.h"
// #define DA_TYPE uint8_t
// #include "jlibc/da.h"
#include "jlibc/datagenerator.h"


// #define MATCH_EXAMPLES
#include "errors.h"

// Magic numbers
#define MAX_MESSAGE_ARGLENGTH 0x400 
#define MAX_MESSAGE_READLENGTH 0x1000000    
#define PRINTLIMIT 0x40

// Utility
#define TOWIDTH(x)  uint8_t x[crc->n]; bitSlice(COUNT_OF(crc->x) - crc->n, crc->n, &crc->x, 0, x);
#define STR2ARR(x, y) uint8_t y[strlen((char*)x)]; for I2(strlen((char*)x)) y[i]=x[i];

// Flags
#define VERBOSE       false
#define VERBOSELOAD   false
#define PRINTMSG      true
#define PRINTSTEPS    false
#define PRINTSTEPSGEN true
#define SELFTEST      true
#define SELFTESTSTEPS false

// Engine
#include "engine.h"
#ifdef CRC_EXPLORER_EXTERNAL
#define EXTERNAL_ENGINE_AVAILABLE true
#else
#define EXTERNAL_ENGINE_AVAILABLE false
#endif


#include "jlibc/common.h"


// Program fields
typedef struct prog_s {
    // Flags
    uint8_t verbose;
    uint8_t printMsg;
    uint8_t printSteps;
    uint8_t prt_noskip;
    uint8_t prt_nogen;
    uint8_t selfTest;
    uint8_t timing;
    uint8_t internal_engine;
    uint8_t testMsg[9];
} prog_t;


// CRC definition, serialized specs 
typedef struct crcdef_s {
        char name[0x80];
        uint64_t specs[11];
} crcdef_t;


// A test of an implemenation 
typedef struct implTest_s {
        char dt[0x20];
        char note[0x400];
        crc_t crc;
        
        // Test with check value
        uint64_t rem_check;
        bool passed_check;
        uint64_t rem_validate_check;
        bool passed_validate_check;
        uint64_t rem_changed_check;
        bool passed_changed_check;

        // Test with lorem
        msg_t msg;
        uint64_t rem_encode;
        bool passed_encode; 
        double elapsed_seconds;  
        uint64_t rem_validate_msg;
        bool passed_validate_msg;
        uint64_t rem_changed_msg;
        bool passed_changed_msg;
} implTest_t;

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
void ArrangeMsg(crc_t* crc, msg_t* msg);

/**
  @brief Set checksum bits in message
  @return  
*/
void checksumMsg(size_t paddedBitLen, uint64_t checksum, size_t width, uint8_t csmsgBits[]);

/**
  @brief Get the remainder, aka the result, aka the checksum
  @return  
*/
// uint64_t PolyDivision(uint8_t msgBits[], size_t msgSize, size_t originalMsgSize, crc_t* crc );
uint64_t PolyDivision(crc_t* crc, msg_t* msg);

/**
  @brief  Assignment requirement
  @return  
*/
void messageLengthCheck(size_t len);

/**
  @brief Validate message
  @return  
*/
bool validate(crc_t* crc, msg_t* msg);
// bool validate(uint8_t msgBits[], size_t msgBitsCount, size_t originalMsgSize, crc_t* crc);


/**
  @brief Print results of validation
  @return  
*/
void ValidPrint(uint8_t msg[], size_t msgSize, bool valid);

/**
  @brief Load CRC definition
  @return  
*/
void LoadDef(crcdef_t zoo[], size_t index, crc_t* out);

/**
  @brief Wrapper for LoadDef, prints info and performs value check test when loading 
  @return  
*/
void LoadDefWrapper(crcdef_t zoo[], size_t index, crc_t* out, bool compact);

/**
  @brief Print CRC inventory
  @return  
*/
void ZooTour(crcdef_t zoo[], size_t zoo_size);


#define HELPTEXT1 "\
 \e[1;53m\e[1;1m\e[1;7mCRC Explorer\e[1;27m\e[1;23m                              \e[1;3mSail the seven seas of CRCs\e[m\n\
  \n\
 \e[1;4mCommands\e[m\n\
  zoo       Where all the CRCs live\n\
  enc       Encode a message\n\
  val       Validate a message\n\
  \n\
 \e[1;4mInput\e[m\n\
  -c        CRC specificaton (visit zoo for indexed list)\n\
  -m        Message\n\
  -s        Checksum for validation\n\
  -in       Input file; message for encode, or checksummed for validation (format below)\n\
  -out      Output file for encode, format: [0xABC]Lorem ipsum dolor sit amet\n\
  \n\
 \e[1;4mOptions\e[m\n\
  -steps   Print step-by-step calculations (with colours!)\n\
  -nogen   \e[1;3mWith steps:\e[m Don't print generator polynominal\n\
  -noskip  \e[1;3mWith steps:\e[m Do print skipped steps (message bit is 0)\n\
\n\
  -t       Timing: simple benchmark\n\
  -v       Verbose: lots of strange debugging text\n\
\n\
\e[1;4mExamples\e[m\n\
           crc enc -s 33 -in message.txt -out output.txt\n\
           crc val -s 33 -in message.txt -c 0xABC\n\
\n\
\e[1;4mDo more\e[m\n\
  Edit crc_zoo.c to add more / custom CRC specifications\n\
  Validate and benchmark your own CRC implementation - see README\n\n"

/**
  @brief Allocation status check
  @return  0: ok  1: NULL
*/
static short allocCheck(void* p);

/**
  @brief Convert direct init value to non-direct
  @return  Converted value
*/
uint64_t ConvertInit(uint64_t poly, uint64_t init, uint8_t width);


/**
  @brief Perform validity test of CRC implementation
*/
bool checkValueTest(crc_t* crc, msg_t msg, bool run_validate);



/**
  @brief Prepare msg_t struct
  @return  
*/
msg_t* PrepareMsg(crc_t* crc, char* message);


/**
 * @brief Validate encode phase of CRC implementation for a CRC definition
 * 
 * @param crc       CRC definition
 * @param output    Print text to stdout?
 *                  0: None
 *                  1: One-line
 *                  2: Verbose
 * @return true     Passed
 * @return false    Failed
 */
uint64_t ValueCheckTest(crc_t* crc, uint8_t type, uint8_t output);

/**
 * @brief Check value-test implementation scenarios
 *        1. Encode => check value
 *        2. Validate with check value => 0
 *        3. Validate with changed message => !0
 * 
 * @param crc 
 * @param msg 
 * @return implTest_t 
 */
implTest_t ImplValid(crc_t* crc);


implTest_t ImplPerf(crc_t* crc, uint64_t set_size);

uint64_t GetRemInternal(crc_t* crc, msg_t* msg, uint64_t check);


typedef uint64_t (*GetRem_ptr_t)(crc_t* crc, msg_t* msg, uint64_t check);
extern GetRem_ptr_t GetRem_ptr;


// typedef void (*int2bits_t)(size_t const size, void const * const ptr, uint8_t out[], bool extraBit); 
// typedef uint32_t (*bits2int_t)(size_t const len, uint8_t* bits);
// extern int2bits_t int2bits;

