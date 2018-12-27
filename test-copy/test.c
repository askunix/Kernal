#include<stdio.h>
//这是注释
#define PI 3.24259265

int main()
{
  //主函数部分
  int r = 5;
  double s= 0;/*注释*/
  double c = 0;
  s = PI * r * r;
  c = 2 * PI * r;
  printf("面积：%f\n", s);
  printf("周长：%f\n", c);
  return 0;
}
