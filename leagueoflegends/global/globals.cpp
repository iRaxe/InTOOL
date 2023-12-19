#include "globals.h"

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