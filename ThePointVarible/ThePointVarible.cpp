#include <iostream>
#include <tchar.h>
#include "ThePointVarible.h"


int _tmain(int argc,_TCHAR* argv[])
{
	int a = 5, b = 9;
	swap_value(a, b);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
	swap_point(&a, &b);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
 	swap_reference(a, b);
	std::cout << "a:" << a << "\t" << "b:" << b << std::endl;
}

void swap_value(int a, int b)
{
	int temp = a;
	a = b;
	b = temp;
}

void swap_point(int* a, int* b)
{
	int ptemp = *a;
	*a = *b;
	*b = ptemp;
}

void swap_reference(int& c, int& d)
{
	int ptemp = c;
	c = d;
	d = ptemp;
}

