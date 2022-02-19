/**
 * @file calculator.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A module to add, subtract, multiply, divide and modulus two unsigned integers.
 * 
 * @version 0.1
 * @date 2021-03-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief This function is used to convert a string to an unsigned integer
 * 
 * @param str The string to convert to unsigned int
 * @param ptr A pointer to an unsigned int which is used to store the number.
 * @return uint32_t* NULL if the string is not a number. Otherwise ptr is returned.
 */
uint32_t *strtouint(const char *str, uint32_t *ptr);

uint32_t add(uint32_t m, uint32_t n);

int32_t subtract(uint32_t m, uint32_t n);

uint32_t multiply(uint32_t m, uint32_t n);

uint32_t divide(uint32_t m, uint32_t n);

uint32_t modulus(uint32_t m, uint32_t n);

#endif /* CALCULATOR_H */
