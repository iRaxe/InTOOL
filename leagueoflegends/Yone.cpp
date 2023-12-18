#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "./Orbwalker.h"
#include "Yone.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Yone;
using namespace UPasta::Plugins::Yone::Config;

float gameTimeYone = 0.0f;
Object* shadowYone = nullptr;

float QCastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastQYone() {
	return gameTimeYone > QCastedTimeYone && globals::localPlayer->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float WCastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastWYone() {
	return gameTimeYone > WCastedTimeYone + database.YoneW.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float ECastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastEYone() {
	return gameTimeYone > ECastedTimeYone + 0.22f && globals::localPlayer->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}


float RCastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastRYone() {
	return gameTimeYone > RCastedTimeYone + 0.25f && globals::localPlayer->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}


SpellIndex convertSlotNameToIndex(const std::string& slotName) {
	if (slotName == "Q") return SpellIndex::Q;
	if (slotName == "W") return SpellIndex::W;
	if (slotName == "E") return SpellIndex::E;
	if (slotName == "R") return SpellIndex::R;
	// Handle other cases if necessary
	return SpellIndex::Q; // Default or error case
}

void InitializeInterruptibleSpellsMenu(Menu* additionalMenu) {
	auto spellDictionary = Interuptables::createSpellDictionary();
	auto interruptibleMenu = additionalMenu->AddMenu("Interruptible Spells Settings", "Interruptible Spells");
	Interuptables::interruptibleMenuCheck = additionalMenu->AddCheckBox("Interupt Spells", "Interupt Spells", true);;
	auto interruptibleWhitelistMenu = interruptibleMenu->AddMenu("Whitelist Settings", "Whitelist");

	for (int i = 0; i < globals::heroManager->GetListSize(); i++)
	{
		auto hero = globals::heroManager->GetIndex(i);
		if (hero != nullptr && hero->IsEnemy())
		{
			if (hero == nullptr) {
				continue;
			}

			std::string championName = hero->GetName();

			// Check if the champion is in the predefined dictionary
			auto dictIt = spellDictionary.find(championName);
			if (dictIt == spellDictionary.end()) {
				continue; // Skip if the champion is not in the dictionary
			}

			auto championMenu = interruptibleWhitelistMenu->AddMenu((championName + " Interruptible Spells").c_str(), championName.c_str());

			// Initialize the selected spells vector with false for each spell slot
			Interuptables::selectedSpells[championName] = std::vector<bool>(dictIt->second.size(), false);

			for (size_t i = 0; i < dictIt->second.size(); ++i) {
				const auto& spell = dictIt->second[i];
				std::string slotName = spell.slot;
				SpellIndex slotIndex = convertSlotNameToIndex(slotName);
				auto spellObject = hero->GetSpellBySlotId(slotIndex);

				if (spellObject == nullptr) {
					continue;
				}

				std::string spellName = spellObject->GetName();

				auto checkbox = championMenu->AddCheckBox(
					spellName.c_str(),
					spellName.c_str(),
					false,
					[championName, i](const CheckBox* self, bool newValue) {
						Interuptables::selectedSpells[championName][i] = newValue;
					});

				if (checkbox->Value) {
					Interuptables::selectedSpells[championName][i] = true;
				}
			}
		}
	}
}





void Functions::InitializeMenu()
{
	const auto YoneMenu = Menu::CreateMenu("Jay.Yone", "Champion Settings");

	const auto comboMenu = YoneMenu->AddMenu("Combo Settings", "Combo Settings");
	YoneCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YoneCombo::UseQ3 = comboMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", true);
	YoneCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	YoneCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	YoneCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	

	const auto harassMenu = YoneMenu->AddMenu("Harass Settings", "Harass Settings");
	YoneHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YoneHarass::UseQ3 = harassMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", true);
	YoneHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	YoneHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	const auto clearMenu = YoneMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	YoneClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
	YoneClear::UseQ3 = laneClearMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", false);
	YoneClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", false);


	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	YoneJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YoneJungle::UseQ3 = jungleMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", false);
	YoneJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);


	const auto additionalMenu = YoneMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	YoneKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YoneKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	YoneKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

	InitializeInterruptibleSpellsMenu(additionalMenu);
	

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	YoneSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);
	YoneSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range For Q", 450.0f, 100, 450.0f, 50);
	YoneSpellsSettings::useQ3Tower = qSpellMenu->AddCheckBox("Use Q3 Under Tower", "Use Q3 Under Tower", true);
	YoneSpellsSettings::DrawQ3 = qSpellMenu->AddCheckBox("Draw Q3", "Draw Range", true);
	YoneSpellsSettings::q3Range = qSpellMenu->AddSlider("maxQ3Range", "Maximum Range For Q3", 1050.0f, 100, 1050.0f, 50);

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	YoneSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);
	YoneSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", 600.0f, 100, 600.0f, 50);
	

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	//YoneSpellsSettings::eBack = eSpellMenu->AddCheckBox("E When Target Can Die", "E When Target Can Die", true);
	YoneSpellsSettings::useETower = eSpellMenu->AddCheckBox("UseEUnderTower", "Use E Under Tower", true);
	YoneSpellsSettings::eBackEnemy = eSpellMenu->AddCheckBox("E2 When No Enemies Around", "E2 When No Enemies Around", false);
	YoneSpellsSettings::eBackAtHp = eSpellMenu->AddCheckBox("E2 When No Hp Is Low (Use Slider)", "E2 When No Hp Is Low (Use Slider)", false);
	YoneSpellsSettings::eBackEnemySlider = eSpellMenu->AddSlider("E2 No Enemies in Range Size", "E2 No Enemies in Range Size", database.YoneR.GetRange() + 450, 100.0f, 3000.0f, 50);
	YoneSpellsSettings::eBackHealthPercentSlider = eSpellMenu->AddSlider("E2 When less than X Percent", "E2 When less than X Percent", 30, 5, 100, 5);
	YoneSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", 725.0f, 100, 725.0f, 50);
	YoneSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	YoneSpellsSettings::rEnemyCount = rSpellMenu->AddSlider("Enemy In Ult Line count", "Enemy In Ult Line count", 2, 1, 5, 1);
	YoneSpellsSettings::rEnemyKnockUpOnly = rSpellMenu->AddCheckBox("R Only Knock Up Targets", "R Only Knock Up Targets From Yone", false);
	YoneSpellsSettings::rKillable = rSpellMenu->AddCheckBox("R Killable Enemies With Combo", "R Killable Enemies With Combo", true);
	YoneSpellsSettings::useRTower = rSpellMenu->AddCheckBox("Use R Under Tower", "Use R Under Tower", true);
	YoneSpellsSettings::rRange = rSpellMenu->AddSlider("maxRRange", "Maximum Range", database.YoneR.GetRange(), 100, database.YoneR.GetRange(), 50);
	YoneSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);

	YoneSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	YoneSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	YoneSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
	TryCatch(Event::Subscribe(Event::OnProcessSpell, &MyTThing), "Error subscribing to OnProcessSpell event");
	//TryCatch(Event::Subscribe(Event::OnCreateObject, &OnCreateObject), "Error subscribing to OnCreateObject event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");
	TryCatch(Event::UnSubscribe(Event::OnProcessSpell, &MyTThing), "Error unsubscribing to OnProcessSpell event");
	//TryCatch(Event::UnSubscribe(Event::OnCreateObject, &OnCreateObject), "Error unsubscribing to OnCreateObject event");
}

