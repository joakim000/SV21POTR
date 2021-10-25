#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Utility
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro
#define EACH (size_t i = 0; i < size; i++)

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
// 12x^3 + 5x^2 + 9x + 9

typedef union byte_union {
    struct byte_struct
    {
        int b0 : 1;
        int b1 : 1;
        int b2 : 1;
        int b3 : 1;
        int b4 : 1;
        int b5 : 1;
        int b6 : 1;
        int b7 : 1;
    } bit;
    int get[8];
} byte;

 struct byte_struct2
 {
     int b0 : 1;
     int b1 : 1;
     int b2 : 1;
     int b3 : 1;
     int b4 : 1;
     int b5 : 1;
     int b6 : 1;
     int b7 : 1;
     int* next;
 } bit2;

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


void test_byte() {
    byte b = {
        .bit.b0 = 1,
        .bit.b1 = 0,
        .bit.b2 = 1,
        .bit.b3 = 0,
        .bit.b4 = 1,
        .bit.b5 = 0,
        .bit.b6 = 1,
        .bit.b7 = 0,        
    };
    for (int i = 0; i < 8; i++) {
        printf("byte %d val %d\n", i, b.get[i]);
    }
    printf("\nsize of byte_union: %d\n", sizeof(b));
    printf("\nsize of byte_struct: %d\n", sizeof(bit2));


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
                // out[i * 8 + bit_index] = '0';
                out[byte_write_index * 8 + bit_write_index++] = 0;
            }
            else {
                // out[i * 8 + bit_index] = '1';
                out[byte_write_index * 8 + bit_write_index++] = 1;
            }
        }
        byte_write_index++;
    }
}

void int2bitsLSF(size_t const size, void const * const ptr, uint8_t out[]) {
    int  byte_index, 
         bit_index;
 
    uint8_t *byte = (uint8_t*) ptr;
    uint8_t bit;

    for (byte_index = 0; byte_index < size; byte_index++) {        //LSF
        for (bit_index = 0; bit_index < 8; bit_index++) {       // LSF
            bit = (byte[byte_index] >> bit_index) & 1;
            if (!bit)
                // out[i * 8 + bit_index] = '0';
                out[byte_index * 8 + bit_index] = 0;
            else
                // out[i * 8 + bit_index] = '1';
                out[byte_index * 8 + bit_index] = 1;
        }
    }
}

