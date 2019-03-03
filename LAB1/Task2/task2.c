#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int encrypta(int a, int b, int sum); /* b>0 add, b <0 substract b=0 nothing at all */
int toLower(int a);
int main(int argc ,char ** argv){
    int a;
    int b = 0;
    int c = 0; /* index of the encoding flag in argv*/
    int d=0; /* check whether the filename option is used*/
    FILE* fp;
    FILE* outputStream = stdout;
    for (int i = 0; i < argc; ++i)
    {   
        if((argv[i][0] == '-') & (argv[i][1] =='e')){
            b=-1;
            c=i;
        }
        else{
            if((argv[i][0] == '+') & (argv[i][1] == 'e')){
                b = 1;
                c=i;
            }
            else{
                if((argv[i][0] == '-') & (argv[i][1] == 'i')){
                    char *filename = argv[i]+2;
                    fp = fopen(filename, "r");
                    if(!fp){
                        fprintf(stderr, "Error! cannot open file for reading! \n");
                        exit(0);
                    }
                    d=1;
                }

            else{
                if ((argv[i][0] == '-') & (argv[i][1] =='o')){
                    char *output = argv[i]+2;
                    outputStream=fopen(output, "w");
                    if(!fp){
                     fprintf(stderr, "Error! cannot open file for reading! \n");
                        exit(0);   
                    }
}
            }
            }
    }
}
    int i=2;
    while ((d==0) && ((a = fgetc(stdin)) != EOF) & (b!=0)){
    fprintf(outputStream, "%c", encrypta(a,b,argv[c][i]));
    if((i==strlen(argv[c])-1) | (a=='\n')){
        i=1;
    }
    i++;
}
    while ((d==1) &&((a = fgetc(fp)) != EOF) & (b!=0)){
        fprintf(outputStream, "%c", encrypta(a,b,argv[c][i]));
        if((i==strlen(argv[c])-1) | (a=='\n')){
            i=1;
        }
        i++;
    }

   while ((d==0) && ((a = fgetc(stdin)) != EOF )& (b==0)){
    fprintf(outputStream, "%c", (toLower(a)));
}


    while ((d==1) && ((a = fgetc(fp)) != EOF )& (b==0)){
    fprintf(outputStream, "%c", (toLower(a)));
}
    fprintf(outputStream, "%s", "^D");
}

int encrypta(int a, int b, int sum) {
    if(b > 0){
        return (a + sum) % 128;
    }
    else{
        return (a - sum) % 128;
    }
}

int toLower(int a) {
    if ((a <= 90) & (a >=65)){
        return a+32;
    }
    else{
        return a;
    }
}