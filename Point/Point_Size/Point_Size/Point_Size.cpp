#include <iostream>
using std::cout;

int main()
{
	double a = 2.0;
	double* p = &a;
	std::cout << "Size of Point=" << sizeof(p);
	std::cout << "Size of Point to Address's Value=" << sizeof(*p);
}
