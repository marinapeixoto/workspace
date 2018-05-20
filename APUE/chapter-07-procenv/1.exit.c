#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void my_exit1(void), my_exit2(void);

int
main(void)
{
    if(atexit(my_exit2) != 0) {
        printf("can't register my_exit2\n");
        exit(0);
    }
    if(atexit(my_exit1) != 0) {
        printf("can't register my_exit1\n");
        exit(0);
    }
    if(atexit(my_exit1) != 0) {
        printf("can't register my_exit1\n");
        exit(0);
    }
    printf("main is done!\n");
    return(0);
}


static void
my_exit1(void)
{
    printf("first exit handler\n");
}

static void
my_exit2(void)
{
    printf("second exit handler\n");
}