bool CanCastSpell(Object* obj, bool TimeToCast) {
	return globals::localPlayer != nullptr && 
		globals::localPlayer->IsAlive() &&
		globals::localPlayer->IsTargetable() && 
		obj != nullptr &&
		obj->IsAlive() && 
		obj->IsTargetable() &&
		Orbwalker::CanCastAfterAttack() && 
		TimeToCast;
}

void CastPrediction(SpellIndex SpellIndex,Skillshot spellDB,Object* obj, float& castedTimeVariable) {
	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex, obj->GetPosition());
		castedTimeVariable = gameTimeYone;
		return;
	}

	Modules::prediction::PredictionOutput Prediction;
	if (GetPrediction(spellDB, Prediction)) {
		Engine::CastToPosition(SpellIndex, Prediction.position);
		castedTimeVariable = gameTimeYone;
	}
}

void Functions::EOnMark(Object* obj) {

	if (!CanCastSpell(obj, isTimeToCastEYone())) return;

	if (EKills(obj)) {
		Engine::CastSelf(SpellIndex::E);
		ECastedTimeYone = gameTimeYone;
	}
}

bool Functions::EKills(Object* target) {
	if (target == nullptr) {
		return false;
	}
	if (target->IsAlive()) {
		return target->HasBuff("YoneEDeathmark");
	}
};

