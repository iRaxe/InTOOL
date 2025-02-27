#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "./Orbwalker.h"
#include "Yone.h"

#include "Yasuo.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Yone;
using namespace UPasta::Plugins::Yone::Config;

float gameTimeYone = 0.0f;
Object* shadowYone = nullptr;

float QCastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastQYone() {
	return gameTimeYone > QCastedTimeYone && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float WCastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastWYone() {
	return gameTimeYone > WCastedTimeYone + database.YoneW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float ECastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastEYone() {
	return gameTimeYone > ECastedTimeYone + 0.22f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}


float RCastedTimeYone = 0.0f;
[[nodiscard]] bool isTimeToCastRYone() {
	return gameTimeYone > RCastedTimeYone + 0.25f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
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

	for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++)
	{
		auto hero = ObjectManager::GetHeroList()->GetIndex(i);
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

	const auto autoMenu = additionalMenu->AddMenu("Auto Settings", "Auto Settings");
	YoneAuto::UseQ = autoMenu->AddCheckBox("Use Q", "Use SpellSlot Q to Stack", true);

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
	TryCatch(Event::Subscribe(Event::OnProcessSpell, &MyTThing), "Error subscribing to OnProcessSpell event");
	TryCatch(Event::Subscribe(Event::OnAfterAttack, &OnAfterAttack), "Error subscribing to OnAfterAttack event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	TryCatch(Event::UnSubscribe(Event::OnProcessSpell, &MyTThing), "Error unsubscribing to OnProcessSpell event");
	TryCatch(Event::UnSubscribe(Event::OnAfterAttack, &OnAfterAttack), "Error unsubscribing to OnAfterAttack event");
}

int CountEnemiesInLineWidth(Vector3 sourcePos, Vector3 endPos, Object* target, float lineWidth) {
	int enemiesInLine = 0;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!hero->IsTargetable()) continue;
		if (!Modules::prediction::IsSpecificObjectInWay(sourcePos, endPos, target, lineWidth)) continue;
		enemiesInLine += 1;
	}

	return enemiesInLine;
}

float GetComboDamage(Object* obj)
{
	float qDamage = 0, wDamage = 0, eDamage = 0, rDamage = 0;
	if (isTimeToCastQYone()) {
		qDamage = Damages::QSpell::GetDamage(obj);
	}
	if (isTimeToCastWYone()) {
		wDamage = Damages::WSpell::GetDamage(obj);
	}
	if (isTimeToCastEYone()) {
		rDamage = Damages::RSpell::GetDamage(obj);
	}

	if (isTimeToCastRYone()) {
		eDamage = Damages::ESpell::GetDamage(obj, wDamage + qDamage + rDamage);
	}

	return qDamage + wDamage + eDamage + rDamage;
}

void Functions::UseQ(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastQYone()) return;

	const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
	if (qSpellSlotName == "YoneQ3") return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetQRange()) return;

	Engine::CastToPosition(SpellIndex::Q, obj->GetPosition());
	QCastedTimeYone = gameTimeYone;
}

bool IsUnderTower(Vector3 pos, Alliance team)
{
	const auto turret = TargetSelector::FindTurret(pos, 992.0f, team);
	if (turret == nullptr) return false;
	if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) return false;

	return pos.Distance(turret->GetPosition()) < 992.0f;
}

void Functions::UseQ3(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastQYone()) return;

	const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
	if (qSpellSlotName != "YoneQ3") return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetQ3Range()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::Q, obj->GetPosition());
		QCastedTimeYone = gameTimeYone;
		return;
	}

	
	Modules::prediction::PredictionOutput q3Prediction;
	if (GetPrediction(ObjectManager::GetLocalPlayer(), obj, database.YoneR, q3Prediction)) {
		const Vector3 dashPosition = ObjectManager::GetLocalPlayer()->GetPosition().Extend(q3Prediction.position, YoneSpellsSettings::GetQ3Range());
		if (YoneSpellsSettings::useQ3Tower->Value == false && IsUnderTower(dashPosition, Alliance::Enemy)) return;
		Engine::CastToPosition(SpellIndex::Q, dashPosition);
		QCastedTimeYone = gameTimeYone;
	}

}

void Functions::UseW(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastWYone()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetWRange()) return;

	Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
	WCastedTimeYone = gameTimeYone;
}

