#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int iarray[3];
float farray[3];
double darray[3];
char carray[3];
printf("%X\n",iarray[0]);
int main (int argc, char** argv){
    printf("Iarray : %X\n", iarray);
    printf("Iarray+1 : %X\n", iarray+1);

    printf("Farray: %X\n", farray);
    printf("Farray+1: %X\n", farray+1);

    printf("Darray:%X\n", darray);
    printf("Darray+1:%X\n", darray+1);

    printf("Carray:%X\n", carray);
    printf("Carray+1:%X\n", carray+1);


}

