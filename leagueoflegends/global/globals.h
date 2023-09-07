#pragma once

#include "../stdafx.h"

namespace globals
{
	extern QWORD moduleBase;
	extern MODULEINFO moduleInfo;

	extern bool eject;
	extern const char* renderType;
	extern bool hookResponse;
	extern ImVec2 menuSize;
	extern bool menuOpen;

	extern float windowWidth;
	extern float windowHeight;

	extern Object* localPlayer;
	extern ObjectManager* heroManager;
	extern ObjectManager* minionManager;
	extern ObjectManager* turretManager;
	extern ObjectManager* inhibitorsManager;
	//extern ObjectManager* missileManager;


	extern ObjectManager* buildingsManager;
	extern ObjectManager* attackableManager;


	extern ObjectManager* objManager;

	//DX9
	extern LPDIRECT3DDEVICE9 pDeviceDX9var;
	extern ID3D11Device* pDeviceDX11var;

	namespace scripts
	{
		namespace orbwalker
		{
			extern OrbwalkState orbwalkState;
		}
	}
}