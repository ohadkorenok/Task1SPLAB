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
int *statusCurrent;

void execute(cmdLine *pCmdLine);

void addProcess(process **process_list, cmdLine *cmd,
                int pid); /*Receive a process list (process_list), a command (cmd), and the process id (pid) of the process running the command.*/
void freeProcessList(process *process_list);

void updateProcessStatus(process *process_list, int pid, int status);

void updateProcessList(
        process **process_list); /*go over the process list, and for each process check if it is done, you can use
 * waitpid with the option WNOHANG. WNOHANG does not block the calling process, the process returns from the call to
 * waitpid immediately. If no process with the given process id exists, then waitpid returns -1.*/

void printProcessList(process **process_list); /* print the processes. */

void removeFromProcessList(int pid);

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
    int killFlag = 0;
    int status;
    int res = 0;
    while (pCmdLine != NULL) {
        pid = fork();
        fprintf(stderr, "PID: %d\nExecuting command: %s \n", pid, pCmdLine->arguments[0]);
        if (pid == 0) {
            if ((strncmp(pCmdLine->arguments[0], "cd", 2) == 0) ||
                (strncmp(pCmdLine->arguments[0], "procs", 5) == 0) ||
                (strncmp(pCmdLine->arguments[0], "kill", 4) == 0) ||
                (strncmp(pCmdLine->arguments[0], "wake", 4) == 0) ||
                    (strncmp(pCmdLine->arguments[0], "suspend", 7) == 0)) {
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
            if (strncmp(pCmdLine->arguments[0], "kill", 4) == 0) {
                int currPid = atoi(pCmdLine->arguments[1]);
                killFlag = 1;
                kill(atoi(pCmdLine->arguments[1]), SIGINT) == 0 ? updateProcessStatus(process_ll, currPid, TERMINATED)
                                                                : perror("kill");
            }

            if (strncmp(pCmdLine->arguments[0], "suspend", 7) == 0) {
                int currPid = atoi(pCmdLine->arguments[1]);
                killFlag = 1;
                kill(atoi(pCmdLine->arguments[1]), SIGTSTP) == 0 ? updateProcessStatus(process_ll, currPid, SUSPENDED)
                                                                 : perror("kill");
            }

            if (strncmp(pCmdLine->arguments[0], "wake", 4) == 0) {
                int currPid = atoi(pCmdLine->arguments[1]);
                killFlag = 1;
                kill(atoi(pCmdLine->arguments[1]), SIGCONT) == 0 ? updateProcessStatus(process_ll, currPid, RUNNING)
                                                                 : perror("kill");
            }
            if ((strncmp(pCmdLine->arguments[0], "procs", 5) != 0) && (killFlag == 0))
                addProcess(&process_ll, pCmdLine, pid);
            if (strncmp(pCmdLine->arguments[0], "cd", 2) == 0) {
                res = chdir(pCmdLine->arguments[1]);
                fprintf(stderr, "the cd response is:  %d", res);
                printf("%s \n", getcwd(pathBuf, PATH_MAX));
            }
            if (strncmp(pCmdLine->arguments[0], "procs", 5) == 0) {
                printProcessList(&process_ll);
            }
            if (pCmdLine->blocking == 1) {
                fprintf(stderr, "Master is waiting... \n");
                waitpid(pid, &status, 0);
            }
//            updateProcessList(&process_ll);
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
    updateProcessList(process_list);
    process *p1 = *process_list;
    process *nextProcess;
    fprintf(stdout, "PID      COMMAND      STATUS\n");
    while (p1 != NULL) {
        fprintf(stdout, "%d      %s      %s\n", p1->pid, p1->cmd->arguments[0],
                p1->status == 1 ? "RUNNING" : p1->status == 0 ? "SUSPENDED" : "TERMINATED");
        if (p1->status == TERMINATED) {
            nextProcess = p1->next;
            removeFromProcessList(p1->pid);
            p1 = nextProcess;
        } else {
            p1 = p1->next;
        }
    }
} /* print the processes. */

void freeProcessList(process *process_list) {
    if (process_list != NULL) {
        free(process_list->cmd);
        process *nextProcess = process_list->next;
        free(process_list);
        freeProcessList(nextProcess);
    }
}

void updateProcessList(process **process_list) {
//    printf("Update ProcessList started!\n");
    process *p1 = *process_list;
    pid_t w;
    int statusToUpdate;
    while (p1 != NULL) {
        int status;
        w = p1->cmd->blocking == 1 ? waitpid(p1->pid, &status, 0) : waitpid(p1->pid, &status, WNOHANG);
        if (w == -1) {
            updateProcessStatus(*process_list, p1->pid, TERMINATED);
        } else {
            if (w != 0) {
                statusToUpdate = WIFEXITED(status) || WIFSIGNALED(status) ? TERMINATED : WIFSTOPPED(status) ? SUSPENDED
                                                                                                            : WIFCONTINUED(
                                                                                                                      status)
                                                                                                              ? RUNNING
                                                                                                              : -900;
                updateProcessStatus(*process_list, p1->pid, statusToUpdate);
            }

        }


        p1 = p1->next;
    }
}

void updateProcessStatus(process *process_list, int pid, int status) {
    process *p1 = process_list;
//    printf("update process for PID: %d with status : %d started \n", pid, status);
    while (p1 != NULL) {
        if (p1->pid == pid) {
            p1->status = status;
            return;
        }
        p1 = p1->next;
    }
}

void removeFromProcessList(int pid) {
    process *p1 = process_ll;
    if (p1->pid == pid) {
        process_ll = p1->next;
        free(p1->cmd);
        free(p1);
//        printf("removed the process : %d from process list \n", pid);
        return;
    }
    p1 = p1->next;
    process *prev = process_ll;
    while (p1 != NULL) {
        if (p1->pid == pid) {
            prev->next = p1->next;
            free(p1->cmd);
            free(p1);
//            printf("removed the process : %d from process list \n", pid);
            return;
        }
        prev = prev->next;
        p1 = p1->next;
    }
}

