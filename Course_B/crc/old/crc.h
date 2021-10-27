// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

// Flags
#define VERBOSE false
#define PRINTMSG true

int32_t getRem(uint8_t msgBits[], size_t msgSize, uint8_t genBits[], size_t genSize, size_t padSize );
void messageLengthCheck(size_t len);
void printBits(char label[], uint8_t bits[], size_t size);
bool validate(uint8_t message[], size_t msgSize, int16_t checksum, size_t padSize, uint8_t genBits[], size_t genSize);
void validPrint(uint8_t msg[], size_t msgSize, bool valid);

