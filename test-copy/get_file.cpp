#include <stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>


void dir_oper(char const*path);

int main(int argc, char const *argv[])
{
  //FILE *file_p;
  FILE *fp = fopen("inf.txt", "w+");
  if(!fp)
  {
    printf("open filepath failed\n");
    return 0; 
  }

	char const* path = argv[1];
	struct stat s_buf;
	stat(path, &s_buf);
	
  if(S_ISDIR(s_buf.st_mode))
	{
		dir_oper(path);
	}
 
	//若输入的路径就是文件
	else if(S_ISREG(s_buf.st_mode))
	{
		printf("[%s] 是普通文件 %d \n",
        path, (int)s_buf.st_size);
   
   fprintf(fp, "[%s]是普通文件  %d \n", 
       path, (int)s_buf.st_size);
	}
	return 0;
}




void dir_oper(char const*path)
{
  FILE *fp = fopen("inf.txt", "w");
  if(!fp)
  {
    printf("filepath open failed\n");
    exit(0);
  }

	printf("%s/ 是目录. \n", path);
  fprintf(fp, "%s/ 是目录. \n", path);

	struct dirent *filename;
	struct stat s_buf;
	DIR *dp = opendir(path);
	//readdir()循环
	while(filename = readdir(dp))
	{
		char file_path[200];
		bzero(file_path,200);

		strcat(file_path,path);
		strcat(file_path,"/");
		strcat(file_path,filename->d_name);
		
		//排除目录下的隐藏文件. ..
		if(strcmp(filename->d_name, ".") == 0 || 
        strcmp(filename->d_name, "..") == 0)
		{
			continue;
		}

		stat(file_path,&s_buf);
		if(S_ISDIR(s_buf.st_mode))
		{
      //printf("进来了， 是目录\n");
			dir_oper(file_path);
			printf("\n");
		}
     
    //char *name = ctime(&s_buf.st_mtim);
    //char mtime[512] = {0};
    //strncpy(mtime, time, strlen(time) - 1);
    
    char *file_user = getpwuid(s_buf.st_uid)->pw_name;

    char power[11] = {0};
    // 文件所有者
    power[0] = '-';
    power[1] = (s_buf.st_mode & S_IRUSR) ? 'r' : '-';
    power[2] = (s_buf.st_mode & S_IWUSR) ? 'w' : '-';
    power[3] = (s_buf.st_mode & S_IXUSR) ? 'x' : '-';
    // 所属组
    power[4] = (s_buf.st_mode & S_IRGRP) ? 'r' : '-';
    power[5] = (s_buf.st_mode & S_IWGRP) ? 'w' : '-';
    power[6] = (s_buf.st_mode & S_IXGRP) ? 'x' : '-';
    // 其他
    power[7] = (s_buf.st_mode & S_IROTH) ? 'r' : '-';
    power[8] = (s_buf.st_mode & S_IWOTH) ? 'w' : '-';
    power[9] = (s_buf.st_mode & S_IXOTH) ? 'x' : '-';


		if(S_ISREG(s_buf.st_mode))
		{
			printf("[%s]是普通文件 %s  %d  %s  \n",
          file_path, power, (int)s_buf.st_size, file_user);
      
      fprintf(fp, "[%s] 是普通文件%s  %d  %s \n", 
          file_path, power, (int)s_buf.st_size, file_user);
		}
	}
}





