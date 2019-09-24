#include"Hook.h"
#include"DrawFunc.h"
#include"GameMain.h"

void* endSceneAddr = NULL;
BYTE endSceneOldCode[5];//记录修改前的字节，用于之后还原

HWND hGame;
WNDPROC lpWndProc = nullptr;
bool isInit= false;

//imgui 窗体消息用的 
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

 LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
	 //消息 操作imgui用的
	 if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return 1;

	 return CallWindowProc(lpWndProc, hWnd, uMsg, wParam, lParam);
 }
 HRESULT STDMETHODCALLTYPE MyEndScene(IDirect3DDevice9* thiz)
{
	 if (!isInit)
	 {
		 isInit = true;
		 //初始化D3D设备
		 DIRECT3DDRAW::Init()->Initialize(thiz);
		 //初始化Imgui字体
		 ImGuiIO& io = ImGui::GetIO();
		 io.DeltaTime = 1.0f / 60.0f;
		 D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
		 thiz->GetCreationParameters(&d3dcp);
		 ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simkai.ttf", 14.0f, NULL, io.Fonts->GetGlyphRangesChinese());
		 ImGui_ImplDX9_Init(d3dcp.hFocusWindow, thiz);
		 ImGui::StyleColorsDark();
		 //开始HookEndScene
		 DIRECT3DHOOK::Init()->UnHook(endSceneAddr, endSceneOldCode);
		 HRESULT hr = thiz->EndScene();
		 DIRECT3DHOOK::Init()->SetHook(endSceneAddr, MyEndScene, endSceneOldCode);
		 return hr;
	 }
	 //核心
	GAMEMAIN::Init()->Start();
	//挂接EndScene
	DIRECT3DHOOK::Init()->UnHook(endSceneAddr, endSceneOldCode);
	HRESULT hr = thiz->EndScene();
	DIRECT3DHOOK::Init()->SetHook(endSceneAddr, MyEndScene, endSceneOldCode);
	return hr;
}

DWORD WINAPI initHookThread(LPVOID dllMainThread) 
{
	WaitForSingleObject(dllMainThread, INFINITE);
	CloseHandle(dllMainThread);
	endSceneAddr = DIRECT3DHOOK::Init()->getDirect3dEndSceneAddr(); 
	DIRECT3DHOOK::Init()->SetHook(endSceneAddr, MyEndScene, endSceneOldCode);// start hookEndScene
	return 0;
}
//dll 入口
BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// 取当前线程句柄
		HANDLE curThread;
		if (!DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &curThread, SYNCHRONIZE, FALSE, 0))
			return FALSE;
		// DllMain中不能使用COM组件，所以要在另一个线程初始化
		CloseHandle(CreateThread(NULL, 0, initHookThread, curThread, 0, NULL));
		hGame  = FindWindow(L"Valve001", L"Counter-Strike: Global Offensive");  //获取自身进程窗口句柄  
		lpWndProc = (WNDPROC)SetWindowLongA(hGame, GWLP_WNDPROC, (LONG)hkWndProc);
		if (lpWndProc == nullptr) return 1;
		ImGui::CreateContext(); //重点 在注入线程外面执行 要不然IMGui不起作用
		break;

	case DLL_PROCESS_DETACH:
		if (endSceneAddr != NULL)
			DIRECT3DHOOK::Init()->UnHook(endSceneAddr, endSceneOldCode);
		break;
	case DLL_THREAD_ATTACH:break;
	case DLL_THREAD_DETACH:break;
	}
	return TRUE;

}