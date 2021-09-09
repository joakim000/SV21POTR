#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int main(){
    int menu;
    float amount;
    float result;
    _Bool error = false;
    _Bool supported = false;

    /*
    printf("Choose currency (1 USD, 2 EUR, 3 GBP: ");
    scanf("%d", &menu);
    printf("Amount: ");
    scanf("%f", &amount);
    */

    while(!supported){
        printf("Choose currency (1 USD, 2 EUR, 3 GBP: ");
        scanf("%d", &menu);
        //if (1 <= menu >= 3) 
        if (menu == 1 || menu == 2 || menu == 3) 

            supported = true;
        /*
            Input bokstav el. tecken ger evig loop av printf men ej scanf?
            Input: 5.4  Som ovan.
            Input: 2.4  Hoppar över amount och ger Result 4.00
        */
    
    }
   
    printf("Amount: "); 
    scanf("%f", &amount);

    switch (menu)
    {
    case 1:
        result = amount * 7;
        break;
    
    case 2:
        result = amount * 10;
        break;

    case 3:
        result = amount * 12;
        break;

    default:
        error = true;
        printf("Unsupported menu selection.");
        break;
    }

    if (!error)
        printf("Result: %4.2f", result);

    return 0;
}

