#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../../../Orbwalker.h"
#include "Tristana.h"

#include "../Amumu/Amumu.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Tristana;
using namespace UPasta::Plugins::Tristana::Config;

float gameTime = 0.0f;

float QCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastQ() {
	return gameTime > QCastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float WCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastW() {
	return gameTime > WCastedTime + database.TristanaW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float ECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastE() {
	return gameTime > ECastedTime + 0.22f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float RCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastR() {
	return gameTime > RCastedTime + 0.25f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

Object* Functions::GetETarget(float range)
{
	Object* heroToReturn = nullptr;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (!HasTristanaE(hero)) continue;
		if (!heroToReturn && hero != nullptr && HasTristanaE(hero) && hero->IsAlive() && hero->IsTargetable()) {
			heroToReturn = hero;
			break;
		}
	}

	if (heroToReturn)
		return heroToReturn;

	return nullptr;
}

inline bool Functions::HasTristanaE(Object* obj) {
	if (obj == nullptr) return false;
	if (!obj->IsAlive()) return false;
	if (!obj->IsTargetable()) return false;

	const auto bombBuff = obj->GetBuffByName("tristanaecharge");
	if (bombBuff == nullptr) return false;
	if (!bombBuff->isActive()) return false;

	return true;
}

inline int Functions::GetBombStacks(Object* obj) {
	if (obj == nullptr) return false;
	if (!obj->IsAlive()) return false;
	if (!obj->IsTargetable()) return false;
	if (!HasTristanaE(obj)) return 0;

	const auto bombBuff = obj->GetBuffByName("tristanaecharge");
	if (bombBuff == nullptr) return 0;
	if (!bombBuff->isActive()) return 0;


	return bombBuff->GetStacks();
}

void Functions::InitializeMenu()
{
	const auto TristanaMenu = Menu::CreateMenu("vezTristana", "Champion Settings");

	const auto comboMenu = TristanaMenu->AddMenu("Combo Settings", "Combo Settings");
	TristanaCombo::ForceAA = comboMenu->AddCheckBox("ForceAA", "Force AA on E Target", true);
	TristanaCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	TristanaCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	const auto harassMenu = TristanaMenu->AddMenu("Harass Settings", "Harass Settings");
	TristanaHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	TristanaHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	TristanaHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

	const auto clearMenu = TristanaMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	TristanaClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
	TristanaClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", false);
	TristanaClear::minMinions = laneClearMenu->AddSlider("minMinions", "Minimum Minions", 3, 1, 6, 1);
	TristanaClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	TristanaJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	TristanaJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	TristanaJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	TristanaJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = TristanaMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	TristanaKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

	const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
	TristanaAntiGapCloser::UseW = antiGapMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
	TristanaAntiGapCloser::UseR = antiGapMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

	const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
	TristanaAntiMelee::UseW = antiMeleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

	for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++) {
		auto obj = ObjectManager::GetHeroList()->GetIndex(i);
		if (obj != nullptr && obj->IsEnemy()) {
			const auto antiGap_checkbox = antiGapMenu->AddCheckBox(obj->GetName().c_str(), obj->GetName().c_str(), true,
				[obj](const CheckBox* self, bool newValue) {
					if (self->Value == false && !TristanaAntiGapCloser::whitelist.empty()) {
						const auto it = std::ranges::find(TristanaAntiGapCloser::whitelist, obj);
						TristanaAntiGapCloser::whitelist.erase(it);
					}
					else {
						TristanaAntiGapCloser::whitelist.push_back(obj);
					}
				});

			if (antiGap_checkbox->Value == true) {
				TristanaAntiGapCloser::whitelist.push_back(obj);
			}

			if (!obj->IsMelee()) continue;
			const auto antiMelee_checkbox = antiMeleeMenu->AddCheckBox(obj->GetName().c_str(), obj->GetName().c_str(), true,
				[obj](const CheckBox* self, bool newValue) {
					if (self->Value == false && !TristanaAntiMelee::whitelist.empty()) {
						const auto it = std::ranges::find(TristanaAntiMelee::whitelist, obj);
						TristanaAntiMelee::whitelist.erase(it);
					}
					else {
						TristanaAntiMelee::whitelist.push_back(obj);
					}
				});

			if (antiMelee_checkbox->Value == true) {
				TristanaAntiMelee::whitelist.push_back(obj);
			}
		}
	}

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	TristanaSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.TristanaW.GetRange(), 100, database.TristanaW.GetRange(), 50);
	TristanaSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	TristanaSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	TristanaSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);

	TristanaSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	TristanaSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	TristanaSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
	TryCatch(Event::Subscribe(Event::OnBeforeAttack, &OnBeforeAttack), "Error subscribing to OnBeforeAttack event");
	TryCatch(Event::Subscribe(Event::OnCastSpell, &OnCastSpell), "Error subscribing to OnCastSpell event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");
	TryCatch(Event::UnSubscribe(Event::OnBeforeAttack, &OnBeforeAttack), "Error unsubscribing to OnBeforeAttack event");
	TryCatch(Event::UnSubscribe(Event::OnCastSpell, &OnCastSpell), "Error unsubscribing to OnCastSpell event");
}

