# Unix 信号
SIGKILL和SIGSTOP不能被忽略。

## 1. singal函数
```
#include <signal.h>
void (*signal(int signo, void (*func)(int)))(int);
```

func常见值：SIG_IGN、SIG_DFL。

## 2. kill & raise
```
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int signo);
int raise(int signo);
```

* pid>0，发送给进程ID为pid的进程
* pid == 0，发送给进程组ID等于发送进程组ID的进程，而且发送进程有许可权向其发送信号的所有进程（不包含交换进程pid0，init进程pid1以及精灵进程pid2）。
* pid<0，发哦少年宫给进程组ID等于pid的绝对值，而且发送进程有许可权向其发送的所有进程。
* pid==-1，POSIX.1未定义。

POSIX.1将信号0定义为空信号。如果signo为0，kill仍执行正常错误检查，但不发送信号。

## 3. alarm和pause
```
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
int pause(void);
```

SIGALRM信号如果不忽略或者不捕捉，则其默认动作是停止该进程。alarm(0)可以取消闹钟，并返回余值。


## 4. 信号集
```
#include <signal.h>
int sigemptyset(sigset_t* set);
int sigfillset(sigset_t* set);
int sigaddset(sigset_t* set, int signo);
int sigdelset(sigset_t* set, int signo);

int sigismember(const sigset_t* set, int signo);
```

sigemptyset，初始化set，全部排除；sigfillset，初始化set，全部包含。


## 5. sigprocmask
```
#include <signal.h>
int sigprocmask(int how, const sigset_t* set, sigset_t* oset);
```
如果set是一个空指针，那么oset返回当前的屏蔽字。
如果set是一个非空指针，参数how只是如何修改当前屏蔽字。

how:

* SIG_BLOCK
* SIG_UNBLOCK
* SIG_SETMASK

## 5. sigpending
```
#include <signal.h>
int sigpending(sigset_t* set);
```

返回对于调用进程被阻塞不能递送和当前未决的信号集。

## 6. sigaction
检查或修改指定信号相关联的处理动作。
```
#include <signal.h>

struct sigaction {
	void (*sa_handler)();
	sigset_t sa_mask;
	int sa_flags;
};

int sigaction(int signo, const struct sigaction* act, struct sigaction* oact);
```

act指针为非空，则修改；oact非空，则返回信号原动作。可以同时检查和修改。

## 7. sigsetjmp,siglongjmp
4.3+BSD 提供了_setjmp,_longjmp，不保存和恢复信号屏蔽字。SVR4 的setjmp,longjmp不提供保存和恢复信号屏蔽字的功能。

```
#include <setjmp.h>
int sigsetjmp(sigjmp_buf env, int savemask);

/* 若直接调用返回0，若从siglongjmp调用返回则非0 */
void siglongjmp(sigjmp_buf env, int val);
```

如果savemask非0，则在sigsetjmp的env中保存进程当前的信号屏蔽字。调用siglongjmp时，自动恢复保存的信号屏蔽字。

## 8. system
POSIX.2要求忽略SIGINT和SIGQUIT，阻塞SIGCHLD。

## 9. 作业控制信号
* SIGCHLD 子进程已停止或终止
* SIGCONT 如果子进程已停止，则时期继续运行
* SIGSTOP 停止信号，不能被捕捉或忽略
* SIGTSTP 交互停止信号
* SIGTTIN 后台进程组的成员读控制终端
* SIGTTOU 后台进程组的成员写控制终端

## 10. 信号名字
某些系统提供了数组 `extern char* sys_signal[];` 记录信号名称。这些系统通常提供函数psignal：

```
#include <signal.h>
void psignal(int signo, const char* msg);
```

SVR4和4.3+BSD都提供了sys_signal和psignal。










