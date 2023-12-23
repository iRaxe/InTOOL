#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "./Orbwalker.h"
#include "Yasuo.h"

#include "Scripts/Champions/Ezreal/Ezreal.h"


using namespace UPasta::SDK;
using namespace UPasta::Plugins::Yasuo;
using namespace UPasta::Plugins::Yasuo::Config;

float Yasuogametime = 0.0f;

float YasuoQCastedTime = 0.0f;

float getQCastTime()
{
	float bonusAttackSpeed = ObjectManager::GetLocalPlayer()->GetAttackSpeed(); 
	float reduction = 0.035f * (bonusAttackSpeed / 0.24); 

	// Cap the reduction at 50% (which occurs at 120% bonus attack speed)
	if (reduction > 0.175)
	{
		reduction = 0.175;
	}

	return 0.35f - reduction; // Apply the reduction
}

float getQ3CastTime()
{
	float bonusAttackSpeed = ObjectManager::GetLocalPlayer()->GetAttackSpeed(); 

	// Calculate reduction
	float reduction;
	if (bonusAttackSpeed <= 0.48)
	{
		
		reduction = 0.035f * (bonusAttackSpeed / 0.24);
	}
	else
	{
		
		reduction = 0.07;
	}

	return 0.35f - reduction; 
}

[[nodiscard]] bool isTimeToCastYasuoQ() {
	return Yasuogametime > YasuoQCastedTime + getQCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

[[nodiscard]] bool isTimeToCastYasuoQDash() {
	return Yasuogametime > YasuoQCastedTime + getQ3CastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}


float YasuoWCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastYasuoW() {
	return Yasuogametime > YasuoWCastedTime + 0.013f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float YasuoECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastYasuoE() {
	return Yasuogametime > YasuoECastedTime + .25f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float YasuoRCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastYasuoR() {
	return Yasuogametime > YasuoRCastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

bool YasuoIsUnderTower(Vector3 pos,Alliance team)
{
	const auto turret = TargetSelector::FindTurret(pos, 992.0f, team);
	if (turret == nullptr) return false;
	if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) return false;

	return pos.Distance(turret->GetPosition()) < 992.0f;
}



void Functions::InitializeMenu()
{
	const auto YasuoMenu = Menu::CreateMenu("vezYasuo", "Champion Settings");

	const auto comboMenu = YasuoMenu->AddMenu("Combo Settings", "Combo Settings");
	YasuoCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YasuoCombo::UseQ3 = comboMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", true);
	YasuoCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	YasuoCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	YasuoCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = YasuoMenu->AddMenu("Harass Settings", "Harass Settings");
	YasuoHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YasuoHarass::UseQ3 = harassMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", true);
	YasuoHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	const auto clearMenu = YasuoMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	YasuoClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YasuoClear::UseQ3 = laneClearMenu->AddCheckBox("Use Q3", "Use SpellSlot Q3", true);
	YasuoClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	YasuoJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YasuoJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	YasuoJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = YasuoMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	YasuoKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	YasuoKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	YasuoKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

	const auto autoMenu = additionalMenu->AddMenu("Auto Settings", "Auto Settings");
	YasuoAuto::UseQ = autoMenu->AddCheckBox("Use Q", "Use SpellSlot Q to Stack", true);


	const auto gapCloseMenu = additionalMenu->AddMenu("Gapclose Settings", "Gapclose Settings");
	YasuoGapClose::GapCloseTarget = gapCloseMenu->AddCheckBox("Use E to GapClose", "Use E to GapClose", true);
	YasuoGapClose::GapCloseTargetMode = gapCloseMenu->AddList("gapcloseMode", "Gapclose Mode", std::vector<std::string>{"To Mouse", "To Target"}, 0);


	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	YasuoSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.YasuoQ.GetRange(), 100, database.YasuoQ.GetRange(), 50);
	YasuoSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

	const auto Q3SpellMenu = spellsMenu->AddMenu("SpellSlot Q3 Settings", "SpellSlot Q3");
	YasuoSpellsSettings::Q3Range = Q3SpellMenu->AddSlider("maxQ3Range", "Maximum Range", 1150.0f, 100, 1150.0f, 50);
	YasuoSpellsSettings::DrawQ = Q3SpellMenu->AddCheckBox("Draw Q3", "Draw Range", true);

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	YasuoSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", 350.0f, 100, 350.0f, 50);
	YasuoSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	YasuoSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", 475.0f, 100, 475.0f, 50);
	YasuoSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);
	YasuoSpellsSettings::useETower = eSpellMenu->AddCheckBox("UseEUnderTower", "Use E Under Tower", false);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	YasuoSpellsSettings::rRange = eSpellMenu->AddSlider("maxRRange", "Maximum Range", 1400, 100, 1400, 50);
	YasuoSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);
	YasuoSpellsSettings::useRTower = rSpellMenu->AddCheckBox("UseRUnderTower", "Use R Under Tower", false);

	YasuoSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	YasuoSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	YasuoSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	//TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
	TryCatch(Event::Subscribe(Event::OnProcessSpell, &OnProcessSpell), "Error unsubscribing to OnProcessSpell event");
	//TryCatch(Event::Subscribe(Event::OnFinishCast, &OnFinishCast), "Error unsubscribing to OnProcessSpell event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	//TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");
	TryCatch(Event::UnSubscribe(Event::OnProcessSpell, &OnProcessSpell), "Error unsubscribing to OnProcessSpell event");
}

