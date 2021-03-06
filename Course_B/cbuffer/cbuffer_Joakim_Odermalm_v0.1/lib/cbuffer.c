/** 
 * @brief SV21POTR Course B - C assignment 2
 * @author Joakim Odermalm
 */

#include "cbuffer.h"
// #include <stdio.h>       // Error messages disabled by requirement     

#if ((CBUFFER_SIZE < 8U) || (CBUFFER_SIZE > 32U))
#error CBUFFER_SIZE should be an integer between 8 and 32
#endif


static bool full = false;
static uint8_t tail = 0U;
static uint8_t head = 0U;
static uint8_t buffer[CBUFFER_SIZE] = {0};
static uint8_t count = 0;

void cbuffer_clear(void) {
    count = 0;
    tail = 0;
    head = 0;
}

void cbuffer_write(uint8_t value) {
    buffer[tail] = value;
    if (count < CBUFFER_SIZE) {
        tail = (tail - 1) % CBUFFER_SIZE;
        count++;
    }
    else
        head = (head - 1) % CBUFFER_SIZE;
}

uint8_t cbuffer_read(void) {
    if (count <= 0) {
        // fprintf(stderr, "Read from empty buffer, returned 0.\n");
        return 0;
    }
    else {    
        uint8_t r = buffer[head];
        head = (head - 1) % CBUFFER_SIZE;
        if (count > 0) 
            count--;
        return r;
    }
}

bool cbuffer_isfull(void) {
    return count >= CBUFFER_SIZE ? true : false;
}

uint8_t cbuffer_peek(void) {
    if (count <= 0) {
        // fprintf(stderr, "Peek on empty buffer, returned 0.\n");
        return 0;
    }
    else
        return buffer[head];
}

uint8_t cbuffer_available(void) {
    return count;
}

void cbuffer_status() {
    // printf("|");
    // for (int i = 0; i < CBUFFER_SIZE; i++)
    //     printf("%c", buffer[i]);
    // printf("| ");
    // printf("head:%llu tail:%llu count:%llu\n", head, tail, count);
}

