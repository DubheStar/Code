#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"
#include<Windows.h>



class GAMEMAIN
{
public:
	static GAMEMAIN* Init();
	void Start(); //������
private:
	static GAMEMAIN *Instance;
	bool isRunning;
	bool no_BoxWall = false;//����͸��          
	bool no_ShowDistance = false;//��ʾ����
	bool no_ShowLine = false;//��ʾ����        
	bool no_ShowHp = false;//��ʾѪ��
	bool no_Aimbot = false;//���鿪��
	bool no_BoneWall = false;//����͸��        
	bool no_HopWall = false;//����͸��
	int Aim_Scop = 50;//���鷶Χ

	static char* items[];
	static  char* item_current;
	int itemChoose = 0;

	void DrawFunc();    //���ƹ���
	void DrawMenu();  //���Ʋ˵�
protected:
};
