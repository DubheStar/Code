#include "GetHostName.h"

WCHAR szHostName[MAX_COMPUTERNAME_LENGTH + 2];

WCHAR* GetHostName()
{
	
	DWORD cbHostName = (sizeof(szHostName)) / 2;
	DWORD lret = GetComputerName(szHostName, &cbHostName);
	return szHostName;
}
