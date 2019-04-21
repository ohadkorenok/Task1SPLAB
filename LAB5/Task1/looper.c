#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

void handle_sigint(int sig)
{
	printf("Caught signal %d which is the %s signal \n", sig, strsignal(sig));
    signal(sig, SIG_DFL);
    raise(sig);
}
int main(int argc, char **argv){ 

	printf("Starting the program\n");
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigint);
    signal(SIGCONT, handle_sigint);


    while(1) {
		sleep(2);
	}

	return 0;
}