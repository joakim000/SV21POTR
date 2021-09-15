#include "devheaders.h"

#include "da.h"


int main() {

    da test = daInit2(100, 1.5);
    double values[] = {1, 2, 3, 4, 5, 6 };
    daCreate(&test, values, COUNT_OF(values));

    printf("\nValues: ");
    for (int i = 0; i < daCount(&test); i++) {
        printf("%4.3f ", daGet(&test, i));        
    }

    da test2 = daInit2(100, 1.5);
    double values2[] = {11, 22, 33, 44, 55, 66 };
    daCreate(&test2, values2, COUNT_OF(values2));

    printf("\nValues: ");
    for (int i = 0; i < daCount(&test2); i++) {
        printf("%4.3f ", daGet(&test2, i));  

    }
    
    printf("\nValues: ");
    for (int i = 0; i < daCount(&test2); i++) {
        printf("%4.3f ", daGet(&test2, i));  
    }
    
    printf("\nValues direct access: ");
    for (int i = 0; i < daCount(&test2); i++) {
        printf("%4.3f ", test2.p[i]);  


    }
    
    int x = daVacs(&test2, 1);
    printf("%d", x);


    return 0;
}

