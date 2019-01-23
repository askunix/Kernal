#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<wait.h>
#include<pthread.h>


#define filePath "./File"
#define DestPath "./Dest"
#define MAX_FILE 1024
#define PTH_COUNT 10
char *file_find_str = "error";


typedef struct file_path
{
    size_t _id;
    //char _file[256];
    char *_file;
}_File_Path;


typedef struct Con 
{
    _File_Path  _count[MAX_FILE];
    int _sz;
}Con, *pCon, **ppCon;



pthread_t thread[2];
pthread_mutex_t mut;
int num = 0;
int i = 0;



/*函数声明*/
int matchingFile_str(char *);






void Init_Struct_Node(pCon _con)
{
    _con->_sz = 0;
    memset(_con->_count, 0, MAX_FILE * sizeof(_File_Path));
}


//尝试打印每个结构体内容
void test_Printf(pCon _myInf)
{
    int i = 0;
    printf("%d\n", _myInf->_sz);
    for (i = 0; i < _myInf->_sz; i++)
    { 
      printf("ID : %-5d,  PATH : %s \n", 
                   _myInf->_count[i]._id,
                   _myInf->_count[i]._file);
    }
}

pCon my_con;



int NUM = 0;



//线程入口函数
#if 0
void *thread1()
{
  for(i=0; i< 22; i++)
  {
    pthread_mutex_lock(&mut);
    matchingFile_str(my_con->_count[i]._file);
    pthread_mutex_unlock(&mut);
  }
  pthread_exit(NULL);
}



void *thread2()
{
  for(i = 0; i<22; i++)
  {
    pthread_mutex_lock(&mut);
    matchingFile_str(my_con->_count[i]._file);
    pthread_mutex_unlock(&mut);
  }
  pthread_exit(NULL);
}
#endif


#if 1
void *thread1(void *PS)     //将 Con 结构体指针传进来
{
  pCon s = (pCon)PS;
  for(i = 0; i < s->_sz; i++)
  {
    pthread_mutex_lock(&mut);
    //任务就是匹配
    printf("thread1 : NUM = %d\n", NUM);
    matchingFile_str(s->_count[i]._file);
    NUM ++;
    pthread_mutex_unlock(&mut);
  }
  printf("thread1: 主线程还在等我完成任务吗????\n");
  pthread_exit(NULL);

}


void *thread2(void *PS)
{
  pCon s = (pCon)PS; 
  for(i = 0; i< s->_sz; i++)
  {
    printf("threa2: NUM = %d\n", NUM);
    pthread_mutex_lock(&mut);
    //任务：
    printf("哈哈哈哈哈\n\n\n\n");
    matchingFile_str(s->_count[i]._file);
    NUM++;
    pthread_mutex_unlock(&mut);
  }
  printf("thread2: 主线程还在等我完成任务吗？？？？？\n");
  pthread_exit(NULL);
}
#endif







void thread_create(void)
{
  int temp;
  memset(&thread, 0, sizeof(thread));
  Con P;
  if((temp = pthread_create(&thread[0], NULL, thread1, &P)) != 0)    //&P
     printf("线程1创建失败\n");
  else 
    printf("线程1创建\n");
   
  if((temp = pthread_create(&thread[1], NULL, thread2, &P)) != 0)
     printf("线程2创建失败\n");
  else 
    printf("线程2创建\n");
}



void thread_wait(void)
{
  if(thread[0] != 0)
  {
    pthread_join(thread[0], NULL);
    printf("线程1结束\n");
  }
  if(thread[1] != 0)
  {
    pthread_join(thread[1], NULL);
    printf("线程2结束\n");
  }
}



void pthread_api()
{
  //默认属性初始化互斥锁
  pthread_mutex_init(&mut, NULL);
  printf("wo是主函数,可以创建线程\n");
  thread_create();
  printf("wo主函数等待线程完成任务\n");
  thread_wait();
  return;
}








size_t ID = 0;
//size_t s = 0;
int readFileList(char *bass_path, pCon _con)           //目录读取函数
{
  printf("\n分析[%s]路径下的所有日志 ...\n\n", filePath);
  DIR *dir = NULL;
  struct dirent *ptr;
  char bass[1000];
  
  //printf("opendir函数执行\n ");
  if((dir = opendir(bass_path)) == NULL){
    printf("%s\n ", dir);
    perror("open dir error ");
    exit(1);
  }
  //else  
  //int ID = 0;
  while( NULL != (ptr = readdir(dir)))
  {
    if((strcmp(ptr->d_name, ".") == 0) || 
        (strcmp(ptr->d_name, "..") == 0))
      continue;
8
    else if(ptr->d_type == 8) //文件
    {
      char s[32] = "./File/";
      strcat(s, ptr->d_name);
      
     // printf("%d： %s      \n ", _con->_sz, s);

      if(_con->_sz <= MAX_FILE)    //还有空间
      {
        strcat(s, "\0");
        char *ps = s;
        _con->_count[_con->_sz]._file = ps;
        _con->_count[_con->_sz]._id = ID;
        ID++;
        printf("ID : %-5d   FILE: %s\n", _con->_sz, 
            _con->_count[_con->_sz]._file);
        _con->_sz ++;
      }
      else 
        printf(" 达到MAX_FILE上限\n ") 
      //s = _con->_sz;

      //matchingFile_str(s);      //匹配字符串
      //并发匹配,采用多线程
      
      //开线程，利用多线程读文件匹配信息
      pthread_api();
      
    }

    else if(ptr->d_type == 10) //链接文件
      continue;

    else if(ptr->d_type == 4)  //目录存在是特殊情况
    {
      memset(bass, '\0', sizeof(bass));
      strcpy(bass, bass_path);
      strcat(bass, ptr->d_name);
      readFileList(bass, _con);
    } 
  }

  closedir(dir);
  return 1;
}









