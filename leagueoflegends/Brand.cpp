#include "stdafx.h"
#include "Brand.h"

#include "Awareness.h"
#include "Damage.h"
#include "Orbwalker.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Brand;
using namespace UPasta::Plugins::Brand::Config;

float gameTime = 0.0f;

float QCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastQ() {
	return gameTime > QCastedTime + database.BrandQ.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float WCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastW() {
	return gameTime > WCastedTime + database.BrandW.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float ECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastE() {
	return gameTime > ECastedTime + database.BrandE.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float RCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastR() {
	return gameTime > RCastedTime + database.BrandR.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

Object* Functions::GetAblazedTarget(float range)
{
	Object* heroToReturn = nullptr;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!HasBrandPassive(hero)) continue;
		if (!heroToReturn && hero != nullptr && HasBrandPassive(hero) && hero->IsAlive() && hero->IsTargetable()) {
			heroToReturn = hero;
			break;
		}
	}

	if (heroToReturn)
		return heroToReturn;

	return nullptr;
}

inline bool Functions::HasBrandPassive(Object* obj) {
	if (obj == nullptr) return false;
	if (!obj->IsAlive()) return false;
	if (!obj->IsTargetable()) return false;

	const auto ablazeBuff = obj->GetBuffByName("BrandAblaze");
	if (ablazeBuff == nullptr) return false;
	if (!ablazeBuff->isActive()) return false;

	return true;
}

void Events::InitializeMenu()
{
	const auto BrandMenu = Menu::CreateMenu("vezBrand", "Champion Settings");

	const auto comboMenu = BrandMenu->AddMenu("Combo Settings", "Combo Settings");
	BrandCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BrandCombo::UseQ2 = comboMenu->AddCheckBox("Use Q2", "Use SpellSlot Q only if Ablazed", false);
	BrandCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	BrandCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	BrandCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	BrandCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = BrandMenu->AddMenu("Harass Settings", "Harass Settings");
	BrandHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BrandHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	BrandHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	BrandHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

	const auto clearMenu = BrandMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	BrandClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
	BrandClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", false);
	BrandClear::minMinions = laneClearMenu->AddSlider("minWMinions", "Minimum Minions", 3, 1, 6, 1);
	BrandClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	BrandJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BrandJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	BrandJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	BrandJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = BrandMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	BrandKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BrandKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	BrandKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	BrandKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	BrandSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.BrandQ.GetRange(), 100, database.BrandQ.GetRange(), 50);
	BrandSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	BrandSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.BrandW.GetRange(), 100, database.BrandW.GetRange(), 50);
	BrandSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	BrandSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.BrandE.GetRange(), 100, database.BrandE.GetRange(), 50);
	BrandSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	BrandSpellsSettings::rRange = eSpellMenu->AddSlider("maxRRange", "Maximum Range", database.BrandR.GetRange(), 100, database.BrandR.GetRange(), 50);
	BrandSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);

	BrandSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	BrandSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	BrandSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

