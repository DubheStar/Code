#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <string>


#include "GetHostName.h"
#include "GetSystemVer.h"

#pragma comment(lib,"ws2_32.lib")

int get_local_ip(int& ip)
{
	char hostname[128];
	int ret = gethostbyname(hostname);
	if (ret == -1) {
		return -1;
	}
	struct hostent* hent;
	hent = gethostbyname(hostname);
	if (NULL == hent) {
		return -1;
	}
	//直接取h_addr_list列表中的第一个地址h_addr
	ip = ntohl(((struct in_addr*)hent->h_addr)->s_addr);
	//int i;
	//for(i=0; hent->h_addr_list[i]; i++) {
	//    uint32_t u = ntohl(((struct in_addr*)hent->h_addr_list[i])->s_addr);
	//    std::cout << u << std::endl;
	//}
	return 0;
}


int _tmain()  
{
	DWORD OsVersion =  GetSystemVer();
	WCHAR* szHostName = GetHostName();
	std::wstring HostName = szHostName;
	std::wcout << HostName << std::endl;

	int ip;
 	int ret = get_local_ip(ip);
	if (ret == 0) {
		std::cout << ip << std::endl; 
	}
	else {
		std::cerr << "wrong" << std::endl;
	}
	return 0;
}