uint32_t bits2intLSF(size_t const len, uint8_t* bits) {
    uint32_t r = 0;
    for (int bit_index = 0; bit_index < len; bit_index++) {       
        if (bits[bit_index]) 
            r |= 1UL << (bit_index);
        else
            r &= ~(1UL << (bit_index) );
    }
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


void ints2bitsLSF(size_t const size, size_t const el_size, void const * const ptr, uint8_t out[]) {
    uint8_t bitsArray[el_size * 8];
    uint8_t* ints = (uint8_t*) ptr;
    for (int i = 0; i < (size / el_size); i++) {
        int2bitsLSF(el_size, &ints[i], bitsArray);
        size_t el_start_index = i * el_size * 8;
        for (int j = 0; j < el_size * 8; j++)
            out[j + el_start_index] = bitsArray[j];
    }
}   

void ints2bitsMSF(size_t const size, size_t const el_size, void const * const ptr, uint8_t out[]) {
    uint8_t bitsArray[el_size * 8];
    uint8_t* ints = (uint8_t*) ptr;
    for (int i = 0; i < (size / el_size); i++) {
        int2bitsMSF(el_size, &ints[i], bitsArray);
        size_t el_start_index = i * el_size * 8;
        for (int j = 0; j < el_size * 8; j++)
            out[j + el_start_index] = bitsArray[j];
    }
}

int main(void)
{
    // test_byte();

    // uint8_t message[] = {0, 1, 2};
    // uint8_t message[] = {0, 1, 2, 3, 4, 5, 6, 7, 157};
    // uint16_t message[] = {0, 1,  32767};
    // uint16_t message[] = {POLYNOMIAL};
    // uint32_t message[] = {0, 1, 2, 3, 4, 5, 6, 7, 157, 65445, 16777216};
    
    uint8_t message[14] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 1}; // 15 zeros have been appended to the message
    
    size_t el_size = sizeof(message[0]); 
    // uint8_t bitsString[el_size * 8 + 1];
    uint8_t bitsArray[el_size * 8];

    size_t msg_size = (sizeof(message));

    uint8_t messageBits[sizeof(message) * 8];
    uint8_t messageBits2[sizeof(message) * 8];

//  /** LSF */
//  for (int i = 0; i < COUNT_OF(message); i++) {
//     int2bitsLSF(el_size, &message[i], bitsArray);
//     size_t el_start_index = i * el_size * 8;
//     for (int j = 0; j < el_size * 8; j++)
//         messageBits[j + el_start_index] = bitsArray[j];
//     printf("\nmsgsize %d size %d len:%d", msg_size, el_size, COUNT_OF(bitsArray));
//     // printf("\t%3d %6x %c %32s", message[i], message[i], message[i],  bitsString);
//     printf("\t%8d %8x %c\t", message[i], message[i], message[i]);
//     for  (int i = 0; i < (el_size * 8); i++) 
//         printf("%d", bitsArray[i]);
//     printf("   reconst: %d", bits2intLSF(el_size * 8, bitsArray));
//  }
//  puts("");

//  for  (int i = 0; i < COUNT_OF(messageBits); i++) 
//         if ((i+1) % (el_size * 8) != 0)
//             printf("%d", messageBits[i]);
//         else   
//             printf("%d ", messageBits[i]);


// ints2bitsLSF(msg_size, el_size, &message, messageBits2);
// puts("");
//  for  (int i = 0; i < COUNT_OF(messageBits2); i++) 
//         if ((i+1) % (el_size * 8) != 0)
//             printf("%d", messageBits2[i]);
//         else   
//             printf("%d ", messageBits2[i]);

// puts("");

//  /** MSF */
//  for (int i = 0; i < COUNT_OF(message); i++) {
//     int2bitsMSF(el_size, &message[i], bitsArray);
//     size_t el_start_index = i * el_size * 8;
//     for (int j = 0; j < el_size * 8; j++)
//         messageBits[j + el_start_index] = bitsArray[j];
//     printf("\nmsgsize %d size %d len:%d", msg_size, el_size, COUNT_OF(bitsArray));
//     // printf("\t%3d %6x %c %32s", message[i], message[i], message[i],  bitsString);
//     printf("\t%8d %8x %c\t", message[i], message[i], message[i]);
//     for  (int i = 0; i < (el_size * 8); i++) 
//         printf("%d", bitsArray[i]);
//     printf("   reconst: %d", bits2intMSF(el_size * 8, bitsArray));
//  }
//  puts("");

//  for  (int i = 0; i < COUNT_OF(messageBits); i++) 
//         if ((i+1) % (el_size * 8) != 0)
//             printf("%d", messageBits[i]);
//         else   
//             printf("%d ", messageBits[i]);


// ints2bitsMSF(msg_size, el_size, &message, messageBits2);
// puts("");
//  for  (int i = 0; i < COUNT_OF(messageBits2); i++) 
//         if ((i+1) % (el_size * 8) != 0)
//             printf("%d", messageBits2[i]);
//         else   
//             printf("%d ", messageBits2[i]);


//  for (int i = 0; i < COUNT_OF(message); i++) {
//         int2bitsLSF(size, &message[i], bitsArray);
//         printf("\nmsgsize %d size %d len:%d", sizeof(message), size, COUNT_OF(bitsArray));
//         // printf("\t%3d %6x %c %32s", message[i], message[i], message[i],  bitsString);
//         printf("\t%8d %8x %c\t", message[i], message[i], message[i]);
//         for  (int i = 0; i < (size * 8); i++) 
//             printf("%d", bitsArray[i]);
//         printf("   reconst: %d", bits2intLSF(size*8, bitsArray));
//     }
//  puts("");



//  for (int i = 0; i < COUNT_OF(message); i++) {
//         int2bitsMSF(size, &message[i], bitsArray);
//         printf("\nmsgsize %d size %d len:%d", sizeof(message), size, COUNT_OF(bitsArray));
//         // printf("\t%3d %6x %c %32s", message[i], message[i], message[i],  bitsString);
//         printf("\t%8d %8x %c\t", message[i], message[i], message[i]);
//         for  (int i = 0; i < (size * 8); i++) 
//             printf("%d", bitsArray[i]);
//         printf("   reconst: %d", bits2intMSF(size*8, bitsArray));
        
//     }




    char msgstr[sizeof(message) + 1];
    // charArrayToString(message, sizeof(message), msgstr);

    // printf("%s\n", msgstr);



    // Calculate the CRC. For example the CRCs of "Hello World!" is 0xB35 and "AB" is 0x54FB
    


    // Checksum the message. I.e replace the zeros with the CRC accroding to the requirements.

    message[1] = 'a';

    // Validate the message.
    // If the remainder is zero print "The data is OK\n";
    // otherwise print "The data is not OK\n"

    return 0;
}
