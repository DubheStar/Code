// GlobalVarible.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int foo = 1;
int addfoo1()
{
	foo++;
	return 0;
}
int addfoo2()
{
	addfoo1();
	return 0;
}

int main()
{
	addfoo2();
	std::cout << foo << std::endl;
}
