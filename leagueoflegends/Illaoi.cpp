#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "./Orbwalker.h"
#include "Illaoi.h"

#include <unordered_set>

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Illaoi;
using namespace UPasta::Plugins::Illaoi::Config;
std::unordered_set<std::string> heroNames;
float IllaoiGameTime = 0.0f;

float IllaoiQCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastIllaoiQ() {
	return IllaoiGameTime > IllaoiQCastedTime + .75f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float IllaoiWCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastIllaoiW() {
	return IllaoiGameTime > IllaoiWCastedTime + .25 && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float IllaoiECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastIllaoiE() {
	return IllaoiGameTime > IllaoiECastedTime + .25f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float IllaoiRCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastIllaoiR() {
	return IllaoiGameTime > IllaoiRCastedTime + .5f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}



void Functions::InitializeMenu()
{
	const auto IllaoiMenu = Menu::CreateMenu("Jay Illaoi", "Champion Settings");

	const auto comboMenu = IllaoiMenu->AddMenu("Combo Settings", "Combo Settings");
	IllaoiCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	IllaoiCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	IllaoiCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	IllaoiCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	IllaoiCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = IllaoiMenu->AddMenu("Harass Settings", "Harass Settings");
	IllaoiHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	IllaoiHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	IllaoiHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	IllaoiHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

	const auto clearMenu = IllaoiMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	IllaoiClear::minMinions = laneClearMenu->AddSlider("minMinions", "Minimum Minions in range", 3, 1, 6, 1);
	IllaoiClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	IllaoiClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	IllaoiClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	IllaoiJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	IllaoiJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	IllaoiJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
	IllaoiLastHit::UseQ = lastHitMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	IllaoiLastHit::UseW = lastHitMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	IllaoiLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = IllaoiMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	IllaoiKillsteal::UseW = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	IllaoiKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	IllaoiKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);



	const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
	IllaoiFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	IllaoiSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.IllaoiQ.GetRange(), 100, database.IllaoiQ.GetRange(), 50);
	IllaoiSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	IllaoiSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", IllaoiSpellsSettings::GetWRange(), 100.0f, IllaoiSpellsSettings::GetWRange(), 50.0f);
	IllaoiSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	IllaoiSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.IllaoiE.GetRange(), 100, database.IllaoiE.GetRange(), 50);
	IllaoiSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	IllaoiSpellsSettings::rRange = rSpellMenu->AddSlider("maxRRange", "Maximum Range", 500.0f, 100, 500.0f, 50);
	IllaoiSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);

	IllaoiSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	IllaoiSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	IllaoiSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

std::unordered_set<std::string> GetEnemyHeroNames() {
	std::unordered_set<std::string> heroNamesTemp;

	// Iterate over the list of enemy heroes and add their names to the set
	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (target) {
			heroNamesTemp.insert(target->GetName());
		}
	}

	return heroNamesTemp;
}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	//TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
	//TryCatch(Event::Subscribe(Event::OnAfterAttack, &OnAfterAttack), "Error subscribing to OnAfterAttack event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	//TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");
	//TryCatch(Event::UnSubscribe(Event::OnAfterAttack, &OnAfterAttack), "Error unsubscribing to OnAfterAttack event");
}

void Functions::UseQ(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastIllaoiQ()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > IllaoiSpellsSettings::GetQRange()) return;

	Modules::prediction::PredictionOutput qPrediction;
	if (GetPrediction(database.IllaoiQ, qPrediction)) {
		Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
		IllaoiQCastedTime = IllaoiGameTime;
	}
}

void Functions::UseW(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastIllaoiW()) return;
	//if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > ObjectManager::GetLocalPlayer()->GetRealAttackRange()) return;
	Engine::CastSelf(SpellIndex::W);
	Orbwalker::Reset();
	IllaoiWCastedTime = IllaoiGameTime;
}

void Functions::UseE(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastIllaoiE()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > IllaoiSpellsSettings::GetERange()) return;

	Modules::prediction::PredictionOutput ePrediction;
	if (GetPrediction(database.IllaoiE, ePrediction)) {
		Engine::CastToPosition(SpellIndex::E, ePrediction.position);
		IllaoiECastedTime = IllaoiGameTime;
	}
}

void Functions::UseR() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastIllaoiR()) return;



	Engine::CastSelf(SpellIndex::R);
	IllaoiRCastedTime = IllaoiGameTime;
}

void Functions::DrawSpellRadius(float range) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), range, COLOR_WHITE, 1.0f);
	return;
}

