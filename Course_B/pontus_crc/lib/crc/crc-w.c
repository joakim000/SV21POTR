#include <stdint.h>
#include "crc_types.h"

#define LSB 1
#define MAX_INPUT_SIZE 1024

static uint64_t reflect(uint64_t input, int bits)
{
    uint64_t i, j = 1, reflected = 0;

    for (uint64_t i = (uint64_t)1 << (bits - 1); i; i >>= 1)
    {
        if (input & i)
        {
            reflected |= j;
        }
        j <<= 1;
    }

    return reflected;
}

uint64_t crc(const uint8_t *msg, uint8_t size, struct crc_type ctype)
{
    uint64_t MSB = (1ULL << ctype.width - 1);
    uint64_t reg = ctype.initial;

    // Silly non direct things
    if (ctype.nondir == 0)
    {
        for (uint64_t i = 0; i < ctype.width; i++)
        {
            uint64_t xor = reg & LSB;

            xor&&(reg ^= ctype.polynomial);
            reg >>= 1;
            xor&&(reg |= MSB);
        }
    }

    for (uint64_t pos = 0; pos < size; pos++) // Not appended bits
    {
        uint8_t bits = msg[pos];
        if (ctype.refin)
        {
            bits = reflect(bits, 8);
        }

        for (size_t i = 0; i < 8; i++)
        {
            uint64_t xor = reg & MSB;
            reg <<= 1;
            reg = reg | ((bits >> (7 - i)) & 0x01);
            xor&&(reg ^= ctype.polynomial);
        }
    }

    // appended bits
    for (uint64_t i = 0; i < ctype.width; i++)
    {
        uint64_t xor = reg & MSB;
        reg <<= 1;
        xor&&(reg ^= ctype.polynomial);
    }

    // reflect out here if needed
    if (ctype.refout)
    {
        reg = reflect(reg, ctype.width);
    }

    // XOR all the time XOR with all zeros is just same value
    reg ^= ctype.xorout;

    // Need to return only the bits of interest
    // And do a dirty 64 W thing
    if (ctype.width != 64)
    {
        uint64_t mask = (1ULL << ctype.width) - 1ULL;
        reg = reg & mask;
    }

    return reg;
}