#include "devheaders.h"

void printBits(size_t const size, void const * const ptr)
/* https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
    Take each byte in ptr (outer loop);
    then for each bit of the current byte (inner loop), mask the byte by the current bit (1 << j).
    Shift that right resulting in a byte containing 0 (0000 0000b) or 1 (0000 0001b). 
    Print the resulting byte printf with format %u.
*/
{
    unsigned char *byte = (unsigned char*) ptr;
    unsigned char bit;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            bit = (byte[i] >> j) & 1;
            printf("%u", bit);
        }
    }
    puts("");
}

void stringBits(size_t const size, void const * const ptr, char out[]) {
    unsigned char *byte = (unsigned char*) ptr;
    unsigned char bit;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            bit = (byte[i] >> j) & 1;
            // bit ? out[j] = '1' : out[j] = '0';
            if (!bit)
                out[i * 8 + j] = '0';
            else
                out[i * 8 + j] = '1';
        }
    }
    out[size*8] = 0;
    strrev(out);
}

#define DATATYPE int

void main(){
    char groupedBits[sizeof(DATATYPE) * 8 + sizeof(DATATYPE) - 1];
    for (int i = 0; i < strlen(groupedBits); i++)
        groupedBits[i] = 0;
    
    char bits[sizeof(DATATYPE) * 8];
    
    for (DATATYPE i = 3000000; i < 3000034; i++) {
        stringBits(sizeof(i), &i, bits);

        // printf("\n%3d  %s", i, bits);

        // for (int i = 0; i < sizeof(DATATYPE); i++) {
        //     char tmp[8];
        //     char space = ' ';
        //     memcpy(tmp, bits + (i * 8),  8);
        //     tmp[8] = 0;
        //     strcat(groupedBits, tmp);
        //     strcat(groupedBits, &space);
        // }
        // printf("\n%3d  %s", i, groupedBits);


        char b1[8];
        char b2[8];
        char b3[8];
        char b4[8];
        memcpy(b1, bits, 8);
        memcpy(b2, bits+8, 8);
        memcpy(b3, bits+16, 8);
        memcpy(b4, bits+24, 8);
        b1[8] = 0;
        b2[8] = 0;
        b3[8] = 0;
        b4[8] = 0;
        printf("\n%3d  %s %s %s %s", i, b1, b2, b3, b4);

    }

 

    int x = 32;
    int y;
    for (int i = 0; i < 33; i++){
        y = x | i;
        // printf("32 | %d = %d\n", i, y);
    }
    
    // int a[x];

   
}

        // Försök till gruppering

        // char tmp[size*8];
        // strcpy(tmp, bits);
        // int offset = 0;
        // for (int i = 0; i < size*8; i++) {
        //     if (i > 0 && i % 8 == 0) {
        //         bits[i] = ' ';
        //         offset++;
        //     }
        // }
        // bits[i+offset] = tmp[i];
