/*
    Exercise:
        Define an enum for the months of a year, use it in a program presenting the number 
        for the first months in all quarters of a year.
        Rearrange enum values in alphabetic order, rerun the program.
*/

#include <stdio.h>

enum months {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};


int main(){

    enum months month;
   

    month = Jan;
        printf("\nJan - %d", month);
        //printf("\nRad 2 - %d %s", month, month);

    month += 3;
        printf("\nApr - %d", month);

    month += 3;
        printf("\nJul - %d", month);

    month += 3;
        printf("\nOct - %d", month);  //%s hänger. Exception: Segmentation fault

    int i;
    for(i=1; i>13; i=i+3){
        month = i;
        printf("\n%d %s", month, month);
    }


    return 0; 
}

int enumIterXXX(int e){


}