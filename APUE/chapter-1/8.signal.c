
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 50

static void sig_int(int); /* our signal-catching function */

int 
main(void)
{
    char    buf[MAXLINE];
    pid_t   pid;
    int     status;

    if(signal(SIGINT, sig_int) == SIG_ERR)
        printf("signal error.\n");

    printf("%% "); /* print prompt */
    while(fgets(buf, MAXLINE, stdin) != NULL) {
        buf[strlen(buf) - 1] = 0; /* replace newline with null */

        if( (pid = fork()) < 0) printf("fork error!\n");
        else if(pid == 0) { /* child */
            execlp(buf, buf, (char*)0);
            /*
             * the exec family of functions replace the current
             * process image with a new process image.
             * the old process will continue if new process is not
             * created!
             */
            printf("couldn't execute: %s\n", buf);
            exit(127);
        }

        /* parent */
        if( (pid = waitpid(pid, &status, 0)) < 0) printf("wait pid error!\n");
        printf("%% "); /* print prompt */
    }
    exit(0);
}

void
sig_int(int signo)
{
    printf("interrupt%% ");
}
