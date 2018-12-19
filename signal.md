### 信号的概念：

信号是软件中断。信号提供一种处理异步事件的方法。
信号被定义为正整数常量，定义在<signal.h>中。

在某个信号出现时，可以告诉内核以三种方式处理：忽略、捕捉、执行系统默认动作。


### 信号相关的函数：

#### 函数signal

``` C++
void (*signal (int signo, void (*func)(int))) (int);
```

signal()函数(它自己是带两个参数,一个为整型,一个为函数指针的函数),而这个signal()函数的返回值也为一个函数指针,这个函数指针指向一个带一个整型参数,并且返回值为void的一个函数。

当一个进程调用fork时，其子进程继承了父进程的信号处理方式。因为子进程在开始时复制了父进程内存映像，所以信号捕捉函数的地址在子进程中是有意义的。

#### 中断的系统调用

早期UNIX系统的一个特性是：如果进程在执行一个低速系统调用而阻塞期间捕获到一个信号，则该系统调用就被中断不再继续执行。

该系统调用返回出错，其errno设置为EINTR。后面的章节会更多的涉及到被中断的系统调用。


进程捕获到信号并对其进行处理时，进程正在执行的正常指令序列就被信号处理程序临时中断，它首先执行该信号处理程序中的指令。


#### 可重入函数

如果从信号处理程序返回，则继续执行在捕获到信号时进程正在执行的正常指令序列。

可重入函数主要用于多任务环境中，该函数在执行过程中可以随时被中断，返回来继续执行的时候不会发生错误。他除了使用自身的栈上的变量外，不依赖任何环境（如果要使用全局变量等等还需要做一些措施）

#### 函数kill和raise

kill函数将信号发送给进程或者进程组；raise函数允许进程向自身发送信号。

```C++
#include <signal.h>
int kill(pid_t pid,int signo);
int raise(int signo);
```


#### 函数alarm和pause

函数alarm设置一个定时器，当定时器超时时，产生SIGALRM信号。

```C++
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
```
pause函数使调用进程挂起直至捕捉到一个信号

```
#include <signal.h>
int pause(void);
```
只有执行了一个信号处理程序并从其返回时，pause才返回。此时，pause返回-1，errno设置为EINTR。

 
#### 信号集

信号集是能表示多个信号的数据结构，

```C++
#include <signal.h>
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set,int signo);
int sigdelset(sigset_t *set,int signo);
int siggismember(const sigset_t *set,int signo);
```

在使用信号集之前，要对该信号集进行初始化。（调用sigemptyset或者sigfillset）


```C++
#include <signal.h>
int sigprocmask(int how,const sigset_t *restrict set,sigset_t *restrict oset);

```

进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集。调用函数sigprocmask可以检测或更改进程的信号屏蔽字。


```cpp
#include <signal.h>
ing sigpending(sigset_t *set);
```

sigpending函数返回一信号集，对于调用进程而言，其中的各信号是阻塞不能递送的，因而也一定是当前未决的。


#### 函数sigaction

sigaction函数的功能是检查或修改与制定信号相关联的动作处理。次函数取代了Unix早期版本使用的signal函数。

```cpp
#include <signal.h>
int sigaction(int signo,const struct sigction *restrict act,
               struct sigaction *restrict oact);
```

次函数运用了以下接口：

```cpp
struct sigaction 
{
        void     (*sa_handler)(int);
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        sigset_t   sa_mask;
        int        sa_flags;
};
```


#### 函数abort

abort函数的功能是使程序异常终止。

```cpp
#include <stdlib.h>
void abort(void);
```
此函数将SIGABRT信号发送给调用进程。让进程捕捉SIGABRT信号目的是在进程终止之前由其执行所需的清理操作。默认情况是终止调用进程。


#### 函数system

POSIX.1要求system函数忽略SIGINT和SITQUIT信号，阻塞SIGCHLD。

 

#### 函数sleep

此函数使调用进程被挂起，直到满足下列条件之一：

（1）已经经过seconds所指定的墙上时钟时间。

（2）调用进程捕捉到一个信号并从信号处理程序返回。


