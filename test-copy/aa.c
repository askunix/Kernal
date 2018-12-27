#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
  int i = 0;
  for(i = 0; i<argc; i++)
    printf("%d : %s \n", i, argv[i]);
  printf("\n%d\n", argc);
  return 0;
}
