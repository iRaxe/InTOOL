#include "../stdafx.h"

namespace globals
{
	QWORD moduleBase;
	MODULEINFO moduleInfo;

	bool eject = false;
	const char* renderType;
	bool hookResponse = false;
	ImVec2 menuSize;
	bool menuOpen = false;

	float windowWidth = 0.0f;
	float windowHeight = 0.0f;

	Object* localPlayer;
	ObjectManager* heroManager;
	ObjectManager* minionManager;
	ObjectManager* turretManager;
	ObjectManager* inhibitorsManager;
	ObjectManager* missileManager;
	ObjectManager* buildingsManager;
	ObjectManager* attackableManager;

	ObjectManager* objManager;
	LPDIRECT3DDEVICE9 pDeviceDX9var;

	ID3D11Device* pDeviceDX11var;

	namespace scripts
	{
		namespace orbwalker
		{
			OrbwalkState orbwalkState;
		}
	}
}