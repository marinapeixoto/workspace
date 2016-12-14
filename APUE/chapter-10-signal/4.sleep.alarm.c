#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void
sig_alarm(int signo)
{
    return; /* noting to do */
}

unsigned int
sleep1(unsigned int nsecs)
{
    if(signal(SIGALRM, sig_alarm) == SIG_ERR)
        return(nsecs);
    alarm(nsecs);
    pause();
    return(alarm(0));
}

int
main(void)
{
    printf("%d\n", sleep1(5));
    return 0;
}
