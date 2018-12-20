#include <stdio.h>
#include <setjmp.h>
 
jmp_buf jmpbuffer;

void f1()
{
   printf("这在函数f1中\n");
   longjmp(jmpbuffer,1);
}

int main(void)
{   
   if(setjmp(jmpbuffer)!=0)
   {
      printf("jmp\n");
      return 0;
   }
 
   printf("进入f1中\n");
   f1();
   return 0;
}
 




