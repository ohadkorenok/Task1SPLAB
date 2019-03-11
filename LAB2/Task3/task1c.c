#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char censor(char c) {
    if (c == '!')
        return '.';
    else
        return c;
}

char encrypt(char c) {
    if (c >= 0x20 && c <= 0x7E)
        return (char) (c + 3);
    else
        return c;
} /* Gets a char c and returns its encrypted form by adding 3 to its value.
          If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c) {
    if (c >= 0x20 && c <= 0x7E)
        return (char) (c - 3);
    else
        return c;
} /* Gets a char c and returns its decrypted form by reducing 3 to its value.
            If c is not between 0x20 and 0x7E it is returned unchanged */
char xprt(char c) {
    fprintf(stdout, "0x%X\n", c);
    return c;
} /* xprt prints the value of c in a hexadecimal representation followed by a
           new line, and returns c unchanged. */
char cprt(char c) {
    if (c >= 0x20 && c <= 0x7E)
        fprintf(stdout, "%c\n", c);
    else
        fprintf(stdout, ".\n");
    return c;
} /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns
                    the value of c unchanged. */
char my_get(char c) {
    return (char) fgetc(stdin);
} /* Ignores c, reads and returns a character from stdin using fgetc. */

char quit(char c) {
    exit(3);
} /* Gets a char c, and ends the program returning 0 */
char *map(char *array, int array_length, char (*f)(char)) {
    char *mapped_array = (char *) (malloc(array_length * sizeof(char)));
    for (int i = 0; i < array_length; ++i) {
        mapped_array[i] = (*f)(array[i]);
    }
    return mapped_array;
}

struct fun_desc {
    char *name;

    char (*fun)(char);
};

int main(int argc, char **argv) {
    char carray[5];
    for (int j = 0; j < 5; ++j) {
        carray[j] = '\0';
    }
    char *cptr = carray;
    struct fun_desc menu[] = {{"Censor",       censor},
                              {"Encrypt",      encrypt},
                              {"Decrypt",      decrypt},
                              {"Print hex",    xprt},
                              {"Print string", cprt},
                              {"Get string",   my_get},
                              {"Quit",         quit},
                              {NULL, NULL}};
    int quitFlag = 0;
    int i = 0;


    /**
     * replace with sizeof/sizeof*
     */
    int bounds = sizeof(menu) / sizeof(*menu) - 2;

    fprintf(stdout, "Please Choose a function: ");
    while (menu[i].name != NULL) {
        fprintf(stdout, "%d) %s\n", i, menu[i].name);
        i++;
    }
    fprintf(stdout, "OPTION: ");
    i = 0;
//    char *a;
    char a[3];
    int b;
    fgets(a, 3, stdin);
    while (a != NULL) {
        b = (a[0] - 48);
        if (b >= 0 && b <= bounds) {
            fprintf(stdout, "Within bounds\n");
            if(cptr[0] != '\0'){
                char* arrayPtrToDelete = cptr;
                cptr = map(arrayPtrToDelete, 5, menu[b].fun);
                free(arrayPtrToDelete);
            }else{
                cptr = map(cptr, 5, menu[b].fun);
            }
            fprintf(stdout, "Done \n\n");
        } else {
            fprintf(stdout, "Not within bounds\n");
        }
        fprintf(stdout, "Please Choose a function: \n");
        while (menu[i].name != NULL) {
            fprintf(stdout, "%d) %s\n", i, menu[i].name);
            i++;
        }
        fprintf(stdout, "OPTION: \n");
        fgets(a, 3, stdin);
        i = 0;
    }

}