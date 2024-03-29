#ifndef CBUFFER_H
#define CBUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define CBUFFER_SIZE (8U) // Size of the buffer

/**
 * @brief This function is used to clear the buffer.
 *
 */
void cbuffer_clear(void);

/**
 * @brief This function is used to check if the buffer is full or not
 *
 * @return true if the bufer is full
 * @return false if the buffer is not full
 */
bool cbuffer_isfull(void);

/**
 * @brief This function is used to peek data in the buffer.
 *        Peeking means; read the data but don't change the head.
 *
 * @return uint8_t The peeked data.
 */
uint8_t cbuffer_peek(void);

/**
 * @brief This function is used to read data from the buffer
 *
 * @return uint8_t The read data.
 */
uint8_t cbuffer_read(void);

/**
 * @brief This function returns the number of elements stored in the buffer.
 *
 * @return uint8_t number of elements stored in the buffer.
 */
uint8_t cbuffer_available(void);

/**
 * @brief This function is used to write data to the buffer.
 *
 * @param value The data which is supposed to be written to the buffer.
 */
void cbuffer_write(uint8_t value);

#endif /* CBUFFER_H */