void Functions::UseQ(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;
	
	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastYasuoQ()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YasuoSpellsSettings::GetQRange()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::Q, obj->GetPosition());
		YasuoQCastedTime = Yasuogametime;
		return;
	}

	Modules::prediction::PredictionOutput qPrediction;
	if (GetPrediction(database.YasuoQ, qPrediction)) {
		Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
		YasuoQCastedTime = Yasuogametime;
	}
}

void Functions::UseQ3(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastYasuoQ()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YasuoSpellsSettings::GetQ3Range()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::Q, obj->GetPosition());
		YasuoQCastedTime = Yasuogametime;
		return;
	}

	Modules::prediction::PredictionOutput qPrediction;
	if (GetPrediction(database.YasuoQ3, qPrediction)) {
		Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
		YasuoQCastedTime = Yasuogametime;
	}
}

void Functions::UseQDash() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;
	LOG("USING DASH QQQQQQSTEP1");
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastYasuoQDash()) return;
	LOG("USING DASH QQQQQQSTEP2");
	Engine::CastToPosition(SpellIndex::Q, ObjectManager::GetLocalPlayer()->GetPosition());
	YasuoQCastedTime = Yasuogametime;
	LOG("USING DASH QQQQQQ");
}

void Functions::UseW(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastYasuoW()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YasuoSpellsSettings::GetWRange()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
		YasuoWCastedTime = Yasuogametime;
		return;
	}

	Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
	YasuoWCastedTime = Yasuogametime;
}

bool hasEBuff(Object* obj)
{
	Buff* yasdash = obj->GetBuffByName("YasuoE");
	if (yasdash == nullptr) false;
	return yasdash != nullptr && yasdash->isActive();
}

void Functions::UseE(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (hasEBuff(obj)) return;
	auto eDashEndPosition = ObjectManager::GetLocalPlayer()->GetPosition().Extend(obj->GetPosition(), 475.0f);
	if (YasuoSpellsSettings::useETower->Value == false && YasuoIsUnderTower(eDashEndPosition,Alliance::Enemy)) return;


	
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastYasuoE()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YasuoSpellsSettings::GetERange() + obj->GetBoundingRadius() / 2) return;
	Engine::CastTargeted(SpellIndex::E, obj);
	YasuoECastedTime = Yasuogametime;
}

bool isKnockedup(Object* target)
{
	if (target == nullptr) return false;
	return target->GetBuffByType(BuffType::Knockback) or target->GetBuffByType(BuffType::Knockup);
}

int countKnockedUpHeroes(Object* target, float radius)
{
	int count = 0;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > YasuoSpellsSettings::GetRRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		if (isKnockedup(hero))
		{
			count++;
		}
	}
	return count;
}

Object* GetBestKnockedUpTarget()
{
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero->IsAlive()) continue;
		if (!hero->IsTargetable()) continue;
		if (!hero) continue;
		if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > YasuoSpellsSettings::GetRRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		if (isKnockedup(hero))
		{
			float dmg = Damages::RSpell::GetDamage(hero) + Damages::QSpell::GetDamage(hero) + Damage::CalculateAutoAttackDamage(ObjectManager::GetLocalPlayer(), hero);
			float enemy_health = hero->GetHealth() + hero->GetShield();
			LOG("Damage: %.2f", dmg);
			LOG("Enemy Health: %.2f", enemy_health);
			if (dmg >= enemy_health)
			{
				return hero;
			}
			int knockTarget = countKnockedUpHeroes(hero, 400.0f);
			if (knockTarget >= 2)
			{           
				return hero;
			}
		}
	}
	return nullptr;
}

