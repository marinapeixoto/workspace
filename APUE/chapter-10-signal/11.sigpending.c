#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


static void sig_quit(int);

int
main(void) 
{
    sigset_t newmask, oldmask, pendmask;
    
    if(signal(SIGQUIT, sig_quit) == SIG_ERR) {
        printf("can't catch SIGQUIT\n");
        exit(1);
    }

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        printf("SIG_BLOCK error\n");
        exit(1);
    }
    sleep(5); /* SIGQUIT here will remain pending */

    if(sigpending(&pendmask) < 0) {
        printf("sigpending error\n");
        exit(1);
    }
    if(sigismember(&pendmask, SIGQUIT)) {
        printf("SIGQUIT pending\n");
    }

    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        printf("SIG_SETMASK error\n");
        exit(1);
    }
    printf("SIGQUIT unblocked\n");

    sleep(5);

    exit(0);
}

static void
sig_quit(int signo)
{
    printf("caught SIGQUIT\n");

    if(signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
        printf("can't reset SIGQUIT\n");
        exit(1);
    }
    return;
}

