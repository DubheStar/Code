#include <iostream>
#include <fstream>
#include <tchar.h>

using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::cin;

int _tmain()
{
	char data[100];
	ofstream writedFile;
	writedFile.open(_T("TestWritedFile.txt"));

	cout << "writing to the file" << endl;
	cout << "你的名字：" << endl;

	cin.getline(data, 100);
	writedFile << data << endl;

	cout << "你的年龄：" << endl;
	cin >> data;
	cin.ignore();

	writedFile << data << endl;
	writedFile.close();

	ifstream readedFile;
	readedFile.open(_T("TestWritedFile.txt"));

	cout << "Reading from the File" << endl;
	readedFile >> data;
	cout << data <<endl;

	readedFile >> data;
	cout << data << endl;

	readedFile.close();
}