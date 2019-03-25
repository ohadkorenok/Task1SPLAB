#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PrintHex(char* buffer, int length);

int main(int argc, char **argv) {
    char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, fp);
    fclose(fp);

    string[fsize] = 0;
    if (!fp) {
        fprintf(stderr, "Error! cannot open file for reading! \n");
        exit(0);
    }
    PrintHex(string, fsize);

}

void PrintHex(char*  buffer, int length){
    for (int i = 0; i < length; ++i) {
        printf("%02X ",(unsigned char) buffer[i]);

    }
}


