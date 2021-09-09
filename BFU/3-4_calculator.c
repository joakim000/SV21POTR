#include <stdio.h>
#include <stdbool.h>

int main(){
    float num1, num2, result;
    char operator;
    _Bool error = false; 
    
    printf("First number: ");
    scanf("%f", &num1);
    printf("Operator: ");
    scanf("%s", &operator);
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

