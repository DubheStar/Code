#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <LM.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <wchar.h>
#include <time.h>
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

void SidtoUser(PSID psid, char* user, char* rid, char* sid, unsigned int max_size)
{
	if (psid == NULL) return;
	if (IsValidSid(psid))
	{
		DWORD saccount = 0;
		char* account = NULL;
		DWORD sdomain = 0;
		char* domain = NULL;
		SID_NAME_USE snu;

		LookupAccountSid(NULL, psid, (LPWSTR)account, &saccount, (LPWSTR)domain, &sdomain, &snu);
		if (!saccount || !sdomain) return;
		account = (LPSTR)HeapAlloc(GetProcessHeap(), 0, saccount);
		domain = (LPSTR)HeapAlloc(GetProcessHeap(), 0, sdomain);

		if (!account || !domain) return;
		account[0] = 0; domain[0] = 0;

		if (LookupAccountSid(NULL, psid, (LPWSTR)account, &saccount, (LPWSTR)domain, &sdomain, &snu))
		{
			//user
			if (user != NULL && domain[0] != 0 && account[0] != 0 && saccount && sdomain)snprintf(user, max_size, "%s\\%s ", domain, account);
			else user[0] = 0;

			//rid
			PSID_IDENTIFIER_AUTHORITY t = GetSidIdentifierAuthority(psid);
			PUCHAR pcSubAuth = GetSidSubAuthorityCount(psid);
			DWORD* SidP;

			unsigned char i, ucMax = *pcSubAuth;
			if (ucMax > 2)
			{
				SidP = GetSidSubAuthority(psid, ucMax - 1);
				if (*SidP < 65535) snprintf(rid, max_size, "%05lu", *SidP);
				else rid[0] = 0;
			}
			else rid[0] = 0;

			//sid
			char tmp[MAX_PATH];
			snprintf(sid, MAX_PATH, "S-%d-%u", ((SID*)psid)->Revision, t->Value[5] + (t->Value[4] << 8) + (t->Value[3] << 16) + (t->Value[2] << 24));

			//sid
			for (i = 0; i < ucMax; ++i)
			{
				SidP = GetSidSubAuthority(psid, i);
				strncpy(tmp, sid, MAX_PATH);
				snprintf(sid, max_size, "%s-%lu", tmp, *SidP);
			}
		}

		HeapFree(GetProcessHeap(), 0, account);
		HeapFree(GetProcessHeap(), 0, domain);
	}
}

void GetSIDFromUser(char* user, char* rid, char* sid, unsigned int max_size)
{
	BYTE BSid[MAX_PATH];
	PSID psid = (PSID)BSid;
	DWORD sid_size = MAX_PATH;
	SID_NAME_USE TypeSid;

	char domain[MAX_PATH];
	DWORD domain_size = MAX_PATH;

	if (LookupAccountName(NULL, (LPCTSTR)user, psid, (LPDWORD)&sid_size, (LPTSTR)domain, (LPDWORD)&domain_size, (PSID_NAME_USE)&TypeSid))
	{
		if (IsValidSid(psid))
		{
			SidtoUser(psid, user, rid, sid, max_size);
		}
	}
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
				BYTE BSid[MAX_PATH];
				PSID psid = (PSID)BSid;
				DWORD sid_size = MAX_PATH;
				SID_NAME_USE TypeSid;
				char domain[MAX_PATH];
				DWORD domain_size = MAX_PATH;

				char name[MAX_PATH], RID[MAX_PATH], SID[MAX_PATH], group[MAX_PATH], type[MAX_PATH],
					description[MAX_PATH];

				DWORD nb_connexion, state_id;
				name[0] = 0;
				RID[0] = 0;
				SID[0] = 0;
				group[0] = 0;
				description[0] = 0;
				nb_connexion = 0;
				state_id = 0;
				type[0] = 0;

				GetSIDFromUser(name, RID, SID, MAX_PATH);

 				std::cout << "SID:\n" <<RID<<SID<< std::endl;
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