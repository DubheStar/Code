#include <iostream>
#include <WINSOCK2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSADATA wsaData;//用来存储被 WSAStartup 函数调用后返回的数据
	WSAStartup(0x0202, &wsaData);
	/*
	必要性：WSAStartup必须是Windows Sockets中第一个使用的函数，成功调用后才能进一步使用Windows Sockets API函数
	作用：指明Windows Sockets API的版本号及获得特定Windows Sockets实现的细节
	用法：前参是一个WORD（双字节）型数值，指定了应用程序需要使用的Winsock规范的最高版本。
		后参是WSADATA数据结构的指针，用来接收Windows Sockets实现的细节。
	*/

	//1．创建套接字（SOCKET神奇的翻译)的创建和关闭
	SOCKET listenSocket; //定义套接字
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字
	/*
	作用：创建套接字时返回小整数作为描述符来标识它
	用法：AF表示ADDRESS FAMILY 地址族，SOCK_STREAM表示能保证数据正确传送到对方的SOCKET，IPPROTO_TCP表示TCP协议
	补充：当不使用socket创建的套接字时，应该调用closesocket函数将它关闭(int closesocket(SOCKET s);  )
	*/

	//2．绑定套接字到指定的IP地址和端口号
	SOCKADDR_IN Server_Addr;
	//用于建立listenSocket的本地关联sockaddr_in结构
	Server_Addr.sin_family = AF_INET;
	//与socket函数中的af参数的含义相同，见上
	Server_Addr.sin_port = htons(3721);
	//如果端口号等于0，执行时系统会自动分配唯一的端口号，其值在1024～5000之间。
	Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);//如果地址等于INADDR_ANY，会自动使用当前主机配置的所有IP地址；
	/*也可以是local.S_un.S_addr.s_addr = htonl(INADDR_ANY)；(因为winsock2.h中定义了#define s_addr  S_un.S_addr)
	sin_port字段和sin_addr字段分别指定套接字需要绑定的端口号和IP地址。放入这两个字段的数据的字节顺序必须是网络字节顺序
	（因为网络字节顺序和Intel CPU的字节顺序刚好相反，所以必须首先使用htons函数进行转换）*/
	bind(listenSocket, (struct sockaddr *) &Server_Addr, sizeof(Server_Addr));
	/*
	作用：bind函数用在没有建立连接的套接字上，它的作用是绑定面向连接的或者无连接的套接字。
		套接字被socket函数创建以后，存在于指定的地址家族里，但它是未命名的。
		bind函数通过安排一个本地名称到未命名的socket而建立此socket的本地关联。
	用法：1.SOCKET 2.SOCKADDR_IN	 3.SOCKADDR_IN的长度
	*/

	//3．设置套接字进入监听状态	
	listen(listenSocket, 1);
	std::cout << "开启监听：\n";
	/*
	作用：设置套接字进入监听状态。
	用法：套接字，监听队列中允许保持的尚未处理的最大连接数量
	特性：函数执行成功后，套接字lisenSocket进入了被动模式，到来的连接会被通知要排队等候接受处理。
		在同一时间处理多个连接请求的服务器通常使用listen函数
		如果一个连接请求到达，并且排队已满，客户端将接收到WSAECONNREFUSED错误。
	*/

	//4．接受连接请求
	SOCKET clientSocket;//忘记了就看上面
	SOCKADDR_IN client;//忘记了就看上面
	int addrSize = sizeof(SOCKADDR_IN);
	clientSocket = accept(listenSocket, (struct sockaddr *) &client, &addrSize);
	/*
	作用：取出未处理连接中的第一个连接，然后为这个连接创建新的套接字，返回它
	特性：程序默认工作在阻塞模式下：如果没有未处理的连接存在，就会等待下去，直到有新连接发生来进行处理
	用法：addrlen参数用于指定addr所指空间的大小，也用于返回地址的实际长度。如果是NULL，则没有关于远程地址的信息返回
	*/
	std::cout << "Accepted client:%s:%d\n"//反馈连接信息
		<< (client.sin_addr.S_un.S_addr)//inet_ntoa将一个十进制网络字节序转换为点分十进制IP格式的字符串。
		<< ntohs(client.sin_port);  //ntohs将一个16位数由网络字节顺序转换为主机字节顺序

//5.与客户端通信
	int const CLIENT_MSG_SIZE = 128;//接收缓冲区长度
	char cacheMSG[CLIENT_MSG_SIZE]; //接收缓冲区的基地址
	while (TRUE) {
		int size = recv(clientSocket, cacheMSG, CLIENT_MSG_SIZE, 0);
		/*
		作用：recv()从接收缓冲区拷贝数据。成功时，返回拷贝的字节数，失败返回-1。
		用法：前三个请找上述的参数定义。最后一个参数为0的意义：默认是普通接受数据模式
		特性：阻塞模式下，recv将会阻塞到缓冲区里至少有一个字节，没有数据时处于休眠状态。
			若非阻塞，则立即返回，有数据则返回拷贝的数据大小，否则返回错误-1。
		*/
		cacheMSG[size] = '\0'; //你懂得   
		std::cout << "Received:" << cacheMSG<< std::endl;
	}

//6.结束收尾
	closesocket(listenSocket);
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}
