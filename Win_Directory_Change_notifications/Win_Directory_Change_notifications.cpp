#include "Win_Directory_Change_notifications.h"
#include <sstream>

int main(int argc, CHAR* argv[])
{
	if (argc <= 1)
	{
		std::wcout << L"Usage: " << argv[0] << L"\t\"Moniter_Path\"";
		return -1;
	}

	std::wstringstream wszMoniterPath;
	wszMoniterPath << argv[1];
	FileMonitor fileMonitor(wszMoniterPath.str().c_str());
	fileMonitor.Init();
	fileMonitor.Run();
	return 0;
}