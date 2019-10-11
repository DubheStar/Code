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

#include "DwordTimetoString.h" 
#include "User2Sid.h"
//隐藏账号检测

VOID Account_Class(DWORD UserPriv)
{
	char* type;
	switch (UserPriv)
	{
	case 0:printf("GUEST"); break;
	case 1:printf("USER"); break;
	case 2:printf("ADMIN"); break;
	default:printf("UNK"); break;
	}
}




int RegSAMQuery()
{
	DWORD Data;
	DWORD DataSize = TOTALBYTES;

	PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(DataSize);
	lRet = RegQueryValueEx(hKey, _T("SS"), 0, NULL, (LPBYTE)PerfData, &DataSize);
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
					std::wcout << dwTotalCount + 1 << "." << wUsernamestr ;
					std::wcout << "(隐藏账号)\n";
				}
				else
				{
					std::wcout << dwTotalCount + 1 << "." << wUsernamestr << "\n";
				}

				//权限码-账号类型
				std::cout << "账号类型: ";
				Account_Class(pTmpBuf->usri2_priv);
				std::cout << std::endl;
			
				//SID展示
				PSID sidUser;
				UCHAR buffer1[2048];
				sidUser = buffer1;
				WCHAR* sid;
				user2sid(pTmpBuf->usri2_name, sidUser);
				ConvertSidToStringSid(sidUser, &sid);
  				std::wcout << "S I D :  " <<sid ;
				if (IsValidSid(sidUser))
				{
					std::cout << "（有效）" << std::endl;
				}
				else
				{
					std::cout << "（无效）" << std::endl;
				}

				//账号状态
				std::cout << "账号是否启用：";
				if (pTmpBuf->usri2_flags&UF_ACCOUNTDISABLE)
				{
					std::cout << "关闭" << std::endl; 
				}
				else
				{
					std::cout << "启用" << std::endl;
				}

				//是否为克隆账号
				WCHAR* sidtmp = sid;
				while (wcsrchr(sidtmp,'-')!=NULL)
				{
					sidtmp = wcsrchr(sidtmp, '-') + 1;
				}
				WCHAR wsid[5] = { 0 };
				wcscpy_s(wsid, sidtmp);
				int isid = _wtoi(sidtmp);
				WCHAR whexsid[10] = { 0 };
				_itow_s(isid, whexsid, 16);
				WCHAR SubKey[200] = L"SAM\\SAM\\Domains\\Account\\Users\\00000";
				wcscat_s(SubKey, whexsid);
				HKEY hKey;
				int lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, SubKey, 0, KEY_ALL_ACCESS, &hKey);
				if (lRet == ERROR_SUCCESS)
				{
					DWORD Data;
					DWORD DataSize = 8192;

					PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(DataSize);
					lRet = RegQueryValueEx(hKey, L"SS", 0, NULL, (LPBYTE)PerfData, &DataSize);
					if (ERROR_SUCCESS != lRet)
					{
						std::cout << "RegQueryValueEX Fail:" << lRet << std::endl;
					}
					else
					{
						std::cout << "查询注册表键值为：" << &PerfData << std::endl;
					}
				}
				//时间戳
   				char strTime[128] = { 0 };
				timeToString(pTmpBuf->usri2_last_logon, strTime, sizeof(strTime));
				std::cout << "最后登录时间: " << strTime << std::endl;
				std::cout << std::endl;
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
	fprintf(stderr, "%d个\n", dwEntriesRead);
	system("PAUSE");
	return 0;

}