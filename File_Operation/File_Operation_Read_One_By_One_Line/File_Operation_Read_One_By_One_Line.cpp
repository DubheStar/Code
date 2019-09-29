#include <iostream>
#include <fstream>
#include <tchar.h>
using std::fstream;
using std::ios;

int _tmain()
{
	fstream f(_T("Test.txt"), ios::out);//供写使用，文件不存在则创建，存在则清空原内容

	
}