void Functions::UseR() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;


	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastYasuoR()) return;
	Object* obj = GetBestKnockedUpTarget();

	if (obj == nullptr) return;
	if (YasuoSpellsSettings::useETower->Value == false && obj->IsUnderTower(Alliance::Enemy)) return;
	Engine::CastToPosition(SpellIndex::R, obj->GetPosition());
	YasuoRCastedTime = Yasuogametime;
}

int CountMinions(Vector3 sourcePos, Vector3 endPos, Object* target, float lineWidth) {
	int enemiesInLine = 0;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!hero->IsTargetable()) continue;
		if (!Modules::prediction::IsSpecificObjectInWay(sourcePos, endPos, target, lineWidth)) continue;
		enemiesInLine += 1;
	}

	return enemiesInLine;
}

Object* FindGapCloseMinion(Vector3 from, float range, Vector3 to, float radius, Object* target) {
	if (target == nullptr) return nullptr;
	if (!target->IsAlive()) return nullptr;
	if (!target->IsTargetable()) return nullptr;
	if (target->IsInvulnerable()) return nullptr;
	//if (!target->IsUnderTower(Alliance::Enemy)) return nullptr;

	
	for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy)) {

		if (!minion) continue;
		if (minion->IsAlly()) continue;
		if (minion->IsAlive() and minion->IsVisible() and minion->IsTargetable() and !minion->IsInvulnerable() and minion->GetPosition().Distance(from) <= range + minion->GetBoundingRadius() / 2)
		{
			if (minion->GetBuffByName("YasuoE") != nullptr) continue;
			const bool distanceCheck = minion->GetDistanceTo(target) < ObjectManager::GetLocalPlayer()->GetDistanceTo(target);
			if (Modules::prediction::IsSpecificObjectInWay(from, Engine::GetMouseWorldPos(), minion, radius) and distanceCheck)
			{
				return minion;
			}
		}
			
	}



	return nullptr;
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

	const float qDamage = Damages::QSpell::GetDamage(obj);
	const float eDamage = Damages::ESpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float comboDamage = qDamage + eDamage + rDamage;

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

	if (isTimeToCastYasuoQ()) {
		const float qDamage = Damages::QSpell::GetDamage(obj);

		render::RenderTextWorld("Q: " + std::to_string(qDamage), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}


	if (isTimeToCastYasuoE()) {
		const float eDamage = Damages::ESpell::GetDamage(obj);
		render::RenderTextWorld("E: " + std::to_string(ceil(eDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastYasuoR()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {
	if (YasuoSpellsSettings::DrawQ->Value == true && (YasuoSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastYasuoQ() || !YasuoSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YasuoSpellsSettings::GetQRange());

	if (YasuoSpellsSettings::DrawW->Value == true && (YasuoSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastYasuoW() || !YasuoSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YasuoSpellsSettings::GetWRange());

	if (YasuoSpellsSettings::DrawE->Value == true && (YasuoSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastYasuoE() || !YasuoSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YasuoSpellsSettings::GetERange());

	if (YasuoSpellsSettings::DrawR->Value == true && (YasuoSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastYasuoR() || !YasuoSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(YasuoSpellsSettings::GetRRange());

	auto nig = 0;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (YasuoSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (YasuoSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}

	for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy)) {

		if (!minion) continue;
		if (minion->IsAlly()) continue;
		if (minion->IsAlive() and minion->IsVisible() and minion->IsTargetable() and !minion->IsInvulnerable() and minion->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) <= 400 + minion->GetBoundingRadius() / 2)
		{
			if (minion->GetBuffByName("YasuoE") != nullptr) continue;
			if (Modules::prediction::IsSpecificObjectInWay(ObjectManager::GetLocalPlayer()->GetPosition(), Engine::GetMouseWorldPos(), minion, 300))
			{
				nig++;
			}
		}

	}
	//render::RenderLineWorld(ObjectManager::GetLocalPlayer()->GetPosition(), Engine::GetMouseWorldPos(), COLOR_RED, 2.0f);
	//render::RenderTextWorld(std::to_string(nig), Engine::GetMouseWorldPos(), 30.0f, COLOR_WHITE, true);


}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	Yasuogametime = Engine::GetGameTime();

	if (Orbwalker::Mode == Attack) {
		Modes::Combo();
	}
	if (Orbwalker::Mode == Harass) {
		Modes::Harass();
	}
	if (Orbwalker::Mode == Clear) {
		Modes::Clear();
	}

	

	Modes::Killsteal();
	Modes::Auto();
	// TODO: HANDLE IS EVADING SPELL
}


void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	/*if (ObjectManager::GetLocalPlayer()->GetAiManager()->IsDashing() || ObjectManager::GetLocalPlayer()->GetAiManager()->GetDashSpeed() > 0) { LOG("He dashing");; }*/

	const auto gapTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), 1500);
	if (gapTarget != nullptr) {
		if (gapTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YasuoSpellsSettings::GetERange()) {
			Vector3 chasePosition = YasuoGapClose::GapCloseTargetMode->Value == 1 ? gapTarget->GetPosition() : Engine::GetMouseWorldPos();
			const auto minion = FindGapCloseMinion(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetERange(), chasePosition, 300, gapTarget);
			if (minion != nullptr)
			{
				Functions::UseE(minion);
			}
		}
	}


	if (YasuoCombo::UseE->Value == true && isTimeToCastYasuoE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}

	}

	if (!ObjectManager::GetLocalPlayer()->GetAiManager()->IsDashing() || ObjectManager::GetLocalPlayer()->GetAiManager()->GetDashSpeed() < 1) {

		if (YasuoCombo::UseQ->Value == true && isTimeToCastYasuoQ()) {

			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetQRange());
			auto spell_name = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (qTarget != nullptr) {
				if (spell_name != "YasuoQ3Wrapper") {
					Functions::UseQ(qTarget);
				}
			}
		}

		if (YasuoCombo::UseQ3->Value == true && isTimeToCastYasuoQ()) {

			const auto Q3Target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetQ3Range());
			auto spell_name = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
			if (Q3Target != nullptr) {
				if (spell_name == "YasuoQ3Wrapper") {
					Functions::UseQ3(Q3Target);
				}
			}
		}
	}


	if (YasuoCombo::UseR->Value == true && isTimeToCastYasuoR()) {
		Functions::UseR();
	}
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), 1500);
	if (minionsInRange > 0) {


		for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy))
		{
			if (!minion) continue;


			//ELogic
			if (YasuoClear::UseE->Value && isTimeToCastYasuoE() and minion->GetPosition().distanceTo(ObjectManager::GetLocalPlayer()->GetPosition()) < YasuoSpellsSettings::GetERange()) {
				if (hasEBuff(minion)) continue;
				if (Damages::ESpell::GetDamage(minion) > minion->GetHealth()) {
					Functions::UseE(minion);
					break;
				}
			}

			//Q Logic
			if (isTimeToCastYasuoQ()) {
				if (!ObjectManager::GetLocalPlayer()->GetAiManager()->IsDashing()) { //Dashing
					auto spell_name = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();

					//Q1|Q2
					if (spell_name == "YasuoQ3Wrapper") {
						if (YasuoClear::UseQ3->Value && minion->GetPosition().distanceTo(ObjectManager::GetLocalPlayer()->GetPosition()) < YasuoSpellsSettings::GetQ3Range()) {
							Functions::UseQ3(minion);
							break;
						}
					}
					//Q3
					else {
						if (YasuoClear::UseQ->Value && minion->GetPosition().distanceTo(ObjectManager::GetLocalPlayer()->GetPosition()) < YasuoSpellsSettings::GetQRange()) {
							Functions::UseQ(minion);
							break;
						}
					}
				}
				else // Dashing
				{
					if (YasuoClear::UseQ->Value) {
						Functions::UseQDash();
						break;
					}
				}
			}
		}


	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetWRange());
		if (jungleMonstersInRange > 0) {


			if (YasuoJungle::UseE->Value && isTimeToCastYasuoE()) {
				const auto eMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetERange());
				if (eMonster != nullptr) {
					Functions::UseE(eMonster);
				}
			}

			if (YasuoJungle::UseQ->Value && isTimeToCastYasuoQ()) {
				const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetQRange());
				if (qMonster != nullptr) {
					Functions::UseQ(qMonster);
				}
			}
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (YasuoHarass::UseE->Value && isTimeToCastYasuoE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}
	}

	if (YasuoHarass::UseQ->Value == true && isTimeToCastYasuoQ()) {

		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetQRange());
		auto spell_name = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (qTarget != nullptr) {
			if (spell_name != "YasuoQ3Wrapper") {
				Functions::UseQ(qTarget);
			}
		}
	}

	if (YasuoHarass::UseQ3->Value == true && isTimeToCastYasuoQ()) {

		const auto Q3Target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), YasuoSpellsSettings::GetQ3Range());
		auto spell_name = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
		if (Q3Target != nullptr) {
			if (spell_name == "YasuoQ3Wrapper") {
				Functions::UseQ3(Q3Target);
			}
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	//for (int i = 0; i < ObjectManager::GetLocalPlayer()->GetBuffListSize(); i++) {
	//	const auto buffEntry = ObjectManager::GetLocalPlayer()->GetBuffManager()->GetBuffEntryByIndex(i);
	//	if (buffEntry) {
	//		const auto buff = buffEntry->GetBuff();
	//		if (buff && buff->GetEndTime() >= Engine::GetGameTime()) {
	//			Engine::PrintChat(buff->GetName());
	//		}
	//	}
	//}

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > YasuoSpellsSettings::GetQRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		//for (int i = 0; i < hero->GetBuffListSize(); i++) {
		//	const auto buffEntry = hero->GetBuffManager()->GetBuffEntryByIndex(i);
		//	if (buffEntry) {
		//		const auto buff = buffEntry->GetBuff();
		//		if (buff && buff->GetEndTime() >= Engine::GetGameTime()) {
		//			Engine::PrintChat(buff->GetName());
		//		}
		//	}
		//}


		const float heroHealth = hero->GetHealth() + hero->GetShield();
		if (YasuoKillsteal::UseE->Value && isTimeToCastYasuoQ() && heroHealth < Damages::ESpell::GetDamage(hero)) {
			Functions::UseE(hero);
			continue;
		}
		if (YasuoKillsteal::UseQ->Value && isTimeToCastYasuoE() && heroHealth < Damages::QSpell::GetDamage(hero)) {
			Functions::UseQ(hero);
			continue;
		}
		//if (YasuoKillsteal::UseR->Value && isTimeToCastYasuoR() && heroHealth < Damages::RSpell::GetDamage(hero)) {
		//	Functions::UseR(hero);
		//	break;
		//}
	}
}

