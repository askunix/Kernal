int ReadLine(int sock, char buf[], ssize_t size)
{
	//一次读一行，将数据读到buf缓冲区
	
	//1、从socket中读，一次读一个字符，读到换行（\n,\r,\r\n）就返回
	
	char c = '\0';
	ssize_t i = 0;      //当前读了多少个字符了
	while(i<size-1 && c!='\n')
	{
		//结束条件：超过缓冲区长度，读到换行
		//还要兼容\r\n
		ssize_t read_size = recv(sock, &c, 1, 0);
		if(read_size < 0)
			return -1;
		
		if(read_size == 0)
			return -1;     //为了读换行，结果读到了EOF，所以失败
		
		if(c == '\r')
		{
			//遇到\r，还要确定下一个是不是'\n'
			recv(sock, &c, 1, MSG_PEEK); 
			//从缓冲区读后，读完一个字符，不会从缓冲区删除这个字符
			if(c == '\n')
			{
				//此时就是\r\n作为换行的
				recv(sock, &c, 1, 0);
			}
			else
			{
				//确定了是\r,就将\r转化成\n
				c = '\n';
			}
		}
		//把\r,\r\n统一转化成\n
		buf[i++] = c;
	}
	buf[i] = '\0';
	return i;
}
			
	
	
#define SIZE 1024
typedef struct Request
{
	char first_line[SIZE];
	char *method;
	char *url;
	char *url_path;
	char *query_string;
	int content_length;
}Request;


httpserver:httpserver.c
	gcc $^ -o $@ -lpthread
	
.PHONY:clear
clean:
	rm httpserver
	
	
	

	
	
while [ 1 ]
do
ps -ef | grep ./httpserver 0 9090 | grep -v grep
if [ $? -ne 0 ]
then
	echo "go,开始执行！！"
./httpserver 0 9090
else
	echo "runing!!"
fi
	sleep 2000
done





void HttpServerStart(const char *ip, short port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0)
	{
		perror("socket");
		return;
	}
	
	int opt = 1;
	setsocketopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	int ret = bind(listen_sock, (sockaddr *)&addr, sizeof(addr));
	if(ret < 0)
	{
		printf("Binf Failed.\n"):
		perror("bind"):
		return;
	}
	
	ret = listern(listen_sock, 5);
	if(ret < 0)
	{
		perror("listen");
		return;
	}
	printf("Serverlint OK!!\n");
	while(1)
	{
		sockaddr_in peer;
		socklen_t len = sizeof(peer);
		int64_t new_sock = accept(listen_sock, (sockaddr *)&peer, &len);
		if(new_sock < 0)
		{
			perror("accept");
			return;
		}
		
		//使用多线程实现TCP服务器
		pthread_t tid;
		pthread_create(&tid, NULL, ThreadEntry, (void *)new_sock);
		pthred_detach(tid);
	}
}

void *ThreadEntry(void *arg)
{
	int64_t new_sock = (int64_t)grg;
	//此处使用HandlenRequest函数完成具体的处理请求过程
	//解耦合
	HandlenRequest(new_sock);
	return NULL;
}

void HandlenRequest(int new_sock)
{
	int err_code = 200;
	//反序列化，把字符串解析成request对象
	Request req;
	memset(&req, 0, sizeof(req));
	//从socket读取首行
	if(ReadLine(new_sock, req.first_line, sizeof(req.first_line))<0)
	{
		err_code = 404;
		goto END;
	}
	
	//解析首行
	if(ParseFirstLine(req.first_line, &req.url, &req.method))
	{
		err_code = 404;
		goto END;
	}
	
	//解析URL
	if(ParseQueryString(req.url, &req.url_path, &req.query_string))
	{
		err_code = 404;
		goto END;
	}
	
	//处理Header,只关心content-length
	if(ParseHeader(new_sock, &req.content_length))
	{
		err_code = 404;
		goto END;
	}
	
	//打印日志
	printf("method:%s\n", req.method);
	printf("url_path:&s\n", req.url_path);
	printf("query_string:%s\n", req.query_string);
	printf("content_length:%d\n", req.content_length);
	printf("\n");
	
	//静态请求、动态请求
	
	if(strcasecmp(req.method, "GET") == 0 && req.query_string == NULL)
	{
		//静态页面
		err_code = HandlerStaticFile(new_sock, &req);
	}
	
	else if(strcasecmp(req.method, "GET") == 0 && req.content_length)
	{
		err_code = HandlerCGI(new_sock, &req);
	}
	
	else if(strcasecmp(req.method, "POST") == 0)
	{
		err_code = HandlerCGI(new_sock, &req);
	}
	
	else
	{
		err_code = 404;
		goto END;
	}
	
	END:
	if(err_code != 200)
	{
		Handler404(new_sock);
	}
	close(new_sock);
	//如果服务器先断开，会出现大量time_wait
}




