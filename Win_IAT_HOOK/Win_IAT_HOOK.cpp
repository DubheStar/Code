#include <iostream>
#include <string>
#include <windows.h>
#include <Dbghelp.h>
#pragma comment (lib,"Dbghelp.lib")
using namespace std;
//typedef PVOID (*pgetiat)(PVOID,BOOLEAN,USHORT,PULONG);

int test_function()
{
	MessageBoxA(NULL, "hook_entry function", "hook", MB_OK);
	return 0;
}
class iat_hook
{
private:
	const char* module;
	const char* procname;
	PROC* newaddr;
	PROC pfnHookAPIAddr;
	//pgetiat getiat;
	HMODULE h;
public:
	iat_hook();
	iat_hook(const char* m, const char* p, PROC* n);
	~iat_hook();
	int hook();
};
int iat_hook::hook()
{
	ULONG ulSize;
	HMODULE hcurrent = GetModuleHandle(NULL);
	PIMAGE_IMPORT_DESCRIPTOR pimportdescriptor =
		(PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
			hcurrent,
			TRUE,
			IMAGE_DIRECTORY_ENTRY_IMPORT,
			&ulSize
		);
	while (pimportdescriptor->Name)
	{
		//cout << (char *)((PBYTE) GetModuleHandle(NULL) + pimportdescriptor->Name) << endl;
		PSTR pszModName = (PSTR)((PBYTE)GetModuleHandle(NULL) + pimportdescriptor->Name);
		if (strcmp(pszModName, module) == 0)
			break;
		pimportdescriptor++;
	}
	PIMAGE_THUNK_DATA pThunk =
		(PIMAGE_THUNK_DATA)((PBYTE)GetModuleHandle(NULL) + pimportdescriptor->FirstThunk);
	while (pThunk->u1.Function)
	{
		PROC* ppfn = (PROC*)&pThunk->u1.Function;
		BOOL bFound = (*ppfn == pfnHookAPIAddr);

		if (bFound)
		{
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(
				ppfn,
				&mbi,
				sizeof(MEMORY_BASIC_INFORMATION)
			);
			VirtualProtect(
				mbi.BaseAddress,
				mbi.RegionSize,
				PAGE_READWRITE,
				&mbi.Protect
			);

			*ppfn = *newaddr;

			DWORD dwOldProtect;
			VirtualProtect(
				mbi.BaseAddress,
				mbi.RegionSize,
				mbi.Protect,
				&dwOldProtect
			);
			break;
		}
		pThunk++;
	}
	return 0;
}
iat_hook::iat_hook()
{
	cout << "you provied wrong parameters!" << endl;
	ExitProcess(NULL);
}
iat_hook::iat_hook(const char* m, const char* p, PROC* n)
{
	module = m;
	procname = p;
	newaddr = n;
	//cout << "debug :  " << hex << (int)*newaddr << endl; //done
	HMODULE hdbghelp = LoadLibraryA("dbghelp.dll");
	/*getiat = (pgetiat) GetProcAddress (hdbghelp,"ImageDirectoryEntryToData");
	if(getiat == NULL)
  {
	  cout << "getprocaddress error" << endl;
	  ExitProcess(NULL);
	}
	cout << "debug:  " << hex << (int)getiat << endl; //done
	*/
	h = GetModuleHandleA(module);
	pfnHookAPIAddr = GetProcAddress(h, procname);
	//cout << "debug:  " << hex << (int)pfnHookAPIAddr << endl; //done
}
iat_hook::~iat_hook()
{
	cout << "yes you are right,this is destructor" << endl;
}

int main()
{
	PROC stub = (PROC)&test_function;
	iat_hook it("KERNEL32.dll", "GetCurrentProcessId", &stub);
	it.hook();
	cout << "current pid :  " << GetCurrentProcessId() << endl;
	return 0;
}