void Functions::DrawDamageOnHPBar(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;

	const Vector2 objHPBarScreenPos = Engine::GetHpBarPosition(obj);
	if (!objHPBarScreenPos.IsValid()) return;

	const float wDamage = Damages::WSpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float comboDamage = wDamage + rDamage;

	static constexpr float yOffset = 23.5f;
	static constexpr float xOffset = -46.0f;
	static constexpr float widthMultiplier = 105;

	const float objHealth = obj->GetHealth();
	const float objMaxHealth = obj->GetMaxHealth();
	const float endOffset2 = xOffset + objHealth / objMaxHealth * widthMultiplier;
	const float startOffset2 = max(endOffset2 - (comboDamage / objMaxHealth * widthMultiplier), xOffset);

	const ImVec2 topLeft = ImVec2(objHPBarScreenPos.x + startOffset2, objHPBarScreenPos.y - yOffset);
	const ImVec2 bottomRight = ImVec2(objHPBarScreenPos.x + endOffset2, objHPBarScreenPos.y - yOffset + 10.0f);

	const bool canKill = comboDamage > objHealth;
	const auto drawColor = canKill ? COLOR_GREEN : COLOR_RED;
	render::RenderRect(topLeft, bottomRight, drawColor, 0.0f, 0, 1.0f, true);
}

void Functions::DrawDamageOnPos(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;

	const auto dmgPos = Engine::GetBaseDrawPosition(obj);
	if (!dmgPos.IsValid()) return;

	float yOffset = 0;
	if (isTimeToCastIllaoiQ()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}

	if (isTimeToCastIllaoiW()) {
		const float wDamage = Damages::WSpell::GetDamage(obj);
		render::RenderTextWorld("W: " + std::to_string(ceil(wDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastIllaoiR()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {
	if (IllaoiSpellsSettings::DrawQ->Value == true && (IllaoiSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastIllaoiQ() || !IllaoiSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(IllaoiSpellsSettings::GetQRange());

	if (IllaoiSpellsSettings::DrawW->Value == true && (IllaoiSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastIllaoiW() || !IllaoiSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(IllaoiSpellsSettings::GetWRange());

	if (IllaoiSpellsSettings::DrawE->Value == true && (IllaoiSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastIllaoiE() || !IllaoiSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(IllaoiSpellsSettings::GetERange());

	if (IllaoiSpellsSettings::DrawR->Value == true && (IllaoiSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastIllaoiR() || !IllaoiSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(IllaoiSpellsSettings::GetRRange());

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (IllaoiSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (IllaoiSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

Object* FindIllaoiSoul() {
	auto* minions = ObjectManager::GetMinionList();
	if (!minions) {
		return nullptr;
	}
	if (!TargetSelector::IsValid(TargetSelector::GetOverrideTarget(), ObjectManager::GetLocalPlayer()->GetPosition(), 1500)) {
		heroNames = heroNames = GetEnemyHeroNames();
		for (auto* minion : *minions) {
			if (minion) {
				if (TargetSelector::IsValid(minion, ObjectManager::GetLocalPlayer()->GetPosition(), 1500)) {
					std::string minionName = minion->GetName();

					// Check if the minion's name is in the set of hero names
					if (heroNames.find(minionName) != heroNames.end()) {
						Engine::PrintChat("Found hero minion: " + minionName);
						LOG("%s", minionName.c_str());
						
						return minion;
					}
				}

			}
		}
	}
	return nullptr;
}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	IllaoiGameTime = Engine::GetGameTime();

	if (Orbwalker::Mode == Attack) {
		Modes::Combo();
	}
	if (Orbwalker::Mode == Harass) {
		Modes::Harass();
	}
	if (Orbwalker::Mode == Clear || Orbwalker::Mode == FastClear) {
		Modes::Clear();
	}
	if (Orbwalker::Mode == Lasthit) {
		Modes::LastHit();
	}

}

float GetFullCombo(Object* rTarget)
{
	if (rTarget == nullptr) return -9999;

	float qDamage = 0, wDamage = 0, rDamage = 0;
	if (isTimeToCastIllaoiQ())
	{
		qDamage = Damages::QSpell::GetDamage(rTarget);
	}

	if (isTimeToCastIllaoiW())
	{
		wDamage = Damages::WSpell::GetDamage(rTarget) + Damages::Tenticle::GetDamage(rTarget);
	}

	if (isTimeToCastIllaoiR())
	{
		rDamage = Damages::RSpell::GetDamage(rTarget);
	}

	return  qDamage + wDamage + rDamage;
		
}

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;


	auto soul = FindIllaoiSoul();
	if (soul != nullptr)
	{
		TargetSelector::SetFocusTarget(soul);
	}

	if (IllaoiCombo::UseE->Value == true && isTimeToCastIllaoiE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			
			Functions::UseE(eTarget);
		}
	}

	if (IllaoiCombo::UseR->Value == true && isTimeToCastIllaoiR()) {
		auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetRRange());
		if (rTarget != nullptr)
		{
			if (rTarget->GetHealth() + rTarget->GetShield() < GetFullCombo(rTarget)
				and rTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) 
				<= IllaoiSpellsSettings::GetRRange() + rTarget->GetBoundingRadius() / 2)
			{
				Functions::UseR();
			}
		}
		int enemyCount = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetRRange());
		if (enemyCount >= IllaoiCombo::enemiesInRange->Value) {
			Functions::UseR();
		}
	}

	if (IllaoiCombo::UseQ->Value == true && isTimeToCastIllaoiQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
		if (qTarget != nullptr) {
			Functions::UseQ(qTarget);
		}
	}

	if (IllaoiCombo::UseW->Value == true && isTimeToCastIllaoiW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange() + 225.f);
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}




}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange());
	if (minionsInRange > 0) {
		for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy))
		{
			if (!minion) continue;



			if (IllaoiClear::UseQ->Value == true && isTimeToCastIllaoiQ()) {
				Functions::UseQ(minion);
			}

			if (IllaoiClear::UseW->Value == true && isTimeToCastIllaoiW()) {
				Functions::UseW(minion);
			}

		}
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
		if (jungleMonstersInRange > 0) {


			if (IllaoiJungle::UseQ->Value && isTimeToCastIllaoiQ()) {
				const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
				if (qMonster != nullptr) {
					Functions::UseQ(qMonster);
				}
			}

			if (IllaoiJungle::UseW->Value && isTimeToCastIllaoiW()) {
				const auto wMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange());
				if (wMonster != nullptr) {
					Functions::UseW(wMonster);
				}
			}
		}
	}
}

void Modes::LastHit() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (IllaoiLastHit::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;
	if (IllaoiLastHit::UseW->Value == false || isTimeToCastIllaoiW()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange());
	if (minionsInRange == 0) return;

	const auto wTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange(), Alliance::Enemy);
	if (wTarget == nullptr) return;
	if (wTarget->GetHealth() > Damages::WSpell::GetDamage(wTarget)) return;

	Functions::UseW(wTarget);
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (IllaoiHarass::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

	if (IllaoiHarass::UseE->Value == true && isTimeToCastIllaoiE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}
	}

	if (IllaoiHarass::UseQ->Value == true && isTimeToCastIllaoiQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
		if (qTarget != nullptr) {
			Functions::UseQ(qTarget);
		}
	}

	if (IllaoiHarass::UseW->Value == true && isTimeToCastIllaoiW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange() + 225.f);
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}

}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;


	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!target) continue;
		

		const float heroHealth = target->GetHealth() + target->GetShield();
		if (IllaoiKillsteal::UseQ->Value && isTimeToCastIllaoiQ() && heroHealth < Damages::QSpell::GetDamage(target)) {
			Functions::UseQ(target);
			break;
		}

		if (IllaoiKillsteal::UseW->Value && isTimeToCastIllaoiW() && heroHealth < Damages::WSpell::GetDamage(target)) {
			Functions::UseW(target);
			break;
		}

		if (IllaoiKillsteal::UseR->Value && isTimeToCastIllaoiR() && heroHealth < Damages::RSpell::GetDamage(target)) {
			Functions::UseR();
			break;
		}
	}
}