void Functions::UseE(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;
	if (shadowYone != nullptr) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastEYone()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetERange()) return;

	if (YoneSpellsSettings::useETower->Value == false and obj->IsUnderTower(Alliance::Enemy)) return;

	Engine::CastToPosition(SpellIndex::E, obj->GetPosition());
	ECastedTimeYone = gameTimeYone;
}

bool CanUseR(Vector3 pos, Object* obj)
{
	
	const auto enemyTurret = TargetSelector::FindTurret(pos, 992.0f, Alliance::Enemy);
	if (YoneSpellsSettings::useRTower->Value == false && enemyTurret != nullptr && pos.Distance(enemyTurret->GetPosition()) < 992.0f) return false;
	if (YoneSpellsSettings::rEnemyKnockUpOnly->Value == true and !obj->GetBuffByName("yoneq3knockup")) return false;
	return true;
}

bool YoneCanKillWithCombo(Object* obj)
{
	return YoneSpellsSettings::rKillable->Value && GetComboDamage(obj) > obj->GetHealth() + obj->GetShield();
}

void UseRMultipleTargets(Object* obj)
{
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastRYone()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetRRange()) return;
	auto farestEnemy = TargetSelector::FindFarestBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), database.YoneR.GetRange());
	if (farestEnemy == nullptr) return;

	Modules::prediction::PredictionOutput rPrediction;
	if (GetPrediction(ObjectManager::GetLocalPlayer(), obj, database.YoneR, rPrediction)) {
		int enemiesInLine = CountEnemiesInLineWidth(ObjectManager::GetLocalPlayer()->GetPosition(), rPrediction.position, farestEnemy, database.YoneR.GetRadius());

		const Vector3 dashPosition = ObjectManager::GetLocalPlayer()->GetPosition().Extend(rPrediction.position, 1000.0f);
		if (CanUseR(dashPosition, obj)) {
			if (enemiesInLine >= YoneSpellsSettings::rEnemyCount->Value)
			{
				Engine::CastToPosition(SpellIndex::R, dashPosition);
				RCastedTimeYone = gameTimeYone;
			}
		}
	}
}

void KillWithR(Object* obj)
{
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastRYone()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetRRange()) return;

	Modules::prediction::PredictionOutput rPrediction;
	
	if (Modules::prediction::GetPrediction(ObjectManager::GetLocalPlayer(), obj, database.YoneR, rPrediction)) {
		const Vector3 dashPosition = ObjectManager::GetLocalPlayer()->GetPosition().Extend(rPrediction.position, 1000.0f);
		if (CanUseR(dashPosition, obj)) {
			Engine::CastToPosition(SpellIndex::R, dashPosition);
			RCastedTimeYone = gameTimeYone;
		}
	}
}

