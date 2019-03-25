#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PrintHex(char *buffer, int length);

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

void printVirus(virus *virus1);

short bytesToShort(char *bytesArr);


int main(int argc, char **argv) {
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */


    long bytesHaveRead = 0;
    while (bytesHaveRead < fsize) {
        char *length = malloc(2);
        fread(length, 2, 1, fp);
        virus *virusObject = malloc(sizeof(virus));
        virusObject->SigSize = bytesToShort(length);
        fread(virusObject->virusName, 16, 1, fp);
        fread(virusObject->sig, virusObject->SigSize - 18, 1, fp);
        bytesHaveRead += virusObject->SigSize;
        printVirus(virusObject);
    }
    fclose(fp);


//    string[fsize] = 0;
    if (!fp) {
        fprintf(stderr, "Error! cannot open file for reading! \n");
        exit(0);
    }
//    PrintHex(string, fsize);
//    PrintHex(length, 2);


}

void printVirus(virus *virus1){
    printf("Virus name: %s \n", virus1->virusName);
    printf("Virus size: %d \n", virus1->SigSize -18);
    printf("Virus signature:\n");
    PrintHex(virus1->sig, virus1->SigSize-18);
    printf("\n");

}

short bytesToShort(char *bytesArr) {
    short result = (unsigned short) ((bytesArr[1] & 0xff) << 8);
    result += (unsigned short) (bytesArr[0] & 0xff);
    return result;
}

void PrintHex(char *buffer, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%02X ", (unsigned char) buffer[i]);

    }
}




