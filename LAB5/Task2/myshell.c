#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <linux/limits.h>
#include <errno.h>
#include <wait.h>
#include "LineParser.h"

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0


typedef struct process {
    cmdLine *cmd;                         /* the parsed command line*/
    pid_t pid;                          /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;                      /* next process in chain */
} process;

//process* node = NULL;
//process** process_ll = &node;
process *process_ll = NULL;

void execute(cmdLine *pCmdLine);

void addProcess(process **process_list, cmdLine *cmd,
                int pid); /*Receive a process list (process_list), a command (cmd), and the process id (pid) of the process running the command.*/
void freeProcessList(process *process_list);

void updateProcessList(
        process **process_list); /*go over the process list, and for each process check if it is done, you can use
 * waitpid with the option WNOHANG. WNOHANG does not block the calling process, the process returns from the call to
 * waitpid immediately. If no process with the given process id exists, then waitpid returns -1.*/

void printProcessList(process **process_list); /* print the processes. */

char pathBuf[PATH_MAX];


int main(int argc, char **argv) {

//    for (int i = 0; i < 5; ++i) {
//        addProcess(&process_ll, parseCmdLines("ls"), i);
//    }

    char string[2048];
//    printProcessList(&process_ll);
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
    int res = 0;
    while (pCmdLine != NULL) {
        pid = fork();
        fprintf(stderr, "PID: %d\nExecuting command: %s \n", pid, pCmdLine->arguments[0]);
        if (pid == 0) {
            if ((strncmp(pCmdLine->arguments[0], "cd", 2) == 0) || (strncmp(pCmdLine->arguments[0], "procs", 5) ==0))  {
                _exit(1);
            } else {
                res = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            }
            if (res < 0) {
                perror("Error:  ");
                _exit(errno);
            } else {
                _exit(1);
            }

        } else {
            if(strncmp(pCmdLine->arguments[0], "procs", 5)!=0)
            addProcess(&process_ll, pCmdLine, pid);
            if (strncmp(pCmdLine->arguments[0], "cd", 2) == 0) {
                res = chdir(pCmdLine->arguments[1]);
                fprintf(stderr, "the cd response is:  %d", res);
                printf("%s \n", getcwd(pathBuf, PATH_MAX));
            }
            if(strncmp(pCmdLine->arguments[0], "procs", 5) == 0){
                printProcessList(&process_ll);
            }
            if (pCmdLine->blocking == 1) {
                fprintf(stderr, "Master is waiting... \n");
                waitpid(pid, &status, 0);
            }
        }
        pCmdLine = pCmdLine->next;
    }
}

void addProcess(process **process_list, cmdLine *cmd,
                int pid) {
    process *toAppend = malloc(sizeof(process));
    toAppend->cmd = cmd;
    toAppend->next = *process_list;
    toAppend->pid = pid;
    toAppend->status = RUNNING;
    process_ll = toAppend;
}

void printProcessList(process **process_list) {
    process *p1 = *process_list;
    fprintf(stdout, "PID      COMMAND      STATUS\n");
    while (p1 != NULL) {
        fprintf(stderr, "%d      %s      %d\n", p1->pid, p1->cmd->arguments[0], p1->status);
        p1 = p1->next;
    }
} /* print the processes. */
