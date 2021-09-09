#include <stdio.h>
#include <stdbool.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

bool charInArray(char a[], char c);
float getNumber();

int main(){
    float num1, num2, result;
    char operator;
    bool error; 

    char operators[] = {'+', '-', '*', '/'};

    printf("First number: ");
    num1 = getNumber();

    printf("Operator: ");
    while (true) {
        fflush(stdin);
        scanf("%s", &operator);
        if (charInArray(operators, operator))
            break;
        printf("Unknown operator, try again: ");
    }

    printf("Second number: ");
    num2 = getNumber();

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
        printf("Result: %4.3f", result);

    return 0;
}


bool charInArray(char a[], char c){
    int i;
    for (i = 0; i < COUNT_OF(a); i++){
        if (a[i] == c) 
            return true;
    } 
    return false;
}

float getNumber() {
    float r;
    while (true) {
        fflush(stdin);
        if (scanf("%f", &r)) 
            return r;
        else
            printf("Not a number, try again: ");        
    }
}

