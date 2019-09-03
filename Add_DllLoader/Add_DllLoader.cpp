// Add_DllLoader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "windows.h"
#include <tchar.h>
#include <stdlib.h>

//lpAddFun是typedef定义的一个名称
//可以用来定义变量
//比如 lpAddFun p;
//那 p就是 int(*p)(int, int);
//首先(*p)说明p是一个指针，(*p)(); 说明p指向函数
//(*p)(int, int)说明p指向的函数有两个int类型参数，
//最后 int(*p)(int, int); 说明 p指向的函数返回 值类型是int
// 函数声明 lpAddFun是一个指向函数的指针，该函数有两个参数都是int类型，函数的返回值也是int类型
typedef int(*lpAddFun)(int, int);

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hDll;
	lpAddFun addFun;

	hDll = LoadLibraryW(L"ADD_Dll.dll");

	if (hDll != NULL)
	{
		addFun = (lpAddFun)GetProcAddress(hDll, "add");

		if (addFun != NULL)
		{
			int result = addFun(1300, 14);
			printf("%d\n", result);
		}
		FreeLibrary(hDll);
	}
	system("pause");
	return 0;
}