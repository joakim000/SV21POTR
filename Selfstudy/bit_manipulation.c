#include <stdio.h>

void setBit(int* num, int n) {
    *num = (*num | (1 << n)); 
}

void clearBit(int* num, int n) {
    *num = (*num & (~(1 << n)));
}

void toggleBit(int* num, int n) {
    *num = (*num ^ (1 << n));
}

int getBit(int* num, int n)  {
    return (*num >> n) & 1;
}

void changeBit(int* num, int n, int val) {
    *num = (*num & (~(1 << n))) | (val << n);
}

void main(){
    int num = 0;
    setBit(&num, 6);
    printf("\n0, set bit 6: %d", num);

    num = 7;
    clearBit(&num, 1);
    printf("\n7, clear bit 1: %d", num);

    num = 7;
    toggleBit(&num, 1);
    printf("\n7, toggle bit 1: %d", num);
    toggleBit(&num, 1);
    printf("\n7 then 5, toggle bit 1 again: %d", num);

    num = 127;
    printf("\n127, get bits: ");
    for (int i = 7; i >= 0; i--) 
        printf("%d", getBit(&num, i));

    num = 8;
    changeBit(&num, 6, 1);
    printf("\n8, change bit 6 to 1: %d", num);
    changeBit(&num, 3, 0);
    printf("\n72, change bit 3 to 0: %d", num);

}

