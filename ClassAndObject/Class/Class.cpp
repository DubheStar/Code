#include <iostream>
#include "Class.h"
using std::cout;
using std::endl;

//类的私有成员访问方式

int main()
{
	Box box;
	box.dLength = 10;
	cout << "dLength = " << box.dLength << endl;
	box.setWidth(10.0);
	cout << "dWidth = " << box.getWidth() << endl;
	return 0;
}