void Functions::UseQ(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastQ()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > TristanaSpellsSettings::GetQRange()) return;

	Engine::CastSelf(SpellIndex::Q);
	QCastedTime = gameTime;
}

void Functions::UseW(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastW()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > TristanaSpellsSettings::GetWRange()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
		WCastedTime = gameTime;
		return;
	}

	Modules::prediction::PredictionOutput wPrediction;
	if (GetPrediction(database.TristanaW, wPrediction)) {
		Engine::CastToPosition(SpellIndex::W, wPrediction.position);
		WCastedTime = gameTime;
	}
}

void Functions::UseE(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastE()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > TristanaSpellsSettings::GetERange()) return;

	Engine::CastTargeted(SpellIndex::E, obj);
	ECastedTime = gameTime;
}

void Functions::UseR(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastR()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > TristanaSpellsSettings::GetRRange()) return;

	Engine::CastTargeted(SpellIndex::R, obj);
	RCastedTime = gameTime;
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
	const float eDamage = Damages::ESpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float comboDamage = wDamage + eDamage + rDamage;

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

	if (isTimeToCastW()) {
		const float wDamage = Damages::WSpell::GetDamage(obj);
		render::RenderTextWorld("W: " + std::to_string(ceil(wDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
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
	if (TristanaSpellsSettings::DrawW->Value == true && (TristanaSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastW() || !TristanaSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(TristanaSpellsSettings::GetWRange());

	if (TristanaSpellsSettings::DrawE->Value == true && (TristanaSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastE() || !TristanaSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(TristanaSpellsSettings::GetERange());

	if (TristanaSpellsSettings::DrawR->Value == true && (TristanaSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastR() || !TristanaSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(TristanaSpellsSettings::GetRRange());

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (TristanaSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (TristanaSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;

	gameTime = Engine::GetGameTime();

	Modes::Killsteal();
	Modes::AntiGapCloser();
	Modes::AntiMelee();

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

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (TristanaCombo::UseE->Value == true && isTimeToCastE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}
	}
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetWRange());
	if (minionsInRange > 0) {
		if (TristanaClear::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

		if (TristanaClear::UseQ->Value && isTimeToCastQ()) {
			const auto qTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange(), Alliance::Enemy);
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}

		if (TristanaClear::UseE->Value && isTimeToCastE()) {
			const auto eTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetERange(), Alliance::Enemy);
			if (eTarget != nullptr) {
				const auto minionsAroundWTarget = ObjectManager::CountMinionsInRange(Alliance::Enemy, eTarget->GetPosition(), 600.0f);
				if (minionsAroundWTarget >= TristanaClear::GetMinimumMinions()) {
					Functions::UseE(eTarget);
				}
			}
		}
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetWRange());
		if (jungleMonstersInRange > 0) {
			if (TristanaJungle::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

			if (TristanaJungle::UseE->Value && isTimeToCastE()) {
				const auto eMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetERange());
				if (eMonster != nullptr) {
					Functions::UseE(eMonster);
				}
			}

			if (TristanaJungle::UseQ->Value && isTimeToCastQ()) {
				const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
				if (qMonster != nullptr) {
					Functions::UseQ(qMonster);
				}
			}

			if (TristanaJungle::UseW->Value && isTimeToCastW()) {
				const auto wMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetWRange());
				if (wMonster != nullptr) {
					if (Functions::GetBombStacks(wMonster) >= 3) {
						Functions::UseW(wMonster);
					}
				}
			}
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (TristanaHarass::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

	if (TristanaHarass::UseE->Value && isTimeToCastE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetERange());
		if (eTarget != nullptr) {
			Functions::UseE(eTarget);
		}
	}

	if (TristanaHarass::UseQ->Value && isTimeToCastQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
		if (qTarget != nullptr) {
			Functions::UseQ(qTarget);
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > TristanaSpellsSettings::GetWRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		const float heroHealth = hero->GetHealth() + hero->GetShield();
		if (TristanaKillsteal::UseR->Value && isTimeToCastR() && heroHealth < Damages::RSpell::GetDamage(hero)) {
			Functions::UseR(hero);
			break;
		}
	}
}

void Modes::AntiGapCloser()
{
	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
	{
		if (!target) continue;
		if (target->GetDistanceTo(ObjectManager::GetLocalPlayer()) > TristanaSpellsSettings::GetWRange()) continue;
		if (!Engine::MenuItemContains(TristanaAntiGapCloser::whitelist, target->GetName().c_str())) continue;
		if (!target->GetAiManager()->IsDashing()) continue;
		if (target->GetBuffByName("rocketgrab2")) continue;

		if (target != nullptr) {
			if (TristanaAntiGapCloser::UseW->Value && isTimeToCastW())
				Functions::UseW(target);

			if (TristanaAntiGapCloser::UseR->Value && isTimeToCastR())
				Functions::UseR(target);
		}
	}
}

void Modes::AntiMelee()
{
	if (TristanaAntiGapCloser::UseW->Value == true && isTimeToCastW())
	{
		for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
		{
			if (!target) continue;
			if (target->GetDistanceTo(ObjectManager::GetLocalPlayer()) > TristanaSpellsSettings::GetWRange()) continue;
			if (!Engine::MenuItemContains(TristanaAntiMelee::whitelist, target->GetName().c_str())) continue;

			if (target != nullptr) {
				Functions::UseW(target);
			}
		}
	}
}

void Events::OnBeforeAttack() {
	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack) 	{
		if (TristanaCombo::UseQ->Value && isTimeToCastQ()) {
			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}

	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear) {
		const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
		if (minionsInRange > 0) {
			if (TristanaClear::UseQ->Value && isTimeToCastQ()) {
				const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
				if (qTarget != nullptr) {
					Functions::UseQ(qTarget);
				}
			}
		}
		else {
			const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
			if (jungleMonstersInRange > 0) {
				if (TristanaJungle::UseQ->Value && isTimeToCastQ()) {
					const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
					if (qTarget != nullptr) {
						Functions::UseQ(qTarget);
					}
				}
			}
		}
	}

	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass) {
		if (TristanaHarass::UseQ->Value && isTimeToCastQ()) {
			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}
}

void Events::OnCastSpell(SpellCast* spellCastInfo) {
	if (TristanaCombo::UseE->Value == false) return;
	if (Engine::GetSpellState(SpellIndex::E) != 0) return;
	if (spellCastInfo == nullptr) return;
	if (spellCastInfo->GetCasterHandle() != ObjectManager::GetLocalPlayer()->GetHandle()) return;
	if (spellCastInfo->GetSpellId() != SpellIndex::W) return;

	const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TristanaSpellsSettings::GetERange());
	if (eTarget != nullptr) {
		if (eTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= TristanaSpellsSettings::GetERange()) {
			Engine::CastTargeted(SpellIndex::E, eTarget);
			ECastedTime = gameTime;
		}
	}
}