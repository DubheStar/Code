#include "GetSystemVer.h"

DWORD OsVersion = 0;

int GetSystemVer()
{
	typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
	fnRtlGetVersion pRtlGetVersion;
	HMODULE hNtdll;
	LONG ntStatus;
	ULONG    dwMajorVersion = 0;
	ULONG    dwMinorVersion = 0;
	ULONG    dwBuildNumber = 0;
	RTL_OSVERSIONINFOW VersionInformation = { 0 };

	do
	{
		hNtdll = GetModuleHandle(L"ntdll.dll");
		if (hNtdll == NULL)break;

		pRtlGetVersion = (fnRtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
		if (pRtlGetVersion == NULL)break;

		VersionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
		ntStatus = pRtlGetVersion(&VersionInformation);

		if (ntStatus != 0)break;

		dwMajorVersion = VersionInformation.dwMajorVersion;
		dwMinorVersion = VersionInformation.dwMinorVersion;
		dwBuildNumber = VersionInformation.dwBuildNumber;

		if (dwMajorVersion == 5 && dwMinorVersion == 1 && dwBuildNumber == 2600)
			OsVersion = WINXP2600;
		else if (dwMajorVersion == 5 && dwMinorVersion == 1)
			OsVersion = WINXP;

		else if (dwMajorVersion == 6 && dwMinorVersion == 1 && dwBuildNumber == 7601)
			OsVersion = WIN77601;
		else if (dwMajorVersion == 6 && dwMinorVersion == 1 && dwBuildNumber == 7600)
			OsVersion = WIN77600;
		else if (dwMajorVersion == 6 && dwMinorVersion == 1)
			OsVersion = WIN7;

		else if (dwMajorVersion == 6 && dwMinorVersion == 2 && dwBuildNumber == 9200)
			OsVersion = WIN89200;
		else if (dwMajorVersion == 6 && dwMinorVersion == 2)
			OsVersion = WIN8;

		else if (dwMajorVersion == 6 && dwMinorVersion == 3 && dwBuildNumber == 9600)
			OsVersion = WIN819600;
		else if (dwMajorVersion == 6 && dwMinorVersion == 3)
			OsVersion = WIN81;

		else if (dwMajorVersion == 10 && dwMinorVersion == 0 && dwBuildNumber == 10240)
			OsVersion = WIN1010240;
		else if (dwMajorVersion == 10 && dwMinorVersion == 0 && dwBuildNumber == 10586)
			OsVersion = WIN1010586;
		else if (dwMajorVersion == 10 && dwMinorVersion == 0 && dwBuildNumber == 14393)
			OsVersion = WIN1014393;
		else if (dwMajorVersion == 10 && dwMinorVersion == 0)
			OsVersion = WIN10;

		else
		{
			return FALSE;
		}


	} while (FALSE);
	return OsVersion;
}