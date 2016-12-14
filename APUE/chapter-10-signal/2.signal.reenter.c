#include <pwd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void my_alarm(int);

int
main(void)
{
    struct passwd *ptr;
    
    signal(SIGALRM, my_alarm);
    alarm(1);

    for( ; ; ) {
        if( (ptr = getpwnam("tkorays")) == NULL) {
            printf("getpwnam error\n");
            exit(1);
        }
        if(strcmp(ptr->pw_name, "tkorays") != 0) 
            printf("return value corrupted, pw_name = %s\n", ptr->pw_name);
    }
    exit(0);
}


static void
my_alarm(signo) 
{
    struct passwd *rootptr;
    printf("in signal handler\n");
    if( (rootptr = getpwnam("root")) == NULL) {
        printf("getpwnam(root) error\n");
        exit(1);
    }
    alarm(1);
    return;
}