bool IsUnderTower(Vector3 pos,Alliance team)
{
	const auto turret = TargetSelector::FindTurret(pos, 992.0f, team);
	if (turret == nullptr) return false;
	if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) return false;

	return pos.distanceTo(turret->GetPosition()) < 992.0f;
}

float GetComboDamage(Object* obj) {
	if (globals::localPlayer == nullptr) return 0;
	if (!globals::localPlayer->IsAlive()) return 0;
	if (obj == nullptr) return 0;
	if (!obj->IsAlive()) return 0;
	if (!obj->IsTargetable()) return 0;
	float qDamage = 0, wDamage = 0, rDamage = 0, eDamage = 0;
	if (isTimeToCastQYone()) {
		qDamage = Damages::QSpell::GetDamage(obj);
		//LOG("SpellState:%d\nQ Damage: %.2f", Engine::GetSpellState(Q),qDamage);
	}
	if (isTimeToCastWYone()) {
		wDamage = Damages::WSpell::GetDamage(obj);
		/*LOG("W Damage: %.2f", wDamage);*/
	}
	if (isTimeToCastRYone()) {
		rDamage = Damages::RSpell::GetDamage(obj);
		/*LOG("E Damage: %.2f", rDamage);*/
	}
	if (isTimeToCastEYone() && !shadowYone) {
		eDamage = Damages::ESpell::GetDamage(obj, wDamage + qDamage + rDamage);
		/*LOG("R Damage: %.2f", eDamage);*/
	}
	//LOG("Full Combo: %.2f", qDamage + wDamage + eDamage + rDamage);
	return qDamage + wDamage + eDamage + rDamage;
}

void Functions::UseQ(Object* obj) {

	if (!CanCastSpell(obj, isTimeToCastQYone())) return;

	if (obj->GetDistanceTo(globals::localPlayer) > YoneSpellsSettings::GetQRange()) return;

	CastPrediction(SpellIndex::Q, database.YoneQ,obj,QCastedTimeYone);

}

void Functions::UseQ3(Object* obj) {

	if (!CanCastSpell(obj, isTimeToCastQYone())) return;

	if (obj->GetDistanceTo(globals::localPlayer) > YoneSpellsSettings::GetQ3Range()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::Q, obj->GetPosition());
		QCastedTimeYone = gameTimeYone;
		return;
	}

	Modules::prediction::PredictionOutput Prediction;
	if (GetPrediction(database.YoneQ2, Prediction)) {
		const auto pred = globals::localPlayer->GetPosition() + ((Prediction.position - globals::localPlayer->GetPosition()).Normalized() * 450);
		if (YoneSpellsSettings::useQ3Tower->Value == false and IsUnderTower(pred.Extend(Prediction.position, globals::localPlayer->GetBoundingRadius() / 2), Alliance::Enemy)) { return; }
		Engine::CastToPosition(SpellIndex::Q, pred);
		QCastedTimeYone = gameTimeYone;
	}

}

void Functions::UseW(Object* obj) {
	if (!CanCastSpell(obj, isTimeToCastWYone())) return;

	if (obj->GetDistanceTo(globals::localPlayer) > YoneSpellsSettings::GetWRange()) return;

	CastPrediction(SpellIndex::W, database.YoneW, obj, WCastedTimeYone);
}

void Functions::UseE(Object* obj) {
	if (!CanCastSpell(obj, isTimeToCastEYone())) return;
	if (YoneSpellsSettings::useETower->Value == false and IsUnderTower(obj->GetPosition(),Alliance::Enemy)) { return; }
	if (obj->GetDistanceTo(globals::localPlayer) > YoneSpellsSettings::GetERange()) return;

	Engine::CastToPosition(SpellIndex::E, obj->GetPosition());
	ECastedTimeYone = gameTimeYone;
}

