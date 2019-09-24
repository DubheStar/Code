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
	DWORD getObjectCamp(DWORD obj);//ȡ������Ӫ
	D3DXVECTOR3 getObjectPosition(DWORD obj);//ȡ��������
	DWORD getObjectHealth(DWORD obj);//ȡ��������ֵ
	DWORD getD3dDistance(D3DXVECTOR3 self, D3DXVECTOR3 obj);//ȡD3D����
	AIMMOUSEANGLE getMouseAngle(D3DXVECTOR3 self, D3DXVECTOR3 obj);//ȡ�������Ƕ�
	D3DXVECTOR3 getBonePointPosition(DWORD obj,DWORD point);//ȡ����������
	DWORD getEnemyToCrosshair(DWORD CrosshairX,DWORD CrosshairY,DWORD EnemyX,DWORD EnemyY);//ȡ������׼�Ǿ���
	D3DXVECTOR3 getObjToScreenPos(D3DXVECTOR3 objPos);//��������ת��Ļ����
	DWORD getGameW();
	DWORD getGameH();
	DWORD getlpMySelf();
	DWORD getlpEnemyArray();
	DWORD getlpLight();//ȡ�����ַ
	DWORD getObjState(DWORD obj);//ˢ��״̬
	DWORD getlpMouseXAngle();//ȡ���X�Ƕȵ�ַ
	DWORD getlpMouseYAngle();//ȡ���Y�Ƕȵ�ַ
	float** getViewWorld();//ȡ����


private:
	static GAMEDATA * Instance;
	DWORD lpMouseXAngle, lpMouseYAngle;
	DWORD lpGameScreenSize;
	DWORD hGameModule, hEngineModule;
	DWORD lpViewWorld, lpEnemyArray, lpMySelf;
	DWORD lpLight;
	HWND hGame;
	float ViewWorld[4][4];//����
protected:
};
