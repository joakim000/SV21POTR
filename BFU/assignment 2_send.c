#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//Constants
#define MAX_ATTEMPTS 10

int main(){
    int number, attempt, guess;
    bool correct = false;
    int scan;

    // Random seed
    time_t t;
    srand((unsigned) time(&t));

    start:
    number = rand() % 100;
    printf("Guess a number (0-99)\n");
    for (attempt = 1; attempt <= MAX_ATTEMPTS; attempt++){
        guess = -1;
        printf("Guess: %d\n", guess);
        while (true) {
            fflush(stdin);
            printf("Attenpt #%d: ", attempt);
            scan = scanf("%d", &guess);
            printf("Scan: %d\n", scan);
            printf("Guess: %d\n", guess);
            if (guess >= 0 && guess <= 99)
                break;
            printf("Out of range, try again.\n");
        }
        printf("Guess: %d\n", guess);
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

