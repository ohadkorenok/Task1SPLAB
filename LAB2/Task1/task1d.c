#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int iarray[3];
char carray[3];
int * iArrayPtr;
int *cArrayPtr;
int main (int argc, char** argv){
    int* p;
    iArrayPtr = iarray;
    for (int i = 0; i < 3; ++i) {
        printf("%d",*iArrayPtr);
    }
    printf("%p\n",p);

}
