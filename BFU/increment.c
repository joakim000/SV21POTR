#include <stdio.h>

int main(){

    int i = 0;
    while(i++ < 5){
        printf("\n%d", i);
    }

    printf("\n");

     i = 0;
    while(++i < 5){
        printf("\n%d", i);
    }

    return 0;
}

/* Output:

1
2
3
4
5

1
2
3
4

*/