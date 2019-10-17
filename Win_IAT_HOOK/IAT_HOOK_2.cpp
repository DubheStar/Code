#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <ShlObj.h>

int __stdcall MyFunction(
	HWND    hWnd,
	LPCTSTR lpText,
	LPCTSTR lpCaption,
	UINT    uType
) {
	TCHAR szDesktopDir[MAX_PATH] = { 0 };
	HANDLE hFile = NULL;
	BOOL fOk = SHGetSpecialFolderPath(NULL, szDesktopDir, CSIDL_DESKTOPDIRECTORY, FALSE);
	TCHAR szText[] = _T("Hello, if you see this, it means IAT Hook succeed! :D");
	DWORD dwSize = 0;
	if (!fOk)
		return(-1);
	_tcscat_s(szDesktopDir, _countof(szDesktopDir) * sizeof(TCHAR), _T("\\hello.txt"));
	hFile = CreateFile(szDesktopDir, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return(-1);
	fOk = WriteFile(hFile, szText, _countof(szText) * sizeof(TCHAR), &dwSize, NULL);
	if (!fOk)
		return(-1);
	CloseHandle(hFile);

	return(0);
}

BOOL RootkitOfIATHook(LPCTSTR pszDllName, LPCTSTR pszOriginApiName, LPVOID lpfnNewFunc) {
	PIMAGE_DOS_HEADER pstDosHeader = NULL;
	PIMAGE_NT_HEADERS pstNtHeaders = NULL;
	PIMAGE_OPTIONAL_HEADER pstOptHeader = NULL;
	PIMAGE_IMPORT_DESCRIPTOR pstImpDes = NULL;
	PIMAGE_THUNK_DATA pThunkData = NULL;
	HMODULE hMod = NULL;
	DWORD dwOldProtect = 0;
	SIZE_T stWritten = 0;

	hMod = GetModuleHandle(NULL);
	if (NULL == hMod)
		return(-1);
	pstDosHeader = (PIMAGE_DOS_HEADER)hMod;
	if (IMAGE_DOS_SIGNATURE != pstDosHeader->e_magic)
		return(-1);
	pstNtHeaders = (PIMAGE_NT_HEADERS)((PBYTE)hMod + pstDosHeader->e_lfanew);
	if (IMAGE_NT_SIGNATURE != pstNtHeaders->Signature)
		return(-1);
	pstOptHeader = (PIMAGE_OPTIONAL_HEADER)((PBYTE)pstNtHeaders + 24);
	pstImpDes = (PIMAGE_IMPORT_DESCRIPTOR)((PBYTE)hMod + pstOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while (pstImpDes->Name) {
		LPCTSTR lpOriginalNameAddr = (LPCTSTR)((PBYTE)hMod + pstImpDes->Name);
		if (!_tcscmp(pszDllName, lpOriginalNameAddr))
			break;
		++pstImpDes;
	}
	pThunkData = (PIMAGE_THUNK_DATA)((PBYTE)hMod + pstImpDes->FirstThunk);
	typedef int(WINAPI* PFNMESSAGEBOX)(
		HWND    hWnd,
		LPCTSTR lpText,
		LPCTSTR lpCaption,
		UINT    uType
		);
	PFNMESSAGEBOX pfnMsgBoxOriginal = (PFNMESSAGEBOX)GetProcAddress(GetModuleHandle(_T("USER32.dll")), "MessageBox");
	if (pfnMsgBoxOriginal == 0)
		return(FALSE);
	while (pThunkData->u1.Function) {
		if ((ULONGLONG)pThunkData->u1.Function == (ULONGLONG)pfnMsgBoxOriginal) {
			LPDWORD pOrigin = (LPDWORD)&pThunkData->u1.Function;
			VirtualProtect(pOrigin, 8, PAGE_READWRITE, &dwOldProtect);
			WriteProcessMemory(GetCurrentProcess(), pOrigin, &lpfnNewFunc, 8, &stWritten);
			VirtualProtect(pOrigin, 8, dwOldProtect, &dwOldProtect);
			break;
		}
		++pThunkData;
	}

	return(TRUE);
}

int _tmain() {
	MessageBox(NULL, _T("Test for IAT Hook"), NULL, MB_OK);
	RootkitOfIATHook(_T("USER32.dll"), _T("MessageBox"), MyFunction);
	MessageBox(NULL, _T("If you see this, it means IAT Hook failed!"), _T("Error"), MB_OK);
	return(0);
}