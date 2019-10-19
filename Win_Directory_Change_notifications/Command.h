#pragma once
#include <stdio.h>

class Command
{
public:
	virtual ~Command() {};
	virtual void execute(const char* filename, const char* filename2 = NULL) = 0;
};

class AddCommand : public Command
{
public:
	virtual void execute(const char* filename, const char* filename2)
	{
		printf("New file/Dir: %s\n", filename);
	}
};

class ModifyCommand : public Command
{
public:
	virtual void execute(const char* filename, const char* filename2)
	{
		printf("the file was modified.: %s\n", filename);
	}
};

class RemoveCommand : public Command
{
public:
	virtual void execute(const char* filename, const char* filename2)
	{
		printf("removed: %s\n", filename);
	}
};

class RenameCommand : public Command
{
public:
	virtual void execute(const char* filename, const char* filename2)
	{
		printf("rename %s to %s.\n", filename, filename2);
	}
};