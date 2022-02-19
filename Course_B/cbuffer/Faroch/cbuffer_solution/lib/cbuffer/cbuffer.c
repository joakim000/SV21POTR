#include "cbuffer.h"

#if ((CBUFFER_SIZE < 8U) || (CBUFFER_SIZE > 255U))
#error CBUFFER_SIZE should be an integer in the range of 8 and 255
#endif

static uint8_t tail = 0;     // tail is used for writing data to the buffer.
static uint8_t head = 0;     // head is used for reading data from the buffer.
static uint8_t elem_num = 0; // Number of elements stored in the buffer.
static uint8_t buffer[CBUFFER_SIZE] = {0};

void cbuffer_clear(void)
{
    tail = 0;
    head = 0;
    elem_num = 0;
}

void cbuffer_write(uint8_t value)
{
    buffer[tail] = value;
    tail = (tail + 1U) % CBUFFER_SIZE;

    if (elem_num == CBUFFER_SIZE) // If the buffer is full, the oldest element is overwritten.
    {
        head = (head + 1U) % CBUFFER_SIZE; // We need to move head
    }
    else
    {
        elem_num++;
    }
}

uint8_t cbuffer_read(void)
{
    uint8_t data = buffer[head];

    if (elem_num > 0) // If the buffer is not empty
    {
        head = (head + 1) % CBUFFER_SIZE;
        elem_num--;
    }

    return data;
}

bool cbuffer_isfull(void)
{
    return (elem_num == CBUFFER_SIZE);
}

uint8_t cbuffer_peek(void)
{
    return buffer[head];
}

uint8_t cbuffer_available(void)
{
    return elem_num;
}
