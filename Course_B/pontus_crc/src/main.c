#include <stdio.h>
#include <stdint.h>
#include "crc-w.h"
#include "crc_types.h"

int main()
{
    uint8_t test_123456789[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    printf("CRC -> 0x%X Expected->0x%X", crc(test_123456789, sizeof(test_123456789), CRC_32_BZIP2), CRC_32_BZIP2.Check);

    return 0;
}