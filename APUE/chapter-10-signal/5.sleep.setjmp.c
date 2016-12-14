#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static jmp_buf env_alarm;

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
    printf("%d\n", sleep2(5));
    return 0;
}
