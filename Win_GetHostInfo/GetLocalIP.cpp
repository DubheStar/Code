#include "GetLocalIP.h"

WCHAR* szHostName;

std::string local_ip[5];
WCHAR* get_local_ip(const WCHAR* szHostName)   
{
	//初始化:如果不初始化，以下代码将无法执行
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		std::cout << "初始化错误,无法获取主机信息..." << std::endl;
	}
	size_t ret;
	char* mbszHostName = new char[MAX_COMPUTERNAME_LENGTH] { 0 };
	wcstombs_s(&ret, mbszHostName, (size_t)MAX_COMPUTERNAME_LENGTH, szHostName, _TRUNCATE);
	struct hostent* hostinfo = gethostbyname(mbszHostName);
	if (hostinfo == 0)
	{
		std::cout << "无法获取计算机主机名及IP..." << std::endl;
	}
	else
	{
		for (int i = 0; hostinfo->h_addr_list[i] != 0; i++)
		{
			//struct in_addr in;
			//memcpy(&in, hostinfo->h_addr, sizeof(struct in_addr));
			local_ip[i] = inet_ntoa(*(struct in_addr*) * hostinfo->h_addr_list[i]);

		}

	}
 	WSACleanup();
	return 0;
} 