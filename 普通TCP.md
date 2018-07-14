服务器端
```C++
#include<stdio.h>
#include<sys/socket.h>     //socket API
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>

/*
1、创建socket
2、绑定端口号
3、把socket转换成被动模式（listen）
4、循环进行accept
5、从accept返回的new_fd中读取客户端请求
6、读取到数据后，处理数据
7、把处理后的结果返回给客户端
*/


#define _PORT_ 8080
#define _BACKLOG_ 10


int main()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  //socket（）打开一个网络通讯端口，成功返回一个文件描述符
  //应用程序可以像读写文件一样用read、write读写文件
  //socket调用出错返回-1
  //对于IPV4，family指定为AF_INET
  //对于TCP协议，type为SOCK_STREAM，表示面向数据流的协议
   //protocal参数指定为0即可

  if(sock < 0)   //失败返回-1
  {
     printf("create socket weeor, error is:%d, errstring is :%s\n", errno, strerror(errno));
  }






  struct sockaddr_in server_socket;
  struct sockaddr_in client_socket;
  bzero(&server_socket, sizeof(server_socket));
  server_socket.sin_family = AF_INET;
  server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
  server_socket.sin_port = htons(_PORT_);


  if(bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in) < 0))


  /*
  服务器端的网络地址和端口号一般不变，客户端知道了服务器IP和端口号后就可以向服务器发起连接，服务器需要调用一个bind绑定一个固定的网络IP和端口号

  成功返回0，失败返回-1
  bind（）的作用是将参数sockfd和myaddr绑定在一起，使sockfd这个用于网络通讯的文件描述符监听myaddr所描述的地址和端口号，

  struct sockaddr是一个通用指针类型，myaddr参数实际上可以接受多种协议的sockaddr结构体，
  而他们的长度各不相同，所以需要第三个参数addrlen指定结构体长度

     bzero(&server_socket, sizeof(server_socket));
     server_socket.sin_family = AF_INET;
     server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
     server_socket.sin_port = htons(_PORT_);
1、将整个结构体清零
2、设置地址类型为AF_INET
3、INADDR_ANY表示任意的IP地址，因为服务器可能有多个网卡，每个网卡可能绑定多个IP，这样设置，可以在所有的IP上监听，
   直到与某个客户端建立了连接，才确定到底用的那个IP
4、_PORT_ 端口号
  */


  { //失败处理
    printf("Bind error, error code is :%d, error string is :%s\n", errno, strerror(errno));
    close(sock);
    return 1;
  }




  if(listen(sock, _BACKLOG_) < 0)

  /*
  listen（）声明sock_fd处于监听状态，最多允许有_BACKLOG_个客户端处于连接等待状态，如果接收到更多的连接就忽略，（注意这里设置一般不大，为5）
  listen（）成功返回0，失败返回-1
  */

  {   //处理失败
    printf("listen error\n");
    return 2;
  }

  printf("bind and listen successed!\n");

  for(;;)
  {
    socklen_t len = 0;
    int client_sock = accept(sock, (struct sockaddr *)&client_sock, &len);

/*
三次握手完成后，服务器调用accept接受数据，
如果服务器调用accept()还没有客户端的连接请求，就阻塞等待，直到有客户端连接。
addr是一个传出参数，accept（）返回时传出客户端的地址和端口号。
如果给addr参数传NULL，表示不关心客户端地址。
addrlen是一个传入，传出参数，传入是调用者提供的，缓冲区addr的长度以避免缓冲区溢出问题，传出的是客户端地址结构体的实际长的（有可能没有占满调用者提供的缓冲区）

若成功则为非负描述符，若出错则为-1
如果accept成功，那么其返回值是由内核自动生成的一个全新描述符，代表与客户端的TCP连接。一个服务器通常仅仅创建一个监听套接字，它在该服务器生命周期内一直存在。内核为每个由服务器进程接受的客户端连接创建一个已连接套接字。当服务器完成对某个给定的客户端的服务器时，相应的已连接套接字就被关闭。
*/

    if(client_sock < 0)
    {
       printf("accept error\n");
       close(sock);
       return 3;
    }

    char buf_ip[INET_ADDRSTRLEN];
    memset(buf_ip, '\0', sizeof(buf_ip));
    inet_ntop(AF_INET, &client_socket.sin_addr, buf_ip, sizeof(buf_ip));

    printf("get connect,ip is :%s , port is :%d \n", buf_ip, ntohs(client_socket.sin_port));

    while(1)
    {
      char buf[1024];
      memset(buf, '\0', sizeof(buf));

      read(client_sock, buf, sizeof(buf));
      printf("client :# %s\n", buf);

      printf("server :$ ");

      memset(buf, '\0', sizeof(buf));
      fgets(buf, sizeof(buf), stdin);
      buf[strlen(buf)-1] = '\0';
      write(client_sock, buf, strlen(buf) + 1);
      /*
      创建一个TCPsocket时，会在内核中创建两个缓冲区：发送缓冲区，接收缓冲区
      
      调用write时，数据会先写入发送缓冲区中；
      
      如果发送的字节过长，会被拆分成多个TCP数据包发出
      
      如果发送的字节数过短，就会先在缓冲区里等待，等到缓冲区长度差不多了，或者其他的何时时机再发出
      
      接收数据的时候，数据也是从网卡驱动程序到达内核的缓冲区。
      
      程序可以调用read函数从缓冲区中拿数据；
      
      另一方面，TCP的一个连接，既可以读数据，也可以写数据，这样得概念就是全双工。
      */
      
      
      printf("please wait ……");
    }
  }
}

```






客户端

```C++

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>


/*
1、创建socket
2、和服务器建立连接
3、给服务器发送数据
4、从服务器读取返回的结果
5、和服务器断开连接
*/


//客户端不需要固定的端口号，因此不必调用bind（），客户端的端口号由内核自动分配
#define SERVER_PORT_ 8080
#define SERVER_IP "118.24.7681"

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("Usage : client IP\n");
    return 1;
  }

  char *str = argv[1];
  char buf[1024];
  memset(buf, '\0', sizeof(buf));

  struct sockaddr_in server_sock;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&server_sock, sizeof(server_sock));
  server_sock.sin_family = AF_INET;
  inet_pton(AF_INET, SERVER_IP, &server_sock.sin_addr);
  server_sock.sin_port = htons(SERVER_PORT_);

  int ret = connect(sock, (struct sockaddr *)&server_sock, sizeof(server_sock));
/*
调用connect连接服务器
connect和bind的参数形式一致，区别在于bind参数是自己的地址，connect参数是对方地址。
成功返回0
失败返回-1
*/
  if(ret < 0)
  {
    printf("connect failed !!!\n");
    return 1;
  }
  printf("connect successed ……\n");

  while(1)
  {
    printf("client:# ");
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf) - 1] = '\0';
    write(sock, buf, sizeof(buf));

    if(strncasecmp(buf, "quit", 4) == 0)
    {
      printf("quit!\n");
      break;
    }
    printf("wait ---\n");
    read(sock, buf, sizeof(buf));
    printf("server $: %s", buf);
  }
  close(sock);
  return 0;

}


```
