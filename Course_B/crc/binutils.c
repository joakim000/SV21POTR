/*************************************
 * Utils for working with bit values
 * 
 * @author Joakim Odermalm
 ************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "binutils.h"

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

void int2bitsLSF(size_t const size, void const * const ptr, uint8_t out[]) {
    int  byte_index, 
         bit_index;
 
    uint8_t *byte = (uint8_t*) ptr;
    uint8_t bit;

    for (byte_index = 0; byte_index < size; byte_index++) {        //LSF
        for (bit_index = 0; bit_index < 8; bit_index++) {       // LSF
            bit = (byte[byte_index] >> bit_index) & 1;
            if (!bit)
                out[byte_index * 8 + bit_index] = 0;
            else
                out[byte_index * 8 + bit_index] = 1;
        }
    }
}

void int2bitsMSF(size_t const size, void const * const ptr, uint8_t out[]) {
    int  byte_index, 
         bit_index,
         byte_write_index = 0,
         bit_write_index;
 
    uint8_t *byte = (uint8_t*) ptr;
    uint8_t bit;

    for (byte_index = size-1; byte_index >= 0; byte_index--) {   //MSF
        bit_write_index = 0;
        for (bit_index = 7; bit_index >= 0; bit_index--) {         // MSF
            bit = (byte[byte_index] >> bit_index) & 1;
            if (!bit) {
                out[byte_write_index * 8 + bit_write_index++] = 0;
            }
            else {
                out[byte_write_index * 8 + bit_write_index++] = 1;
            }
        }
        byte_write_index++;
    }
}

uint32_t bits2intLSF(size_t const size, uint8_t* bits) {
    uint32_t r = 0;
    for (int bit_index = 0; bit_index < size; bit_index++) {       
        if (bits[bit_index]) 
            r |= 1UL << (bit_index);
        else
            r &= ~(1UL << (bit_index) );
    }

    // printf("bits2intLSF called. Returning %d. Input bits: ", r);
    // for EACH 
    //     printf("%d", bits[i]);
    // puts("");

    return r; 
}

uint32_t bits2intMSF(size_t const len, uint8_t* bits) {
    uint32_t r = 0;
    int bit_write_index = len - 1;
    for (int bit_index = 0; bit_index < len; bit_index++) {       
        if (bits[bit_index]) 
            r |= 1UL << (bit_write_index--);
        else    
            r &= ~(1UL << (bit_write_index--) );
    }
    return r; 
}

void ints2bitsLSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t padBits[]) {
    uint8_t bitsArray[type_size * 8];

    uint8_t*  ints8  = (uint8_t*) ptr;
    uint16_t* ints16 = (uint16_t*) ptr;
    uint32_t* ints32 = (uint32_t*) ptr;

    for (int i = 0; i < (size / type_size); i++) {
        switch (type_size) {
        case 1:
            int2bitsLSF(type_size, &ints8[i], bitsArray);
            break;
        case 2:
            int2bitsLSF(type_size, &ints16[i], bitsArray);
            break;
        case 4:
            int2bitsLSF(type_size, &ints32[i], bitsArray);
            break;
        default:
            fprintf(stderr, "Unsupported type size, exiting.");
            exit(EXIT_FAILURE);
        }

        size_t el_start_index = i * type_size * 8;
        for (int j = 0; j < type_size * 8; j++)
            out[j + el_start_index] = bitsArray[j];
    }
    if (padSize) {
        size_t bitCount = size * type_size * 8 + padSize; 
        for (int i = (bitCount - 1); i > (bitCount - padSize - 1); i--)
            out[i] = 0; 
    }
}   

void ints2bitsMSF(size_t const size, size_t const type_size, void const * const ptr, uint8_t out[], size_t padSize, uint8_t padBits[]) {
    uint8_t bitsArray[type_size * 8];

    uint8_t*  ints8  = (uint8_t*) ptr;
    uint16_t* ints16 = (uint16_t*) ptr;
    uint32_t* ints32 = (uint32_t*) ptr;
    
    for (int i = 0; i < (size / type_size); i++) {
        switch (type_size) {
        case 1:
            int2bitsMSF(type_size, &ints8[i], bitsArray);
            break;
        case 2:
            int2bitsMSF(type_size, &ints16[i], bitsArray);
            break;
        case 4:
            int2bitsMSF(type_size, &ints32[i], bitsArray);
            break;
        default:
            fprintf(stderr, "Unsupported type size, exiting.");
            exit(EXIT_FAILURE);
        }

        size_t el_start_index = i * type_size * 8;
        for (int j = 0; j < type_size * 8; j++)
            out[j + el_start_index] = bitsArray[j];
    }
    if (padSize) {
        size_t bitCount = size * type_size * 8 + padSize; 
        for (int i = (bitCount - 1), j = (padSize - 1); i > (bitCount - padSize - 1); i--, j--)
            if (padBits == NULL) 
                out[i] = 0;
            else
                out[i] = padBits[j]; 
    }
}

void bits2intsMSFtest(size_t const total_bits, size_t const type_size, uint8_t const bits[], void const * const out_ptr) {
    uint8_t bitsArray[type_size * 8];
    size_t elem_total = total_bits / (type_size * 8);
    
    size_t bits_iter = 0;
    size_t bitsArray_iter = 0;
    size_t out_iter = 0;
    
    uint8_t*  out8  = (uint8_t*) out_ptr;
    uint16_t* out16 = (uint16_t*) out_ptr;
    uint32_t* out32 = (uint32_t*) out_ptr;

    while (bits_iter <= total_bits) {      
        if (bitsArray_iter < (type_size * 8 )) {
            bitsArray[bitsArray_iter] = bits[bits_iter];
            bitsArray_iter++; bits_iter++;
        }
        else {
             switch (type_size) {
        case 1:
            out8[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
            break;
        case 2:
            out16[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
            break;
        case 4:
            out32[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
            break;
        default:
            fprintf(stderr, "Unsupported type size, exiting.");
            exit(EXIT_FAILURE);
        }
            
            bitsArray_iter = 0;
        }
    } 
}   

void bits2intsMSF(size_t const total_bits, size_t const type_size, uint8_t const bits[], uint32_t out[]) {
    uint8_t bitsArray[type_size * 8];
    size_t elem_total = total_bits / (type_size * 8);
    
    size_t bits_iter = 0;
    size_t bitsArray_iter = 0;
    size_t out_iter = 0;
    
    while (bits_iter <= total_bits) {      
        if (bitsArray_iter < (type_size * 8 )) {
            bitsArray[bitsArray_iter] = bits[bits_iter];
            bitsArray_iter++; bits_iter++;
        }
        else {
            out[out_iter++] = bits2intMSF(type_size * 8, bitsArray);
            bitsArray_iter = 0;
        }
    } 
}   

void bits2intsLSF(size_t const total_bits, size_t const type_size, uint8_t const bits[], uint32_t out[]) {
    uint8_t bitsArray[type_size * 8];
    size_t elem_total = total_bits / (type_size * 8);
    
    // puts("\n\nInside bits2intsLSF");
    // printf("Elem total: %d\n", elem_total);

    size_t bits_iter = 0;
    size_t bitsArray_iter = 0;
    size_t out_iter = 0;

    // puts("\nbits array inputs: ");
    // for (int i = 0; i < total_bits; i++) 
    //     printf("%d", bits[i]);
    // puts("");

    while (bits_iter <= total_bits) {      
        if (bitsArray_iter < (type_size * 8 )) {
            bitsArray[bitsArray_iter] = bits[bits_iter];

            // printf("\nIn 1st conditional: bits_iter:%d bits[i]:%d bitsarray_i:%d bitsarray[i]:%d",
            // bits_iter, bits[bits_iter], bitsArray_iter, bitsArray[bitsArray_iter]);

            // puts("\ninternal bits array in bits2ints 1st conditional: ");
            // for (int i = 0; i <= bitsArray_iter; i++) 
            //     printf("%d", bitsArray[i]);
            // puts("");

            bitsArray_iter++; bits_iter++;
        }
        else {
            // puts("\ninternal bits array in bits2ints 2st conditional: ");
            // for (int i = 0; i < bitsArray_iter; i++) 
            //     printf("%d", bitsArray[i]);
            // puts("");
    
            out[out_iter++] = bits2intLSF(type_size * 8, bitsArray);
            bitsArray_iter = 0;
        }
    } 
}   

void setMSF() {
    int2bits = int2bitsMSF;
    bits2int = bits2intMSF;
    ints2bits = ints2bitsMSF;
    bits2ints = bits2intsMSF;
}

void setLSF() {
    int2bits = int2bitsLSF;
    bits2int = bits2intLSF;
    ints2bits = ints2bitsLSF;
    bits2ints = bits2intsLSF;
}

void charArrayToString(char ca[], size_t size, char* out) {
    // char buf[size + 1];
    int  count = 0;
    for EACH {
        if ( isprint(ca[i]) ) {
            out[i] = ca[i];
            count++;
        }
    }
    out[count] = '\0';
}    

void bitSlice(int start, int count, void const * const ptr, size_t size, uint8_t out[]) {
    uint8_t* in = (uint8_t*) ptr;
    if (start == -1) 
        for (int i = size - 1, j = count - 1; i > size - count - 1; i--, j--)
            out[j] = in[i];
    else
        for (int i = start, j = 0; j < count; i++, j++)
            out[j] = in[i];
}

void i2p(void const * const ptr, size_t size, char separator, int newline){
    // uint32_t *nums = (uint32_t*) ptr;
    uint8_t *nums = (uint8_t*) ptr;
    for EACH 
        if (i < size - 1)
            printf("%d%c", nums[i], separator);
        else 
            printf("%d", nums[i]);
    for (int i = 0; i < newline; i++)
        printf("\n");
}

// char* i2s(void const * const ptr, size_t size, char separator){
//     char buf[0x40] = "";
//     char r[0x400] = "";
//     uint32_t *nums = (uint32_t*) ptr;
//     for EACH {
//         if (i < size - 1)
//             sprintf(buf, "%d%c", nums[i], separator);
//         else 
//             sprintf(buf, "%d", nums[i]);
//         strcat(r, buf);
//     }
//     return r;
// }
