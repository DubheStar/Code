#pragma once
#include<windows.h>
#include <d3dx9.h>
#include<math.h>
#include<vector>

typedef struct TEXTURESTRUCT
{
		RECT r;
		D3DXVECTOR3 d3dVector1;
		D3DXVECTOR3 d3dVector2;
		IDirect3DTexture9 *pTexture;
}MyTexture;

class DIRECT3DDRAW
{
public:
	static DIRECT3DDRAW* Init();
	void Initialize(IDirect3DDevice9* device);
	void PaintProcessBegin();
	void PaintProcessEnd();
	void DrawLine(float x, float y, float xx, float yy, float width,D3DCOLOR color);
	void DrawPoint(float x, float y, D3DCOLOR color);
	void DrawRect( float x, float y, float width, float height, float w, D3DCOLOR color);
	void DrawCircle(float x, float y, float r, D3DCOLOR color);
	void _DrawText( LPCWSTR text,RECT rect, D3DCOLOR color);
	void LoadfTexture(LPCTSTR filePath, float x, float y,int w,int h);//加载资源到容器
	void Present();//显示所有资源到屏幕上
	bool Clear();//释放所有资源
private:
	static DIRECT3DDRAW* Instance;
	IDirect3DDevice9* pDevice;
	ID3DXLine* pLineDevice;
	ID3DXFont* pFontDevice; 
	ID3DXSprite* pSpriteDevice;
	DWORD   d3rsColorWrite;
	IDirect3DVertexDeclaration9* vertexDeclaration;
	IDirect3DVertexShader9* vertexShader;
	std::vector<MyTexture*> VTexture;//存放贴图
protected:
};
