#include "cbuffer.h"
#include "stdio.h"        

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
    tail = (tail - 1) % CBUFFER_SIZE;
    count++;
}

uint8_t cbuffer_read(void) {
    if (count <= 0) {
        fprintf(stderr, "Read from empty buffer, returned 0.");
        return 0;
    }
    else {    
        uint8_t r = buffer[head];
        head = (head - 1) % CBUFFER_SIZE;
        count--;
        return r;
    }
}

bool cbuffer_isfull(void) {
    return count >= CBUFFER_SIZE ? true : false;
}

uint8_t cbuffer_peek(void) {
    return buffer[head];
}

uint8_t cbuffer_available(void) {
    return count;
}

void cbuffer_status() {
    printf("|");
    for (int i = 0; i < CBUFFER_SIZE; i++)
        printf("%c", buffer[i]);
    printf("| ");
    printf("head:%llu tail:%llu count:%llu\n", head, tail, count);
}

