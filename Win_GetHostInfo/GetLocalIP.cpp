#include "GetLocalIP.h"

WCHAR* szHostName;

std::string local_ip[5];
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