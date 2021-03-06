##### 1、HTTP协议与TCP协议的关系？？

HTTP的长连接和短连接实质上是TCP的长连接和短连接。HTTP属于应用层协议，在传输层使用TCP协议，在网络层使用IP协议。

IP层：解决网络路由和寻址问题
TCP协议：解决如何在IP层之上可靠的传输数据报，并在另一端收到发送的包，并且顺序与发出顺序一致。TCP有可靠，面向连接特点。

--------------

###### 2、如何理解HTTP协议是无状态的？？

无状态指的是协议对事务处理没有记忆能力，服务器不知道客户端是什么状态。也就是说，打开一个服务器网页和你之前打开它之间没有任何联系。

HTTP是一个无状态的面向连接的协议，无状态不代表HTTP不能保持TCP连接，更不能代表HTTP使用的是UDP协议（无连接协议）。

--------------

##### 3、什么是长连接、短连接？？

在HTTP/1.0 中，默认使用短连接。
就是说：浏览器和服务器每进行一次HTTP操作，就建立一次连接，但任务结束就中断连接。

如果客户端浏览器访问某个HTML或者其他类型的Web页面中包含其他Web资源的这种情况，
如：JavaScript文件、图像文件、CSS文件等，浏览器遇到这样的Web资源，就会建立一个会话。

从 HTTP/1.1 起，默认使用长连接，保持连接持续性。
一个网页打开完成后，客户端和服务器之间用于传输HTTP数据的TCP连接不会关闭，如果客户端再次访问这个服务器上的的网页，
会继续使用这一条已经建立好的连接。

Keep-Alive不会永久保持连接，它有一个保持的时间，可以在不同服务器软件中设定这个时间（Apache）。
要实现长连接，客户端、服务器端首先得支持。

HTTP协议长连接、短连接实质上是TCP协议的长连接、短连接。


##### TCP短连接   

模拟一下TCP短连接的情况，Client向server发起连接请求，server接到请求，然后双方建立连接，Client向server发送消息，
server回应client，然后一次读写就完成了，这是双方任意一放都可以发起close请求。

一般都是client发起close请求。


短连接优点：
1、管理起来简单，存在的连接都是有用的，不需要额外的控制手段。



##### TCP长连接


模拟TCP长连接情况：client向server发送连接请求，server接受client连接，双方建立连接，client与server完成一次读写之后，
他们并不会主动关闭，后续的读写操作会继续使用这个连接。


首先说一下TCP/IP上说到的TCP保活功能，保活功能主要为服务器应用提供，服务器应用希望知道客户端主机是否崩溃，
从而可以代表客户使用资源。如果客户已经消失，使得服务器上保留一半开放的连接，而服务器又在等待来自客户端的数据，
保活功能就是试图在服务器端检测这种半开放的连接。


如果给定的连接在两个小时内没有任何的动作，则服务器就向客户端发送一个探测报文段，

客户端必须处于以下四种转态。

1、客户端主机依然正常运行，并从服务器可达，客户的TCP响应正常，而服务器也知道对方是正常的，服务器在两小时之后将保活计时器复位。

2、客户端已经崩溃，并且关闭或者正在重启。在任何一种情况下，客户端的TCP都没有响应，那么服务器端也就不能收到探测响应。
并在75秒后超时，服务器共发送10个这样的探测，每个间隔75秒，如果客户端没有收到一个响应，就认为客户端主机已经关闭并终止连接。

3、客户端主机已经崩溃，但是已经重新启动。服务器将收到一个对其保活探测的响应，这个响应是一个复位，使得服务器终止这个连接。

4、客户端正常运行，但服务器不可达，类似2.TCP能发现的就是没有收到的响应。


##### 短连接步骤：

建立连接--数据传输--关闭连接  -------

##### 长连接步骤：

建立连接--数据传输--（保持连接） ----数据传输----数据传输------关闭连接--------------

--------------

##### 4、长连接和短连接的优缺点？？

由上可知，长连接省去了较多的TCP建立、关闭操作，减少了浪费，节约时间。对于频繁请求资源的客户来说，适合用长连接。
不过保活功能探测周期比较长，而且只能探测TCP连接的存活，遇到恶意连接，保活功能就有点不够用了。

在长连接的应用场景下，client端一般不会主动关闭它们之间的连接，Client与server之间的连接如果一直不关闭的话，会存在一个问题，随着客户端连接越来越多，server早晚有扛不住的时候，这时候server端需要采取一些策略，如关闭一些长时间没有读写事件发生的连接，这样可 以避免一些恶意连接导致server端服务受损；如果条件再允许就可以以客户端机器为颗粒度，限制每个客户端的最大长连接数，这样可以完全避免某个蛋疼的客户端连累后端服务。

短连接对于服务器来说管理较为简单，存在的连接都是有用的连接，不需要额外的控制手段。但如果客户请求频繁，将在TCP的建立和关闭操作上浪费时间和带宽。

长连接和短连接的产生在于client和server采取的关闭策略，具体的应用场景采用具体的策略，没有十全十美的选择，只有合适的选择。

--------------

##### 5. 什么时候用长连接，短连接？

长连接多用于操作频繁，点对点的通讯，而且连接数不能太多情况，每个TCP连接都需要三步握手，这需要时间，如果每个操作都是先连接，再操作的话那么处理速度会降低很多，所以每个操作完后都不断开，次处理时直接发送数据包就OK了，不用建立TCP连接。

例如：数据库的连接用长连接， 如果用短连接频繁的通信会造成socket错误，而且频繁的socket 创建也是对资源的浪费。

而像WEB网站的http服务一般都用短链接，因为长连接对于服务端来说会耗费一定的资源，而像WEB网站这么频繁的成千上万甚至上亿客户端的连接用短连接会更省一些资源。

如果用长连接而且同时有成千上万的用户，如果每个用户都占用一个连接的话，那可想而知吧。所以并发量大，但每个用户无需频繁操作情况下需用短连好。
