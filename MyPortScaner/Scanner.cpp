#include <iostream>
#include "Scanner.h"


CScanner::CScanner()
{
	WSADATA data = { 0 };
	int iRet = 0;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) 
	{
		std::cout << "WSA��ʼ��ʧ�ܣ�" << std::endl;
		return;
	}
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


CScanner::~CScanner()
{
}

void CScanner::Addip(char * szIP)
{
	if (strstr(szIP, "-")) 
	{
		char *szStartIP = szIP;
		char *szEndIP = szIP;
		DWORD dLocaltion = 0;
		for (unsigned int i = 0; i < strlen(szIP); i++)
		{
			if (szIP[i] == '-') 
			{
				dLocaltion++;
				szEndIP = szIP + i + 1;
			}
		}

		for (unsigned int i = 0; i < strlen(szIP); i++)
		{
			if (szIP[i] == '-')
			{
				szIP[i] = '\0';
				szStartIP = szIP;
			}
		}

		if (dLocaltion != 1)
		{
			std::cout << "IP�ηǷ�������������IP��IP�Σ�" << std::endl;
			return;
		}

		//����IP��ַ�ַ�����IN_ADDR��ʽ
		DWORD start = htonl(inet_addr(szStartIP));
		DWORD end = htonl(inet_addr(szEndIP));

		if (end <= start) 
		{
			return;
		}
		for (DWORD i = start; i <= end; i++) 
		{
			Addip(ntohl(i));
		}
		return ;
	}
	else
	{
		return Addip(inet_addr(szIP));
	}
}

void CScanner::Addip(ULONG addr)
{
	if (addr == -1)return;
	ScanUnit * tmp = new ScanUnit();
	tmp->addr = (ULONG)addr;
	tmp->openPortList.clear();
	tmp->scanPortList.clear();
	tmp->ulScanning = false;
	m_list.push_back(tmp);
}



void CScanner::AddPort(char* szPort)
{
	if(strstr(szPort, "-"))
	{
		char* szStartPort = szPort;
		char* szEndPort = szPort;
		DWORD dLocaltion = 0;
		//����ָ���ȡ����IP
		for (unsigned int i = 0; i < strlen(szPort); i++)
		{
			if (szPort[i] == '-')
			{
				dLocaltion++;
				szEndPort = szPort + i + 1;
			}
		}
		//����-���û��õ���ʼIP
		for (unsigned int i = 0; i < strlen(szPort); i++)
		{
			if (szPort[i] == '-')
			{
				szPort[i] = '\0';
				szStartPort = szPort;
			}
		}
		if (dLocaltion != 1)
		{
			std::cout << "�˿ڻ�˿ڶηǷ�������������˿ڻ�Σ�" << std::endl;
			return;
		}

		DWORD dstart = atoi(szStartPort);
		DWORD dend = atoi(szEndPort);

		if (dend <= dstart)
		{
			return;
		}
		for (DWORD i = dstart; i <= dend; i++)
		{
			AddPort(i);
		}
		return;
	}
	else
	{
		return AddPort(atoi(szPort));
	}
}

void CScanner::AddPort(int nPort)
{
	UnitIter iter = m_list.begin();
	for (; iter != m_list.end(); iter++)
	{
		pScanUnit tmp = *iter;
		tmp->scanPortList.push_back(nPort);
	}
}

bool CScanner::Start()
{
	for (int i = 0; i < 20; i++)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ScanProc, this, 0, 0);
	Sleep(10);
	return false;
}

void CScanner::PrintfResult()
{
	UnitIter iter = m_list.begin();
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	for (; iter != m_list.end(); iter++) {
		pScanUnit tmp = *iter;
		char * szIp = inet_ntoa(*(IN_ADDR *)&tmp->addr);
		list<unsigned int>::iterator portIter = tmp->openPortList.begin();
		for (; portIter != tmp->openPortList.end(); portIter++) {
			//std::cout << szIp << ":" << (int)*portIter << std::endl;
		}
	}
}

DWORD WINAPI CScanner::ScanProc(LPARAM LParam)
{
	CScanner * pThis = (CScanner *)LParam;
	UnitIter iter = pThis->m_list.begin();
	SOCKET sTest = INVALID_SOCKET;
	sockaddr_in addrTest = { 0 };

	for (; iter != pThis->m_list.end(); iter++) {
		pScanUnit tmp = *iter;

		if (InterlockedExchange(&tmp->ulScanning,TRUE) == TRUE)
			continue;

		char * szIP = inet_ntoa(*(IN_ADDR *)&tmp->addr);
		list<unsigned int>::iterator portIter = tmp->scanPortList.begin();
		for (; portIter != tmp->scanPortList.end(); portIter++) 
		{
			unsigned int nPort = *portIter;
			//std::cout << "��ʼɨ��" << szIP << "�Ķ˿�" << nPort << std::endl;
			sTest = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sTest < 0) {
				WSACleanup();
				printf("�������\n");
				return -1;
			}
			addrTest.sin_port = htons(nPort);
			addrTest.sin_family = AF_INET;
			addrTest.sin_addr.S_un.S_addr = tmp->addr;

			//��SOCKET����Ϊ������״̬
//  			unsigned long ul = 1;
//  			DWORD ret = ioctlsocket(sTest, FIONBIO, (unsigned long *)&ul);
// 			if (ret = SOCKET_ERROR) { 
// 				printf("SOCKET_ERROR\n");
// 				return 0; 
// 			}

			unsigned long ul = 1;
			DWORD ret = ioctlsocket(sTest, FIONBIO, (unsigned long *)&ul);
			if (ret == SOCKET_ERROR) {
				printf("SOCKET_ERROR\n");
				return 0;
			}

			DWORD nRet = connect(sTest, (sockaddr *)&addrTest, sizeof(sockaddr));

 			struct timeval timeout;
 			fd_set fd;
 			FD_ZERO(&fd);
// 
 			FD_SET(sTest, &fd);
 			timeout.tv_sec = 0;
 			timeout.tv_usec = 20000;
 			nRet = select(1, 0, &fd, 0, &timeout);
			if (nRet<=0) {
				//std::cout << szIP << "�Ķ˿�" <<  nPort << "û�п���" << std::endl;
			}
			else {
				SetConsoleTextAttribute(pThis->hConsole, FOREGROUND_RED);
				//std::cout << szIP << ":" << nPort << "����" <<std::endl;
				printf("%s:%d ����\n", szIP, nPort);
				tmp->openPortList.push_back(nPort);
				SetConsoleTextAttribute(pThis->hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}


// 			if (nRet == SOCKET_ERROR) {
// 				printf("%s:%d δ����\n", szIP, nPort);
// 			}
// 			else {
// 				printf("%s:%d ����\n", szIP, nPort);
// 			}
			closesocket(sTest);
		}
	}
	return 0;
}
