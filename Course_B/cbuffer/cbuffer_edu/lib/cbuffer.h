#ifndef CBUFFER_H
#define CBUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define CBUFFER_SIZE (10U)   // Size of the buffer 
#define INDEXINIT (1U)       // Buffer starting index
#define CLOCKWISE false      // Buffer direction

/**
 * @brief This function is used to reset the required variables like head and tail of the buffer.
 *        The head is used for reading data and the tail is used for writing data.
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
 * @brief This function returns the number of available data in the buffer which can be read.
 *
 * @return uint8_t number of available elements
 */
uint8_t cbuffer_available(void);

/**
 * @brief This function is used to write data to the buffer.
 *
 * @param value The data which is supposed to be written to the buffer.
 */
void cbuffer_write(uint8_t value);


#endif /* CBUFFER_H */
