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

int main(int argc, char **argv) {
    char string[2048];
    cmdLine *cmdLines;
    FILE *inputPipe = stdin;
    while (1) {
        printf("%s \n", getcwd(pathBuf, PATH_MAX));
        fgets(string, 2048, inputPipe);
        fflush(inputPipe);
        if (strncmp(string, "quit", 4) == 0) {
            exit(1);
        } else {
            cmdLines = parseCmdLines(string);
            execute(cmdLines);
        }

    }
}

void execute(cmdLine *pCmdLine) {
    int pid;
    int status;
    FILE * input ;
    FILE * output ;
    int res = 0;
    while (pCmdLine != NULL) {
        pid = fork();
        fprintf(stderr, "PID: %d\nExecuting command: %s \n", pid, pCmdLine->arguments[0]);
        if (pid == 0) {
            if (strncmp(pCmdLine->arguments[0], "cd", 2) == 0) {
                _exit(1);

            } else {
                if(pCmdLine->inputRedirect != NULL){
                    close(0);
                    fopen(pCmdLine->inputRedirect,"r");

                }
                if(pCmdLine->outputRedirect!=NULL){
                    close(1);
//                    dup2()
                    output = fopen(pCmdLine->outputRedirect, "w");
                }
                res = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            }
            if (res < 0) {
                perror("Error:  ");
                _exit(errno);
            } else {
                if(pCmdLine->inputRedirect != NULL) {
                    fclose(input);
                }

                if(pCmdLine->outputRedirect!= NULL) {
                    fclose(output);
                }
                _exit(0);
            }
//
//            if(strncmp(pCmdLine->arguments[0], "cd", 2) ==0){
//                res = chdir(pCmdLine->arguments[1]);
//                if(res<0){
//                    perror("Problem with chdir");
//                    _exit(errno);
//                }
//                else{
//                    _exit(1);
//                }
//            }
//
//            if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1){
//                perror("Error printed by perror");
//                _exit(errno);
//            }
//            else{
//                _exit(1);
//            }

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
        pCmdLine = pCmdLine->next;
    }
}
