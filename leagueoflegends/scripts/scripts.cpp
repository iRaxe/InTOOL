#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../Orbwalker.h"

namespace scripts
{
	static float gameTime = 0.0f;
	static float nextRngBuffer = 0.0f;

	std::vector<std::pair<std::string, std::vector<std::string>>> settingsOrder = {};

	void AddSetting(std::string group, std::string key, settings::SettingValue value, settings::SettingValue min, settings::SettingValue max)
	{
		std::pair<std::string, std::vector<std::string>>* groupData = nullptr;
		for (auto& pair : settingsOrder)
			if (pair.first == group)
				groupData = &pair;

		if (groupData) {
			groupData->second.push_back(key);
		}
		else {
			settingsOrder.push_back({ group, {key} });
		}

		settings::Get(group, key, value);

		if (std::holds_alternative<int>(value))
			settings::AddBounds(group, key, std::get<int>(min), std::get<int>(max));
		if (std::holds_alternative<float>(value))
			settings::AddBounds(group, key, std::get<float>(min), std::get<float>(max));
	}

	void Init()
	{
		srand(static_cast <unsigned> (time(0)));

		drawings::Init();

		UPasta::SDK::TargetSelector::Configs::Initialize();
		UPasta::SDK::Orbwalker::Functions::Initialize();

		debug::Init();
		champions::Init();
	}

	void Update()
	{
		__try { champions::Update(); }
		__except (1) { LOG("ERROR IN SCRIPTS -> CHAMPIONS UPDATE"); }
		
		if (UPasta::SDK::Orbwalker::Configs::status->Value)
		{
			__try {	UPasta::SDK::Orbwalker::Functions::Update();}
			__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER UPDATE"); }
		}
	}

	namespace drawings
	{
		void Init()
		{
			ADD_SETTING("Drawings", "Draw AA Range", false);
			ADD_SETTING("Drawings", "Draw EnemyList", false);
			ADD_SETTING("Drawings", "Draw BuffList", false);
			ADD_SETTING("Drawings", "Draw Spells cooldown", false);
		}
	}

	namespace debug
	{
		void Init()
		{
			ADD_SETTING("debug", "draw object data", false);
			ADD_SETTING("debug", "draw bounding radius", false);
			ADD_SETTING("debug", "draw cursor world", false);
			ADD_SETTING("debug", "draw player paths", false);
		}
	}

	namespace champions
	{
		static ChampionModule* activeChampModule = nullptr;

		void Init()
		{
			activeChampModule = ChampionModuleManager::GetModule(globals::localPlayer->GetName());
			if (!activeChampModule) return;

			activeChampModule->Init();
		}

		void Update()
		{
			if (!activeChampModule) return;

			activeChampModule->Update();

			switch (globals::scripts::orbwalker::orbwalkState)
			{
			case OrbwalkState::Attack:
				activeChampModule->Attack();
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

		void DoBeforeAttack()
		{
			if (!activeChampModule)
				return;

			activeChampModule->OnBeforeAttack();
		}

		void DoCastSpell()
		{
			if (!activeChampModule)
				return;

			activeChampModule->OnCastSpell();
		}
	}
}