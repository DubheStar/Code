#include <windows.h>
#include <stdio.h>
#include "define.h"

#define host "127.0.0.1"
#define port 65530
#pragma comment(lib, "ws2_32.lib")
int CapturePrecess()
{
	HANDLE        hDevice;
	int        status;
	HANDLE        m_hCommEvent;
	ULONG        dwReturn;
	char        outbuf[255];
	CHECKLIST    CheckList;
	SOCKET sock;
	WSADATA wsaData;
	struct sockaddr_in saddr;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("error");
	}
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(host);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	hDevice = NULL;
	m_hCommEvent = NULL;
	hDevice = CreateFileA("\\\\.\\MonitorProcess",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("createfile wrong\n");
		getchar();
		return 0;
	}
	m_hCommEvent = CreateEvent(NULL,
		0,
		0,
		NULL);
	printf("hEvent:%08x\n", m_hCommEvent);
	status = DeviceIoControl(hDevice,
		IOCTL_PASSEVENT,
		&m_hCommEvent,
		sizeof(m_hCommEvent),
		NULL,
		0,
		&dwReturn,
		NULL);
	if (!status)
	{
		printf("IO wrong+%d\n", GetLastError());
		getchar();
		return 0;
	}
	CheckList.ONLYSHOWREMOTETHREAD = TRUE;
	CheckList.SHOWTHREAD = TRUE;
	CheckList.SHOWTERMINATETHREAD = FALSE;
	CheckList.SHOWTERMINATEPROCESS = FALSE;
	status = DeviceIoControl(hDevice,
		IOCTL_PASSEVSTRUCT,
		&CheckList,
		sizeof(CheckList),
		NULL,
		0,
		&dwReturn,
		NULL);
	if (!status)
	{
		printf("IO wrong+%d\n", GetLastError());
		getchar();
		return 0;
	}
	while (1)
	{
		ResetEvent(m_hCommEvent);
		WaitForSingleObject(m_hCommEvent, INFINITE);
		status = DeviceIoControl(hDevice,
			IOCTL_PASSBUF,
			NULL,
			0,
			&outbuf,
			sizeof(outbuf),
			&dwReturn,
			NULL);
		if (!status)
		{
			printf("IO wrong+%d\n", GetLastError());
			getchar();
			return 0;
		}
		sendto(sock, outbuf, strlen(outbuf), 0, (struct sockaddr*) & saddr, sizeof(saddr));
	}
	status = DeviceIoControl(hDevice,
		IOCTL_UNPASSEVENT,
		NULL,
		0,
		NULL,
		0,
		&dwReturn,
		NULL);
	if (!status)
	{
		printf("UNPASSEVENT wrong+%d\n", GetLastError());
		getchar();
		return 0;
	}
	status = CloseHandle(hDevice);
	status = CloseHandle(m_hCommEvent);
	getchar();
	return 0;
}

int main(int argc,WCHAR* argv[])
{
	CapturePrecess();
	getchar();
}