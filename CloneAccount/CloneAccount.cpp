#include <iostream>
#include <windows.h>
#include <tchar.h>
using std::cout;

LONG lRet = 1;

int _tmain()
{
	HKEY key;
	LPCTSTR lpSubkey = _T("SAM\\SAM\\Domains\\Account\\Users"); //DEBUG:SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run
	
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubkey, 0, KEY_ALL_ACCESS, &key);
	if (ERROR_SUCCESS != lRet)
	{
		std:cout << "注册表打开失败!\n";
		std::cout<<lRet;
	}
	else
	{
		
	}
	system("pause");
}