#include"Hook.h"

DIRECT3DHOOK* DIRECT3DHOOK::Instance = NULL;

DIRECT3DHOOK* DIRECT3DHOOK::Init()
{
	if (Instance == NULL)
	{
		Instance = new DIRECT3DHOOK();
	}

	return Instance;
}

void DIRECT3DHOOK::SetHook(void* originalFunction, void* hookFunction, BYTE* oldCode)
{
	JmpCode code((uintptr_t)originalFunction, (uintptr_t)hookFunction);
	DWORD oldProtect, oldProtect2;
	VirtualProtect(originalFunction, sizeof(code), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(oldCode, originalFunction, sizeof(code));
	memcpy(originalFunction, &code, sizeof(code));
	VirtualProtect(originalFunction, sizeof(code), oldProtect, &oldProtect2);
}

void DIRECT3DHOOK::UnHook(void* originalFunction, BYTE* oldCode)
{
	DWORD oldProtect, oldProtect2;
	VirtualProtect(originalFunction, sizeof(JmpCode), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(originalFunction, oldCode, sizeof(JmpCode));
	VirtualProtect(originalFunction, sizeof(JmpCode), oldProtect, &oldProtect2);
}


void* DIRECT3DHOOK::getDirect3dEndSceneAddr()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName =TEXT ("TextWindow");
	if (RegisterClassEx(&wc) == 0)
	{
		MessageBox(NULL, TEXT("注册窗口类失败"), TEXT(""), MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(0, wc.lpszClassName, TEXT(""), WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("创建窗口失败"), TEXT(""), MB_OK);
		return 0;
	}

	// 初始化D3D

	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d9 == NULL)
	{
		MessageBox(NULL, TEXT("创建D3D失败"), TEXT(""), MB_OK);
		DestroyWindow(hwnd);
		return 0;
	}

	D3DPRESENT_PARAMETERS pp = {};
	pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_COPY;

	IDirect3DDevice9* device;
	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &device)))
	{
		MessageBox(NULL, TEXT("创建设备失败"), TEXT(""), MB_OK);
		d3d9->Release();
		DestroyWindow(hwnd);
		return 0;
	}

	//赋值
	pEndSceneAddr = (*(void***)device)[42]; // EndScene是IDirect3DDevice9第43个函数

	// 释放
	d3d9->Release();
	device->Release();
	DestroyWindow(hwnd);//创建Windows只不过是需要
	return pEndSceneAddr;

}