#include "Awarenesss.h"
#include "Awareness.h"
#include "Cooldowns.h"
#include "Experience.h"
#include "LastHitDamage.h"
#include "LastPosition.h"
#include "Path.h"
#include "Sidebar.h"

void Awarenesss::DrawChampIcon(Object* obj, const ImVec2& fromPos, const ImVec2& toPos) {
	const auto champName = str_tolower(std::string(obj->GetName()));
	const auto it = ChampionSpells::_spell_map.find(champName);

	if (it != ChampionSpells::_spell_map.end())
		render::Texture2D(it->second, fromPos, toPos);
	else
		LOG("Texture %s non trovata", champName);
}

void Awarenesss::InitializeWardPosDrawingsMenu()
{
	const auto WardPosMenu = Configs::AwarenessMenu->AddMenu("WardMenu", "Ward Position Tracker");
	Configs::EnemyTracker::showWardPosition = WardPosMenu->AddCheckBox("showWardPosition", "Show ward position", true);
	Configs::EnemyTracker::showWardTimer = WardPosMenu->AddCheckBox("showWardTimer", "Show ward timer", true);
	Configs::EnemyTracker::showWardRange = WardPosMenu->AddCheckBox("showWardRange", "Show ward range", true);
}

void Awarenesss::Initialize()
{
	HPBar::InitializeMenu();
	HPBar::Experience::InitializeMenu();

	PathTracker::InitializeMenu();
	LastPosition::InitializeMenu();
	LastHitDamage::InitializeMenu();
	Sidebar::InitializeMenu();

	InitializeWardPosDrawingsMenu();
}