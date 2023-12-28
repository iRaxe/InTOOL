#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../Orbwalker.h"
#include "Jinx.h"



using namespace UPasta::SDK;
using namespace UPasta::Plugins::Jinx;
using namespace UPasta::Plugins::Jinx::Config;

float Jinxgametime = 0.0f;

float JinxQCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastJinxQ() {
	return Jinxgametime > JinxQCastedTime + .25 && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}



float JinxWCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastJinxW() {
	return Jinxgametime > JinxWCastedTime + database.JinxW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float JinxECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastJinxE() {
	return Jinxgametime > JinxECastedTime + database.JinxE.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float JinxRCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastJinxR() {
	return Jinxgametime > JinxRCastedTime + database.JinxR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

bool JinxIsUnderTower(Vector3 pos, Alliance team)
{
	const auto turret = TargetSelector::FindTurret(pos, 992.0f, team);
	if (turret == nullptr) return false;
	if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) return false;

	return pos.Distance(turret->GetPosition()) < 992.0f;
}



void Functions::InitializeMenu()
{
	const auto JinxMenu = Menu::CreateMenu("vezJinx", "Champion Settings");

	const auto comboMenu = JinxMenu->AddMenu("Combo Settings", "Combo Settings");
	JinxCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	JinxCombo::UseW = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	JinxCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	JinxCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	JinxCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = JinxMenu->AddMenu("Harass Settings", "Harass Settings");
	JinxHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);

	const auto clearMenu = JinxMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	JinxClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	JinxJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);

	JinxJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = JinxMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	JinxKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	JinxKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

	const auto autoMenu = additionalMenu->AddMenu("Auto Settings", "Auto Settings");
	JinxAuto::UseE = autoMenu->AddCheckBox("Use Q", "Use SpellSlot E on CC", true);

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");
	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	JinxSpellsSettings::Weave = qSpellMenu->AddCheckBox("WeaveQ", "Use Q Passive To Weave", true);

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	JinxSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.JinxW.GetRange(), 100, database.JinxW.GetRange(), 50);
	JinxSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);
	JinxSpellsSettings::dontWinAARange = wSpellMenu->AddCheckBox("DontWAA", "Don't W in AA Range", true);

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	JinxSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.JinxE.GetRange(), 100, database.JinxE.GetRange(), 50);
	JinxSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);
	JinxSpellsSettings::useETower = eSpellMenu->AddCheckBox("UseEUnderTower", "Use E Under Tower", false);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	JinxSpellsSettings::rMinRange = rSpellMenu->AddSlider("minRRange", "Minimum Range", ObjectManager::GetLocalPlayer()->GetRealAttackRange(), 100, 3999, 50);
	JinxSpellsSettings::rRange = rSpellMenu->AddSlider("maxRRange", "Maximum Range", 3000, 100, 25000, 50);
	JinxSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);
	JinxSpellsSettings::useRTower = rSpellMenu->AddCheckBox("UseRUnderTower", "Use R Under Tower", false);

	JinxSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	JinxSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	JinxSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");

}

void Functions::UseQ() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;


	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastJinxQ()) return;

	Engine::CastSelf(SpellIndex::Q);
	JinxQCastedTime = Jinxgametime;
}



void Functions::UseW(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastJinxW()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > JinxSpellsSettings::GetWRange()) return;
	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) < 525.0f and JinxSpellsSettings::dontWinAARange->Value)  return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
		JinxWCastedTime = Jinxgametime;
		return;
	}

	Modules::prediction::PredictionOutput wPrediction;
	if (GetPrediction(database.JinxW, wPrediction)) {
		Engine::CastToPosition(SpellIndex::W, wPrediction.position);
		JinxWCastedTime = Jinxgametime;
	}
}

static bool IsControlledJinx(Object* target) {
	if (target == nullptr) return false;
	return target->GetBuffByType(BuffType::Stun) or target->GetBuffByType(BuffType::Snare) or target->GetBuffByType(BuffType::Suppression);
}

bool hasPowPowBuff()
{
	Buff* jinxpowpow = ObjectManager::GetLocalPlayer()->GetBuffByName("jinxqramp");
	if (jinxpowpow == nullptr) false;
	return jinxpowpow != nullptr && jinxpowpow->isActive();
}

