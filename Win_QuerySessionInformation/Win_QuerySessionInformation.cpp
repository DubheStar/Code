#include <Windows.h>
#include <WtsApi32.h>
#include <Wtsdefs.h >
#include <stdio.h>
#include <wchar.h>

void WTSGetString(HANDLE serverHandle, DWORD sessionid, WTS_INFO_CLASS command, wchar_t* commandStr)
{
	DWORD bytesReturned = 0;
	LPTSTR pData = NULL;
	if (WTSQuerySessionInformation(serverHandle, sessionid, command, &pData, &bytesReturned))
	{
		printf(L"\tWTSQuerySessionInformationW - session %d - %s returned \"%s\"\n", sessionid, commandStr, pData);
	}
	else
	{
		printf(L"\tWTSQuerySessionInformationW - session %d - %s failed - error=%d - ", sessionid, commandStr, GetLastError());
	}
	WTSFreeMemory(pData);
}


void ExtractFromWTS(HANDLE serverHandle, DWORD sessionid)
{

	WTSGetString(serverHandle, sessionid, WTSInitialProgram, L"WTSInitialProgram");
	WTSGetString(serverHandle, sessionid, WTSApplicationName, L"WTSApplicationName");
	WTSGetString(serverHandle, sessionid, WTSWorkingDirectory, L"WTSWorkingDirectory");
	WTSGetString(serverHandle, sessionid, WTSOEMId, L"WTSOEMId");
	WTSGetString(serverHandle, sessionid, WTSSessionId, L"WTSSessionId");
	WTSGetString(serverHandle, sessionid, WTSUserName, L"WTSUserName");
	WTSGetString(serverHandle, sessionid, WTSWinStationName, L"WTSWinStationName");
	WTSGetString(serverHandle, sessionid, WTSDomainName, L"WTSDomainName");
	WTSGetString(serverHandle, sessionid, WTSConnectState, L"WTSConnectState");
	WTSGetString(serverHandle, sessionid, WTSClientBuildNumber, L"WTSClientBuildNumber");
	WTSGetString(serverHandle, sessionid, WTSClientName, L"WTSClientName");
	WTSGetString(serverHandle, sessionid, WTSClientDirectory, L"WTSClientDirectory");
	WTSGetString(serverHandle, sessionid, WTSClientProductId, L"WTSClientProductId");
	WTSGetString(serverHandle, sessionid, WTSClientHardwareId, L"WTSClientHardwareId");
	WTSGetString(serverHandle, sessionid, WTSClientAddress, L"WTSClientAddress");
	WTSGetString(serverHandle, sessionid, WTSClientDisplay, L"WTSClientDisplay");
	WTSGetString(serverHandle, sessionid, WTSClientProtocolType, L"WTSClientProtocolType");
}

int _tmain(int argc, _TCHAR* argv[])
{
	PWTS_SESSION_INFOW ppSessionInfo = 0;
	DWORD pCount;
	if (!WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &ppSessionInfo, &pCount))
	{
		printLastError(L"WTSEnumerateSessions", GetLastError());
		return 1;
	}

	wprintf(L"%d WTS sessions found on host\n", pCount);

	for (unsigned int i = 0; i < pCount; i++)
	{
		wprintf(L"> session=%d, stationName = %s\n", ppSessionInfo[i].SessionId, ppSessionInfo[i].pWinStationName);
		ExtractFromWTS(WTS_CURRENT_SERVER_HANDLE, ppSessionInfo[i].SessionId);
		LPWSTR sessionstr = new wchar_t[200];
		wsprintf(sessionstr, L"%d", ppSessionInfo[i].SessionId);
	}

	return 0;
}