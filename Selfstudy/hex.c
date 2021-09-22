#include "devheaders.h"


void main(){

    for (int i = 0; i < 33; i++) {
        // long long num = (long long)-pow(2,i); 
        long long num = (long long)pow(2,i); 

        
        // printf("2^%-2d %#10o %10d %#10x\n", i, num, num, num);

        // _printf_p("2^%1$-2d %2$#24llo %2$24llu %2$#24llx\n", i, num);

        _printf_p("%2$16lld  2^%1$-2d  %2$-#16llx\n", i, num);
        

    }

    // int x = 0x80, y = -0x80;
        // _printf_p("%1$d %1$x %2$d %2$x\n", x, y);
}