Object* FindBestNiggaTarget(Vector3 from, float range) {
	Object* nigga = nullptr;

	std::vector<Object*> possible_targets;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {

		if (!hero) continue;

		if (!hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->GetPosition().Distance(from) <= range + hero->GetBoundingRadius() / 2)
			possible_targets.push_back(hero);

	}

	if (possible_targets.empty()) {
		nigga = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = globals::localPlayer;

				auto distance_first = me->GetDistanceTo(pFirst);
				auto distance_second = me->GetDistanceTo(pSecond);

				return distance_first > distance_second;

			});

	nigga = possible_targets.front();
	return nigga;
}


void EnemiesInALine() {
	int nig = 0;
	auto nigga = FindBestNiggaTarget(globals::localPlayer->GetPosition(), database.YoneR.GetRange());
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!hero->IsTargetable()) continue;
		if (!Modules::prediction::IsSpecificObjectInWay(globals::localPlayer->GetPosition(), nigga->GetPosition(), hero, database.YoneR.GetRadius())) continue;
		nig += 1;
		
		
	}
	if (nig >= YoneSpellsSettings::rEnemyCount->Value)
	{
		Modules::prediction::PredictionOutput rPrediction;
		if (GetPrediction(globals::localPlayer, nigga, database.YoneR, rPrediction)) {
			auto newPosition = globals::localPlayer->GetPosition().Extend(rPrediction.position, 1000.0f);

			Engine::CastToPosition(SpellIndex::R, newPosition);
			RCastedTimeYone = gameTimeYone;
		}

	}


}


void Functions::UseR(Object* obj, bool override = false) {
	if (!CanCastSpell(obj, isTimeToCastRYone())) return;


	if (obj->GetDistanceTo(globals::localPlayer) > YoneSpellsSettings::GetRRange()) return;

	//Line segmeant check

	Modules::prediction::PredictionOutput rPrediction;
	if (GetPrediction(database.YoneR, rPrediction)) {
		if (YoneSpellsSettings::useRTower->Value == false and IsUnderTower(rPrediction.position, Alliance::Enemy)) { return; }
		if (YoneSpellsSettings::rEnemyKnockUpOnly->Value == true and !obj->GetBuffByName("yoneq3knockup")) {
			return;
		}
		if (YoneSpellsSettings::rKillable->Value and GetComboDamage(obj) >= obj->GetHealth() + obj->GetShield()) {
			Engine::CastToPosition(SpellIndex::R, rPrediction.position);
			RCastedTimeYone = gameTimeYone;
		}
		
	}
	EnemiesInALine();
	



	
}

void Functions::DrawSpellRadius(float range) {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (!globals::localPlayer->IsTargetable()) return;

	Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), range, COLOR_WHITE, 1.0f);
	return;
}

