#include <iostream>
#include <windows.h>
#include <tchar.h>
using std::cout;

int _tmain()
{
	HKEY key;
	LPCTSTR subkey = _T("\\SAM\\SAM\\Domains\\Account\\Users\\000003EA");
	if (ERROR_SUCCESS == (HKEY_LOCAL_MACHINE, subkey, 0, KEY_ALL_ACCESS, &key))
	{
		std:cout << "open success!";
	}
	system("pause");
}