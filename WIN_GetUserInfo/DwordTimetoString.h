#pragma once
#include <iostream>
#include <time.h>
#include <Windows.h>

char* filetimeToString(FILETIME FileTime, char* str, unsigned int string_size);
char* timeToString(DWORD t, char* str, unsigned int string_size);