void Functions::UseR(Object* obj, bool override = false) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastRYone()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetRRange()) return;
	



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
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;

	const auto dmgPos = Engine::GetBaseDrawPosition(obj);
	if (!dmgPos.IsValid()) return;
	const float qDamage = Damages::QSpell::GetDamage(obj);
	const float wDamage = Damages::WSpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float eDamage = Damages::ESpell::GetDamage(obj, wDamage + qDamage + rDamage);
	//LOG("Shield: %.2f", obj->GetShield());
	//LOG("PhysicalShield: %.2f", obj->GetPhysicalShield());
	//LOG("MagicShield: %.2f", obj->GetMagicalShield());
	//LOG("BaseArmor: %.2f", obj->GetBaseArmor());
	//LOG("BonusArmor: %.2f", obj->GetBonusArmor());
	float yOffset = 0;
	if (isTimeToCastQYone()) {
		render::RenderTextWorld("Q: " + std::to_string(ceil(qDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}

	if (isTimeToCastWYone()) {
		render::RenderTextWorld("W: " + std::to_string(ceil(wDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}

	if (isTimeToCastRYone()) {
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 30;
	}

	if (isTimeToCastEYone()) {
		render::RenderTextWorld("E: " + std::to_string(ceil(eDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}


Object* FindYoneShadow() {


	auto* minions = ObjectManager::GetMinionList();
	if (!minions) {
		return nullptr;
	}

	for (auto* minion : *minions) { // Dereference minions if it's a pointer to a container
		if (minion && minion->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= 2000) {
			std::string minionName = minion->GetName();
			if (minionName == "TestCubeRender10Vision") {
				/*Engine::PrintChat("Found minion with matching substring: " + minionName);
				LOG("%s", minionName.c_str());*/
				return minion;
			}
		}
	}
	return nullptr;
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

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!hero->IsTargetable()) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;
		
		if (YoneSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (YoneSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	gameTimeYone = Engine::GetGameTime();

	if (Orbwalker::Mode == Attack) {
		Modes::Combo();
	}
	if (Orbwalker::Mode == Harass) {
		Modes::Harass();
	}
	if (Orbwalker::Mode == Clear) {
		Modes::Clear();
	}

	shadowYone = FindYoneShadow();
	Modes::Killsteal();
	Modes::Auto();
}

void Modes::Combo() {

	if (YoneCombo::UseQ->Value == true && isTimeToCastQYone()) {
		const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (qSpellSlotName == "YoneQ3" && YoneCombo::UseQ3->Value == true) {
			const auto q3Target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
			if (q3Target != nullptr) {
				Functions::UseQ3(q3Target);
			}
		}
		else {
			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}

	if (YoneCombo::UseE->Value == true && isTimeToCastEYone()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetERange());
		if (eTarget != nullptr and !shadowYone) {
			Functions::UseE(eTarget);

		}
	}

	if (YoneCombo::UseW->Value == true && isTimeToCastWYone()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}

	if (YoneCombo::UseR->Value == true && isTimeToCastRYone()) {
		const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetRRange());
		if (rTarget != nullptr) {
			if (YoneCanKillWithCombo(rTarget))
			{
				KillWithR(rTarget);

			}else
			{
				UseRMultipleTargets(rTarget);
			}
			
		}
	}
	
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
	if (minionsInRange > 0) {
		for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy))
		{
			if (YoneClear::UseW->Value && isTimeToCastWYone()) {
				Functions::UseW(minion);
			}
			if (YoneClear::UseQ->Value == true && isTimeToCastQYone()) {
				const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
				if (qSpellSlotName == "YoneQ3" && YoneClear::UseQ3->Value == true) {
					Functions::UseQ3(minion);
				}
				else {
					Functions::UseQ(minion);
				}
			}
		}
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
		if (jungleMonstersInRange > 0) {
			if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange()) > 0)
			{
				if (YoneJungle::UseW->Value && isTimeToCastWYone()) {
					const auto wMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetWRange());
					if (wMonster != nullptr) {
						Functions::UseW(wMonster);
					}
				}
			}
			else
			{
				if (YoneJungle::UseQ->Value == true && isTimeToCastQYone()) {
					const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
					if (qSpellSlotName == "YoneQ3" && YoneJungle::UseQ3->Value == true) {
						const auto q3Target = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
						if (q3Target != nullptr) {
							Functions::UseQ3(q3Target);
						}
					}
					else {
						const auto qTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
						if (qTarget != nullptr) {
							Functions::UseQ(qTarget);
						}
					}
				}
			}
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange()) > 0)
	{
		if (YoneHarass::UseW->Value == true && isTimeToCastWYone()) {
			const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetWRange());
			if (wTarget != nullptr) {
				Functions::UseW(wTarget);
			}
		}
	}
	else
	{
		if (YoneHarass::UseQ->Value == true && isTimeToCastQYone()) {
			const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (qSpellSlotName == "YoneQ3" && YoneHarass::UseQ3->Value == true) {
				const auto q3Target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
				if (q3Target != nullptr) {
					Functions::UseQ3(q3Target);
				}
			}
			else {
				const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
				if (qTarget != nullptr) {
					Functions::UseQ(qTarget);
				}
			}
		}

		if (YoneHarass::UseE->Value == true && isTimeToCastEYone()) {
			const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetERange());
			if (eTarget != nullptr and !shadowYone) {
				Functions::UseE(eTarget);

			}
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetRRange()) continue;
		if (hero->IsInvulnerable()) continue;

		const float heroHealth = hero->GetHealth() + hero->GetShield();
		if (YoneKillsteal::UseR->Value && isTimeToCastRYone() && heroHealth < Damages::RSpell::GetDamage(hero)) {
			Functions::UseR(hero);
		}

		if (YoneKillsteal::UseQ->Value && isTimeToCastQYone() && heroHealth < Damages::QSpell::GetDamage(hero)) {
			const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (qSpellSlotName == "YoneQ3") {
					Functions::UseQ3(hero);
			}
			else {
					Functions::UseQ(hero);
			}
		}
	}
}


void Modes::Auto() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (!ObjectManager::GetLocalPlayer()->IsUnderTower(Alliance::Enemy)){
		if (YoneAuto::UseQ->Value == true && isTimeToCastQYone()) {
			const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (qSpellSlotName != "YoneQ3" && YoneHarass::UseQ->Value == true) {
				const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
				if (qTarget != nullptr) {
					Functions::UseQ(qTarget);
				}
			}
		}
	}

	if (shadowYone)
	{
		if (YoneSpellsSettings::eBackAtHp->Value) {
			const auto eBackTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), 500);
			if (eBackTarget != nullptr) {
				if (!isTimeToCastEYone()) return;
				if (eBackTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YoneSpellsSettings::GetERange() - YoneSpellsSettings::GetQRange()) return;

				float myHealthPercent = static_cast<float>(ObjectManager::GetLocalPlayer()->GetHealth()) / ObjectManager::GetLocalPlayer()->GetMaxHealth() * 100.0f;
				float healthThresholdPercent = static_cast<float>(YoneSpellsSettings::eBackHealthPercentSlider->Value);

				if (myHealthPercent <= healthThresholdPercent) {
					Engine::CastSelf(SpellIndex::E);
					ECastedTimeYone = gameTimeYone;
					return;
				}
			}
		}

		if (YoneSpellsSettings::eBackEnemy->Value) {
			if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetE2Range()) == 0)
			{
				if (ObjectManager::GetLocalPlayer() != nullptr &&
					ObjectManager::GetLocalPlayer()->IsAlive() &&
					ObjectManager::GetLocalPlayer()->IsTargetable() &&
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
	if (spellData->GetCasterHandle() == ObjectManager::GetLocalPlayer()->GetHandle()) return;
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
					auto spell_name = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
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

void Events::OnAfterAttack() {
	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack) {
		if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange()) > 0)
		{
			if (YoneCombo::UseW->Value == true && isTimeToCastWYone()) {
				const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetWRange());
				if (wTarget != nullptr) {
					Functions::UseW(wTarget);
				}
			}
		}
		if (YoneCombo::UseQ->Value == true && isTimeToCastQYone()) {
			const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (qSpellSlotName == "YoneQ3" && YoneCombo::UseQ3->Value == true) {
				const auto q3Target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
				if (q3Target != nullptr) {
					Functions::UseQ3(q3Target);
				}
			}
			else {
				const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
				if (qTarget != nullptr) {
					Functions::UseQ(qTarget);
				}
			}
		}
	}

	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear) {

		const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetWRange());
		if (minionsInRange > 0) {
			if (YoneClear::UseQ->Value == true && isTimeToCastQYone()) {
				const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
				if (qSpellSlotName == "YoneQ3" && YoneClear::UseQ3->Value == true) {
					const auto q3Target = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range(), Alliance::Enemy);
					if (q3Target != nullptr) {
						Functions::UseQ3(q3Target);
					}
				}
				else {
					const auto qTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange(), Alliance::Enemy);
					if (qTarget != nullptr) {
						Functions::UseQ(qTarget);
					}
				}
			}
		}
		else {
			const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetWRange());
			if (jungleMonstersInRange > 0) {
				if (YoneJungle::UseQ->Value == true && isTimeToCastQYone()) {
					const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
					if (qSpellSlotName == "YoneQ3" && YoneJungle::UseQ3->Value == true) {
						const auto q3Target = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
						if (q3Target != nullptr) {
							Functions::UseQ3(q3Target);
						}
					}
					else {
						const auto qTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
						if (qTarget != nullptr) {
							Functions::UseQ(qTarget);
						}
					}
				}
			}
		}

	}

	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass) {
		if (YoneHarass::UseQ->Value == true && isTimeToCastQYone()) {
			const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (qSpellSlotName == "YoneQ3" && YoneHarass::UseQ3->Value == true) {
				const auto q3Target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQ3Range());
				if (q3Target != nullptr) {
					Functions::UseQ3(q3Target);
				}
			}
			else {
				const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YoneSpellsSettings::GetQRange());
				if (qTarget != nullptr) {
					Functions::UseQ(qTarget);
				}
			}
		}
	}
}
