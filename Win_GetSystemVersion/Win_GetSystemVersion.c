#include <stdio.h>
#include <Windows.h>


/* WINDOWS版本源https://docs.microsoft.com/zh-cn/windows/release-information/ */
#define WINXP					51
#define WINXP2600				512600

#define WIN7					61
#define WIN77600				617600
#define WIN77601				617601

#define	WIN8					62
#define	WIN89200				629200

#define WIN81					63
#define	WIN819600				639600


#define WIN10					100
#define WIN1010240				10010240
#define WIN1010586				10010586
#define WIN1014393				10014393
#define WIN1015063				10015063
#define WIN1016299				10016299
#define WIN1017763				10017763
#define WIN1017134				10017134
#define WIN1017763				10017763
#define WIN1018362				10018362

int main()
{
	typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
	fnRtlGetVersion pRtlGetVersion;
	HMODULE hNtdll;
	LONG ntStatus;
	ULONG    dwMajorVersion = 0;
	ULONG    dwMinorVersion = 0;
	ULONG    dwBuildNumber = 0;
	RTL_OSVERSIONINFOW VersionInformation = { 0 };
	DWORD OsVersion = 0;

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

	printf("%d\n", OsVersion);
	return 0;
	
}

