#pragma once
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"
#include<Windows.h>



class GAMEMAIN
{
public:
	static GAMEMAIN* Init();
	void Start(); //主程序
private:
	static GAMEMAIN *Instance;
	bool isRunning;
	bool no_BoxWall = false;//方框透视          
	bool no_ShowDistance = false;//显示距离
	bool no_ShowLine = false;//显示射线        
	bool no_ShowHp = false;//显示血量
	bool no_Aimbot = false;//自瞄开关
	bool no_BoneWall = false;//骨骼透视        
	bool no_HopWall = false;//热能透视
	int Aim_Scop = 50;//自瞄范围

	static char* items[];
	static  char* item_current;
	int itemChoose = 0;

	void DrawFunc();    //绘制功能
	void DrawMenu();  //绘制菜单
protected:
};
