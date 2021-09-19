#include "devheaders.h"

void SOprintBits(size_t const size, void const * const ptr)
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
    // char bits[size * 8];

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
    // char groupedBits[size * 8 + size - 1];
    // groupString(out, 8, groupedBits);



}

#define DATATYPE int

// void var2bitstring(size_t size, char out_bits[], char out_groupedBits[]) {
//     char groupedBits[size * 8 + size - 1];
//     for (int i = 0; i < strlen(groupedBits); i++)
//         groupedBits[i] = 0;
// }

void groupString(char str[], int groupSize, char out[]) {
    char b1[groupSize];
    char b2[groupSize];
    char b3[groupSize];
    char b4[groupSize];
    memcpy(b1, str, groupSize);
    memcpy(b2, str+groupSize, groupSize);
    memcpy(b3, str+groupSize*2, groupSize);
    memcpy(b4, str+groupSize*3, groupSize);
    b1[groupSize] = 0;
    b2[groupSize] = 0;
    b3[groupSize] = 0;
    b4[groupSize] = 0;
    sprintf(out, "%s %s %s %s", b1, b2, b3, b4);

    // char tmp[8];
    // for (int i = 0; i < sizeof(DATATYPE); i++) {
    //     char space = ' ';
    //     memcpy(tmp, bits + (i * 8),  8);
    //     tmp[8] = 0;
    //     strcat(groupedBits, tmp);
    //     strcat(groupedBits, &space);
    // }
    // printf("\n%3d  %s", i, groupedBits);
}

void printBits(size_t datasize, void const * const arr, size_t len) {
    char bits[datasize * 8];
    
    // int foo[] = {1, 2, 4, 8, 16, 32, 64};
    // char foo[] = {'a', 'b', 'c'};

    // for (DATATYPE i = 3000000; i < 3000034; i++) {
    //     stringBits(sizeof(i), &i, bits);

    for (int i = 0; i < len; i++) {
        stringBits(datasize, &arr[i], bits);

        char groupedBits[sizeof(DATATYPE) * 8 + sizeof(DATATYPE) - 1];
        for (int i = 0; i < strlen(groupedBits); i++)
            groupedBits[i] = 0;

        // printf("\n%3d  %s", i, bits);
    
        groupString(bits, 8, groupedBits);
        printf("\n%3d  %s", foo[i], groupedBits);
    }


}



void main(){
    int foo[] = {1, 2, 4, 8, 16, 32, 64};
    char foo[] = {'a', 'b', 'c'};

    char bits[sizeof(foo[0]) * 8];
    for (int i = 0; i < COUNT_OF(foo); i++) {
        stringBits(sizeof(foo[i]), &foo[i], bits);
        char groupedBits[sizeof(DATATYPE) * 8 + sizeof(DATATYPE) - 1];
        groupString(bits, 8, groupedBits);
        
        printf("\n%3d  %s", foo[i], groupedBits);   
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
