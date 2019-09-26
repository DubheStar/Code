#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WinSock2.h"
#include <ws2tcpip.h> 
#include <list>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

typedef struct _ScanUnit //ɨ�赥Ԫ
{
	ULONG addr;  //�洢��ַ
	list<unsigned int> scanPortList;	//��ɨ��Ķ˿�
	list<unsigned int> openPortList;	//���ŵĶ˿ڼ�ɨ����
	ULONG ulScanning;	//�������������ڶ��̣߳���ֹ���ݳ�ͻ
}ScanUnit,*pScanUnit;
typedef list<pScanUnit> UnitList; //pScanUnit��list�б�
typedef list<pScanUnit>::iterator UnitIter;//pScanUnit��list�б�ĵ�����

class CScanner //ɨ����
{
public:
	CScanner();  //���캯��
	~CScanner(); //��������
	void Addip(char * szIP);//���ip��������һ�γ���������Ϸ�����֤,Ȼ��������غ���
	void Addip(ULONG addr);//д��ip��ַ��ScanUnit
	void AddPort(char* szPort);//����˿����
	void AddPort(int nPort);//��һ�˿����
	bool Start();//��ʼɨ��
	void PrintfResult();//���ɸѡ���ɨ����

	UnitList m_list;//���ScanUnit

	static DWORD WINAPI ScanProc(LPARAM LParam); //ɨ���߳�
	HANDLE hConsole; //��ſ���̨������Ը���������ɫ�������Ż���
};