void Functions::DrawDamageOnHPBar(Object* obj) {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	/*float qDamage = 0, wDamage = 0, rDamage = 0, eDamage = 0;*/
	const Vector2 objHPBarScreenPos = Engine::GetHpBarPosition(obj);
	
	/*const float qDamage = Damages::QSpell::GetDamage(obj);
	const float wDamage = Damages::WSpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float eDamage = Damages::ESpell::GetDamage(obj, Damages::WSpell::GetDamage(obj) + Damages::QSpell::GetDamage(obj) + Damages::RSpell::GetDamage(obj));*/

	const float comboDamage = GetComboDamage(obj);
	

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
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;

	const auto dmgPos = Engine::GetBaseDrawPosition(obj);
	if (!dmgPos.IsValid()) return;

	float yOffset = 0;
	float qDamage = 0;
	float wDamage = 0;
	float rDamage = 0;
	if (isTimeToCastQYone()) {
		qDamage = Damages::QSpell::GetDamage(obj);
		render::RenderTextWorld("Q: " + std::to_string(ceil(qDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}

	if (isTimeToCastWYone()) {
		wDamage = Damages::WSpell::GetDamage(obj);
		render::RenderTextWorld("W: " + std::to_string(ceil(wDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}

	if (isTimeToCastRYone()) {
		rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}

	if (isTimeToCastEYone()) {

		const float eDamage = Damages::ESpell::GetDamage(obj, qDamage + wDamage + rDamage);
		render::RenderTextWorld("E: " + std::to_string(ceil(eDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}
}

void Events::OnDraw() {
	if (YoneSpellsSettings::DrawQ->Value == true && (YoneSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastQYone() || !YoneSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YoneSpellsSettings::GetQRange());

	if (YoneSpellsSettings::DrawW->Value == true && (YoneSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastWYone() || !YoneSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YoneSpellsSettings::GetWRange());

	if (YoneSpellsSettings::DrawE->Value == true && (YoneSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastEYone() && !shadowYone|| !YoneSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YoneSpellsSettings::GetERange());

	if (YoneSpellsSettings::DrawE->Value == true && (YoneSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastEYone() && shadowYone || !YoneSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YoneSpellsSettings::GetE2Range());

	if (YoneSpellsSettings::DrawR->Value == true && (YoneSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastRYone() || !YoneSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YoneSpellsSettings::GetRRange());
	auto nig = 0;
	Vector3 bridge = Vector3(0, 0, 0);
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!hero->IsTargetable()) continue;
		if (hero->GetDistanceTo(globals::localPlayer) > 1500.0f) continue;
		
		if (YoneSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (YoneSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}




	}
	/*auto nigga = FindBestNiggaTarget(globals::localPlayer->GetPosition(), database.YoneR.GetRange());
	if (nigga != nullptr)
	{
		render::RenderCircleWorld(nigga->GetPosition(), 50, 300, COLOR_GREEN, 2);
	}
	render::RenderLineWorld(globals::localPlayer->GetPosition(), bridge, COLOR_RED, 2);
	Geometry::Polygon poly = Geometry::Rectangle(globals::localPlayer->GetPosition(), bridge, database.YoneR.GetRadius()).ToPolygon();
	render::RenderPolygonWorld(poly, COLOR_ORANGE, 1.0f);
	render::RenderTextWorld(std::to_string(nig), Engine::GetMouseWorldPos(), 30.0f, COLOR_WHITE, true);*/
	
}

Object* findShadow() {
	std::vector<Object*> minions = ObjectManager::GetMinions();
	for (auto* minion : minions) {
		if (minion && minion->GetDistanceTo(globals::localPlayer) <= 2000) {
			std::string minionName = minion->GetName();
			if (minionName == "TestCubeRender10Vision") {
				//Engine::PrintChat("Found minion with matching substring: " + minionName);
				//LOG("%s", minionName.c_str());
				return minion;
			}
		}
	}
	return nullptr;
}

void Events::OnGameUpdate() {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;

	gameTimeYone = Engine::GetGameTime();
	shadowYone = findShadow();
	Modes::Killsteal();
	Modes::Auto();

	// TODO: HANDLE IS EVADING SPELL
}

void Events::OnWndProc(UINT msg, WPARAM param) {
	if (param == OrbwalkerConfig::comboKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Combo(); break;
		case WM_KEYUP: break;
		}
	}

	if (param == OrbwalkerConfig::harassKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Harass(); break;
		case WM_KEYUP: break;
		}
	}

	if (param == OrbwalkerConfig::laneClearKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Clear(); break;
		case WM_KEYUP: break;
		}
	}

	if (param == OrbwalkerConfig::fastClearKey->Key) {
		switch (msg) {
		case WM_KEYDOWN: Modes::Clear(); break;
		case WM_KEYUP: break;
		}
	}

}

void EcHeck()
{
	int x = Engine::GetSpellState(E);
	LOG("NIGGA IS USING E : %d", x);

}

void Modes::Combo() {

	//for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
	//	if (!hero) continue;

	//	if (hero->GetPosition().Distance(globals::localPlayer->GetPosition()) > YoneSpellsSettings::GetRRange() + hero->GetBoundingRadius() / 2) continue;
	//	const SpellCast* spell = hero->GetActiveSpellCast()
	//	if (spell and spell->IsSpell())
	//	{
	//		Object* obj = ObjectManager::GetClientByHandle(spell->GetCasterHandle());
	//		int slot = spell->GetSpellId();
	//		if (obj != nullptr)
	//		{
	//			auto spell_name = obj->GetSpellBySlotId(slot)->GetName();
	//			LOG("%s CASTED!!!!!!!!! by %s", spell_name, obj->GetName());
	//		}

	//	}
	//}

	//if (YoneSpellsSettings::eBack->Value == true && isTimeToCastEYone() && shadowYone) {
	//	const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), 1000.00f);
	//	if (eTarget != nullptr and Functions::EKills(eTarget)) {
	//		Functions::EOnMark(eTarget);
	//	}
	//}
	/*EcHeck();*/
	
	if (YoneCombo::UseQ3->Value and isTimeToCastQYone()) {
		auto spell_name = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (spell_name == "YoneQ3") {
			const auto q3Target = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQ3Range());
			if (q3Target != nullptr) {
				Functions::UseQ3(q3Target);
			}
		}
	}
	

	if (YoneCombo::UseE->Value == true && isTimeToCastEYone()) {
		const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetERange());
		if (eTarget != nullptr and !shadowYone) {
			Functions::UseE(eTarget);

		}
	}

	if (YoneCombo::UseQ->Value) {
		const auto spell_name = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (spell_name != "YoneQ3") {
			const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}

	if (YoneCombo::UseW->Value == true && isTimeToCastWYone()) {
		const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}


	if (YoneCombo::UseR->Value == true && isTimeToCastRYone()) {
		const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetRRange());
		if (rTarget != nullptr) {
			Functions::UseR(rTarget);

		}


	}
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack())
	{
		return;
	}



	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), YoneSpellsSettings::GetWRange());
	if (minionsInRange > 0) {

		if (YoneClear::UseQ->Value && isTimeToCastQYone()) {
			const auto qTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQRange(), Alliance::Enemy);
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}

		if (YoneClear::UseQ3->Value && isTimeToCastQYone()) {
			const auto qTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQ3Range(), Alliance::Enemy);
			if (qTarget != nullptr) {
				Functions::UseQ3(qTarget);
			}
		}

		if (YoneClear::UseW->Value && isTimeToCastWYone()) {
			const auto wTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetWRange(), Alliance::Enemy);
			if (wTarget != nullptr) {
				Functions::UseW(wTarget);
			}
		}
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetWRange());
		if (jungleMonstersInRange > 0) {

			if (YoneJungle::UseQ->Value && isTimeToCastQYone()) {
				const auto qMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQRange());
				if (qMonster != nullptr) {
					Functions::UseQ(qMonster);
				}
			}

			if (YoneJungle::UseQ3->Value && isTimeToCastQYone()) {
				const auto q3Monster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQ3Range());
				if (q3Monster != nullptr) {
					Functions::UseQ3(q3Monster);
				}
			}

			if (YoneJungle::UseW->Value && isTimeToCastWYone()) {
				const auto wMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetWRange());
				if (wMonster != nullptr) {
					Functions::UseW(wMonster);
				}
			}
		}
	}
	
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack())
	{
		return;
	}

	if (YoneCombo::UseQ3->Value and isTimeToCastQYone()) {
		auto spell_name = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (spell_name == "YoneQ3") {
			const auto q3Target = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQ3Range());
			if (q3Target != nullptr) {
				Functions::UseQ3(q3Target);
			}
		}
	}


	if (YoneHarass::UseE->Value == true && isTimeToCastEYone()) {
		const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetERange());
		if (eTarget != nullptr and !shadowYone) {
			Functions::UseE(eTarget);

		}
	}

	if (YoneHarass::UseQ->Value) {
		const auto spell_name = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (spell_name != "YoneQ3") {
			const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}

	if (YoneHarass::UseW->Value == true && isTimeToCastWYone()) {
		const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), YoneSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}


}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;

		if (hero->GetPosition().Distance(globals::localPlayer->GetPosition()) > YoneSpellsSettings::GetRRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;



		const float heroHealth = hero->GetHealth() + hero->GetShield();
		if (YoneKillsteal::UseR->Value && isTimeToCastRYone() && heroHealth < Damages::RSpell::GetDamage(hero)) {
			Functions::UseR(hero);
			break;
		}
		if (YoneKillsteal::UseQ->Value && isTimeToCastQYone() && heroHealth < Damages::QSpell::GetDamage(hero)) {
			const auto spell_name = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (spell_name == "YoneQ3") {
				Functions::UseQ(hero);
				break;
			}
			Functions::UseQ3(hero);
			break;
		}


	}
}

