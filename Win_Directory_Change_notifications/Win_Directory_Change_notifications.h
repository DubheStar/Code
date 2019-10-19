#pragma once

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include "Command.h"

class FileMonitor
{
public:
	explicit FileMonitor(std::wstring path);

	~FileMonitor();

	bool Init();
	void Run();
	void Uninit();

private:
	std::wstring path_;
	HANDLE hDir_;
	std::unique_ptr<Command> addAction_;
	std::unique_ptr<Command> removeAction_;
	std::unique_ptr<Command> modifyAction_;
	std::unique_ptr<Command> renameAction_;
};

FileMonitor::FileMonitor(std::wstring path)
{
	path_ = path;
}

FileMonitor::~FileMonitor()
{
	Uninit();
}

bool FileMonitor::Init()
{
	hDir_ = CreateFile(
		path_.c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
	);
	addAction_ = std::unique_ptr<Command>(new AddCommand());
	removeAction_ = std::unique_ptr<Command>(new RemoveCommand());
	modifyAction_ = std::unique_ptr<Command>(new ModifyCommand());
	renameAction_ = std::unique_ptr<Command>(new RenameCommand());

	return INVALID_HANDLE_VALUE != hDir_;
}

void FileMonitor::Run()
{
	char notify[1024];
	memset(notify, 0, 1024);
	DWORD cbBytes;

	char str1[MAX_PATH], str2[MAX_PATH];

	while (TRUE)
	{
		BOOL bResult = ReadDirectoryChangesW(
			hDir_,
			&notify,
			sizeof(notify),
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
			&cbBytes,
			NULL,
			NULL
		);

		FILE_NOTIFY_INFORMATION* pNotify = NULL;
		size_t offset = 0;

		if (bResult)
		{
			do {
				pNotify = (FILE_NOTIFY_INFORMATION*)(&notify[offset]);
				offset += pNotify->NextEntryOffset;

				memset(str1, 0, MAX_PATH);
				WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, str1, 99, NULL, NULL);

				switch (pNotify->Action)
				{
				case FILE_ACTION_ADDED:
					addAction_->execute(str1);
					break;
				case FILE_ACTION_MODIFIED:
					modifyAction_->execute(str1);
					break;
				case FILE_ACTION_REMOVED:
					removeAction_->execute(str1);
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					renameAction_->execute(str2, str1);
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					memcpy(str2, str1, MAX_PATH);
					break;
				default:
					printf("Unknown command.\n");
				}
			} while (pNotify->NextEntryOffset != 0);
		}
	}
}

void FileMonitor::Uninit()
{
	CloseHandle(hDir_);
}
