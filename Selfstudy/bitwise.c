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
    
    // for (i = size-1; i >= 0; i--) {
        // for (j = 7; j >= 0; j--) {
    for (i = 0; i <= size-1; i++) {
        for (j = 0; j <= 7; j++) {
            bit = (byte[i] >> j) & 1;
            // bit ? out[j] = '1' : out[j] = '0';
            if (!bit)
                out[i * 8 + j] = '0';
            else
                out[i * 8 + j] = '1';
        }
    }
}

void my_strrev(char str[]) {  
    char tmp[strlen(str)];
    strcpy(tmp, str);

    // Reverse
    int j = 0;
    for (int i = strlen(str)-1; i >= 0; i--) {
        str[j] = tmp[i];
        j++;
    }
} 


void main(){
    char bits[sizeof(int) * 8];
    for (int i = 0; i < 34; i++) {
        stringBits(sizeof(i), &i, bits);

        printf("bits len:%d", strlen(bits));
        // my_strrev(bits);

        // strrev(bits);
        printf("\n%3d  %32s", i, bits);
    }

    // for (int i = 0; i < 34; i++) {
    //     printf("\n%3d  ", i);
    //     printBits(sizeof(i), &i);
    // }


    int x = 32;
    int y;
    for (int i = 0; i < 33; i++){
        y = x | i;
        // printf("32 | %d = %d\n", i, y);
    }
    
    // int a[x];

   
}

