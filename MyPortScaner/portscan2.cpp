#include "conio.h"
#include "Scanner.h"
#define TRUE 1

int main()
{
	CScanner scanner;
	printf("Port Scanner \r\n");
	printf("��ӭʹ�ö��߳�ɨ���� \r\n");
	printf("����IP��IP��: \r\n");
	char szIP[128];
	scanf_s("%s", szIP, 128);
	//ip����
	scanner.Addip(szIP);
	printf("������˿ڻ�˿ڷ�Χ���á�-�����ӣ�:\r\n");
	char szPort[128];
	scanf_s("%s", szPort, 128);
	//�˿ڴ���
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
	//��ʼɨ�� 
	scanner.Start();

	scanner.PrintfResult();
	printf("ɨ�����~~~\r\n");
	_getch(); //�ȴ�����
    return 0;
}

