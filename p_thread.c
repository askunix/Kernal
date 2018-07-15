#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>

void *route(void *arg)
{
  //int i;
  for(;;)
  {
    printf("i am thread @@@@@@@\n");
    sleep(2);
  }
}


int main(void)
{
  pthread_t tid;
  int ret;
  if((ret = pthread_create(&tid, NULL, route, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  //int i;
  for(;;)
  {
    printf("I am Main thread!!\n");
    sleep(2);
  }

  return 0;
}
