#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>


void *thread_run(void *arg)
{
  pthread_detach(pthread_self());
  printf("%s\n", (char *)arg);
  return NULL;
}


int main()
{
  pthread_t tid;
  //线程创建，执行thread_run函数
  if(pthread_create(&tid, NULL, thread_run, "thread1 run----") != 0)
  {
    printf("create thread error\n");
    return 1;
  }


 int ret = 0;
 sleep(1);
 //等待线程结束
 if(pthread_join(tid, NULL) == 0)
 {
    printf("prthed wait success\n");
    ret =1;
 }
 return ret;

  return 0;
}
