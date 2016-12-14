#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 6;
char buf[] = "a write to stdout\n";

/*
 * 如果重定向输出到文件，则before fork输出两遍
 * 因为标准库函数printf带缓存
 * 输出到终端是行缓存的，否则是全缓存
 * 所以，交互方式只输出一次，而文件全缓存输出两次
 */

int 
main(void)
{
    int var;
    pid_t pid;

    var = 88;
    if(write(STDOUT_FILENO, buf, sizeof(buf)) != sizeof(buf)) {
        printf("write error\n");
        exit(1);
    }
    printf("before fork\n"); /* don't flush stdout */

    if((pid = fork()) < 0) {
        printf("fork error\n");
        exit(1);
    } else if(pid == 0) {
        glob++;
        var++;
    } else sleep(2);

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
    exit(0);
}

