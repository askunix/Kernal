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

#define filepath_log "/home/hjh/HTTP_FILE/inf.log"   //将程序分析结果存在inf.log中
#define filepath "/home/hjh/HTTP_FILE/File"          //众多待分析日志所在绝对路径
#define Dest_path "./Dest/"         //创建的信息的日志信息存放目录
char *file_find_str = "error";    //待匹配的字符串

typedef struct FileNode
{
    char *str_name;
    size_t num;
    struct FileNode *next;
}FileNode, *pFileNode, **PPFileNode;

char *file[2048];

/*函数声明*/
void daemon_init(const char *, int);
int readFileList(char *);
int matchingFile_str(char *);
int open_log_file(void);
int get_string_count(char *, const char *);
FILE *CreateNewInfFile(char *);



//定义一个全局的文件，将程序信息输出到文件中
int open_log_file()
{
  FILE *fp_log;
  if((fp_log = fopen(filepath_log, "w+")) == NULL)
  {
    printf("%s open fail\n", filepath_log);
    exit(-1);
  }
}



int main(int argc, char *argv[])
{
  time_t t;
  char *bassPath = filepath;

  //daemon_init(argv[0], 0);    //守护进程
 
  //open_log_file();      //定义一个文件句柄，将分析众多日志所得结果写到文件

  readFileList(bassPath);       //读指定文件目录


  /*
  while(1)
  {
    char *buf;
    
    t = time(NULL);
    buf = asctime(localtime(&t));

    //fprintf(filepath_log, "%s\n", buf);
    printf("%s ", buf);
    sleep(6);    //second
  }

  //fp.close();
  return 0;
  */
}



//线程池函数




//目录读取函数
int readFileList(char *bass_path)
{
  printf("\n正在分析[%s]路径下的所有日志 ...\n\n", filepath);
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
  while( NULL != (ptr = readdir(dir)))
  {
    //printf("[%d]  ", count );
    //count ++;
    if((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
      continue;

    else if(ptr->d_type == 8) //文件
    {
      char s[32] = "./File/";
      strcat(s, ptr->d_name);
      
      //这一步匹配要开始输出相关信息了，在这创建时间命名的文件，返回文件描述符，方便后续信息输出到文件
      matchingFile_str(s);      //匹配字符串
    }
    else if(ptr->d_type == 10) //链接文件
      continue;

    else if(ptr->d_type == 4)  //目录
    {
      memset(bass, '\0', sizeof(bass));
      strcpy(bass, bass_path);
      strcat(bass, ptr->d_name);
      readFileList(bass);
    } 
  }
  closedir(dir);
  return 1;
}

int32_t num_C = 1;
//单个文件中匹配给定的字符串
int matchingFile_str(char *str)
{
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

  fp = fopen(str, "r+");
  if(NULL == fp){
    printf("open file error");
    return -1;
  }

  t = time(NULL);
  buffer = asctime(localtime(&t));
  
  //以当前时间创建文件,将描述符拿过来
  FILE *new_file_out_inf = CreateNewInfFile(str);
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

      fprintf(new_file_out_inf, "行号：%-8d", line);
      fprintf(new_file_out_inf, "输出所在行：%s", file_tmp_str);
    }
  }
  
  
  if(0 != tail_count)
  {
    //每次时间更新
    //创建新的文件，返回句柄信息，并将相应信息写入对应的文件中
    //printf("时间: %s", buffer);
    //printf("日志名: %s\n", str);
    //printf("[%s]出现次数: %d\n",file_find_str, tail_count);
    //printf("****************************************************\n\n");

    fprintf(new_file_out_inf, "时间：%s", buffer);
    fprintf(new_file_out_inf, "日志名：%s\n", str);
    fprintf(new_file_out_inf, "[%s]串在原日志中出现次数：%d\n", file_find_str, tail_count);
    fprintf(new_file_out_inf, "***************************************\n\n");
    
    if(fclose(new_file_out_inf)){
      printf("fclose new_file_out_inf ERROR\n");
    }
  }
  //printf("PATH: %s \n", str);
  //printf("Count: %d\n\n", tail_count);
  //fclose(new_file_out_inf);
  fclose(fp);
  return -1;
}



//以时间创建文件，并将程序输出信息打印进这个文件里








//以当前时间为名字创建文件
FILE *CreateNewInfFile(char *str)     //str是当前所分析的文件名字
{
  time_t tt;
  struct tm *newtime;
  char outfile[128];
  int i = 0;
  char *new_buffer = Dest_path;
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
  char dest_file_path[128] = Dest_path;
  strcat(dest_file_path, outfile);
  printf("\n ++ %s ++\n", dest_file_path);
  
  FILE *fpp = fopen(dest_file_path, "w+");
  if(NULL == fpp){
    perror("fopen error");
    exit(-1);
  }
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



//守护进程
void daemon_init(const char* pname,int facility)
{
  int i;
  pid_t pid;
  struct rlimit rl;
  struct sigaction sa;
	//清除文件模式创建掩码
  umask(0);
  if(getrlimit(RLIMIT_NOFILE,&rl) < 0){
    perror("getrlimit() error");
    exit(-1);
  }
  if((pid = fork()) < 0){
    perror("fork() error");
    exit(-1);
  }
  //printf("父进程终止\n");
  else if(pid > 0){
    exit(0);
  }   /*父进程终止 */
  
  setsid();         /* 子进程成为会话首进程*/
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if(sigaction(SIGHUP,&sa,NULL) < 0)
  {
    perror("sigaction() error");
    exit(-1);
  }
  if((pid = fork()) < 0)
  {
    perror("fork() error");
    exit(-1);
  }
  else if(pid > 0)
    exit(0);        /* 第一个子程进终止,保证后面操作会分配终端 */
    
  if(chdir("/")<0)    /* 改变工作目录 */
  {
    perror("chdir() error");
    exit(-1);
  }
  if(rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = 1024;
    
  for(i = 3; i < (int) rl.rlim_max; ++i)  /*关闭所有打开的文件描述字*/
  {
    close(i);
  }
  openlog(pname, LOG_PID, facility);  /*用syslogd处理错误*/
  //printf("从守护进程出来\n\n");
}


