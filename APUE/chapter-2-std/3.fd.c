#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef OPEN_MAX 
static int openmax = OPEN_MAX;
#else 
static int openmax = 0;
#endif

#define OPEN_MAX_GUESS 256

int 
open_max(void)
{
    if(openmax == 0) {
        errno = 0;
        if( (openmax = sysconf(_SC_OPEN_MAX)) < 0 ) 
            openmax = OPEN_MAX_GUESS;
        else
            printf("sysconf error for _SC_OPEN_MAX\n");
    }
    return(openmax);
}

int 
main(void) 
{
    printf("openmax: %d\n", open_max());
    exit(0);
}

