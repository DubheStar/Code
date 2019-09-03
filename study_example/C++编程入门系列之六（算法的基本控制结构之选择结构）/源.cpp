#include <iostream>
#include "tchar.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	int nYear;
	bool bIsLeapYear;
	cout << "Enter the year:";
	cin >> nYear;
	bIsLeapYear = ((nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0));
	if (bIsLeapYear)
		cout << nYear << "is a leap year." << endl;
	else
		cout << nYear << "is not a leap year." << endl;
	return 0;
}