void Modes::Auto() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (ObjectManager::GetLocalPlayer()->IsUnderTower(Alliance::Enemy)) return;
	if (YasuoAuto::UseQ->Value) {
		for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
			if (!hero) continue;
			if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > YasuoSpellsSettings::GetQRange()) continue;
			if (hero->IsInvulnerable()) continue;


			if (isTimeToCastYasuoQ()) {
				const auto qSpellSlotName = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetName();
				if (qSpellSlotName != "YasuoQ3Wrapper") {
					Functions::UseQ(hero);
				}
			}
		}
	}
}

void Events::OnProcessSpell(int state, SpellCast* spellData) {
	if (spellData == nullptr) return;
	if (state != 12) return;
	if (spellData->GetCasterHandle() != ObjectManager::GetLocalPlayer()->GetHandle()) return;
	if (spellData->IsAutoAttack()) return;

	const auto caster = ObjectManager::GetClientByHandle(spellData->GetCasterHandle());
	if (caster == nullptr) return;
	if (!caster->IsHero()) return;

	auto spellID = spellData->GetSpellId();
	if (spellID < 0 || spellID > 5) return;

	Spell* spell = caster->GetSpellBySlotId(spellID);
	if (spell == nullptr) return;

	const int spellLevel = spell->GetLevel();
	if (spellLevel == 0) return;

	LOG("%s", spell->GetName());

	if (spell->GetName() == "YasuoE" && isTimeToCastYasuoQDash() and globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack)
	{
		LOG("SHOULD USE Q");
		auto spellEndPos = spellData->GetEndPosition().ToGround();
		const int heroesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, spellEndPos, YasuoSpellsSettings::GetQRange());
		
		LOG("SHOULD USE Q at position: x = %f, y = %f, z = %f", spellEndPos.x, spellEndPos.y, spellEndPos.z);
		LOG("SHOULD USE Q %d", heroesInRange);
		if (heroesInRange > 0)
		{
			Functions::UseQDash();
		}
	}
}

//           