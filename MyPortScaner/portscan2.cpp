#include "conio.h"
#include "Scanner.h"
#define TRUE 1

int main()
{
	CScanner scanner;
	printf("Port Scanner \r\n");
	printf("欢迎使用多线程扫描器 \r\n");
	printf("输入IP或IP段: \r\n");
	char szIP[128];
	scanf_s("%s", szIP, 128);
	//ip处理
	scanner.Addip(szIP);
	printf("请输入端口或端口范围（用“-”连接）:\r\n");
	char szPort[128];
	scanf_s("%s", szPort, 128);
	//端口处理
	if (strstr(szPort, "-") == 0) 
	{
		if (atoi(szPort) != 0) 
		{
			scanner.AddPort(atoi(szPort));
		}
		else
		{
			char * szStartPort = szPort;
			char * szEndPort = strstr(szPort, "-") + 1;
			*(szEndPort - 1) = 0;
			scanner.AddPort(atoi(szStartPort), atoi(szEndPort));
		}
	}
	//开始扫描 
	scanner.Start();

	scanner.PrintfResult();
	printf("扫描结束~~~\r\n");
	_getch(); //等待结束
    return 0;
}

