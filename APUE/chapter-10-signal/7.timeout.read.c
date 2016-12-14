#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 24

static void
sig_alarm(int signo)
{
    return ;
}

int
main(void)
{
    int n ;
    char line[MAXLINE];
    if(signal(SIGALRM, sig_alarm) == SIG_ERR) {
        printf("signal(SIGALRM) error\n");
        exit(1);
    }
    alarm(10);
    if( (n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
        printf("read error\n");
        exit(1);
    }
    alarm(0);
    write(STDOUT_FILENO, line, n);
    exit(0);
}

