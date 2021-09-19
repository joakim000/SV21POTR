#include "devheaders.h"


/* https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
    Take each byte in ptr (outer loop);
    then for each bit of the current byte (inner loop), mask the byte by the current bit (1 << j).
    Shift that right resulting in a byte containing 0 (0000 0000b) or 1 (0000 0001b). 
    Print the resulting byte printf with format %u.
*/
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

void main(){
    int x = 16;
    int foo[8];
    for (int i = 0; i < COUNT_OF(foo); i++) {
        // foo[i] = 16 >> (int)pow(2, i);
        foo[i] = 16 >> i;

    }




    // int powers[] = {x, 2, 4, 8, 16, 32, 64};
    // char foo[] = {'a', 'b', 'c'};

    // int x = 9 & 1;


    // int foo[] = {x};

    char bits[sizeof(foo[0]) * 8];
    for (int i = 0; i < COUNT_OF(foo); i++) {
        stringBits(sizeof(foo[i]), &foo[i], bits);
        char groupedBits[sizeof(foo[0]) * 8 + sizeof(foo[0]) - 1];
        groupString(bits, 8, groupedBits);
        
        // printf("\n%3d  %3d  %s", (int)pow(2,i), foo[i], groupedBits);   
        // printf("\n%3d  %3d  %s", i, foo[i], groupedBits);   
    
    }


 


    printf("\n");



    // // long long y = 10000000000;
    // long long y = 9223372036854775807;

    // //            1410065408
    // long long y_recon = 0;
    // for (int i = 63; i >= 0; i--){
    //     long long foo = y & (long long)pow(2,i);
    //     // printf("a & %d = %d\n", (int)pow(2,i), y);
    //     if (foo) {
    //         printf("%d", 1);
    //         y_recon += (long long)pow(2,i);
    //     }
    //     else {
    //         printf("%lld", 0);
    //     }
    // }
    // printf("\n%d recon: %d", y, y_recon);

    unsigned char c = 'a';
    // int c = 15;
    bool bitarr[8];
    int y_recons = 0;
    for (int i = 7; i >= 0; i--){
        int foo = c & (int)pow(2,i);
        // printf("a & %d = %d\n", (int)pow(2,i), y);
        if (foo) {
            printf("%d", 1);
            y_recons += (int)pow(2,i);
            bitarr[i] = true;
        }
        else {
            printf("%d", 0);
            bitarr[i] = false;
        }
    }
    printf("\n%d %c  reconstruct: %d %c\n", c, c, y_recons, y_recons);

    // Reverse
    int y_rev = 0;
    for (int i = 0; i < 8; i++) {
        if (bitarr[i]) {
            printf("%d", 1);
        }
        else {
            printf("%d", 0);
        }
    }
    int j = 0;
    for (int i = 7; i >= 0; i--) {
        if (bitarr[i]) {
            y_rev += (int)pow(2,j);
        }
        j++;
    }
    printf("\n%d %c  reverse: %d  %c", c, c, y_rev, y_rev);
}






   /*
    char bits[sizeof(x) * 8];
    stringBits(sizeof(x), &x, bits);
    char groupedBits[sizeof(x) * 8 + sizeof(x) - 1];
    groupString(bits, 8, groupedBits);
    printf("\n%3d  %s", x, groupedBits);   
    */