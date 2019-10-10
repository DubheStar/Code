#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32.lib")
#pragma comment(lib,"Advapi32.lib")
#include <iostream>
#include <Windows.h>
#include <LM.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <wchar.h>
#include <time.h>
#include <sddl.h>
//隐藏账号检测

char* filetimeToString(FILETIME FileTime, char* str, unsigned int string_size)
{
	str[0] = 0;
	SYSTEMTIME SysTime;
	if (FileTimeToSystemTime(&FileTime, &SysTime) != 0)
		snprintf(str, string_size, "%02d/%02d/%02d %02d:%02d:%02d", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	return str;
}

char* timeToString(DWORD t, char* str, unsigned int string_size)
{
	FILETIME FileTime, LocalFileTime;
	LONGLONG lgTemp = Int32x32To64(t, 10000000) + 116444736000000000;
	FileTime.dwLowDateTime = (DWORD)lgTemp;
	FileTime.dwHighDateTime = (DWORD)(lgTemp >> 32);
	FileTimeToLocalFileTime(&FileTime, &LocalFileTime);
	filetimeToString(LocalFileTime, str, string_size);
	return str;
}

bool user2sid(LPWSTR csUsername, PSID Sid)
{
	UCHAR buffer2[2048];
	UCHAR buffer3[4];
	DWORD length = 900;
	LPCTSTR lpAccountName = csUsername;
	LPDWORD cbSid = &length;
	LPTSTR ReferencedDomainName = (LPTSTR)buffer2;
	LPDWORD cbReferencedDomainName = &length;
	PSID_NAME_USE peUse = (PSID_NAME_USE)buffer3;
	if (LookupAccountName(NULL, lpAccountName, Sid, cbSid,
		ReferencedDomainName, cbReferencedDomainName,
		peUse))
	{
		return 1;
	}
	return 0;
}



int main()
{
	LPUSER_INFO_2 pBuf = NULL;
	LPUSER_INFO_2 pTmpBuf;
	DWORD dwLevel = 2;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	nStatus = NetUserEnum((LPCWSTR)pszServerName,
		dwLevel,
		FILTER_NORMAL_ACCOUNT, // global users
		(LPBYTE*)&pBuf,
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries,
		&dwResumeHandle);

	if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
	{
		if ((pTmpBuf = pBuf) != NULL)
		{
			//
			// Loop through the entries.
			//
			
			for (i = 0; (i < dwEntriesRead); i++)
			{
				assert(pTmpBuf != NULL);

				if (pTmpBuf == NULL)
				{
					fprintf(stderr, "An access violation has occurred\n");
					break;
				}
				std::cout << "账号名：";
				LPWSTR szUsername = (pTmpBuf->usri2_name);

				std::wstring wUsernamestr(szUsername);
				const WCHAR* fuhao = L"$";

				if (wcsstr(pTmpBuf->usri2_name, fuhao))
				{
					std::wcout << wUsernamestr;
					std::wcout << "(隐藏账号)\n";
				}
				else
				{
					std::wcout << dwTotalCount + 1 << "." << wUsernamestr << "\n";
				}

				//权限码
				std::cout << "账号权限码:" << pTmpBuf->usri2_priv << std::endl;
				//SID
				PSID sidUser;
				UCHAR buffer1[2048];
				sidUser = buffer1;
				WCHAR* sid;

				user2sid(pTmpBuf->usri2_name, sidUser);
				ConvertSidToStringSid(sidUser, &sid);
 				std::wcout << "SID:" <<sid << std::endl;
				//时间戳
				char strTime[128] = { 0 };
				timeToString(pTmpBuf->usri2_last_logon, strTime, sizeof(strTime));
				std::cout << "上次登录时间:" << strTime << std::endl;
				
 				pTmpBuf++;
				dwTotalCount++;
			}
		}
		else
			fprintf(stderr, "A system error has occurred: %d\n", nStatus);
		//
		// Free the allocated buffer.
		//
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	// Continue to call NetUserEnum while 
	//  there are more entries. 
	// 
	while (nStatus == ERROR_MORE_DATA); // end do
	//
	// Check again for allocated memory.
	//
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);
	//
	// Print the final count of users enumerated.
	//
	fprintf(stderr, "%d个\n", dwTotalCount);
	system("PAUSE");
	return 0;

}