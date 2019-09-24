#pragma once
#include<windows.h>
#include <d3dx9.h>

#pragma pack(push)
#pragma pack(1)
#ifndef _WIN64
struct JmpCode
{
private:
	const BYTE jmp;
	DWORD address;

public:
	JmpCode(DWORD srcAddr, DWORD dstAddr) : jmp(0xE9)
	{
		setAddress(srcAddr, dstAddr);
	}

	void setAddress(DWORD srcAddr, DWORD dstAddr)
	{
		address = dstAddr - srcAddr - sizeof(JmpCode);
	}
};
#else
struct JmpCode
{
private:
	BYTE jmp[6];
	uintptr_t address;

public:
	JmpCode(uintptr_t srcAddr, uintptr_t dstAddr)
	{
		static const BYTE JMP[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 };
		memcpy(jmp, JMP, sizeof(jmp));
		setAddress(srcAddr, dstAddr);
	}

	void setAddress(uintptr_t srcAddr, uintptr_t dstAddr)
	{
		address = dstAddr;
	}
};
#endif
#pragma pack(pop)

class DIRECT3DHOOK
{
public:
	static DIRECT3DHOOK* Init();
	void SetHook(void* originalFunction, void* hookFunction, BYTE* oldCode);
	void UnHook(void* originalFunction, BYTE* oldCode);
	void* getDirect3dEndSceneAddr();
private:
	static DIRECT3DHOOK* Instance;
	void* pEndSceneAddr;
protected:
};
