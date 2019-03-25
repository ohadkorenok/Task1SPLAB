#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void PrintHex(char *buffer, int length);

struct fun_desc {
    char *name;

    void (*fun)();
};

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};

void PrintHex(char *buffer, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%02X ", (unsigned char) buffer[i]);

    }
}

void list_print(link *virus_list);

/* Print the data of every link in list. Each item followed by a newline character. */

link *list_append(link *virus_list, virus *data);

/* Add a new link with the given data to the list
   (either at the end or the beginning, depending on what your TA tells you),
   and return a pointer to the list (i.e., the first link in the list).
   If the list is null - create a new entry and return a pointer to the entry. */

void list_free(link *virus_list); /* Free the memory allocated by the list. */



void printVirus(virus *virus1);


short bytesToShort(char *bytesArr);

void printVirus(virus *virus1) {
    printf("Virus name: %s \n", virus1->virusName);
    printf("Virus size: %d \n", virus1->SigSize - 18);
    printf("Virus signature:\n");
    PrintHex(virus1->sig, virus1->SigSize - 18);
    printf("\n");

}

short bytesToShort(char *bytesArr) {
    short result = (unsigned short) ((bytesArr[1] & 0xff) << 8);
    result += (unsigned short) (bytesArr[0] & 0xff);
    return result;
}


void list_print(link *virus_list) {
    printVirus(virus_list->vir);
    if (virus_list->nextVirus != NULL) {
        list_print(virus_list->nextVirus);
    }
}

link *list_append(link *virus_list, virus *data) {
    link *toAppend = malloc(sizeof(link));
    toAppend->nextVirus = virus_list;
    toAppend->vir = data;
    virus_list = toAppend;
    return toAppend;
}

void list_free(link *virus_list) {
    if (virus_list != NULL) {
        free(virus_list->vir);
        list_free(virus_list->nextVirus);
    }
}

void quit() {
    exit(3);
}

link *virus_ll = NULL;

void load_signatures() {
    char buffer[200];
    fgets(buffer, sizeof(buffer), stdin);
    size_t length;
    length = strlen(buffer);
    if (buffer[length - 1] == '\n')
        buffer[length - 1] = '\0';

    FILE *fp = fopen(buffer, "r");
    if (!fp) {
        fprintf(stderr, "Error! cannot open file for reading! \n");
        exit(0);
    }
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    long bytesHaveRead = 0;
    int size;
    while (bytesHaveRead < fsize) {
        char *length = malloc(2);
        fread(length, 2, 1, fp);
        size = bytesToShort(length);
        virus *virusObject = malloc(size * sizeof(char));
        virusObject->SigSize = size -18;
        fread(virusObject->virusName, size - 2, 1, fp);
        bytesHaveRead += size;
        virus_ll = list_append(virus_ll, virusObject);
    }
    fclose(fp);

}

void print_signatures() {
    list_print(virus_ll);
}


int main(int argc, char **argv) {
    struct fun_desc menu[] = {
            {"Load Signatures",  load_signatures},
            {"Print Signatures", print_signatures},
            {"Quit",             quit},
            {NULL, NULL}};

    int i = 0;
    /**
     * replace with sizeof/sizeof*
     */
    int bounds = sizeof(menu) / sizeof(*menu) - 2;

    fprintf(stdout, "Please Choose a function: ");
    while (menu[i].name != NULL) {
        fprintf(stdout, "%d) %s\n", i+1, menu[i].name);
        i++;
    }
    fprintf(stdout, "OPTION: ");
    i = 0;
    char a[3];
    int b;
    fgets(a, 3, stdin);
    while (a != NULL) {
        b = (a[0] - 48);
        if (b > 0 && b <= bounds+1) {
            fprintf(stdout, "Within bounds\n");
            menu[b-1].fun();
            fprintf(stdout, "Done \n\n");
        } else {
            fprintf(stdout, "Not within bounds\n");
        }
        fprintf(stdout, "Please Choose a function: \n");
        while (menu[i].name != NULL) {
            fprintf(stdout, "%d) %s\n", i+1, menu[i].name);
            i++;
        }
        fprintf(stdout, "OPTION: \n");
        fgets(a, 3, stdin);
        i = 0;
    }

}




