/* 
    Exercise:
        Write a program that converts an American length into European.
        Input should use format foot, inch and output should be in m, cm.
        1 ft = 12 inch   1 inch = 2.54 cm
*/


#include <stdio.h>
#include <math.h>

int main(){
    float ft, in; //Input storage
    float m=0, cm=0; //Output

    printf("\nInput imperial length (ft,in): ");
    scanf("%f,%f", &ft, &in);
 
    float ftFromIn = in / 12;
    if (ftFromIn >= 1){
        int ftToAdd = (int) floor(ftFromIn);
        ft += ftToAdd;
        in -= ftToAdd * 12;
    } 

    cm = (ft * 12 + in) * 2.54;
    float mFromCm = cm / 100;
    int wholeMeters = (int) floor(mFromCm);
    m += wholeMeters;
    cm -= wholeMeters * 100;

    printf("%4.2f meters, %4.2f centimeters.", m, cm);

    return 0;
}

