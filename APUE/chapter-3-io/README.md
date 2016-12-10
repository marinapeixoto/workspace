# I/O
大多数UNIX文件I/O只需要用到5个函数：open, read, write, lseek, close。

unbuffered I/O，不带缓存指的是每个read、write都是调用内核的一个系统调用。它们不是ANSI C的组成部分，而是POSIX.1和XPG3的组成部分。

write是原子操作。


## 1. 文件描述符
UNIX shell中使用文件描述符0与进程的标准输入结合，文件描述符1与标准输出相结合，文件描述符2与标准输出相结合。

STDIN_FILENO，STDOUT_FILENO，STDERR_FILENO在unistd.h中定义。

文件描述符范围0~OPEN_MAX。见第二章节demo。

## 2. open
```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char* pathname, int oflag, .../*, mode_t mode */);

/*
oflag：
O_RDONLY,O_WRONLY,ORDWR
可选模式：
O_APPEND
O_CREAT
O_EXEC，如果同时指定O_CREAT，而文件存在则出错。测试文件是否存在，原子操作。
O_TRUNC，如果文件存在，而且只为只读只写打开，则文件长度截断为0
O_NOCTTY，如果pathname是终端设备，则不将此设备分配作为进程的控制终端。
O_NONBLOCK
O_SYNC
*/
``` 


## 3. creat
```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int creat(const char *pathname, mode_t mode);
等效于：
open(pathname, O_WRONLY|O_CREAT|O_TRUNC, mode);
```
不足：打开后只写，需要关闭后重新打开才能读。

## 4. close
```
#include <unistd.h>

int close(int filedes);
```

关闭文件也会释放进程加载该文件上所有的锁！

## 5. lseek
每个打开的文件都有一个与其关联的“当前文件偏移量”。用以度量从文件开始处计算的字节数。lseek可以显式地定位一个打开的文件：

```
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int filedes, off_t offset, int whence);

/*
whence:
SEEK_SET，距离文件开始处offset个字节
SEEK_CUR，距离但钱位置处offset个字节，可为正或负
SEEK_END，文件偏移量为文件长度加offset，可为正或负(超出文件长度会自动扩展)

获取当前位置：curpos = lseek(fd, 0, SEEK_CUR);
*/
```

## 6. read
```
#include <unistd.h>

ssize_t read(int filedes, void *buf, size_t nbytes);
```

## 8. write
```
#include <unistd.h>

ssize_t write(int filedes, const void* buf, size_t nbytes);
```

如果返回值不等于nbytes，则表示出错。

## 9. dup，dup2
复制文件描述符，新描述符和原始的共享一个文件表项。

```
#include <unistd.h>

int dup(int filedes);
int dup(int filedes, int filedes2); 
```
dup2复制一个文件描述符，其描述符值为filedes2的值。如果filedes已经打开需要先将其关闭。如果filedes等于filedes2，则返回filedes，而不关闭它。

dup(filedes) == fcntl(iledes, F_DUPFD, 0);

dup2(filedes, filedes2) == close(filedes2); fcntl(filedes, F_DUPFD, filedes2);

实际上并不完全等效，dup2是一个原子操作。

## 10. fcntl
fcntl可以改变已经打开文件的性质

```
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int fcntl(int filedes, int cmd, …/* int arg */);
```

fcntl函数的5中功能：
* 复制一个现存的描述符（cmd = F_DUPFD），新描述符与filedes共享同一文件表项，但新的描述符有自己的一套文件描述符标志。
* 获得/设置文件描述符标记（cmd = F_GETFD，F_SETFD）
* 获得/设置文件状态标志（cmd = FGETFL，F_SETFL）
* 获得_设置异步I_O所有权（cmd=F_GETOWN，F_SETOWN）
* 获得/设置记录锁（cmd=F_GETLK，F_SETLK，F_SETLKW）

## 11. ioctl
ioctl是I/O操作的杂物箱。

```
#include <unistd.h>    /* SVR4 */
#include <sys/ioctl.h> /* 4.3+BSD */

int ioctl(int filedes, int request,...);
```

ioctl不是POSIX.1的一部分。4.3+BSD的ioctl操作：

|类型|常数名|头文件|ioctl数|
| — |:——|:——-:|——:| -----:|
|盘标号|DIOxxx|<disklabel.h>|10|
|文件I/O|FIOxxx|<ioctl.h>|7|
|磁带I/O|MTIOxxx|<mtio.h>|4|
|套接口I/O|SIOxxx|<ioctl.h>|25|
|中断I/O|TIOxxx|<ioctl.h>|35|

## 12. /def/fd
比较新的系统都提供_dev_fd的目录，其目录项是名为0、1、2的文件，打开文件_dev_fd/n等效于复制描述符n（假设n是打开的）。