void Modes::Auto() {
	if (!Orbwalker::CanCastAfterAttack()) return;



	if (shadowYone)
	{
		if (YoneSpellsSettings::eBackAtHp->Value) {
			const auto eBackTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), 500);
			if (eBackTarget != nullptr) {
				if (!CanCastSpell(eBackTarget, isTimeToCastEYone())) return;
				if (eBackTarget->GetDistanceTo(globals::localPlayer) > YoneSpellsSettings::GetERange() - YoneSpellsSettings::GetQRange()) return;

				float myHealthPercent = static_cast<float>(globals::localPlayer->GetHealth()) / globals::localPlayer->GetMaxHealth() * 100.0f;
				float healthThresholdPercent = static_cast<float>(YoneSpellsSettings::eBackHealthPercentSlider->Value);

				if (myHealthPercent <= healthThresholdPercent) {
					Engine::CastToPosition(SpellIndex::E, eBackTarget->GetPosition());
					ECastedTimeYone = gameTimeYone;
					return;
				}
			}
		}

		if (YoneSpellsSettings::eBackEnemy->Value) {
			if (ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), YoneSpellsSettings::GetE2Range()) == 0)
			{
				if (globals::localPlayer != nullptr &&
					globals::localPlayer->IsAlive() &&
					globals::localPlayer->IsTargetable() &&
					Orbwalker::CanCastAfterAttack() &&
					isTimeToCastEYone())
				{
					Engine::CastSelf(SpellIndex::E);
					ECastedTimeYone = gameTimeYone;
				}
			}
		}
	}
	

}