int getPowPowBuffCount()
{
	Buff* jinxpowpow = ObjectManager::GetLocalPlayer()->GetBuffByName("jinxqramp");
	if (jinxpowpow == nullptr) false;
	if (jinxpowpow != nullptr && jinxpowpow->isActive())
	{
		return jinxpowpow->GetStacksAlt();
	}
}

float addedQRange()
{
	return 50 + 25 * ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
}

bool hasJinxQBuff()
{
	Buff* jinxRocket = ObjectManager::GetLocalPlayer()->GetBuffByName("JinxQ");
	if (jinxRocket == nullptr) false;
	return jinxRocket != nullptr && jinxRocket->isActive();
}

bool CanRocket(Object* me, Object* qTarget )
{
	if (qTarget == nullptr) return false;
	if (hasJinxQBuff()) return false;
	return qTarget->GetDistanceTo(me) > 525.0f + qTarget->GetBoundingRadius() / 2
	|| (JinxSpellsSettings::Weave->Value && getPowPowBuffCount() > 2 and qTarget->GetDistanceTo(me) < me->GetRealAttackRange() + addedQRange() + qTarget->GetBoundingRadius() / 2);
}

bool CanMiniGun(Object* me, Object* qTarget)
{
	if (qTarget == nullptr) return false;
	if (!hasJinxQBuff()) return false;
	if (JinxSpellsSettings::Weave->Value and getPowPowBuffCount() > 2 && qTarget->GetDistanceTo(me) < 525.5 + qTarget->GetBoundingRadius() / 2 ) return false;
	if (JinxSpellsSettings::Weave->Value and getPowPowBuffCount() <= 2 && qTarget->GetDistanceTo(me) < 525.5 + qTarget->GetBoundingRadius() / 2) return true;
	return qTarget->GetDistanceTo(me) < 525.0f + qTarget->GetBoundingRadius() / 2;
}

bool CanUseJinxQ(Object* qTarget)
{
	if (qTarget == nullptr) return false;
	auto me = ObjectManager::GetLocalPlayer();
	return CanRocket(me, qTarget) || CanMiniGun(me, qTarget);
		
}

void Functions::UseE(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	

	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastJinxE()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > JinxSpellsSettings::GetERange() + obj->GetBoundingRadius() / 2) return;
	Modules::prediction::PredictionOutput ePrediction;
	if (GetPrediction(database.JinxE, ePrediction)) {
		Engine::CastToPosition(SpellIndex::E, ePrediction.position);
		JinxECastedTime = Jinxgametime;
	}
}

bool isControledJinx(Object* target)
{
	if (target == nullptr) return false;
	return target->GetBuffByType(BuffType::Knockback) or target->GetBuffByType(BuffType::Knockup);
}





