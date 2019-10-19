#include "Win_Directory_Change_notifications.h"

void usage()
{
	printf("Invalid parameter\n");
	printf("Usage: FileMonitor \"path\"\n");
	printf("eg. FileMonitor d:\\test\\");
}

int main(int argc, WCHAR* argv[])
{
	//if (argc <= 1)
	//{
	//	usage();
	//	return -1;
	//}

	FileMonitor fileMonitor(argv[1]);
	fileMonitor.Init();
	fileMonitor.Run();
	return 0;
}