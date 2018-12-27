#include<stdio.h>


#if 0
int numberOfOne(int n)
{
  int count = 0;
  int flag = 1;
  while(flag)
  {
    if(flag & n)
      ++count;
    flag = flag << 1;
  }
  return count;
}
#endif

int numberOfOne(int n)
{
  int count = 0;
  while(n)
  {
    ++count;
    n = n & (n-1);
  }
  return count;
}

void test1()
{
  int num = 5;
  int ret = numberOfOne(num);
  printf("%d\n", ret);
}






int main()
{
  test1();
  return 0;
}
