#include"DrawFunc.h"

DIRECT3DDRAW* DIRECT3DDRAW::Instance = NULL;

 DIRECT3DDRAW* DIRECT3DDRAW::Init()
{
	 if (Instance == NULL)
	 {
		 Instance = new DIRECT3DDRAW();
	 }

	 return Instance;
}
 void DIRECT3DDRAW::Initialize(IDirect3DDevice9* device)
 {	
	 if (pFontDevice == NULL)
	 {
		 pDevice = device;
		 //创建字体
		 D3DXCreateFontA(device, 18, 7, 400, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "微软雅黑", &pFontDevice);
		 //创建线条
		 D3DXCreateLine(device, &pLineDevice);
		 //创建场景
		 D3DXCreateSprite(device, &pSpriteDevice);
	 }
 }
void DIRECT3DDRAW::DrawLine(float x, float y, float xx, float yy,float width ,D3DCOLOR color)
{
	if (pLineDevice)
	{
		D3DXVECTOR2 dLine[2];

		pLineDevice->SetWidth(width);

		dLine[0].x = x;
		dLine[0].y = y;

		dLine[1].x = xx;
		dLine[1].y = yy;

		pLineDevice->Draw(dLine, 2, color);
	}
}
void DIRECT3DDRAW::DrawPoint( float x, float y, D3DCOLOR color)
{
	if (pLineDevice)
		DrawLine(x,y,x+1,y+1,1, color);
}
void DIRECT3DDRAW::DrawRect( float x, float y, float width, float height, float w, D3DCOLOR color)
{
	if (pLineDevice)
	{
		D3DXVECTOR2 points[5];
		points[0] = D3DXVECTOR2(x, y);
		points[1] = D3DXVECTOR2(x + width, y);
		points[2] = D3DXVECTOR2(x + width, y + height);
		points[3] = D3DXVECTOR2(x, y + height);
		points[4] = D3DXVECTOR2(x, y);
		pLineDevice->SetWidth(w);//宽度
		pLineDevice->Draw(points, 5, color);
	}
}
void DIRECT3DDRAW::DrawCircle(float x, float y, float r,  D3DCOLOR color)
{
	if (pLineDevice)
	{
		float d = 1.25f - r;
		float tx = 0,ty=r,fx=r/1.4f;
		while ((int)tx != (int)fx)
		{

			if (d < 0)
				d += 2 * tx + 3;
			else
			{
				d += 2 * (tx - ty) + 5;
				--ty;
			}

			//八分画法
			DrawPoint(x + tx,y + ty, color);
			DrawPoint(x + tx, y - ty, color);
			DrawPoint(x - tx, y + ty, color);
			DrawPoint(x - tx, y - ty, color);
			DrawPoint(x + ty, y - tx, color);
			DrawPoint(x + ty, y + tx, color);
			DrawPoint(x - ty, y +tx, color);
			DrawPoint(x - ty, y - tx, color);
			++tx;
		}
	}
}
void DIRECT3DDRAW::_DrawText(LPCWSTR text, RECT rect, D3DCOLOR color)
{
	if (pFontDevice)
	{
		RECT r = { rect.left,rect .top,rect .left+ rect .right,rect .top+ rect .bottom};
		pFontDevice->DrawText(NULL, text, -1, &r, DT_TOP | DT_LEFT, color);
	}
		
}
void DIRECT3DDRAW::LoadfTexture(LPCTSTR filePath, float x, float y, int w, int h)
{
	MyTexture* temp;
	IDirect3DTexture9 *pTexture;
	//HRESULT result = D3DXCreateTextureFromFile(pDevice,filePath, &pTexture);
	HRESULT result = D3DXCreateTextureFromFileEx(pDevice, 
																				filePath, 
																				D3DX_DEFAULT, 
																				D3DX_DEFAULT, 
																				1,
																				0, 
																				D3DFMT_A8R8G8B8,
																				D3DPOOL_MANAGED, 
																				1, 
																				-1, 
																				NULL,               //透明色，设定这个颜色，在显示时，这图像中的这个颜色将忽略
																				NULL,
																				NULL,
																				&pTexture);
	if (result != D3D_OK)
	{
		MessageBox(NULL, L"加载纹理失败", L"提示", 0);
		return;
	}

	temp = new MyTexture();
	temp->r = { 0,0,w,h };
	temp->d3dVector1 = { 0,0,0, };
	temp->d3dVector2 = { x,y,0, };
	temp->pTexture = pTexture;
	//加入容器列表
	VTexture.push_back(temp);
}
void DIRECT3DDRAW::Present()
{
	for (std::vector<MyTexture*>::iterator Vitem = VTexture.begin(); Vitem != VTexture.end(); ++Vitem)
	{
		
		//开始画图
		if (SUCCEEDED(pSpriteDevice->Begin(D3DXSPRITE_ALPHABLEND)))
		{
			pSpriteDevice->Draw((*Vitem)->pTexture, &(*Vitem)->r, &(*Vitem)->d3dVector1, &(*Vitem)->d3dVector2, D3DCOLOR_ARGB(255, 255, 255, 255));
			pSpriteDevice->End();
		}
	}
}
bool DIRECT3DDRAW::Clear()
{
	bool result = false;
	//释放所有纹理
	for (std::vector<MyTexture*>::iterator Vitem = VTexture.begin(); Vitem != VTexture.end(); ++Vitem)
	{
		(*Vitem)->pTexture->Release();
	}
	VTexture.clear();//清空容器
	pSpriteDevice->Release();
	pFontDevice->Release();
	pLineDevice->Release();
	pDevice->Release();
	result = true;
	return result;
}
void DIRECT3DDRAW::PaintProcessBegin()
{

	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &d3rsColorWrite);
	pDevice->GetVertexDeclaration(&vertexDeclaration);
	pDevice->GetVertexShader(&vertexShader);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
}
void DIRECT3DDRAW::PaintProcessEnd()
{
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, d3rsColorWrite);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	pDevice->SetVertexDeclaration(vertexDeclaration);
	pDevice->SetVertexShader(vertexShader);
}