//void Events::OnAfterAttack() {
//	if (Orbwalker::Mode == Attack) {
//		if (IllaoiCombo::UseQ->Value && isTimeToCastIllaoiQ()) {
//			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
//			if (qTarget != nullptr) {
//				Functions::UseQ(qTarget);
//			}
//		}
//
//		if (IllaoiCombo::UseW->Value && isTimeToCastIllaoiW()) {
//			const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange());
//			if (wTarget != nullptr) {
//				Functions::UseW(wTarget);
//			}
//		}
//	}
//
//	if (Orbwalker::Mode == Clear) {
//		const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
//		if (minionsInRange > 0) {
//			if (IllaoiClear::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;
//			if (minionsInRange < IllaoiClear::GetMinimumMinions()) return;
//
//			if (IllaoiClear::UseQ->Value && isTimeToCastIllaoiQ()) {
//				const auto qTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange(), Alliance::Enemy);
//				if (qTarget != nullptr) {
//					Functions::UseQ(qTarget);
//				}
//			}
//			if (IllaoiClear::UseW->Value && isTimeToCastIllaoiW()) {
//				const auto wTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange(), Alliance::Enemy);
//				if (wTarget != nullptr) {
//					if (wTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) < ObjectManager::GetLocalPlayer()->GetRealAttackRange()) return;
//
//					Functions::UseW(wTarget);
//				}
//			}
//		}
//		else {
//			const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange());
//			if (jungleMonstersInRange > 0) {
//				if (IllaoiJungle::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;
//
//				if (IllaoiClear::UseW->Value && isTimeToCastIllaoiW()) {
//					if (IllaoiJungle::UseQ->Value && isTimeToCastIllaoiQ()) {
//						const auto qTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
//						if (qTarget != nullptr) {
//							Functions::UseQ(qTarget);
//						}
//					}
//
//					const auto wTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetWRange());
//					if (wTarget != nullptr) {
//						if (wTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) < ObjectManager::GetLocalPlayer()->GetRealAttackRange()) return;
//
//						Functions::UseW(wTarget);
//					}
//				}
//			}
//		}
//	}
//
//	if (Orbwalker::Mode == Harass) {
//		if (IllaoiHarass::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;
//
//		if (IllaoiHarass::UseQ->Value && isTimeToCastIllaoiQ()) {
//			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), IllaoiSpellsSettings::GetQRange());
//			if (qTarget != nullptr) {
//				Functions::UseQ(qTarget);
//			}
//		}
//	}
//}