#include<stdlib.h>
#include<stdlib.h>
#include<sys/types.h>
int main()
{
  int i = 0;
  pid_t pid;

  if((pid = fork()) < 0)
    printf("error\n");

  else if(pid == 0)
  {

  }
}
