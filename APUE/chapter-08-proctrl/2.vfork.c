#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 6;

int 
main(void)
{
    int var;
    pid_t pid;

    var = 88;

    printf("before vfork\n");
    if((pid = vfork()) < 0) {
        printf("vfork error\n");
        exit(1);
    } else if(pid == 0) {
        glob++;
        var++;
        /* 
         * 不能调用清除缓存的exit，因为
         * 子进程在父进程空间中运行
         * 如果调用exit会清除父进程中的标准输出缓存
         * mac上效果一样？？、
         */
        /* _exit(0); */
        exit(0);
    }

    printf("pid = %d, glob = %d, var = %d\n", pid, glob, var);
    exit(0);
}

