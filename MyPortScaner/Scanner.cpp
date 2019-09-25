#include <iostream>
#include "Scanner.h"


CScanner::CScanner()
{
	WSADATA data = { 0 };
	int iRet = 0;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) 
	{
		std::cout << "WSA初始化失败！" << std::endl;
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
			std::cout << "IP段非法，请重新输入IP或IP段！" << std::endl;
			return;
		}

		//解析IP地址字符串成IN_ADDR格式
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
		//利用指针截取结束IP
		for (unsigned int i = 0; i < strlen(szPort); i++)
		{
			if (szPort[i] == '-')
			{
				dLocaltion++;
				szEndPort = szPort + i + 1;
			}
		}
		//将“-”置换得到起始IP
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
			std::cout << "端口或端口段非法，请重新输入端口或段！" << std::endl;
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
			//std::cout << "开始扫描" << szIP << "的端口" << nPort << std::endl;
			sTest = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sTest < 0) {
				WSACleanup();
				printf("网络出错\n");
				return -1;
			}
			addrTest.sin_port = htons(nPort);
			addrTest.sin_family = AF_INET;
			addrTest.sin_addr.S_un.S_addr = tmp->addr;

			//把SOCKET设置为非阻塞状态
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
				//std::cout << szIP << "的端口" <<  nPort << "没有开放" << std::endl;
			}
			else {
				SetConsoleTextAttribute(pThis->hConsole, FOREGROUND_RED);
				//std::cout << szIP << ":" << nPort << "开放" <<std::endl;
				printf("%s:%d 开放\n", szIP, nPort);
				tmp->openPortList.push_back(nPort);
				SetConsoleTextAttribute(pThis->hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}


// 			if (nRet == SOCKET_ERROR) {
// 				printf("%s:%d 未开放\n", szIP, nPort);
// 			}
// 			else {
// 				printf("%s:%d 开放\n", szIP, nPort);
// 			}
			closesocket(sTest);
		}
	}
	return 0;
}
