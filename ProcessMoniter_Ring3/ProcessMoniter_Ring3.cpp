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
	PVOID outbuf[255];
	CHECKLIST CheckList;

	hDevice = NULL;
	m_hCommEvent = NULL;
	hDevice = CreateFileA("\\\\.\\MonitorProcess",//打开的文件名
		GENERIC_READ | GENERIC_WRITE,//访问权限
		FILE_SHARE_READ | FILE_SHARE_WRITE,//共享模式
		NULL,//安全属性
		OPEN_EXISTING,//文件存在与不存在时的文件创建模式
		FILE_ATTRIBUTE_NORMAL,//文件属性设定（隐藏、只读、压缩、指定为系统文件等）
		NULL);//文件副本句柄
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

	status = DeviceIoControl(hDevice, //驱动的设备对象句柄
		IOCTL_PASSEVENT,//控制码
		&m_hCommEvent, // 发往R0层的数据
		sizeof(m_hCommEvent),//输入缓冲区的大小
		NULL,//提供一个缓冲区，接受R0返回的数据
		0,//输出缓冲区的大小
		&dwReturn,//实际返回的字节数，对应驱动程序中pIrp->IoStatus.Information。
		NULL);//重叠操作结构指针。同步设为NULL，DeviceIoControl将进行阻塞调用；否则，应在编程时按异步操作设计
	if (!status)
	{
		printf("IO wrong+%d\n", GetLastError());
		getchar();
		return 0;
	}

	CheckList.ONLYSHOWREMOTETHREAD = FALSE;
	CheckList.SHOWTHREAD = TRUE;
	CheckList.SHOWEXITPROCESS = TRUE;
	CheckList.SHOWEXITTHREAD = TRUE;
	CheckList.SHOWIMAGE = FALSE;
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

	printf("Moniter Started！\n");
	while (1)
	{
		ResetEvent(m_hCommEvent);
		WaitForSingleObject(m_hCommEvent, INFINITE);
		status = DeviceIoControl(hDevice,//驱动的设备对象句柄
			IOCTL_PASSBUF,//控制码
			NULL,// 发往R0层的数据
			0,//输入缓冲区的大小
			&outbuf,//提供一个缓冲区，接受R0返回的数据
			sizeof(outbuf),//输出缓冲区的大小
			&dwReturn,//实际返回的字节数，对应驱动程序中pIrp->IoStatus.Information。
			NULL);//重叠操作结构指针。同步设为NULL，DeviceIoControl将进行阻塞调用；否则，应在编程时按异步操作设计
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