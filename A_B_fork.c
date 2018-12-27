#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	//创建管道，重定向子进程标准输出、标准错误
	//父进程读管道，并将数据写到文件
    int fd[2];
    pid_t pid;
    int len = 0;

    if( pipe(fd)<0  )
    {
        perror("failed to pipe");
        exit(1);
    } 
    int read_data = fd[0];
    int write_data = fd[1];

    if( (pid = fork())<0  )
    {
        perror("failed to fork");
        exit(1);
    }
	
    else if(pid > 0)
    {
		//father
        printf("父进程开始\n");
        close(write_data);
        char buf = '\0';
    
        int fd = open("info.txt",O_CREAT|O_RDWR|O_APPEND,0777);
        if(fd <0 ){
            printf("open error");
            exit(-1);
        }

        while( read(read_data,&buf,1) >0 )
        {
            printf("%c",buf);
            if((write(fd,&buf,sizeof(buf)))<0)   //写到fd
			{
                printf("write error");
                exit(-1);
            }

        }

        close(fd);
        printf("父进程结束\n");
        exit(0);

    }
    else
    {
        //printf("子进程开始\n");
        close(read_data);
        dup2(write_data,STDERR_FILENO);
        dup2(write_data,STDOUT_FILENO);

        printf("这是子进程的一个标准输出\n");
        printf("子进程运行完了\n");
        printf("子进程结束\n");

    }
    return 0;

}

