#include <stdio.h>
#include <stdbool.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

/*  
    Inlämningsuppgift BFU HT21 - calculator  
    Joakim Odermalm
*/


bool charInArray(char a[], char c){
    int i;
    //for (i = 0; i < COUNT_OF(a); i++){
    for (i = 0; i < sizeof(a); i++){    
        if (a[i] == c) 
            return true;
    } 
    return false;
}

int main(){
    float num1, num2, result;
    char operator;
    bool error; 
    char operators[] = {'+', '-', '*', '/'};

    printf("First number: ");
    scanf("%f", &num1);
  
    while (true) {
        printf("Operator: ");
        scanf("%s", &operator);
        if (charInArray(operators, operator))
            break;
        printf("Unknown operator.\n");
    }

    printf("Second number: ");
    scanf("%f", &num2);
    
    switch (operator)
    {
    case '+':
        result = num1 + num2;
        break;
    
    case '-':
        result = num1 - num2;
        break;

    case '*':
        result = num1 * num2;
        break;

    case '/':
        if (num2 == 0){
            error = true;
            printf("Undefined."); 
        }  
        else
            result = num1 / num2; 
        break;

    default:
        error = true;
        printf("Unknown operator.");
        break;
    }

    if (!error)
        printf("Result: %4.2f", result);

    return 0;
}


