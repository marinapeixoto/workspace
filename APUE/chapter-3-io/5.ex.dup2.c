#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(void) 
{
    int     fd, fd2, nfd;
    if((fd = open("tmp.out", O_WRONLY, O_CREAT|O_TRUNC)) < 0) {
        printf("open file error\n");
        exit(1);
    }

    printf("dup fd%d -> %d\n", fd, dup(fd));
    
    fd2 = fd + 1;
    if((nfd = dup2(fd, fd2)) < 0) {
        printf("fail to dup fd\n");
        exit(1);
    }
    printf("dup fd%d to %d\n", fd, nfd);
    exit(0);
}