void Functions::UseR(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;


	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastJinxR()) return;

	if (obj == nullptr) return;
	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > JinxSpellsSettings::GetRRange() + obj->GetBoundingRadius() / 2) return;
	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) < JinxSpellsSettings::rMinRange->Value + obj->GetBoundingRadius() / 2) return;
	Modules::prediction::PredictionOutput rPrediction;
	if (GetPrediction(database.JinxR, rPrediction)) {
		Engine::CastToPosition(SpellIndex::R, rPrediction.position);
		JinxRCastedTime = Jinxgametime;
	}
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

	const float aDamage = Damages::WSpell::GetDamage(obj) + Damages::RSpell::GetDamage(obj);
	const float comboDamage = aDamage;

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



	if (isTimeToCastJinxW()) {
		const float wDamage = Damages::WSpell::GetDamage(obj);
		render::RenderTextWorld("W: " + std::to_string(ceil(wDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastJinxR()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {


	if (JinxSpellsSettings::DrawW->Value == true && (JinxSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastJinxW() || !JinxSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(JinxSpellsSettings::GetWRange());

	if (JinxSpellsSettings::DrawE->Value == true && (JinxSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastJinxE() || !JinxSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(JinxSpellsSettings::GetERange());

	if (JinxSpellsSettings::DrawR->Value == true && (JinxSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastJinxR() || !JinxSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(JinxSpellsSettings::GetRRange());

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (JinxSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (JinxSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}


}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	Jinxgametime = Engine::GetGameTime();

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

	
	if (JinxCombo::UseR->Value == true && isTimeToCastJinxR()) {
		for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
			if (!hero) continue;
			if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > JinxSpellsSettings::GetRRange() + hero->GetBoundingRadius() / 2) continue;
			if (hero->IsInvulnerable()) continue;

			const float heroHealth = hero->GetHealth() + hero->GetShield();
			if (Damages::RSpell::GetDamage(hero) >= heroHealth)
			{
				Functions::UseR(hero);
			}

		}
	}

	if (JinxCombo::UseW->Value == true && isTimeToCastJinxW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), JinxSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}

	}

	if (JinxCombo::UseE->Value == true && isTimeToCastJinxE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), JinxSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}

	}
	
	if (JinxCombo::UseQ->Value == true && isTimeToCastJinxQ()) {
		auto me = ObjectManager::GetLocalPlayer();
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), 1500.0f);
		if (qTarget != nullptr) {
			float distance = ObjectManager::GetLocalPlayer()->GetDistanceTo(qTarget);
			auto predictedSpot = Modules::prediction::PredictTargetPosition(qTarget, distance + .25f);
			if (CanUseJinxQ(qTarget)){
				Functions::UseQ();
			}
		}
	}


	
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), 1500);
	if (minionsInRange > 0) {

		if (isTimeToCastJinxQ()) {
			auto minion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange(), Alliance::Enemy);
			if (minion != nullptr) {
				if (JinxClear::UseQ->Value) {
					
					const auto surroundingMinions = ObjectManager::CountMinionsInRange(Alliance::Enemy, minion->GetPosition(), 250);

					if (!hasJinxQBuff() && surroundingMinions >= 3) {// Has Mini Use Rocket
						Functions::UseQ();
					}
					if (surroundingMinions <= 3 and hasJinxQBuff()) {
						Functions::UseQ();
					}
				}
			}
		}



	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), JinxSpellsSettings::GetWRange());
		if (jungleMonstersInRange > 0) {


			if (isTimeToCastJinxQ()) {
				auto minion = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), 525.0f + addedQRange());
				if (minion != nullptr) {
					if (JinxJungle::UseQ->Value) {

						const auto surroundingMinions = ObjectManager::CountJungleMonstersInRange(minion->GetPosition(), 250);

						if (!hasJinxQBuff() && surroundingMinions >= 3) {// Has Mini Use Rocket
							Functions::UseQ();
						}
						if (surroundingMinions <= 3 and hasJinxQBuff()) {
							Functions::UseQ();
						}
					}
				}
			}
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (JinxHarass::UseW->Value == true && isTimeToCastJinxW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), JinxSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}

	}


	if (JinxHarass::UseQ->Value == true && isTimeToCastJinxQ()) {
		auto me = ObjectManager::GetLocalPlayer();
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), 1500.0f);
		if (qTarget != nullptr) {
			float distance = ObjectManager::GetLocalPlayer()->GetDistanceTo(qTarget);
			auto predictedSpot = Modules::prediction::PredictTargetPosition(qTarget, distance + .25f);
			if (CanUseJinxQ(qTarget)) {
				Functions::UseQ();
			}
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;



	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) < JinxSpellsSettings::GetWRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		const float heroHealth = hero->GetHealth() + hero->GetShield();

		if (JinxKillsteal::UseW->Value && isTimeToCastJinxW() && heroHealth < Damages::WSpell::GetDamage(hero)) {
			Functions::UseW(hero);
			continue;
		}
		if (JinxKillsteal::UseR->Value && isTimeToCastJinxR() && heroHealth < Damages::RSpell::GetDamage(hero)) {
			Functions::UseR(hero);
		}
		

	}
}

void Modes::Auto() {
	if (!Orbwalker::CanCastAfterAttack()) return;



	if (ObjectManager::GetLocalPlayer()->IsUnderTower(Alliance::Enemy)) return;
	if (JinxAuto::UseE->Value) {
		for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
			if (!hero) continue;
			if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > JinxSpellsSettings::GetERange()) continue;
			if (hero->IsInvulnerable()) continue;
			if (!IsControlledJinx(hero)) continue;

			if (isTimeToCastJinxE()) {
				Functions::UseE(hero);
			}
		}
	}
}


//           