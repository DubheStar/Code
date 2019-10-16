#include "GetLocalIP.h"

WCHAR* szHostName;
char addrbuff[INET6_ADDRSTRLEN];
WCHAR szAddrBuff[INET6_ADDRSTRLEN];

WCHAR* get_local_ip(const WCHAR* szHostName)   
{
	//��ʼ��:�������ʼ�������´��뽫�޷�ִ��
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		std::cout << "��ʼ������,�޷���ȡ������Ϣ..." << std::endl;
	}
	size_t ret;
	char* mbszHostName = new char[MAX_COMPUTERNAME_LENGTH] { 0 };
	wcstombs_s(&ret, mbszHostName, (size_t)MAX_COMPUTERNAME_LENGTH, szHostName, _TRUNCATE);
	struct hostent* hostinfo = gethostbyname(mbszHostName);
	if (hostinfo == 0)
	{
		std::cout << "�޷���ȡ�������������IP..." << std::endl;
	}
	else
	{
		char** paddrlist = hostinfo->h_addr_list;
		while (*paddrlist !=NULL )
		{
			char addrbuff[INET6_ADDRSTRLEN];
			if (inet_ntop(hostinfo->h_addrtype,*paddrlist,addrbuff,hostinfo->h_addrtype == AF_INET ? INET_ADDRSTRLEN :INET6_ADDRSTRLEN))
			{
				if (strncmp(addrbuff, "192.168.", 8) == 0)
				{
					std::cout << addrbuff << "|";
				}
				else if (strncmp(addrbuff, "172.16.", 7) == 0)
				{
					std::cout << addrbuff << "|";
				}
				else if (strncmp(addrbuff, "10.", 3) == 0)
				{
					std::cout << addrbuff << "|";
				}
			}
			paddrlist++;
		}

	}
 	WSACleanup();
	return 0;
} 