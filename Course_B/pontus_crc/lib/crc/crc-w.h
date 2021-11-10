#ifndef CRC_W_H
#define CRC_W_H

#include <stdint.h>
#include "crc_types.h"

#define MAX_INPUT_SIZE 1024

uint64_t crc(const uint8_t *msg, uint8_t size, struct crc_type ctype);

#endif