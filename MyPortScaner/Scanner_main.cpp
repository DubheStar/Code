#include "conio.h"
#include "Scanner.h"
#include <iostream>

int main()
{
	CScanner scanner;
	std::cout << "��ӭʹ��DubheStar���߳�ɨ���� \r\n" << std::endl;
	std::cout << "����IP��IP��: \r\n" << std::endl;
	char szIP[128];
	scanf_s("%s", szIP, 128);
	//ip����
	scanner.Addip(szIP);
	std::cout << "������˿ڻ�˿ڷ�Χ���á�-�����ӣ�:\r\n" << std::endl;
	char szPort[128];
	scanf_s("%s", szPort, 128);
	std::cout << std::endl;
	//�˿ڴ���
	scanner.AddPort(szPort);
	//��ʼɨ�� 
	scanner.Start();
	scanner.PrintfResult();
	_getch(); //�ȴ�����
    return 0;
}

