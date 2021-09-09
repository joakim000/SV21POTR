#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

//Functions
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro

//Constants
#define MAX_ATTEMPTS 10

int main(){
    int number, attempt, guess;
    bool correct = false;

    // Random seed
    time_t t;
    srand((unsigned) time(&t));

    start:
    number = rand() % 100;
    printf("Guess a number (0-99)\n");
    for (attempt = 1; attempt <= MAX_ATTEMPTS; attempt++){
        while (true) {
            fflush(stdin);
            printf("Attenpt #%d: ", attempt);
            scanf("%d", &guess);
            if (guess >= 0 && guess <= 99)
                break;
            printf("Out of range, try again.\n");
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
    if (!correct){
        printf("No more attempts, correct number was %d.\n", number);
        printf("Try again? (y / N) ");
        char response;
        scanf("%s", &response);
        if (response == 'y')
            goto start;
    }
    
  
    return 0;
} 

