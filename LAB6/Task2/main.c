#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t cpid;
    pid_t cpid2;
    int debug = 0;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-D") == 0) {
            debug = 1;
        }
    }
    void printDebug(char *string, int debugFlag);
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    printDebug("parent_process > parent forking \n", debug);
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    /* Child reads from pipe */
        printDebug("child1 > redirecting stdout to the write end of the pipe…\n",debug);
        close(1);
        char *array[3] = {"ls", "-l", '\0'};
        dup(pipefd[1]);
        close(pipefd[1]);
        printDebug("child1 > going to execute cmd: …",debug);
        execvp("ls", array);

        _exit(0);
    } else {
        if (debug == 1)
            fprintf(stderr, "parent_process > created with process id: %d \n", cpid);
        printDebug("parent_process > closing the write end of the pipe...", debug);
        close(pipefd[1]); // WE MUST CLOSE IT FOR FLUSHING EVERYTHING TO THE READEND
        cpid2 = fork();
        if (cpid2 == 0) {
            printDebug("child2 > redirecting stdin to the read end of the pipe…",debug);

            close(0);
            dup(pipefd[0]);
            printDebug("parent_process > closing the read end of the pipe",debug);
            close(pipefd[0]);
            char *array2[4] = {"tail", "-n", "2", '\0'};
            printDebug("child2 > going to execute cmd: …",debug);

            execvp("tail", array2);
        } else {
            if (debug == 1)
                fprintf(stderr, "parent_process > created with process id: %d \n", cpid2);
            close(pipefd[1]);
            close(pipefd[0]); //NOTHING CHAnges bro!
            int status2;
            int status;
            printDebug("parent_process > waiting for the child processes to terminate",debug);
            waitpid(cpid, &status, 0);
            waitpid(cpid2, &status2, 0);
        }
    }
    printDebug("parent_process > exiting... ",debug);
    exit(EXIT_SUCCESS);
}


void printDebug(char *string, int debugFlag) {
    if (debugFlag == 1) {
        fprintf(stderr, string);
        fprintf(stderr, "\n");
    }
}