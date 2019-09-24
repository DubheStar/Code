#include "GameData.h"

GAMEDATA* GAMEDATA::Instance = NULL;

GAMEDATA* GAMEDATA::Init()
{
	if (Instance == NULL)
	{
		Instance = new GAMEDATA();

		//数据初始化
		Instance->hGameModule = (DWORD)GetModuleHandleA("client_panorama.dll");
		Instance->hEngineModule = (DWORD)GetModuleHandleA("engine.dll");
		Instance->lpViewWorld = Instance->hGameModule + VIEWADDR;
		Instance->lpEnemyArray= Instance->hGameModule + ENEMYADDR;
		Instance->lpMySelf= Instance->hGameModule + SELFADDR;
		Instance->lpMouseXAngle = *(DWORD*)(Instance->hEngineModule + MOUSEADDR) + 0x4D8C;
		Instance->lpMouseYAngle = *(DWORD*)(Instance->hEngineModule + MOUSEADDR) + 0x4D88;
		Instance->hGame= FindWindow(L"Valve001", L"Counter-Strike: Global Offensive");
		Instance->lpLight= Instance->hGameModule + LIGHTADDR;
		Instance->lpGameScreenSize= Instance->hGameModule + WINDOWINFOADDR;
	}

	return Instance;
}
DWORD GAMEDATA::getObjectCamp(DWORD obj)
{
	return *(DWORD*)(obj+0xf4);
}
D3DXVECTOR3 GAMEDATA::getObjectPosition(DWORD obj)
{
	D3DXVECTOR3 tmp;
	tmp.x = *(FLOAT*)(obj + 0x138);
	tmp.y = *(FLOAT*)(obj + 0x13c);
	tmp.z = *(FLOAT*)(obj + 0x140);
	return tmp;
}
DWORD GAMEDATA::getObjectHealth(DWORD obj)
{
	return *(DWORD*)(obj + 0x100);
}
DWORD GAMEDATA::getD3dDistance(D3DXVECTOR3 self, D3DXVECTOR3 obj)
{
	float d = (obj.x - self.x)*(obj.x - self.x) + (obj.y - self.y)*(obj.y - self.y) + (obj.z - self.z)* (obj.z - self.z);
	return int(sqrt(d))/30;
}
AIMMOUSEANGLE GAMEDATA::getMouseAngle(D3DXVECTOR3 self, D3DXVECTOR3 obj)
{
	D3DXVECTOR3 AimTmp = { self .x-obj.x,self .y-obj.y,(self .z-obj.z)+65};
	AIMMOUSEANGLE AngleTmp;
	AngleTmp.x = atan(AimTmp.y /  AimTmp.x) / 3.14 * 180;
	if (AimTmp.x >= 0)
		AngleTmp.x += 180;
	else if(AngleTmp.x >= 180)
		AngleTmp.x -= 360;

	AngleTmp.y = atan(AimTmp.z / sqrt((AimTmp.x * AimTmp.x) + (AimTmp.y * AimTmp.y))) / 3.14 * 180;
	return AngleTmp;
}
D3DXVECTOR3 GAMEDATA::getBonePointPosition(DWORD obj, DWORD point)
{
	D3DXVECTOR3 tmpBonePos;
	DWORD BoneArrayHead = *(DWORD*)(obj + 0x26A8);
	BoneArrayHead += 0xc + (point - 1) * 0x30;	//48为下一个敌人骨骼X坐标
	tmpBonePos.x = *(FLOAT*)BoneArrayHead;
	tmpBonePos.y = *(FLOAT*)(BoneArrayHead+0x10);
	tmpBonePos.z = *(FLOAT*)(BoneArrayHead+0x20);
	return tmpBonePos;
}
DWORD GAMEDATA::getEnemyToCrosshair(DWORD CrosshairX, DWORD CrosshairY, DWORD EnemyX, DWORD EnemyY)
{
	float d = (EnemyX - CrosshairX)* (EnemyX - CrosshairX) + (EnemyY - CrosshairY)* (EnemyY - CrosshairY);
	return int(sqrt(d));
}
float** GAMEDATA::getViewWorld()
{
	//读取矩阵
	ViewWorld[0][0] = *(FLOAT*)(lpViewWorld);
	ViewWorld[0][1] = *(FLOAT*)(lpViewWorld+0x4);
	ViewWorld[0][2] = *(FLOAT*)(lpViewWorld+0x8);
	ViewWorld[0][3] = *(FLOAT*)(lpViewWorld+0xc);

	ViewWorld[1][0] = *(FLOAT*)(lpViewWorld+0x10);
	ViewWorld[1][1] = *(FLOAT*)(lpViewWorld + 0x14);
	ViewWorld[1][2] = *(FLOAT*)(lpViewWorld + 0x18);
	ViewWorld[1][3] = *(FLOAT*)(lpViewWorld + 0x1c);

	ViewWorld[2][0] = *(FLOAT*)(lpViewWorld + 0x20);
	ViewWorld[2][1] = *(FLOAT*)(lpViewWorld + 0x24);
	ViewWorld[2][2] = *(FLOAT*)(lpViewWorld + 0x28);
	ViewWorld[2][3] = *(FLOAT*)(lpViewWorld + 0x2c);

	ViewWorld[3][0] = *(FLOAT*)(lpViewWorld + 0x30);
	ViewWorld[3][1] = *(FLOAT*)(lpViewWorld + 0x34);
	ViewWorld[3][2] = *(FLOAT*)(lpViewWorld + 0x38);
	ViewWorld[3][3] = *(FLOAT*)(lpViewWorld + 0x3c);

	return (float**)ViewWorld;
}
DWORD GAMEDATA::getGameW()
{
	return *(DWORD*)Instance->lpGameScreenSize;
}
DWORD GAMEDATA::getGameH()
{
	return *(DWORD*)(Instance->lpGameScreenSize+0x4);
} 
DWORD GAMEDATA::getlpMySelf()
{
	return lpMySelf;
}
DWORD GAMEDATA::getlpEnemyArray()
{
	return lpEnemyArray;
}
DWORD GAMEDATA::getObjState(DWORD obj)
{
	return *(DWORD*)(obj + 0xED);
}
DWORD GAMEDATA::getlpLight()
{
	return lpLight;
}
DWORD GAMEDATA::getlpMouseXAngle()
{
	return lpMouseXAngle;
}
DWORD GAMEDATA::getlpMouseYAngle()
{
	return lpMouseYAngle;
}
D3DXVECTOR3 GAMEDATA::getObjToScreenPos(D3DXVECTOR3 objPos)
{
	D3DXVECTOR3 toScreen = { 0,0,0 };
	int SrceenW = GAMEDATA::Init()->getGameW() / 2;
	int ScreenH = GAMEDATA::Init()->getGameH() / 2;
	float* ViewWorld = (float*)GAMEDATA::Init()->getViewWorld();

	float ViewW = *(ViewWorld + 8)* objPos.x + *(ViewWorld + 9) * objPos.y + *(ViewWorld + 10) * objPos.z + *(ViewWorld + 11); //相机宽
	if (ViewW < 0.01f)
		return toScreen;

	ViewW = 1 / ViewW;
	toScreen.x = SrceenW + (*(ViewWorld)* objPos.x + *(ViewWorld + 1) * objPos.y + *(ViewWorld + 2) * objPos.z + *(ViewWorld + 3)) * ViewW * SrceenW;
	toScreen.y = ScreenH - (*(ViewWorld + 4) * objPos.x + *(ViewWorld + 5) * objPos.y + *(ViewWorld + 6) * (objPos.z + 76) + *(ViewWorld + 7)) * ViewW * ScreenH;
	toScreen.z = ScreenH - (*(ViewWorld + 4) * objPos.x + *(ViewWorld + 5) * objPos.y + *(ViewWorld + 6) * (objPos.z - 4) + *(ViewWorld + 7)) * ViewW * ScreenH;
	//异常处理
	if (toScreen.x < 0 && toScreen.y < 0)
		return { 0,0 ,0 };
	return toScreen;
}