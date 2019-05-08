#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t cpid;
    char buf;
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    /* Child reads from pipe */
        write(pipefd[1], "hello", 5);
        close(pipefd[1]);
        _exit(0);
    }
    int status;
    waitpid(cpid, &status, 0);
    close(pipefd[1]);
    while (read(pipefd[0], &buf, 1) > 0) {
        write(1, &buf, 1);
    }
    write(1, "\n", 1);
    close(pipefd[0]);
    exit(EXIT_SUCCESS);
}
