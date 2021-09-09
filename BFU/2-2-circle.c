/* Exercise: Define formula for area and circumference of a circle as macros. Produce a 
    program that prints out calculated values for r=3, r=5, r=10. */

#include <stdio.h>
#include <math.h>


#define CIRCLEAREA(r) M_PI*r*r
#define CIRCLECIRCUMFERENCE(r) 2*M_PI*r

int main(){
    
    /*
    int i = 3;
    while(i <= 10){
        if (i==3 || i==5 || i==10){    
            printf("\nr=%d A=%f C=%f", i, CIRCLEAREA(i), CIRCLECIRCUMFERENCE(i) );
        }
        i++;
    }
    */

    int i;
    for(i=3; i<=10; i++){
        if (i==3 || i==5 || i==10){    
            printf("\nr=%d A=%f C=%f", i, CIRCLEAREA(i), CIRCLECIRCUMFERENCE(i) );
        }
    }


    return 0;
}

