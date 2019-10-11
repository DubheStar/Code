#include "DwordTimetoString.h"

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