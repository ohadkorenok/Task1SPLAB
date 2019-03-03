#include <stdio.h>
#include <string.h>

int encrypta(int a, int b, int sum); /* b>0 add, b <0 substract b=0 nothing at all */
int main(int argc ,char ** argv){
    int a;
    int b = 0;
    if((argv[1][0] == '-') & (argv[1][1] =='e')){
        b=-1;
    }
    else{
        if((argv[1][0] == '+') & (argv[1][1] == 'e')){
            b = 1;
        }
    }
        // fprintf(stderr, "length of argv[1] is: : %d   and  ", strlen(argv[1]));

    int i=2;
    while ((a = fgetc(stdin)) != EOF){
        fprintf(stderr, "sum is: %d   and  ", argv[1][i]);
        fprintf(stderr, "i is : %d\n", i);
        fprintf(stdout, "%c", encrypta(a,b,argv[1][i]));
        if((i==strlen(argv[1])-1) | (a=='\n')){
            i=1;
        }
        i++;
    }
    fprintf(stdout, "%s", "^D");
}

int encrypta(int a, int b, int sum) {
    if(b > 0){
        return (a + sum) % 128;
    }
    else{
        return (a - sum) % 128;
    }
}