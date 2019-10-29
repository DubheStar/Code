#include <windows.h>
#include <stdio.h>
#include <windef.h>

#include "define.h"

int main()
{
	HANDLE hDevice;
	BOOL status;
	HANDLE m_hCommEvent;
	ULONG dwReturn;
	char outbuf[255];
	CHECKLIST CheckList;

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
		printf("CreateFile wrong\n");
		getchar();
		return 0;
	}
	/*
	HANDLE CreateEventA(
	//指向SECURITY_ATTRIBUTES结构的指针。如果此参数为NULL，则子进程不能继承该句柄。
	LPSECURITY_ATTRIBUTES lpEventAttributes, 
	
	//如果此参数为TRUE，则该函数创建一个手动重置事件对象，该对象需要使用 ResetEvent函数将事件状态设置为非信号状态。
	//如果此参数为FALSE，则该函数将创建一个自动重置事件对象，并且在释放单个等待线程之后，系统会自动将事件状态重置为无信号。
	BOOL                  bManualReset, 
	//如果此参数为TRUE，则表示事件对象的初始状态；否则，将通知事件对象。否则，它是无信号的。
	BOOL                  bInitialState,
	//如果lpName为NULL，则创建事件对象时不使用名称。
	LPCSTR                lpName	);
	*/
	m_hCommEvent = CreateEvent(NULL,
		FALSE,
		FALSE,
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

	printf("      [Process Name]    [PID]    [TID]    [Parent Process Name]    [PID]  [TID]\n");
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
		printf("%s", outbuf);
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