#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

//Functions
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

//Constants
#define MAX_ATTEMPTS 10

int main(){
    int number, attempt, guess;
    bool correct = false;
    char input[256];

    // Random seed
    time_t t;
    srand((unsigned) time(&t));

    start:
    number = rand() % 100;
    printf("Guess a number (0-99)\n");
    for (attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        while (true) {
            fflush(stdin);
            printf("Attenpt #%d: ", attempt);
            scanf("%s", &input);
            

            
/*
            int i = 0;
            char c;
            bool isNumber = true;


            while (input[i] != '\0') {
               
                //c = input[i];  
                 printf("%d %c | ", i, c);
                 if (!isdigit(c)) {
                    isNumber = false;
                    break;
                } 
                i++;
            }

            for (i = 0; c != '\0'; i++){
                c = input[i];
                printf("%d %c | ", i, c);
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }
*/            
            if (isNumber){
                //char* endptr;
                //guess = strtoimax(&input, &endptr, 10);
                guess = strtoimax(&input, NULL, 10);
                printf("Converted to: %d\n", guess);
            }
            else {
                 printf("Not a number, try again.\n");
                 continue;
            }
           
            if (guess < 0 || guess > 99)
                printf("Out of range, try again.\n");
            else
                break;

            

            
        }
        if (guess < number) 
            printf("Too small, try again.\n");
        else if (guess > number)
            printf("Too big, try again.\n");
        else {
            printf("Correct!");
            correct = true;
            break;    
        }  
        
    }
    if (!correct) {
        printf("No more attempts, correct number was %d.\n", number);
        printf("Try again? (y / N) ");
        char response;
        scanf("%s", &response);
        if (response == 'y')
            goto start;
    }
    
  
    return 0;
} 

