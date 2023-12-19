#include "Awarenesss.h"
#include "Awareness.h"
#include "Cooldowns.h"
#include "Experience.h"
#include "LastHitDamage.h"
#include "LastPosition.h"
#include "ListManager.h"
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

//TODO: MOVE IN A PROPER FILE
void Awarenesss::PushCooldowns(int state, SpellCast* spellCastInfo) {
	if (spellCastInfo == nullptr) return;
	if (state != 12) return;
	if (spellCastInfo->GetCasterHandle() == ObjectManager::GetLocalPlayer()->GetHandle()) return;
	if (spellCastInfo->IsAutoAttack()) return;

	const auto caster = ObjectManager::GetClientByHandle(spellCastInfo->GetCasterHandle());
	if (caster == nullptr) return;
	if (!caster->IsHero()) return;
	if (caster->IsAlly()) return;

	const auto spellID = spellCastInfo->GetSpellId();
	if (spellID < 0 || spellID > 5) return;

	Spell* spell = caster->GetSpellBySlotId(spellID);
	if (spell == nullptr) return;

	const int spellLevel = spell->GetLevel();
	if (spellLevel == 0) return;

	const auto cooldownValue = spell->GetSpellInfo()->GetSpellData()->GetCooldownArray()->GetArrayIndex(spellLevel)->GetBaseCooldown();

	const auto reduction = 100 / (100 + caster->GetAbilityHaste());
	const auto readyAt = Engine::GetGameTime() + cooldownValue * reduction;
	ListManager::Functions::InsertCooldown(caster, spellID, readyAt);
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
	TryCatch(Event::Subscribe(Event::OnProcessSpell, &PushCooldowns), "Error subscribing to OnProcessSpell event for cooldowns");

}