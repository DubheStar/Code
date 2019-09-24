#pragma once
#include<Windows.h>
#include <d3dx9.h>
#include<math.h>

#define  VIEWADDR 0x04CD4F24 
#define  ENEMYADDR 0x04CE350C
#define SELFADDR 0x00CD2764
#define MOUSEADDR 0x0058BCFC
#define LIGHTADDR 0x005223730
#define WINDOWINFOADDR 0x00C95A28

#define AIMBOTDISTANCE 1000
#define AIMCROSSHAIR 100

typedef struct MOUSEANGLE
{
	float x;
	float y;
}AIMMOUSEANGLE;

class GAMEDATA
{
public:
	static GAMEDATA* Init();
	DWORD getObjectCamp(DWORD obj);//取对象阵营
	D3DXVECTOR3 getObjectPosition(DWORD obj);//取对象坐标
	DWORD getObjectHealth(DWORD obj);//取对象生命值
	DWORD getD3dDistance(D3DXVECTOR3 self, D3DXVECTOR3 obj);//取D3D距离
	AIMMOUSEANGLE getMouseAngle(D3DXVECTOR3 self, D3DXVECTOR3 obj);//取鼠标自瞄角度
	D3DXVECTOR3 getBonePointPosition(DWORD obj,DWORD point);//取骨骼点坐标
	DWORD getEnemyToCrosshair(DWORD CrosshairX,DWORD CrosshairY,DWORD EnemyX,DWORD EnemyY);//取敌人与准星距离
	D3DXVECTOR3 getObjToScreenPos(D3DXVECTOR3 objPos);//世界坐标转屏幕坐标
	DWORD getGameW();
	DWORD getGameH();
	DWORD getlpMySelf();
	DWORD getlpEnemyArray();
	DWORD getlpLight();//取发光地址
	DWORD getObjState(DWORD obj);//刷新状态
	DWORD getlpMouseXAngle();//取鼠标X角度地址
	DWORD getlpMouseYAngle();//取鼠标Y角度地址
	float** getViewWorld();//取矩阵


private:
	static GAMEDATA * Instance;
	DWORD lpMouseXAngle, lpMouseYAngle;
	DWORD lpGameScreenSize;
	DWORD hGameModule, hEngineModule;
	DWORD lpViewWorld, lpEnemyArray, lpMySelf;
	DWORD lpLight;
	HWND hGame;
	float ViewWorld[4][4];//矩阵
protected:
};
