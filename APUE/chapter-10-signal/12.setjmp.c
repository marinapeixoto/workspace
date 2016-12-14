#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void sig_usr1(int),sig_alrm(int);
static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjmp;
static void pr_mask(const char* str);

int 
main(void)
{
    if(signal(SIGUSR1, sig_usr1) == SIG_ERR) {
        printf("signal(SIGUSR1) error\n");
        exit(1);
    }
    if(signal(SIGALRM, sig_alrm) == SIG_ERR) {
        printf("signal(SIGALRM) error\n");
        exit(1);
    }
    pr_mask("starting main:");

    if(sigsetjmp(jmpbuf, 1)) {
        pr_mask("ending main:");
        exit(0);
    }
    canjmp = 1;

    for(;;)
        pause();
}

void
pr_mask(const char* str)
{
    sigset_t    sigset;
    int         errno_save;

    errno_save = errno;
    if(sigprocmask(0, NULL, &sigset) < 0) {
        printf("sigprocmask error\n");
        exit(1);
    }

    printf("%s\n", str);

    if(sigismember(&sigset, SIGINT)) printf("SIGINT ");
    if(sigismember(&sigset, SIGQUIT)) printf("SIGQUIT ");
    if(sigismember(&sigset, SIGUSR1)) printf("SIGUSR1 ");
    if(sigismember(&sigset, SIGALRM)) printf("SIGALRM ");
    printf("\n");
    errno = errno_save;
}

static void
sig_usr1(int signo)
{
    time_t starttime;
    
    /* unexpected signal, ignore */
    if(canjmp == 0)
        return ;
    pr_mask("starting sig_usr1");
    alarm(3);

    starttime = time(NULL);
    for( ; ; )
        if(time(NULL) > starttime + 5)
            break;
    pr_mask("finishing sig_usr1:");

    canjmp = 0;
    /* jmp back to main, don't return */
    siglongjmp(jmpbuf, 1);
}

static void 
sig_alrm(int signo)
{
    pr_mask("in sig_alrm:");
    return;
}



