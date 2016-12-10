#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define PATH_MAX_GUESS 1024 

char*
path_alloc(int* size) 
{
    char* ptr;
    
    if(pathmax == 0) {
        errno = 0;
        if( (pathmax = pathconf("/", _PC_PATH_MAX)) <0 ) {
            if(errno == 0) pathmax = PATH_MAX_GUESS;
            else printf("pathcof error of _PC_PATH_MAX\n");
        } else pathmax++; /* add noe since it's relative to the root */
    }

    if((ptr = malloc(pathmax + 1)) == NULL) 
        printf("malloc error for pathname\n");
    if(size != NULL)
        *size = pathmax + 1;
    return(ptr);
}


int
main(void)
{
    int size = 0;
    char* ptr = NULL;

    ptr = path_alloc(&size);
    printf("path: %d, size: %d\n", (int)ptr, size);
    exit(0);
}

