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

void replaceCommandLineArguments(cmdLine *cmdLine1);

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
    while (1) {
        printf("%s \n", getcwd(pathBuf, PATH_MAX));
        fgets(string, 2048, inputPipe);
        fflush(inputPipe);
        if (strncmp(string, "vars", 4) == 0) {
            printLinkedList();
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


void execute(cmdLine *pCmdLine) {
    int pid;
    int pipefd[2];
    int status;
    FILE *input = STDIN_FILENO;
    FILE *output = STDOUT_FILENO;
    int res = 0;
    int cpid2;
    int pipeSupport = 0;
    replaceCommandLineArguments(pCmdLine);

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
            if (((strncmp(pCmdLine->arguments[0], "cd", 2) == 0) || (strncmp(pCmdLine->arguments[0], "set", 3) == 0) ||
                 (strncmp(pCmdLine->arguments[0], "delete", 6) == 0))) {
                _exit(1);
            } else {
                if (pCmdLine->inputRedirect != NULL) {
                    close(0);
                    input = fopen(pCmdLine->inputRedirect, "r");
                }
                if (pCmdLine->outputRedirect != NULL) {
                    close(1);
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
                    if (strncmp(pCmdLine->arguments[1], "~", 1) == 0) {
                        replaceCmdArg(pCmdLine, 1, getenv("HOME"));
                    }
                    res = chdir(pCmdLine->arguments[1]);
                    fprintf(stderr, "the cd response is:  %d", res);
                    printf("%s \n", getcwd(pathBuf, PATH_MAX));
                }
                if (strncmp(pCmdLine->arguments[0], "set", 3) == 0) {
                    char *varDecl = strdup(pCmdLine->arguments[1]);
                    char *value = strdup(pCmdLine->arguments[2]);
                    addPair(varDecl, value);
                }
                if (strncmp(pCmdLine->arguments[0], "delete", 6) == 0) {
                    delete(pCmdLine->arguments[1]);
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
    int toReplace = 0;
    toReplace = strcmp(getPairValue(varDecl),"ERROR")==0 ? 0 : 1;
    if(toReplace==1){
        delete(varDecl);
    }
    pair *toAppend = malloc(sizeof(pair));
    toAppend->next = pair_ll;
    toAppend->value = value;
    toAppend->varDecl = varDecl;
    pair_ll = toAppend;
}

void printLinkedList() {
    pair *p1 = pair_ll;
    fprintf(stdout, "Var        Value \n");
    while (p1 != NULL) {
        fprintf(stdout, "%s        %s\n", p1->varDecl, p1->value);
        p1 = p1->next;
    }
}

void replaceCommandLineArguments(cmdLine *cmdLine1) {
    size_t sizeOfArray = sizeof(cmdLine1->arguments) / sizeof(cmdLine1->arguments[0]);
    char *s;
    char *tempVal;
    int i = 0;
    while (cmdLine1->arguments[i] != NULL) {
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
        i++;
    }
}

char *getPairValue(char *s1) {
    pair *p1 = pair_ll;
    while (p1 != NULL) {
        if (strcmp(p1->varDecl, s1) == 0) {
            return p1->value;
        }
        p1=p1->next;
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
    if (strcmp(p1->varDecl, varDecl1) == 0) {
        pair_ll = p1->next;
        p1->next = NULL;
        free(p1->varDecl);
        free(p1->value);
        free(p1);
        return;
    }
    pair *prev = pair_ll;
    p1 = p1->next;
    while (p1 != NULL) {
        if (strcmp(p1->varDecl, varDecl1) == 0) {
            prev->next = p1->next;
            p1->next = NULL;
            free(p1->varDecl);
            free(p1->value);
            free(p1);
            return;
        }
        prev=prev->next;
        p1=p1->next;
    }
    fprintf(stderr, "Error! variable not found! %s", varDecl1);
}