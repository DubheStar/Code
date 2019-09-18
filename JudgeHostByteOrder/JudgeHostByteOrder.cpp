// JudgeHostByteOrder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <atlstr.h>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

int main()
{
	DWORD dwSmallNum = 0x01020304;
	if (dwSmallNum == htonl(dwSmallNum))
	{
		printf("Big Sequence.\r\n");
	}
	else
	{
		printf("Small Sequence.\r\n");
	}
	return 0;
}

