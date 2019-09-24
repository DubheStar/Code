#include"GameMain.h"
#include"GameData.h"
#include"DrawFunc.h"


GAMEMAIN* GAMEMAIN::Instance = NULL;
char* GAMEMAIN::items[] = { "MoseLeft","MoseRight","Shift", "Alt", "Ctrl", "Caps" };
char* GAMEMAIN::item_current = items[0];

GAMEMAIN* GAMEMAIN::Init()
{
	if (Instance == NULL)
	{
		Instance = new GAMEMAIN();
	}

	return Instance;
}
void GAMEMAIN::Start()
{
	DIRECT3DDRAW::Init()->PaintProcessBegin(); //处理进游戏不绘图开始
	DrawMenu();
	DrawFunc();
	DIRECT3DDRAW::Init()->PaintProcessEnd();//-处理进游戏不绘图结束
}
void GAMEMAIN::DrawMenu() 
{
	//---------------------------------------------绘制
	ImGui_ImplDX9_NewFrame();
	ImGui::Begin(u8"CSGO-PlaneJun辅助",0, ImGuiWindowFlags_NoResize);
	if (ImGui::CollapsingHeader(u8"透视区"))
	{
		ImGui::Checkbox(u8"方框透视", &no_BoxWall); ImGui::SameLine();
		ImGui::Checkbox(u8"骨骼透视", &no_BoneWall); ImGui::SameLine();
		ImGui::Checkbox(u8"显示距离_文本", &no_ShowDistance);
		ImGui::Checkbox(u8"热能透视", &no_HopWall); ImGui::SameLine();
		ImGui::Checkbox(u8"显示血量", &no_ShowHp); ImGui::SameLine();
		ImGui::Checkbox(u8"显示距离_直线", &no_ShowLine);
	}
	if (ImGui::CollapsingHeader(u8"自瞄区"))
	{
		ImGui::Checkbox(u8"启动自瞄", &no_Aimbot);
		if (ImGui::BeginCombo(u8"自瞄按键", item_current))
		{
			for (int n = 0; n < 6; n++)
			{
				bool is_selected = (item_current == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
				{
					item_current = items[n];
					itemChoose = n;
				}	
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SliderInt(u8"自瞄范围", &Aim_Scop, 0, 1000, u8"  %f");
		ImGui::Text(u8"敌人距离变成红色时会自瞄离鼠标最近的敌人");
	}
	ImGui::End();
	//---------------------------------------------刷新
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
void GAMEMAIN::DrawFunc()
{
	SHORT isPress;
	int AimKey;
	//人物自身
	DWORD Temp_lpMyself = *(DWORD*)GAMEDATA::Init()->getlpMySelf();
	if (Temp_lpMyself == 0)
	{
		return;//未进游戏
	}
	else
	{
		D3DXVECTOR3 ViewWorld;
		DWORD Temp_dwMycamp = GAMEDATA::Init()->getObjectCamp(Temp_lpMyself);
		D3DXVECTOR3 Temp_d3dMyPos = GAMEDATA::Init()->getObjectPosition(Temp_lpMyself);
		//遍历对象
		for (int EnemyArrayindex = 0; EnemyArrayindex < 20; EnemyArrayindex++)
		{
			//对象
			DWORD Temp_lpObjself = *(DWORD*)(GAMEDATA::Init()->getlpEnemyArray() + 0x10 * EnemyArrayindex);//遍历下一个对象
			if (Temp_lpObjself == 0)
			{
				continue;//对象为0则遍历下一个对象
			}
			else
			{
				DWORD Temp_dwObjcamp = GAMEDATA::Init()->getObjectCamp(Temp_lpObjself);
				D3DXVECTOR3 Temp_d3dObjPos = GAMEDATA::Init()->getObjectPosition(Temp_lpObjself);
				DWORD Temp_fObjhealth = GAMEDATA::Init()->getObjectHealth(Temp_lpObjself);
				DWORD Temp_dwObjreflash = GAMEDATA::Init()->getObjState(Temp_lpObjself);

				if (Temp_lpMyself == Temp_lpObjself)
					continue;
				else if (Temp_dwMycamp == Temp_dwObjcamp)
					continue;//对象为队友，不显示直接跳过
				else if (/*Temp_fObjhealth>0*/  Temp_dwObjreflash == 0)
				{
					ViewWorld = GAMEDATA::Init()->getObjToScreenPos(Temp_d3dObjPos);//取对象在屏幕坐标

					 //方框透视
					if (no_BoxWall)
						DIRECT3DDRAW::Init()->DrawRect(ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4, ViewWorld.y, (ViewWorld.z - ViewWorld.y) / 2, ViewWorld.z - ViewWorld.y, 1, D3DCOLOR_XRGB(0, 255, 0));
					//射线
					if (no_ShowLine) 
						DIRECT3DDRAW::Init()->DrawLine((float)GAMEDATA::Init()->getGameW() / 2, 0, ViewWorld.x, ViewWorld.y, 1,D3DCOLOR_XRGB(255, 255, 255));
					//骨骼透视
					if (no_BoneWall) 
					{
						//详细看工程下的骨骼图
						int Temp_Point[12][2] = { {9,1},{40,12},{40,41},{41,15},{12,13},{13,34},{1,75},{75,76},{76,77},{1,68},{68,69},{69,70} };
						D3DXVECTOR3 Temp_Bone[2];
						for (int i = 0; i < 12; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								Temp_Bone[j] = GAMEDATA::Init()->getObjToScreenPos(GAMEDATA::Init()->getBonePointPosition(Temp_lpObjself, Temp_Point[i][j]));//读取骨骼
							}
							if (i == 0)
								DIRECT3DDRAW::Init()->DrawCircle(Temp_Bone[0].x, Temp_Bone[0].z, (Temp_Bone[0].z - Temp_Bone[0].y) / 8, D3DCOLOR_XRGB(255, 255, 255));//在头部画一个圈
							DIRECT3DDRAW::Init()->DrawLine(Temp_Bone[0].x, Temp_Bone[0].z, Temp_Bone[1].x, Temp_Bone[1].z, 1, D3DCOLOR_XRGB(255, 255, 255));//画身体
						}
					}
					//热能透视
					if (no_HopWall) 
					{
						/*
							
							mov [ecx+edx*8+24],bl  [[发光地址]+[[人物对象]+A3F8]*38+24]
							movss [eax+ecx*8+04],xmm0    颜色1
							movss [eax+ecx*8+08],xmm0   颜色2
							movss [eax+ecx*8+0C],xmm0  颜色3
							 movss [eax+ecx*8+10],xmm0  颜色4
						*/
						DWORD Temp_LightAddr = *(DWORD*)GAMEDATA::Init()->getlpLight();
						DWORD Temp_ObjLight = *(DWORD*)(Temp_lpObjself + 0xA3F8);
						DWORD *Temp_Open =(DWORD*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x24);
						FLOAT* Temp_Color1 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x4);
						FLOAT* Temp_Color2 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x8);
						FLOAT* Temp_Color3 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0xC);
						FLOAT* Temp_Color4 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x10);
						*Temp_Open = 1;//打开
						*Temp_Color1 = 255;//写入颜色1
						*Temp_Color2 =0;//写入颜色2
						*Temp_Color3 =0;//写入颜色3
						*Temp_Color4 = 255;//写入颜色4
					}
					//显示文字距离
					if (no_ShowDistance) 
					{

						char szStr[999];
						WCHAR wszClassName[256];
						DWORD d = GAMEDATA::Init()->getD3dDistance(Temp_d3dMyPos, Temp_d3dObjPos);
						sprintf_s(szStr, "敌人距离:%d",d);
						MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,sizeof(wszClassName) / sizeof(wszClassName[0]));
						if(d> Aim_Scop)
							DIRECT3DDRAW::Init()->_DrawText(wszClassName, { LONG(ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4 -20),(LONG)ViewWorld.z,200,100}, D3DCOLOR_XRGB(255,255,0));  //未进入自瞄范围
						else
							DIRECT3DDRAW::Init()->_DrawText(wszClassName, {LONG( ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4 - 20),(LONG)ViewWorld.z ,200,100}, D3DCOLOR_XRGB(255, 0, 0)); //进入自瞄范围
					}
					//显示血量
					if (no_ShowHp)
					{
						RECT rHp;
						DWORD Temp_dwHP = GAMEDATA::Init()->getObjectHealth(Temp_lpObjself);
						D3DCOLOR Temp_colr;
						rHp.left = ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4-4;//x1
						rHp.top = ViewWorld.y;//y1
						rHp.right = ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4-4;//x2
						rHp.bottom = (ViewWorld.z - ViewWorld.y) / 100 * Temp_dwHP;//y2
						switch (Temp_dwHP/10)
						{
							case 10:
							case 9:Temp_colr = D3DCOLOR_XRGB(0, 255, 0); break;//绿色
							case 8:Temp_colr = D3DCOLOR_XRGB(0, 255, 127); break;//青绿
							case 7:Temp_colr = D3DCOLOR_XRGB(154, 255, 154); break;//青绿
							case 6:Temp_colr = D3DCOLOR_XRGB(238, 238, 0); break;//嫩黄
							case 5: 
							case 4:Temp_colr = D3DCOLOR_XRGB(255, 255, 0); break;//黄色
							case 3:
							case 2:
							case 1:Temp_colr = D3DCOLOR_XRGB(255, 0, 0); break;//红色
							default:Temp_colr = D3DCOLOR_XRGB(255, 255, 255); break;//白色
						}
						DIRECT3DDRAW::Init()->DrawLine(rHp.left, rHp.top, rHp.right, rHp.top + rHp.bottom, 4, Temp_colr);
					}
					//自瞄开关
					if (no_Aimbot)
					{
						//判断自瞄按键
						switch (itemChoose)
						{
						case 0:AimKey = VK_LBUTTON; break;//MoseLeft
						case 1:AimKey = VK_RBUTTON; break;//MoseRight
						case 2:AimKey = VK_SHIFT; break;//Shift
						case 3:AimKey = VK_MENU; break;//Alt
						case 4:AimKey = VK_CONTROL; break;//Ctrl
						case 5:AimKey = VK_CAPITAL; break;//Caps
						}
						isPress =GetAsyncKeyState(AimKey) ^ 0xFFFF8001;  //按下返回值为 0xFFFF8000  异或运算一下返回就是1
						if (isPress == 1 )
						{
							DWORD WorldD = GAMEDATA::Init()->getD3dDistance(Temp_d3dMyPos, Temp_d3dObjPos); //自瞄范围
							DWORD CrossHairD = GAMEDATA::Init()->getEnemyToCrosshair(GAMEDATA::Init()->getGameW()/2, GAMEDATA::Init()->getGameH() / 2,ViewWorld.x,ViewWorld.y);
							if (WorldD < (DWORD)Aim_Scop )
							{
								if (CrossHairD < AIMCROSSHAIR)
								{
									//计算自瞄角度
									AIMMOUSEANGLE Temp_AIMANGLE = GAMEDATA::Init()->getMouseAngle(Temp_d3dMyPos, GAMEDATA::Init()->getBonePointPosition(Temp_lpObjself, 9));
									//写入鼠标XY
									*(FLOAT*)GAMEDATA::Init()->getlpMouseXAngle() = Temp_AIMANGLE.x;
									*(FLOAT*)GAMEDATA::Init()->getlpMouseYAngle() = Temp_AIMANGLE.y;
								}

							}

							char szStr[999] ;
							WCHAR wszClassName[256];
							sprintf_s(szStr,"敌人：%d    鼠标：%d", WorldD,CrossHairD);
							MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,sizeof(wszClassName) / sizeof(wszClassName[0]));
							DIRECT3DDRAW::Init()->_DrawText(wszClassName, {0,0,300,100}, D3DCOLOR_XRGB(255,255,255));

						}
					}
				}
			}
		}
	}
}
