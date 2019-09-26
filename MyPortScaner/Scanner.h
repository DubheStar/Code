#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WinSock2.h"
#include <ws2tcpip.h> 
#include <list>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

typedef struct _ScanUnit //扫描单元
{
	ULONG addr;  //存储地址
	list<unsigned int> scanPortList;	//待扫描的端口
	list<unsigned int> openPortList;	//开放的端口即扫描结果
	ULONG ulScanning;	//自锁变量，用于多线程，防止数据冲突
}ScanUnit,*pScanUnit;
typedef list<pScanUnit> UnitList; //pScanUnit的list列表
typedef list<pScanUnit>::iterator UnitIter;//pScanUnit的list列表的迭代器

class CScanner //扫描类
{
public:
	CScanner();  //构造函数
	~CScanner(); //析构函数
	void Addip(char * szIP);//添加ip函数，第一次初步处理与合法性验证,然后调用重载函数
	void Addip(ULONG addr);//写入ip地址到ScanUnit
	void AddPort(char* szPort);//多个端口添加
	void AddPort(int nPort);//单一端口添加
	bool Start();//开始扫描
	void PrintfResult();//输出筛选后的扫描结果

	UnitList m_list;//存放ScanUnit

	static DWORD WINAPI ScanProc(LPARAM LParam); //扫描线程
	HANDLE hConsole; //存放控制台句柄，以更改文字颜色，用于优化。
};

