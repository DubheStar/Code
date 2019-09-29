﻿#include <iostream>
#include <fstream>
#include <tchar.h>
using namespace std;



int _tmain()
{
	fstream f(_T("Test.txt"), ios::out);//供写使用，文件不存在则创建，存在则清空原内容
	if (f.is_open())
	{
		cout << "检查：文件已经被打开！" << endl;
	}
	f << 1234 << ' ' << 3.14 << 'A' << "How are you"; //写入数据
	f.close();//关闭文件以使其重新变为可访问，函数一旦调用，原先的流对象就可以被用来打开其它的文件
	if (f.is_open() != 1)
	{
		cout << "检查：文件已经被关闭！" << endl;
	}
	f.open(_T("Test.txt"), ios::in);//打开文件，供读
	int i;
	double d;
	char c;
	char s[20];
	f >> i >> d >> c;               //读取数据
	f.getline(s, 20);
	cout << i << endl;             //显示各数据
	cout << d << endl;
	cout << c << endl;
	cout << s << endl;
	f.close();
}