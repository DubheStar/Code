#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32.lib")

#include <iostream>
#include <Windows.h>
#include <LM.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <wchar.h>
//隐藏账号检测


int main()
{
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
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
			std::cout << "共有账号：\n";
			for (i = 0; (i < dwEntriesRead); i++)
			{
				assert(pTmpBuf != NULL);

				if (pTmpBuf == NULL)
				{
					fprintf(stderr, "An access violation has occurred\n");
					break;
				}

				LPWSTR szUsername = (pTmpBuf->usri0_name);
				
				const WCHAR* fuhao = L"$";
				if (wcsstr(pTmpBuf->usri0_name, fuhao))
				{
					std::wstring wUsernamestr(szUsername);
					std::wcout << dwTotalCount + 1 << "." << wUsernamestr;
					std::wcout << "(隐藏账号)\n";
				}
				else
				{
					std::wstring wUsernamestr(szUsername);
					std::wcout << dwTotalCount + 1 << "." << wUsernamestr << "\n";
				}
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