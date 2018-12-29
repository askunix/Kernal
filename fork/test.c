#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

void init_deamon(void);


int main()
{
  FILE *fp;
  time_t t;
  init_deamon();
  while(1){
    sleep(5);
    fp = fopen("print_time", "a");
    if(fp != NULL){
      t = time(0);
      fprintf(fp, "The time right now is: %s \n",
          asctime(localtime(&t)));
      fclose(fp);
    }
  }
  return 0;
}
