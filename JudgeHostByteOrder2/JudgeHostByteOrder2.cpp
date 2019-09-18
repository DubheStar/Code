// JudgeHostByteOrder2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <atlstr.h>
int main()
{
	DWORD dwSmallNum = 0x01020304;
	if ( * (BYTE *)&dwSmallNum == 0x04 )
	{
		printf("Small Sequence.\r\n");
	}
	else
	{
		printf("Big Sequence.\r\n");
	}
	return 0;
}