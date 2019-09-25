#include "conio.h"
#include "Scanner.h"
#include <iostream>

int main()
{
	CScanner scanner;
	std::cout << "欢迎使用DubheStar多线程扫描器 \r\n" << std::endl;
	std::cout << "输入IP或IP段: \r\n" << std::endl;
	char szIP[128];
	scanf_s("%s", szIP, 128);
	//ip处理
	scanner.Addip(szIP);
	std::cout << "请输入端口或端口范围（用“-”连接）:\r\n" << std::endl;
	char szPort[128];
	scanf_s("%s", szPort, 128);
	std::cout << std::endl;
	//端口处理
	scanner.AddPort(szPort);
	//开始扫描 
	scanner.Start();
	scanner.PrintfResult();
	_getch(); //等待结束
    return 0;
}

