#include <iostream>
#include <fstream>
#include <tchar.h>
using std::fstream;
using std::ios;

int _tmain()
{
	fstream writedFile(_T("Test.txt"),ios::out);
	int i;
	char a = 'a';
	for (i = 1; i <= 26; i++)
	{
		if (i < 10)
		{
			writedFile << "0" << i <<"\t" << a << "\r\n";
			a++;
		}

		else
		{
			writedFile << i <<"\t" << a << "\r\n";
			a++;
		}
	}
	writedFile.close();
}
