#include "devheaders.h"
#include "sorting.h"


void main(){

    int num[] = {2, 1, 23, 43, 3, 34 };
    int sel = select(2, num, COUNT_OF(num));

    printf("selected: %d", sel);
}

