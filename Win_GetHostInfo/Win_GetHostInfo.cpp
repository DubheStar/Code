#include "GetLocalIP.h"
#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <string>



#include "GetHostName.h"
#include "GetSystemVer.h"




int _tmain()  
{
	DWORD OsVersion =  GetSystemVer();
	const WCHAR* szHostName = GetHostName();
	std::wstring HostName = szHostName;
	std::wcout << HostName << std::endl;
	get_local_ip(szHostName);
 }