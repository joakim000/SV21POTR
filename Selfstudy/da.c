#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>


#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))) //Google's clever array size macro


// return # of elements or -1 (error)
// union för att tillåta olika typer?


int daInit(char name[]){}
int daCreate(char name[], double values[]){}
int daAdd(char name[], int index, double value){}  //index -1 == end
int daRemove(char name[], int index){}
int daSnip(char name[], int startIndex, int endIndex){}
int daGet(char name[], int index){}

struct da {
   double  *ptr;
   int  elements;
   int slots;
   
};



int main(){


    return 0;
}

