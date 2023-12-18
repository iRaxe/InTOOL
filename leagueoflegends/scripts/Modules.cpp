#include "../Awareness.h"
#include "../Evade.h"
#include "../JustEvade.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../Orbwalker.h"
#include "Champions/Brand/Brand.h"
#include "Champions/Tristana/Tristana.h"
#include "../Yone.h"

namespace Modules
{

	static float gameTime = 0.0f;

	void Init()
	{
		Utilities::Init();
		Champions::Init();
	}

	void Update()
	{
		ObjectManager::Update();

		__try { Champions::Update(); }
		__except (1) { LOG("ERROR IN SCRIPTS -> CHAMPIONS UPDATE"); }
	}

	namespace Utilities
	{
		void Init()
		{
			TargetSelector::Initialize();
			UPasta::SDK::Awareness::Functions::Initialize();
			Orbwalker::Init();
			//Evade::Core::Initalize();
		}
	}

	namespace Champions
	{
		static ChampionModule* activeChampModule = nullptr;

		void Init()
		{
			if (globals::localPlayer->GetName() == "Brand") {
				UPasta::Plugins::Brand::Events::Initialize();
			}
			else if (globals::localPlayer->GetName() == "Tristana") {
				UPasta::Plugins::Tristana::Events::Initialize();
			}
			else if (globals::localPlayer->GetName() == "Yone") {
				UPasta::Plugins::Yone::Events::Initialize();
			}
			else {
				activeChampModule = ChampionModuleManager::GetModule(globals::localPlayer->GetName());
				if (!activeChampModule) return;

				activeChampModule->Initialize();
			}
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
