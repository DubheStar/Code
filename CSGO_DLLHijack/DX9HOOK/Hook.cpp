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
		MessageBox(NULL, TEXT("ע�ᴰ����ʧ��"), TEXT(""), MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(0, wc.lpszClassName, TEXT(""), WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, wc.hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("��������ʧ��"), TEXT(""), MB_OK);
		return 0;
	}

	// ��ʼ��D3D

	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d9 == NULL)
	{
		MessageBox(NULL, TEXT("����D3Dʧ��"), TEXT(""), MB_OK);
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
		MessageBox(NULL, TEXT("�����豸ʧ��"), TEXT(""), MB_OK);
		d3d9->Release();
		DestroyWindow(hwnd);
		return 0;
	}

	//��ֵ
	pEndSceneAddr = (*(void***)device)[42]; // EndScene��IDirect3DDevice9��43������

	// �ͷ�
	d3d9->Release();
	device->Release();
	DestroyWindow(hwnd);//����Windowsֻ��������Ҫ
	return pEndSceneAddr;

}