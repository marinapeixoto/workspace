#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env_alarm;

static void
sig_int(int signo)
{
    int i;
    volatile int j;
    printf("\nsig_int starting\n");
    for(i = 0; i< 2000000; i++)
        j += i*i;
    printf("sig_int finished\n");
    return;
}


static void
sig_alarm(signo)
{
    longjmp(env_alarm, 1);
}

unsigned int
sleep2(unsigned int nsecs)
{
    if(signal(SIGALRM, sig_alarm) == SIG_ERR)
        return nsecs;
    if(setjmp(env_alarm) == 0) {
        alarm(nsecs);
        pause();
    }
    return alarm(0);
}


int
main(void)
{
    unsigned int unslept;
    if(signal(SIGINT, sig_int) == SIG_ERR) {
        printf("signal(SIGINT) error\n");
        exit(1);
    }
    unslept = sleep2(5);
    printf("sleep2 returned: %d\n", unslept); 
    exit(0);
}
