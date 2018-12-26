#include<stdio.h>                                               
#include<unistd.h>
#include<sys/types.h>
#include<wait.h>
#include<fcntl.h>

//思路：子进程将标准输出、标准错误重定向到管道。父进程从管道读数据，并写入文件。

int main()
{
  pid_t pid;
  int fd1[2];
  int fd2[2];
  //
  int ret = pipe(fd1);
  if(ret < 0)
  {
    printf("pipe error\n");
    return 1;
  }

  ret = pipe(fd2);
  if(ret < 0)
  {
     close(fd1[0]);
     close(fd1[1]);
     return 1;
  }

  int father_read = fd1[0];
  int father_write = fd1[1];
  int child_read = fd2[0];
  int child_write = fd2[1];

 

  if((pid = vfork()) < 0 )       //保证子进程先运行
  {
    printf("error\n");
    return 0;
  }


  if(pid > 0)
  {
    //parent
    printf("进入父进程\n");
    //close(child_write);
    //close(child_read);
    
    //父进程循环从管道中读取数据，并把数据写入到文件
    char c = '\0';
    FILE *fp = fopen("inf.txt", "wr+");
    if(!fp)
    {
      printf("open failed\n");
      return 0;
    }

    while(read(child_read, &c, 1) > 0)
    {
      fprintf(fp, "%c", c);
    }
    printf("父进程执行完了\n");
  }

  else 
  {
    close(father_write);
    close(father_read);

    // printf("这是子进程的一个输出\n");
    //把标准输出、标准错误重定向到管道中
    dup2(child_write, 0);
    dup2(child_write, 2);
  
    printf("这是子进程的一个标准输出\n");
    printf("子进程运行完了\n");
    //sleep(2);
  }

  close(fd1[0]);
  close(fd2[0]);
  close(fd1[1]);
  close(fd2[1]);
  return 0;

}
