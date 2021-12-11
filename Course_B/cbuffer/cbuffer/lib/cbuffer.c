/** 
 * @brief SV21POTR Course B - C assignment 2
 * @author Joakim Odermalm
 * @version 0.2
 */

// #define DEBUG             // Debug messages 

/* These are here instead of in .h for demo purposes */
#define CLOCKWISE false      // Buffer direction
#define INDEXINIT (5U)       // Buffer starting index

#include "cbuffer.h"
#ifdef DEBUG
#include <stdio.h>      
#endif

#if CLOCKWISE == true
#define DIR +
#else
#define DIR + CBUFFER_SIZE -
#endif

#if ((CBUFFER_SIZE < 2U) || (CBUFFER_SIZE > 127U))
#error CBUFFER_SIZE should be an integer between 2 and 127   // Passed tests with 2 - 127. Size 1 fails write overflow test in expected way. Size >127 fails because of testing with chars.
#endif

#if ((INDEXINIT < 0) || (INDEXINIT > CBUFFER_SIZE-1))
#error INDEXINIT should be an between 0 and (buffer size - 1)   
#endif

static uint8_t head, tail = INDEXINIT;
static uint8_t buffer[CBUFFER_SIZE] = {0};
static uint8_t count = 0;

void cbuffer_clear(void) {
    count = 0;
    head = INDEXINIT;
    tail = INDEXINIT;
}

void cbuffer_write(uint8_t value) {
    buffer[tail] = value;
    if (count < CBUFFER_SIZE) {
        tail = (tail DIR 1) % CBUFFER_SIZE;
        count++;
    }
    else
        head = (head DIR 1) % CBUFFER_SIZE;
}

uint8_t cbuffer_read(void) {
    if (count == 0) {            
        #ifdef DEBUG
        fprintf(stderr, "Read from empty buffer, returned 0.\n");
        #endif
        return 0;
    }
    else {    
        uint8_t r = buffer[head];
        head = (head DIR 1) % CBUFFER_SIZE;
        count--;
        return r;
    }
}

bool cbuffer_isfull(void) {
    return count >= CBUFFER_SIZE ? true : false;
}

uint8_t cbuffer_peek(void) {
    if (count <= 0) {
        #ifdef DEBUG
        fprintf(stderr, "Peek on empty buffer, returned 0.\n");
        #endif
        return 0;
    }
    else
        return buffer[head];
}

uint8_t cbuffer_available(void) {
    return count;
}

void cbuffer_status() {
    #ifdef DEBUG
    printf("|");
    for (int i = 0; i < CBUFFER_SIZE; i++)
        printf("%c", buffer[i]);
    printf("| ");
    printf("head:%llu tail:%llu count:%llu\n", head, tail, count);
    #endif
}

void cbuffer_harderase() {
    for (int i = 0; i < CBUFFER_SIZE; i++)
        buffer[i] = 0; 
}