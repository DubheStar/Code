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
	DIRECT3DDRAW::Init()->PaintProcessBegin(); //�������Ϸ����ͼ��ʼ
	DrawMenu();
	DrawFunc();
	DIRECT3DDRAW::Init()->PaintProcessEnd();//-�������Ϸ����ͼ����
}
void GAMEMAIN::DrawMenu() 
{
	//---------------------------------------------����
	ImGui_ImplDX9_NewFrame();
	ImGui::Begin(u8"CSGO-PlaneJun����",0, ImGuiWindowFlags_NoResize);
	if (ImGui::CollapsingHeader(u8"͸����"))
	{
		ImGui::Checkbox(u8"����͸��", &no_BoxWall); ImGui::SameLine();
		ImGui::Checkbox(u8"����͸��", &no_BoneWall); ImGui::SameLine();
		ImGui::Checkbox(u8"��ʾ����_�ı�", &no_ShowDistance);
		ImGui::Checkbox(u8"����͸��", &no_HopWall); ImGui::SameLine();
		ImGui::Checkbox(u8"��ʾѪ��", &no_ShowHp); ImGui::SameLine();
		ImGui::Checkbox(u8"��ʾ����_ֱ��", &no_ShowLine);
	}
	if (ImGui::CollapsingHeader(u8"������"))
	{
		ImGui::Checkbox(u8"��������", &no_Aimbot);
		if (ImGui::BeginCombo(u8"���鰴��", item_current))
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
		ImGui::SliderInt(u8"���鷶Χ", &Aim_Scop, 0, 1000, u8"  %f");
		ImGui::Text(u8"���˾����ɺ�ɫʱ���������������ĵ���");
	}
	ImGui::End();
	//---------------------------------------------ˢ��
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
void GAMEMAIN::DrawFunc()
{
	SHORT isPress;
	int AimKey;
	//��������
	DWORD Temp_lpMyself = *(DWORD*)GAMEDATA::Init()->getlpMySelf();
	if (Temp_lpMyself == 0)
	{
		return;//δ����Ϸ
	}
	else
	{
		D3DXVECTOR3 ViewWorld;
		DWORD Temp_dwMycamp = GAMEDATA::Init()->getObjectCamp(Temp_lpMyself);
		D3DXVECTOR3 Temp_d3dMyPos = GAMEDATA::Init()->getObjectPosition(Temp_lpMyself);
		//��������
		for (int EnemyArrayindex = 0; EnemyArrayindex < 20; EnemyArrayindex++)
		{
			//����
			DWORD Temp_lpObjself = *(DWORD*)(GAMEDATA::Init()->getlpEnemyArray() + 0x10 * EnemyArrayindex);//������һ������
			if (Temp_lpObjself == 0)
			{
				continue;//����Ϊ0�������һ������
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
					continue;//����Ϊ���ѣ�����ʾֱ������
				else if (/*Temp_fObjhealth>0*/  Temp_dwObjreflash == 0)
				{
					ViewWorld = GAMEDATA::Init()->getObjToScreenPos(Temp_d3dObjPos);//ȡ��������Ļ����

					 //����͸��
					if (no_BoxWall)
						DIRECT3DDRAW::Init()->DrawRect(ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4, ViewWorld.y, (ViewWorld.z - ViewWorld.y) / 2, ViewWorld.z - ViewWorld.y, 1, D3DCOLOR_XRGB(0, 255, 0));
					//����
					if (no_ShowLine) 
						DIRECT3DDRAW::Init()->DrawLine((float)GAMEDATA::Init()->getGameW() / 2, 0, ViewWorld.x, ViewWorld.y, 1,D3DCOLOR_XRGB(255, 255, 255));
					//����͸��
					if (no_BoneWall) 
					{
						//��ϸ�������µĹ���ͼ
						int Temp_Point[12][2] = { {9,1},{40,12},{40,41},{41,15},{12,13},{13,34},{1,75},{75,76},{76,77},{1,68},{68,69},{69,70} };
						D3DXVECTOR3 Temp_Bone[2];
						for (int i = 0; i < 12; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								Temp_Bone[j] = GAMEDATA::Init()->getObjToScreenPos(GAMEDATA::Init()->getBonePointPosition(Temp_lpObjself, Temp_Point[i][j]));//��ȡ����
							}
							if (i == 0)
								DIRECT3DDRAW::Init()->DrawCircle(Temp_Bone[0].x, Temp_Bone[0].z, (Temp_Bone[0].z - Temp_Bone[0].y) / 8, D3DCOLOR_XRGB(255, 255, 255));//��ͷ����һ��Ȧ
							DIRECT3DDRAW::Init()->DrawLine(Temp_Bone[0].x, Temp_Bone[0].z, Temp_Bone[1].x, Temp_Bone[1].z, 1, D3DCOLOR_XRGB(255, 255, 255));//������
						}
					}
					//����͸��
					if (no_HopWall) 
					{
						/*
							
							mov [ecx+edx*8+24],bl  [[�����ַ]+[[�������]+A3F8]*38+24]
							movss [eax+ecx*8+04],xmm0    ��ɫ1
							movss [eax+ecx*8+08],xmm0   ��ɫ2
							movss [eax+ecx*8+0C],xmm0  ��ɫ3
							 movss [eax+ecx*8+10],xmm0  ��ɫ4
						*/
						DWORD Temp_LightAddr = *(DWORD*)GAMEDATA::Init()->getlpLight();
						DWORD Temp_ObjLight = *(DWORD*)(Temp_lpObjself + 0xA3F8);
						DWORD *Temp_Open =(DWORD*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x24);
						FLOAT* Temp_Color1 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x4);
						FLOAT* Temp_Color2 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x8);
						FLOAT* Temp_Color3 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0xC);
						FLOAT* Temp_Color4 = (FLOAT*)(Temp_LightAddr + Temp_ObjLight * 0x38 + 0x10);
						*Temp_Open = 1;//��
						*Temp_Color1 = 255;//д����ɫ1
						*Temp_Color2 =0;//д����ɫ2
						*Temp_Color3 =0;//д����ɫ3
						*Temp_Color4 = 255;//д����ɫ4
					}
					//��ʾ���־���
					if (no_ShowDistance) 
					{

						char szStr[999];
						WCHAR wszClassName[256];
						DWORD d = GAMEDATA::Init()->getD3dDistance(Temp_d3dMyPos, Temp_d3dObjPos);
						sprintf_s(szStr, "���˾���:%d",d);
						MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,sizeof(wszClassName) / sizeof(wszClassName[0]));
						if(d> Aim_Scop)
							DIRECT3DDRAW::Init()->_DrawText(wszClassName, { LONG(ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4 -20),(LONG)ViewWorld.z,200,100}, D3DCOLOR_XRGB(255,255,0));  //δ�������鷶Χ
						else
							DIRECT3DDRAW::Init()->_DrawText(wszClassName, {LONG( ViewWorld.x - (ViewWorld.z - ViewWorld.y) / 4 - 20),(LONG)ViewWorld.z ,200,100}, D3DCOLOR_XRGB(255, 0, 0)); //�������鷶Χ
					}
					//��ʾѪ��
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
							case 9:Temp_colr = D3DCOLOR_XRGB(0, 255, 0); break;//��ɫ
							case 8:Temp_colr = D3DCOLOR_XRGB(0, 255, 127); break;//����
							case 7:Temp_colr = D3DCOLOR_XRGB(154, 255, 154); break;//����
							case 6:Temp_colr = D3DCOLOR_XRGB(238, 238, 0); break;//�ۻ�
							case 5: 
							case 4:Temp_colr = D3DCOLOR_XRGB(255, 255, 0); break;//��ɫ
							case 3:
							case 2:
							case 1:Temp_colr = D3DCOLOR_XRGB(255, 0, 0); break;//��ɫ
							default:Temp_colr = D3DCOLOR_XRGB(255, 255, 255); break;//��ɫ
						}
						DIRECT3DDRAW::Init()->DrawLine(rHp.left, rHp.top, rHp.right, rHp.top + rHp.bottom, 4, Temp_colr);
					}
					//���鿪��
					if (no_Aimbot)
					{
						//�ж����鰴��
						switch (itemChoose)
						{
						case 0:AimKey = VK_LBUTTON; break;//MoseLeft
						case 1:AimKey = VK_RBUTTON; break;//MoseRight
						case 2:AimKey = VK_SHIFT; break;//Shift
						case 3:AimKey = VK_MENU; break;//Alt
						case 4:AimKey = VK_CONTROL; break;//Ctrl
						case 5:AimKey = VK_CAPITAL; break;//Caps
						}
						isPress =GetAsyncKeyState(AimKey) ^ 0xFFFF8001;  //���·���ֵΪ 0xFFFF8000  �������һ�·��ؾ���1
						if (isPress == 1 )
						{
							DWORD WorldD = GAMEDATA::Init()->getD3dDistance(Temp_d3dMyPos, Temp_d3dObjPos); //���鷶Χ
							DWORD CrossHairD = GAMEDATA::Init()->getEnemyToCrosshair(GAMEDATA::Init()->getGameW()/2, GAMEDATA::Init()->getGameH() / 2,ViewWorld.x,ViewWorld.y);
							if (WorldD < (DWORD)Aim_Scop )
							{
								if (CrossHairD < AIMCROSSHAIR)
								{
									//��������Ƕ�
									AIMMOUSEANGLE Temp_AIMANGLE = GAMEDATA::Init()->getMouseAngle(Temp_d3dMyPos, GAMEDATA::Init()->getBonePointPosition(Temp_lpObjself, 9));
									//д�����XY
									*(FLOAT*)GAMEDATA::Init()->getlpMouseXAngle() = Temp_AIMANGLE.x;
									*(FLOAT*)GAMEDATA::Init()->getlpMouseYAngle() = Temp_AIMANGLE.y;
								}

							}

							char szStr[999] ;
							WCHAR wszClassName[256];
							sprintf_s(szStr,"���ˣ�%d    ��꣺%d", WorldD,CrossHairD);
							MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,sizeof(wszClassName) / sizeof(wszClassName[0]));
							DIRECT3DDRAW::Init()->_DrawText(wszClassName, {0,0,300,100}, D3DCOLOR_XRGB(255,255,255));

						}
					}
				}
			}
		}
	}
}
