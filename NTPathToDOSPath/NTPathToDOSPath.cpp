#include <Windows.h>
#include <stdio.h>

BOOLEAN NtPathToDosPathW(WCHAR* FullNtPath, WCHAR* FullDosPath)
{
	WCHAR DosDevice[4] = { 0 };//dos设备名最大长度为4
	WCHAR NtPath[64] = { 0 };//nt设备名最大长度为64
	WCHAR* RetStr = NULL;
	size_t NtPathLen = 0;
	if (!FullNtPath || !FullDosPath)
	{
		return FALSE;
	}
	for (short i = 65; i < 26 + 65; i++)
	{
		DosDevice[0] = i;
		DosDevice[1] = L':';
		if (QueryDosDeviceW(DosDevice, NtPath, 64))
		{
			if (NtPath)
			{
				NtPathLen = wcslen(NtPath);
				if (!_wcsnicmp(NtPath, FullNtPath, NtPathLen))
				{
					wcscpy(FullDosPath, DosDevice);
					wcscat(FullDosPath, FullNtPath + NtPathLen);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOLEAN DosPathToNtPathW(WCHAR* FullDosPath, WCHAR* FullNtPath)
{
	WCHAR DosDevice[4] = { 0 };//dos设备名最大长度为4
	WCHAR NtPath[64] = { 0 };//nt设备名最大长度为64
	WCHAR* RetStr = NULL;
	size_t NtPathLen = 0;
	if (!FullNtPath || !FullDosPath)
	{
		return FALSE;
	}
	DosDevice[0] = FullDosPath[0];
	DosDevice[1] = L':';
	if (QueryDosDeviceW(DosDevice, NtPath, 64))
	{
		if (NtPath)
		{
			wcscpy(FullNtPath, NtPath);
			wcscat(FullNtPath, FullDosPath + 2);
			return TRUE;
		}
	}
	return FALSE;
}

BOOLEAN NtPathToDosPathA(CHAR* FullNtPath, CHAR* FullDosPath)
{
	CHAR DosDevice[4] = { 0 };//dos设备名最大长度为4
	CHAR NtPath[64] = { 0 };//nt设备名最大长度为64
	CHAR* RetStr = NULL;
	size_t NtPathLen = 0;
	if (!FullNtPath || !FullDosPath)
	{
		return FALSE;
	}
	for (short i = 65; i < 26 + 65; i++)
	{
		DosDevice[0] = i;
		DosDevice[1] = L':';
		if (QueryDosDeviceA(DosDevice, NtPath, 64))
		{
			if (NtPath)
			{
				NtPathLen = strlen(NtPath);
				if (!strnicmp(NtPath, FullNtPath, NtPathLen))
				{
					strcpy(FullDosPath, DosDevice);
					strcat(FullDosPath, FullNtPath + NtPathLen);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOLEAN DosPathToNtPathA(CHAR* FullDosPath, CHAR* FullNtPath)
{
	CHAR DosDevice[4] = { 0 };//dos设备名最大长度为4
	CHAR NtPath[64] = { 0 };//nt设备名最大长度为64
	CHAR* RetStr = NULL;
	size_t NtPathLen = 0;
	if (!FullNtPath || !FullDosPath)
	{
		return FALSE;
	}
	DosDevice[0] = FullDosPath[0];
	DosDevice[1] = L':';
	if (QueryDosDeviceA(DosDevice, NtPath, 64))
	{
		if (NtPath)
		{
			strcpy(FullNtPath, NtPath);
			strcat(FullNtPath, FullDosPath + 2);
			return TRUE;
		}
	}
	return FALSE;
}

void test()
{
	WCHAR dspW1[] = L"c:\\windows\\system32\\config\\sam", dspW2[500] = { 0 };
	WCHAR ntpW[500] = { 0 };
	DosPathToNtPathW(dspW1, ntpW);
	printf("%S\n", ntpW);
	NtPathToDosPathW(ntpW, dspW2);
	printf("%S\n", dspW2);
	//
	CHAR dspA1[] = "c:\\windows\\system32\\config\\sam", dspA2[500] = { 0 };
	CHAR ntpA[500] = { 0 };
	DosPathToNtPathA(dspA1, ntpA);
	printf("%s\n", ntpA);
	NtPathToDosPathA(ntpA, dspA2);
	printf("%s\n", dspA2);
}