#include <iostream>
#include <tchar.h>
#include "ThePointVarible.h"


int _tmain(int argc,_TCHAR* argv[])
{
	int a = 5, b = 9;
	int* pa=&a, * pb=&b;
	swap_value(a, b);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
	swap_point(pa, pb);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
 	swap_reference(a, b);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
}

void swap_point(int* pa, int* pb)
{
	int ptemp = *pa;
	*pa = *pb;
	*pb = ptemp;
}

void swap_reference(int& c, int& d)
{
	int ptemp = c;
	c = d;
	d = ptemp;
}

void swap_value(int a, int b)
{
	int temp = a;
	a = b;
	b = temp;
}