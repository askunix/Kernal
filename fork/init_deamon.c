#include<stdio.h>
#include<stdlib.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>

void init_deamon(void)
{
  int pid;
  int i;

  if(signal(SIGCHLD, SIG_IGN) == SIG_ERR){
    printf("cant signal in init_deamon\n");
    exit(1);
  }

  if((pid = fork()) > 0)
    exit(0);
  else if(pid < 0){
    perror("file to fork");
    exit(1);
  }

  setsid();

  if((pid = fork()) > 0)
    exit(0);
  else if(pid < 0)
    exit(1);

  for(i = 0; i< getdtablesize(); ++i)
    close(i);
  chdir("./");
  umask(0);
  return;
}
