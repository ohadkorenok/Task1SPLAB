#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <linux/limits.h>
#include <errno.h>
#include <wait.h>
#include "LineParser.h"


void execute(cmdLine *pCmdLine);

char pathBuf[PATH_MAX];

typedef struct pair {
    char *value;                         /* the parsed command line*/
    char *varDecl;                          /* the process id that is running the command*/
    struct pair *next;                      /* next process in chain */
} pair;

char *getPairValue(char *s1);

void delete(char *varDecl1);

pair *getPair(char *s1);

void addPair(char *varDecl, char *value);

void printLinkedList();

pair *pair_ll = NULL;

int main(int argc, char **argv) {
    char string[2048];
    cmdLine *cmdLines;
    FILE *inputPipe = stdin;
    int i = 0;
    char*args[2];
    char c[2048];
    char *s;
    while (1) {
        printf("%s \n", getcwd(pathBuf, PATH_MAX));
//        fgets(string, 2048, inputPipe);
//        fflush(inputPipe);

        fgets(string, 2048, inputPipe);
        memset(c,'\0',2048);
        strcpy(c, string);
        fflush(inputPipe);
        s=c;

        if (strncmp(string, "set", 3) == 0) {
            char *varDecl = "";
            char *value = "";
            s = s + 4;
            varDecl = strtok(s, " ");
            value = strtok(NULL, " ");
            s = value;
            while (s[i] != '\n' && s[i] != EOF) {
                i++;
            }
            value[i] = '\0';
            i = 0;

            addPair(varDecl, value);
        } else {
            if (strncmp(string, "vars", 4) == 0) {
                printLinkedList();
            } else {
                if (strncmp(string, "delete", 6) == 0) {
                    s = s + 7;
                    char *varRef = strtok(s, " ");
                    s = varRef;
                    i = 0;
                    while (s[i] != '\n' && s[i] != EOF) {
                        i++;
                    }
                    varRef[i] = '\0';
                    i = 0;
                    delete(varRef);
                } else {
                    if (strncmp(string, "quit", 4) == 0) {
                        exit(1);
                    } else {
                        cmdLines = parseCmdLines(string);
                        execute(cmdLines);
                    }
                }
            }
        }

    }
}

void execute(cmdLine *pCmdLine) {
    int pid;
    int pipefd[2];
    int status;
    FILE *input = STDIN_FILENO;
    FILE *output = STDOUT_FILENO;
    int res = 0;
    int cpid2;
    int pipeSupport = 0;
    while (pCmdLine != NULL) {


        if (pCmdLine->next != NULL) {
            pipeSupport = 1;
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        pid = fork();
        fprintf(stderr, "PID: %d\nExecuting command: %s \n", pid, pCmdLine->arguments[0]);
        if (pid == 0) {
            if (strncmp(pCmdLine->arguments[0], "cd", 2) == 0) {
                _exit(1);
            } else {
                if (pCmdLine->inputRedirect != NULL) {
                    close(0);
                    input = fopen(pCmdLine->inputRedirect, "r");
                }
                if (pCmdLine->outputRedirect != NULL) {
                    close(1);
//                    dup2()
                    output = fopen(pCmdLine->outputRedirect, "w");
                }
                if (pipeSupport == 1) {
                    close(output);
                    dup(pipefd[1]);
                    close(pipefd[1]);
                    res = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
                } else {
                    res = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
                }
            }
            if (res < 0) {
                perror("Error:  ");
                _exit(errno);
            } else {
                if (pCmdLine->inputRedirect != NULL) {
                    fclose(input);
                }
                if (pCmdLine->outputRedirect != NULL) {
                    fclose(output);
                }
                _exit(0);
            }

        } else {
            if (pipeSupport == 1) {
                close(pipefd[1]);
                cpid2 = fork();
                if (cpid2 == 0) {
                    close(input);
                    dup(pipefd[0]);
                    close(pipefd[0]);
                    pCmdLine = pCmdLine->next;
                    execvp(pCmdLine->arguments[0], pCmdLine->arguments);
                    _exit(0);
                } else {
                    close(pipefd[0]);
                    close(pipefd[1]);
                    int status2;
                    waitpid(pid, &status, 0);
                    waitpid(cpid2, &status2, 0);
                }
            } else {
                if (strncmp(pCmdLine->arguments[0], "cd", 2) == 0) {
                    res = chdir(pCmdLine->arguments[1]);
                    fprintf(stderr, "the cd response is:  %d", res);
                    printf("%s \n", getcwd(pathBuf, PATH_MAX));
                }
                if (pCmdLine->blocking == 1) {
                    fprintf(stderr, "Master is waiting... \n");
                    waitpid(pid, &status, 0);
                }
            }
        }
        pCmdLine = pCmdLine->next;
    }
}


void addPair(char *varDecl, char *value) {
    pair *toAppend = malloc(sizeof(pair));
    toAppend->next = pair_ll;
    toAppend->value = value;
    toAppend->varDecl = varDecl;
    pair_ll = toAppend;
}

void printLinkedList() {
    pair *p1 = pair_ll;
    fprintf(stdout, "Var        Value\n");
    while (p1 != NULL) {
        fprintf(stdout, "%s        %s", p1->varDecl, p1->value);
        p1 = p1->next;
    }
}

void replaceCommandLineArguments(cmdLine *cmdLine1, int num1) {
    size_t sizeOfArray = sizeof(cmdLine1->arguments) / sizeof(cmdLine1->arguments[0]);
    char *s;
    char *tempVal;
    for (int i = 0; i < sizeOfArray; ++i) {
        if (cmdLine1->arguments[i][0] == '$') {
            s = cmdLine1->arguments[i];
            s = s + 1;
            tempVal = getPairValue(s);
            if (strcmp(tempVal, "ERROR") != 0) {
                replaceCmdArg(cmdLine1, i, tempVal);
            } else {
                fprintf(stderr, "ERROR! variable %s not found! ", s);
            }
        }
    }
}

char *getPairValue(char *s1) {
    pair *p1 = pair_ll;
    while (p1 != NULL) {
        if (strcmp(p1->varDecl, s1) == 0) {
            return p1->value;
        }
    }
    return "ERROR";
}

pair *getPair(char *s1) {
    pair *p1 = pair_ll;
    while (p1 != NULL) {
        if (strcmp(p1->varDecl, s1) == 0) {
            return p1;
        }
    }
}

void delete(char *varDecl1) {
    pair *p1 = pair_ll;
    if (strcmp(p1->varDecl, varDecl1)) {
        pair_ll = p1->next;
        p1->next = NULL;
        free(p1);
        return;
    }
    pair *prev = pair_ll;
    p1 = p1->next;
    while (p1 != NULL) {
        if (strcmp(p1->varDecl, varDecl1)) {
            prev->next = p1->next;
            p1->next = NULL;
            free(p1);
            return;
        }
    }
    fprintf(stderr, "Error! variable not found! %s", varDecl1);
}