#include "devheaders.h"

/* Tuesday exercises
  
/*
## Reversing
3. a. Reverse a string with a maximum of 213 letters.
   b. Reverse the bit order of unsinged char. ie 1000 0000 -> 0000 0001
*/

void my_strrev(char str[]) {  
    char tmp[strlen(str)];
    strcpy(tmp, str);

    // Reverse
    int j = 0;
    for (int i = strlen(str)-1; i >= 0; i--) {
        str[j] = tmp[i];
        j++;
    }
} 

void reverseString() {
    // 3a. Reverse a string with a maximum of 213 letters.
    char str213[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThe";
    char str214[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogTheQ";
    if (strlen(str213) <= 213)
        strrev(str213);
    if (strlen(str214) <= 213)
        strrev(str214);
    printf("\nLib reverse:\n213: %s\n214: %s", str213, str214);
    

    char str213_2[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThe";
    char str214_2[] = "ThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogThequickbrownfoxjumpsoverthelazydogTheQ";
    if (strlen(str213_2) <= 213)
        strrev(str213_2);
    if (strlen(str214_2) <= 213)
        strrev(str214_2);
    printf("\nMy reverse:\n213: %s\n214: %s\n\n", str213_2, str214_2);
  
}

void reverseChar(unsigned char c) {
    // 3b. Reverse the bit order of unsinged char. ie 1000 0000 -> 0000 0001 
    bool bits[8];
    int recons = 0;
    for (int i = 7; i >= 0; i--) {
        int res = (int)c & (int)pow(2,i);
        if (res) {
            printf("%d", 1);
            recons += (int)pow(2,i);
            bits[i] = true;
        }
        else {
            printf("%d", 0);
            bits[i] = false;
        }
    }
    printf("\n%3d %3c   reconstruct: %3d %3c\n", c, c, recons, recons);

    // Reverse
    int rev = 0;
    for (int i = 0; i < 8; i++) {
        if (bits[i]) 
            printf("%d", 1);
        else 
            printf("%d", 0);
    }

    for (int i = 7, j = 0; i >= 0; i--, j++) {
        if (bits[i]) {
            rev += (int)pow(2,j);
        }
    }
    printf("\n%3d %3c   reverse: %3d %3c", c, c, rev, rev);
}



void main() {
    /* 3a */
    reverseString();
    
    /* 3b */
    reverseChar(10);

}