//以当前时间为名字创建文件
FILE *CreateNewInfFile(char *str)     //str是当前所分析的文件名字
{
  time_t tt;
  struct tm *newtime;
  char outfile[128];
  int i = 0;
  char *new_buffer = DestPath;
  //nwe_buf is: /home/hjh/HTTP_FILE/Dest
  tt = time(NULL);
  strftime( outfile, 128, "inf_%Y-%m-%d_%I:%M:%S_", localtime(&tt) );
  strcat(outfile, str);
  //新的日志名字保证唯一性，并能根据日志名字找到来源
  while(outfile[i] != '\0')
  {
    if(outfile[i] == '.' || outfile[i] == '/')
    {
      outfile[i] = '_';
    }
    i++;
  }
  
  strcat(outfile, ".log");
  char dest_file_path[128] = DestPath;
  strcat(dest_file_path, outfile);
  printf("\n新增日志: %s \n", dest_file_path);
  
  FILE *fpp = fopen(dest_file_path, "w+");
  if(NULL == fpp){
    perror("fopen error");
    exit(-1);
  }
  //printf("\n");
  return fpp;
}








//在一行中获取目标串出现的次数
int get_string_count(char *str, const char *dest)
{
  //line ++;
  char *p = str;
  int time = 0;
  while(p = strstr(p, dest))
  {
    p = p + strlen(dest);
    time++;
  }
  return time;
}








int matchingFile_str(char *str)
{
  printf("终于可以分析文件了\n");
  //num_C ++;
  char *find_str = file_find_str;
  FILE *fp;
  char file_tmp_str[2048];
  int line = 1;
  time_t t;
  char *buffer;
  int string_count = 0;
  int tail_count = 0;
  int new_log_name_count = 0;
  char new_name[64];

  fp = fopen(str, "r");
  if(NULL == fp)
  {
    printf("     str: %s   fp:%s : open file error\n", str, fp);
    return -1;
  }

  t = time(NULL);
  buffer = asctime(localtime(&t));
  
  //以当前时间创建文件,将描述符拿过来
  FILE *new_file_out_inf = CreateNewInfFile(str);       //str是待分析日志
  printf("  %s  ", new_file_out_inf);

  while(fgets(file_tmp_str, sizeof(file_tmp_str), fp))//按行缓冲方式检查文件每一行
  {
    line ++;
    string_count =  get_string_count(file_tmp_str, find_str);    //在一行中获取指定字符串出现次数
    tail_count += string_count;
    if(string_count != 0)
    {
      //printf("行号:%-5d", line);
      //printf("输出所在行: %s\n", file_tmp_str);

      fprintf(new_file_out_inf, "行号：%-6d", line);
      fprintf(new_file_out_inf, "输出所在行：%s", file_tmp_str);
    }


  }
    
  if(0 != tail_count)
  {
    //每次时间更新
	//创建新的文件，返回句柄信息，并将相应信息写入对应�文件中
    //printf("时间: %s", buffer);
    //printf("日志名: %s\n", str);
    //printf("[%s]出现次数: %d\n",file_find_str, tail_count);
    //printf("****************************************************\n\n");

    fprintf(new_file_out_inf, "时间：%s", buffer);
    fprintf(new_file_out_inf, "日志名：%s\n", str);
    fprintf(new_file_out_inf, "[%s]串在原日志中出现次数：%d\n", file_find_str, tail_count);
    fprintf(new_file_out_inf, "***************************************\n\n");
    
    if(fclose(new_file_out_inf))
    {
      printf("fclose new_file_out_inf ERROR\n");
    }
  }
  //printf("PATH: %s \n", str);
  //printf("Count: %d\n\n", tail_count);
  //fclose(new_file_out_inf);
  fclose(fp);
  return -1;
}












pCon my_con;
int main()
{
   //Con my_con;
   Init_Struct_Node(my_con);
   char *bassPath = filePath;
   //解析目录，将文件编号，以及编号对应的文件名存在结构体中
   printf("解析 ./File 目录，并打印测试\n");
   readFileList(bassPath, my_con);
   
   //printf("解析完成\n");
   //printf("打印结构体测试\n");
   //test_Printf(&my_con);
   printf("任务完成，退出函数\n");
   return 0;
}








