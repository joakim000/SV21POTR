#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
            if (!scanf("%d", &guess)) {
                printf("Not a number, try again.\n");
                continue;
            }
            if (guess < 0 || guess > 99) {
                printf("%d is out of range, try again.\n", guess);
                continue;
            }    
            break;   
        }
        if (guess < number) 
            printf("%d is too small, try again.\n", guess);
        else if (guess > number)
            printf("%d is too big, try again.\n", guess);
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

