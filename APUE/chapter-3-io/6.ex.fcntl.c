#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int
main(void)
{
    int fd, fd2, nfd, val;

    if((fd = open("tmp.out", O_RDWR, O_CREAT)) < 0) {
        printf("open error\n");
        exit(1);
    }
    
    /* now we open the same file */
    if((fd2 = open("tmp.out", O_RDWR)) < 0) {
        printf("open2 error\n");
        exit(1);
    }

    if((nfd = fcntl(fd, F_DUPFD, 0)) < 0) {
        printf("fcntl F_DUPFD error\n");
        exit(1);
    }

    /* only FD_CLOEXEC is used */
    if((val = fcntl(fd, F_GETFD, 0)) < 0) {
        printf("fcntl F_GETFD error\n");
        exit(1);
    }
    printf("fd(%d) getfd %d\n", fd, val);
    if((val = fcntl(fd, F_SETFD, FD_CLOEXEC)) < 0) {
        printf("fcntl F_SETFD error. val:%d\n", val);
        exit(1);
    }
    if((val = fcntl(fd, F_GETFD, 0)) < 0) {
        printf("fcntl F_GETFD error\n");
        exit(1);
    }
    printf("fd(%d) getfd %d\n", fd, val);
    
    /* now we set FL of fd, what about FL about nfd and fd2? */
    if(fcntl(fd, F_SETFL, O_TRUNC|O_CREAT|O_EXCL|O_NONBLOCK) < 0) {
        printf("fcntl F_SETFL error\n");
        exit(1);
    }

    printf("fd[%d] is dup of fd[%d]\n", fd, nfd);

    if((val = fcntl(fd, F_GETFL, 0)) < 0) {
        printf("fcntl F_GETFL error\n");
        exit(1);
    }
    printf("fd[%d] FL[%d]\n", fd, val);
    

    if((val = fcntl(nfd, F_GETFL, 0)) < 0) {
        printf("fcntl F_GETFL error\n");
        exit(1);
    }
    printf("fd[%d] FL[%d]\n", nfd, val);


    if((val = fcntl(fd2, F_GETFL, 0)) < 0) {
        printf("fcntl F_GETFL error\n");
        exit(1);
    }
    printf("fd[%d] FL[%d]\n", fd2, val);

    exit(0);
}