std::string getSlotNameFromIndex(SpellIndex index) {
	switch (index) {
	case SpellIndex::Q: return "Q";
	case SpellIndex::W: return "W";
	case SpellIndex::E: return "E";
	case SpellIndex::R: return "R";
	default: return "Unknown"; // Handle unknown or error cases
	}
}

void Events::MyTThing(int state, SpellCast* spellData) {
	if (Interuptables::interruptibleMenuCheck->Value == false) return;
	if (spellData == nullptr) return;
	if (state != 12) return;
	if (spellData->GetCasterHandle() == globals::localPlayer->GetHandle()) return;
	if (spellData->IsAutoAttack()) return;

	const auto caster = ObjectManager::GetClientByHandle(spellData->GetCasterHandle());
	if (caster == nullptr) return;
	if (!caster->IsHero()) return;
	if (caster->IsAlly()) return;

	auto spellID = spellData->GetSpellId();
	if (spellID < 0 || spellID > 5) return;

	Spell* spell = caster->GetSpellBySlotId(spellID);
	if (spell == nullptr) return;

	const int spellLevel = spell->GetLevel();
	if (spellLevel == 0) return;

	// Check if the spell is interruptible and selected for interruption
	auto spellDictionary = Interuptables::createSpellDictionary();
	std::string championName = caster->GetName();
	std::string slotName = getSlotNameFromIndex(static_cast<SpellIndex>(spellID));
	LOG("Casted Without Logic:%s is casting %s\n with id %d ", championName.c_str(), slotName.c_str(), spellID);
	// Find the index in the spellDictionary that matches the slotName
	auto dictIt = spellDictionary.find(championName);
	if (dictIt != spellDictionary.end()) {
		for (size_t i = 0; i < dictIt->second.size(); ++i) {
			if (dictIt->second[i].slot == slotName) {
				if (Interuptables::selectedSpells[championName][i]) {
					// Spell is interruptible and selected for interruption
					LOG("Interruptible Spell Detected: %s is casting %s\n", championName.c_str(), slotName.c_str());
					auto spell_name = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetName();
					if (spell_name == "YoneQ3") {
						if (caster != nullptr) {
							Functions::UseQ3(caster);
						}
					}
				}
				break;
			}
		}
	}
}




void OnCreateObject(Object* unit) {
	if (unit == nullptr) {
		return;
	}
	if (unit->GetDistanceTo(globals::localPlayer) <= 500.0f) {
		Engine::PrintChat(unit->GetName());
		return;
	}

	
	
}