void Handler404(int sock)
{
	//构造完整的HTTP响应
	const char *type_line = "Content-Type:test/html;charset=utf-8\n";
	const char *first_line = "HTTP/1.1 404 Not Found\n";
	const char *blank_line = "\n";
	const char *html = "
	<head><meta http-equiv=\"content-type\"content=\"text/html;charset=utf-8\"><\head>"
	"<h2>你的页面找不到了</h2>";
	
	send(sock, first_line, strlen(first_len), 0);
	send(sock, type_line, strlen(type_line), 0);
	send(sock, blank_line, strlen(blank_line), 0);
	send(sock, html, strlen(html), 0);
	return;
}




int  Split(char input[], char *split_char, char *output[], int output_size)
{
	char *pch;
	int i = 0;
	char *tmp = NULL;
	pch = strtok_r(intput, split_char, &tmp);
	while(pch != NULL)
	{
		if(i >= output_size)
		{
			return i;
		}
		output[i++] = pch;
		pch = strtok_r(NULL, split_char, &tmp);
	}
	return i;
}



int ParseFirstLine(char first_line[], char **p_url, char **p_method)
{
	char *tok[10];
	int tok_size = Split(first_line, " ", tok, 10);
	if(tok_size != 3)
	{
		printf("Split failed!!\n");
		return -1;
	}
	*p_method = tok[0];
	*p_url = tok[1];
	return 0;
}


int HandlerCGI(int new_sock, Request *req)
{
	int err_code = 200;
	
	int fd1[2];
	int fd2[2];
	int ret = pipe(fd1);
	if(ret < 0)
		return 404;
	ret = pipe(fd2);
	if(ret < 0)
	{
		close(fd1[0]);
		close(fd1[1]);
		return 404;
	}
	
	int father_read = fd1[0];
	int child_write = fd1[1];
	int father_write = fd2[0];
	int father_read = fd2[0];
	
	ret = fork();
	
	if(ret > 0 )
	{
		//父进程
		close(child_read);
		close(chile_write);
		err_code = HanderCGIFather(new_sock, father_read, father_write, ret, req)
	}
	
	else if
	{
		//子进程
		close(child_write);
		close(child_read);
		err_code = HandlerCGIChild(child_read, child_write, req);
	}
	
	else
	{
		perror("fork");
		err_code = 404;
		goto END;
	}
	END:
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
}




int HandlerCGIFather(int new_sock, int father_read, int father_read,
     int father_write, int child_pid, Request *req)
	 {
		 //把请求部分的内容写到管道,POST就是body部分
		 if(strcasecmp(req->method, "POST") == 0)
		 {
			 int i = 0;
			 char c = '\0';
			 
			 for(; i< req->content_length; ++i)
			 {
				 read(new_sock, &c, 1);
				 write(father_write, &c, 1);
			 }
		 }
		 
		 //构造HTTP响应
		 const char *first_line = "HTTP/1.1 200 OK\n";
		 send(new_sock, first_line, strlen(first_line), 0);
		 
		 const char *type_line = "Content-Type:text/html;charset=utf-8\n";
		 send(new_sock, type_line, strlen(type_line), 0);
		 
		 const chat *blank_line = "\n";
		 send(new_sock, blank_line, strlen(blank_line), 0);
		 
		 //循环从管道读取数据并写入到socket
		 char *c = '\0';
		 while(read(father_read, &c, 1) > 0)
		 {
			 send(new_sock, &c, 1, 0);
		 }
		 //read<=0，读到文件结束
		 //对于管道，写端全部关闭，再继续读，read返回0
		 //对于管道，读端全部关闭，再继续写，产生GIGPIPE信号
		 
		 //子进程回收
		 //wait是无差别等待，等待任意子进程,所以不能用
		 waitpid(child_pid, NULL, 0);
		 return 200;
	 }

	 
	 
	 int HanaderCGIChild(int child_read, int child_write, Request *req)
	 {
		 char method_env[SIZE] = {0};
		 sprintf(method_env, "REQUEST_METHOD=%s", req->method);
		 putenv(method_env);
		 
		 if(strcasecmp(req->method, "GET") == 0)
		 {
			 char query_string_env[SIZE]  = {0};
			 sprintf(query_string_env, "QUERY_STRING=%s", req->query_string);
			 putenv(query_string_env);
		 }
		 
		 else
		 {
			 cahr content_length_env[SIZE] = {0};
			 sprintf(content_length_env, "CONTENT_LENGTH+%d", req->content_length);
			 putenv(content_length_env);
		 }
		 
		 //把标准输入、标准输出重定向到管道中
		 dup2(child_read, 0);
		 dup2(child_write, 1);
		 
		 //对子进程进行程序替换
		 char file_path[SIZE] = {0};
		 HandlerFilePath(req->url_path, file_path);
		 
		 execl(file_path, file_path, NULL);
		 exit(1);
		 
		 return 200;
	 }

	 


