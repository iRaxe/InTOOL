#include "../Awareness.h"
#include "../Evade.h"
#include "../JustEvade.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../Orbwalker.h"
#include "Champions/Brand/Brand.h"
#include "Champions/Tristana/Tristana.h"
#include "../Yone.h"
#include "../Yasuo.h"
#include "../Illaoi.h"
#include "Champions/Ashe/Ashe.h"
#include "../Activator.h"
#include "Champions/Blitzcrank/Blitzcrank.h"
#include "Champions/Jinx/Jinx.h"
#include "Champions/Twitch/Twitch.h"

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
			UPasta::Plugins::Activator::Events::Initialize();
			//Evade::Core::Initalize();
		}
	}

	namespace Champions
	{
		static ChampionModule* activeChampModule = nullptr;

		void Init()
		{
			if (ObjectManager::GetLocalPlayer()->GetName() == "Brand") {
				UPasta::Plugins::Brand::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Tristana") {
				UPasta::Plugins::Tristana::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Illaoi") {
				UPasta::Plugins::Illaoi::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Yone") {
				UPasta::Plugins::Yone::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Yasuo") {
				UPasta::Plugins::Yasuo::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Ashe") {
				UPasta::Plugins::Ashe::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Blitzcrank") {
				UPasta::Plugins::Blitzcrank::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Twitch") {
				UPasta::Plugins::Twitch::Events::Initialize();
			}
			else if (ObjectManager::GetLocalPlayer()->GetName() == "Jinx") {
				UPasta::Plugins::Jinx::Events::Initialize();
			}
			else {
				activeChampModule = ChampionModuleManager::GetModule(ObjectManager::GetLocalPlayer()->GetName());
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