void Events::Initialize() {
	TryCatch(InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");

	//TryCatch(Event::UnSubscribe(Event::OnProcessSpell, &OnProcessSpell), "Error unsubscribing to OnProcessSpell event");
}

void Functions::UseQ(Object* obj) {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (!globals::localPlayer->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastQ()) return;

	if (obj->GetDistanceTo(globals::localPlayer) > BrandSpellsSettings::GetQRange()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastSpell(SpellIndex::Q, obj->GetPosition());
		QCastedTime = gameTime;
		return;
	}

	Modules::prediction::PredictionOutput qPrediction;
	if (GetPrediction(database.BrandQ, qPrediction)) {
		Engine::CastSpell(SpellIndex::Q, qPrediction.position);
		QCastedTime = gameTime;
	}
}

void Functions::UseW(Object* obj) {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (!globals::localPlayer->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastW()) return;

	if (obj->GetDistanceTo(globals::localPlayer) > BrandSpellsSettings::GetWRange()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastSpell(SpellIndex::W, obj->GetPosition());
		WCastedTime = gameTime;
		return;
	}

	Modules::prediction::PredictionOutput wPrediction;
	if (GetPrediction(database.BrandW, wPrediction)) {
		Engine::CastSpell(SpellIndex::W, wPrediction.position);
		WCastedTime = gameTime;
	}
}

void Functions::UseE(Object* obj) {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (!globals::localPlayer->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastE()) return;

	if (obj->GetDistanceTo(globals::localPlayer) > BrandSpellsSettings::GetERange()) return;

	Engine::CastSpell(SpellIndex::E, obj);
	ECastedTime = gameTime;
}

void Functions::UseR(Object* obj) {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;
	if (!globals::localPlayer->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastR()) return;

	if (obj->GetDistanceTo(globals::localPlayer) > BrandSpellsSettings::GetRRange()) return;

	Engine::CastSpell(SpellIndex::R, obj);
	RCastedTime = gameTime;
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
		
	const Vector2 objHPBarScreenPos = Engine::GetHpBarPosition(obj);
	if (!objHPBarScreenPos.IsValid()) return;

	const float qDamage = Damages::QSpell::GetDamage(obj);
	const float wDamage = Damages::WSpell::GetDamage(obj);
	const float eDamage = Damages::ESpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float comboDamage = qDamage + wDamage + eDamage + rDamage;

	static constexpr float yOffset = 23.5f;
	static constexpr float xOffset = -46.0f;
	static constexpr float widthMultiplier = 105;

	const float objHealth = obj->ReadClientStat(Object::Health);
	const float objMaxHealth = obj->ReadClientStat(Object::MaxHealth);
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

	if (isTimeToCastQ()) {
		const float qDamage = Damages::QSpell::GetDamage(obj);

		render::RenderTextWorld("Q: " + std::to_string(qDamage), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastW()) {
		const float wDamage = Damages::WSpell::GetDamage(obj);
		const float wDamageAblazed = Damages::WSpell::GetDamageAblazed(obj);
		const float finalDamage = HasBrandPassive(obj) ? wDamageAblazed : wDamage;
		render::RenderTextWorld("W: " + std::to_string(ceil(finalDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastE()) {
		const float eDamage = Damages::ESpell::GetDamage(obj);
		render::RenderTextWorld("E: " + std::to_string(ceil(eDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastR()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {
	if (BrandSpellsSettings::DrawQ->Value == true && (BrandSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastQ() || !BrandSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(BrandSpellsSettings::GetQRange());

	if (BrandSpellsSettings::DrawW->Value == true && (BrandSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastW() || !BrandSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(BrandSpellsSettings::GetWRange());

	if (BrandSpellsSettings::DrawE->Value == true && (BrandSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastE() || !BrandSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(BrandSpellsSettings::GetERange());

	if (BrandSpellsSettings::DrawR->Value == true && (BrandSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastR() || !BrandSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(1500.0f);

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(globals::localPlayer) > 1500.0f) continue;

		if (BrandSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (BrandSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

void Events::OnGameUpdate() {
	if (globals::localPlayer == nullptr) return;
	if (!globals::localPlayer->IsAlive()) return;

	gameTime = Engine::GetGameTime();

	Modes::Killsteal();
	// TODO: HANDLE IS EVADING SPELL
}

void Events::OnWndProc(UINT msg, WPARAM param) {
	if (param == OrbwalkerConfig::comboKey->Key) {
		Modes::Combo();
	}
	if (param == OrbwalkerConfig::fastClearKey->Key || OrbwalkerConfig::laneClearKey->Key) {
		Modes::Clear();
	}
	if (param == OrbwalkerConfig::harassKey->Key) {
		Modes::Harass();
	}
}

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (BrandCombo::UseW->Value == true && isTimeToCastW()) {
		const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}

	if (BrandCombo::UseE->Value == true && isTimeToCastE()) {
		const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}
	}

	if (BrandCombo::UseQ->Value == true && isTimeToCastQ()) {
		const auto ablazedTarget = Functions::GetAblazedTarget(BrandSpellsSettings::GetQRange());
		if (BrandCombo::UseQ2->Value == true && ablazedTarget != nullptr) {
			Functions::UseQ(ablazedTarget);
		}

		const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetQRange());
		if (qTarget != nullptr) {
			Functions::UseQ(qTarget);
		}
	}

	const int heroesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), BrandSpellsSettings::GetRRange());
	if (BrandCombo::UseR->Value == true && isTimeToCastR() && BrandCombo::GetMinimumEnemies() <= heroesInRange) {
		const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetRRange());
		if (rTarget != nullptr) {
			Functions::UseR(rTarget);
		}
	}
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), BrandSpellsSettings::GetWRange());
	if (minionsInRange > 0) {
		if (BrandClear::GetMinimumMana() >= globals::localPlayer->GetPercentMana()) return;

		if (BrandClear::UseW->Value && isTimeToCastW()) {
			const auto wTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetWRange(), Alliance::Enemy);
			if (wTarget != nullptr) {
				const auto minionsAroundWTarget = ObjectManager::CountMinionsInRange(Alliance::Enemy, wTarget->GetPosition(), database.BrandW.GetRadius());
				if (minionsAroundWTarget >= BrandClear::GetMinimumMinions()) {
					Functions::UseW(wTarget);
				}
			}
		}

		if (BrandClear::UseE->Value && isTimeToCastE()) {
			const auto eTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetERange(), Alliance::Enemy);
			if (eTarget != nullptr) {
				const auto minionsAroundWTarget = ObjectManager::CountMinionsInRange(Alliance::Enemy, eTarget->GetPosition(), database.BrandW.GetRadius());
				if (minionsAroundWTarget >= BrandClear::GetMinimumMinions()) {
					Functions::UseE(eTarget);
				}
			}
		}
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetWRange());
		if (jungleMonstersInRange > 0) {
			if (BrandJungle::GetMinimumMana() >= globals::localPlayer->GetPercentMana()) return;

			if (BrandJungle::UseE->Value && isTimeToCastW()) {
				const auto wMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetWRange());
				if (wMonster != nullptr) {
					Functions::UseW(wMonster);
				}
			}

			if (BrandJungle::UseE->Value && isTimeToCastE()) {
				const auto eMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetERange());
				if (eMonster != nullptr) {
					Functions::UseE(eMonster);
				}
			}

			if (BrandJungle::UseQ->Value && isTimeToCastQ()) {
				const auto qMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetQRange());
				if (qMonster != nullptr) {
					Functions::UseQ(qMonster);
				}
			}
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (BrandHarass::GetMinimumMana() >= globals::localPlayer->GetPercentMana()) return;

	if (BrandHarass::UseW->Value && isTimeToCastW()) {
		const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetWRange());
		if (wTarget != nullptr) {
			Functions::UseW(wTarget);
		}
	}

	if (BrandHarass::UseE->Value && isTimeToCastE()) {
		const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}
	}

	if (BrandHarass::UseQ->Value && isTimeToCastQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), BrandSpellsSettings::GetQRange());
		if (qTarget != nullptr) {
			Functions::UseQ(qTarget);
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetPosition().Distance(globals::localPlayer->GetPosition()) > BrandSpellsSettings::GetWRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		const float heroHealth = hero->ReadClientStat(Object::Health) + hero->ReadClientStat(Object::Shield);
		if (BrandKillsteal::UseE->Value && isTimeToCastQ() && heroHealth < Damages::ESpell::GetDamage(hero)) {
			Functions::UseE(hero);
			continue;
		}
		if (BrandKillsteal::UseW->Value && isTimeToCastQ() && heroHealth < Damages::WSpell::GetDamage(hero)) {
			Functions::UseW(hero);
			continue;
		}
		if (BrandKillsteal::UseQ->Value && isTimeToCastQ() && heroHealth < Damages::QSpell::GetDamage(hero)) {
			Functions::UseQ(hero);
			continue;
		}
		if (BrandKillsteal::UseR->Value && isTimeToCastQ() && heroHealth < Damages::RSpell::GetDamage(hero)) {
			Functions::UseR(hero);
			break;
		}
	}
}