// ConsoleRegWrite.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <tchar.h>

#define TOTALBYTES    8192

LONG lRet = 1;
HKEY hKey;

int ConsoleRegOpen()
{
	LPCTSTR lpSubKey = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	
	REGSAM flag = KEY_WOW64_64KEY;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_ALL_ACCESS, &hKey);
	
	if (ERROR_SUCCESS != lRet)
	{
		std::cout << "RegOpenKeyEx fail!" << lRet << std::endl;
	}
	else
	{
		std::cout << "注册表打开成功" << std::endl;
	}
	return 0;
}

int ConsoleRegWrite()
{
	LPCTSTR programName = _T("D:\\Shadowsocks-4.1.7.1\\Shadowsocks.exe");
	//strlen返回的是字节数（对中英文不一致，中文占两个字节，不包括'/0'）。
	//wcslen返回的是字符数（对中英文一致）。
	//sizeof返回的是字节数（包含'/0'，而'/0'在Unicode下也是占两个字节的）。

	lRet = RegSetValueEx(hKey,_T("SS"),0,REG_SZ,(LPBYTE)programName,wcslen(programName) * sizeof(TCHAR)+1);
	if (ERROR_SUCCESS != lRet)
	{
		std::cout << "RegSetValueEX Fail:" << lRet << std::endl;
	}
	else
	{
		std::cout << "注册表添加成功！" << std::endl;
	}
	return 0;
}

int ConsoleRegQuery()
{
	DWORD Data;
	DWORD DataSize =   TOTALBYTES;

	PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(DataSize);
	lRet = RegQueryValueEx(hKey, _T("SS"), 0, NULL, (LPBYTE)PerfData,&DataSize);
	if (ERROR_SUCCESS != lRet)
	{
		std::cout << "RegQueryValueEX Fail:" << lRet << std::endl;
	}
	else
	{
		std::cout << "查询注册表键值为：" << &PerfData << std::endl;
	}
	return 0;
}

int main()
{
	std::cout << lRet << std::endl;
	ConsoleRegOpen();
	ConsoleRegWrite();
	ConsoleRegQuery();
	system("pause");
	return 0;
}
