#include <iostream>
#include <tchar.h>
#include "PointVArible.h"


int _tmain(int argc,_TCHAR* argv[])
{
	int a = 5, b = 9;
	int* pa=&a, * pb=&b;
	swap(pa, pb);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
}

void swap(int* pa, int* pb)
{
	int ptemp = *pa;
	*pa = *pb;
	*pb = ptemp;
}