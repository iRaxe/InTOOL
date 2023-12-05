#include "../Awareness.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../Orbwalker.h"

namespace Modules
{
	
	static float gameTime = 0.0f;
	static float nextRngBuffer = 0.0f;

	void Init()
	{
		Utilities::Init();
		Champions::Init();
	}

	void Update()
	{
		__try { Champions::Update(); }
		__except (1) { LOG("ERROR IN SCRIPTS -> CHAMPIONS UPDATE"); }
		
		if (UPasta::SDK::Orbwalker::Configs::status->Value)
		{
			__try {	UPasta::SDK::Orbwalker::Functions::Update();}
			__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER UPDATE"); }
		}
	}

	namespace Utilities
	{
		void Init()
		{
			UPasta::SDK::TargetSelector::Configs::Initialize();
			UPasta::SDK::Orbwalker::Functions::Initialize();
			UPasta::SDK::Awareness::Functions::Initialize();
		}
	}

	namespace Champions
	{
		static ChampionModule* activeChampModule = nullptr;

		void Init()
		{
			activeChampModule = ChampionModuleManager::GetModule(globals::localPlayer->GetName());
			if (!activeChampModule) return;

			activeChampModule->Initialize();
		}

		void Update()
		{
			if (!activeChampModule) return;

			activeChampModule->Update();

			switch (globals::scripts::orbwalker::orbwalkState)
			{
			case OrbwalkState::Attack:
				activeChampModule->Combo();
				break;
			case OrbwalkState::Clear:
				activeChampModule->Clear();
				break;
			case OrbwalkState::Harass:
				activeChampModule->Harass();
				break;
			case OrbwalkState::Lasthit:
				activeChampModule->Lasthit();
				break;
			case OrbwalkState::Flee:
				activeChampModule->Flee();
				break;
			}
		}

		void RenderUpdate()
		{
			if (!activeChampModule) 
				return;

			activeChampModule->Render();
		}

		void DoBeforeAttack() {
			if (!activeChampModule)
				return;

			activeChampModule->OnBeforeAttack();
		}

		void DoAfterAttack() {
			if (!activeChampModule)
				return;

			activeChampModule->OnAfterAttack();
		}

		void DoCreateMissile() {
			if (!activeChampModule)
				return;

			activeChampModule->OnCreateMissile();
		}

		void DoDeleteMissile() {
			if (!activeChampModule)
				return;

			activeChampModule->OnDeleteMissile();
		}
	}
}