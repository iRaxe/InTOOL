#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../../../Orbwalker.h"
#include "Ashe.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Ashe;
using namespace UPasta::Plugins::Ashe::Config;

float asheGameTime = 0.0f;

float AsheQCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastAsheQ() {
	return asheGameTime > AsheQCastedTime + 0.25f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float AsheWCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastAsheW() {
	return asheGameTime > AsheWCastedTime + database.AsheW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float AsheRCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastAsheR() {
	return asheGameTime > AsheRCastedTime + database.AsheR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

inline bool Functions::HasAsheQ() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return false;
	auto Ashe = ObjectManager::GetLocalPlayer();
	if (!Ashe->IsAlive()) return false;
	if (!Ashe->IsTargetable()) return false;

	const auto qBuff = Ashe->GetBuffByName("asheqcastready");
	if (qBuff == nullptr) return false;
	if (!qBuff->isActive()) return false;

	return true;
}

void Functions::InitializeMenu()
{
	const auto AsheMenu = Menu::CreateMenu("vezAshe", "Champion Settings");

	const auto comboMenu = AsheMenu->AddMenu("Combo Settings", "Combo Settings");
	AsheCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	AsheCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	AsheCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	AsheCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = AsheMenu->AddMenu("Harass Settings", "Harass Settings");
	AsheHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	AsheHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	AsheHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

	const auto clearMenu = AsheMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	AsheClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	AsheClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	AsheClear::minMinions = laneClearMenu->AddSlider("minMinions", "Minimum Minions in W Width", 3, 1, 6, 1);
	AsheClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	AsheJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	AsheJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	AsheJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
	AsheLastHit::UseW = lastHitMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	AsheLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = AsheMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	AsheKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	AsheKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

	const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
	AsheAntiGapCloser::UseW = antiGapMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
	AsheAntiGapCloser::UseR = antiGapMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

	const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
	AsheAntiMelee::UseW = antiMeleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
	AsheAntiMelee::UseR = antiMeleeMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

	for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++) {
		auto obj = ObjectManager::GetHeroList()->GetIndex(i);
		if (obj != nullptr && obj->IsEnemy()) {
			const auto antiGap_checkbox = antiGapMenu->AddCheckBox(obj->GetName().c_str(), obj->GetName().c_str(), true,
				[obj](const CheckBox* self, bool newValue) {
					if (self->Value == false && !AsheAntiGapCloser::whitelist.empty()) {
						const auto it = std::ranges::find(AsheAntiGapCloser::whitelist, obj);
						AsheAntiGapCloser::whitelist.erase(it);
					}
					else {
						AsheAntiGapCloser::whitelist.push_back(obj);
					}
				});

			if (antiGap_checkbox->Value == true) {
				AsheAntiGapCloser::whitelist.push_back(obj);
			}

			if (!obj->IsMelee()) continue;
			const auto antiMelee_checkbox = antiMeleeMenu->AddCheckBox(obj->GetName().c_str(), obj->GetName().c_str(), true,
				[obj](const CheckBox* self, bool newValue) {
					if (self->Value == false && !AsheAntiMelee::whitelist.empty()) {
						const auto it = std::ranges::find(AsheAntiMelee::whitelist, obj);
						AsheAntiMelee::whitelist.erase(it);
					}
					else {
						AsheAntiMelee::whitelist.push_back(obj);
					}
				});

			if (antiMelee_checkbox->Value == true) {
				AsheAntiMelee::whitelist.push_back(obj);
			}
		}
	}

	const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
	AsheFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	AsheSpellsSettings::UseWIfInAARange = wSpellMenu->AddCheckBox("Use W In AA Range", "Use SpellSlot W In AA Range", true);
	AsheSpellsSettings::UseWIfFullAASpeed = wSpellMenu->AddCheckBox("UseWIfFullAASpeed", "Use SpellSlot W If Full AA Speed", true);

	AsheSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.AsheW.GetRange(), 100, database.AsheW.GetRange(), 50);
	AsheSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	AsheSpellsSettings::targetMode = rSpellMenu->AddList("targetMode", "Target Mode", std::vector<std::string>{"Inherit", "Near Mouse", "Near Champion"}, 0);
	AsheSpellsSettings::rTapKey = rSpellMenu->AddKeyBind("rTapKey", "Aim SpellSlot R Key", VK_CONTROL, false, false);
	AsheSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "Minimum Distance", 1000, 100, database.AsheR.GetRange(), 100);
	AsheSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "Maximum Distance", 3000, 100, database.AsheR.GetRange(), 100);

	AsheSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	AsheSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	AsheSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	AsheSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
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
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastAsheQ()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > AsheSpellsSettings::GetQRange()) return;

	Engine::CastSelf(SpellIndex::Q);
	AsheQCastedTime = asheGameTime;
}

void Functions::UseW(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastAsheW()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > AsheSpellsSettings::GetWRange()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
		AsheWCastedTime = asheGameTime;
		return;
	}

	Modules::prediction::PredictionOutput wPrediction;
	if (GetPrediction(database.AsheW, wPrediction)) {
		Engine::CastToPosition(SpellIndex::W, wPrediction.position);
		AsheWCastedTime = asheGameTime;
	}
}

void Functions::UseR(Object* obj) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	if (obj == nullptr) return;
	if (!obj->IsAlive()) return;
	if (!obj->IsTargetable()) return;
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastAsheR()) return;

	if (obj->GetDistanceTo(ObjectManager::GetLocalPlayer()) > AsheSpellsSettings::GetMinRRange()) return;

	Modules::prediction::PredictionOutput rPrediction;
	if (GetPrediction(database.AsheW, rPrediction)) {
		Engine::CastToPosition(SpellIndex::R, rPrediction.position);
		AsheRCastedTime = asheGameTime;
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

	if (isTimeToCastAsheW()) {
		const float wDamage = Damages::WSpell::GetDamage(obj);
		render::RenderTextWorld("W: " + std::to_string(ceil(wDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastAsheR()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {
	if (AsheSpellsSettings::DrawW->Value == true && (AsheSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastAsheW() || !AsheSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(AsheSpellsSettings::GetWRange());

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (AsheSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (AsheSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;

	asheGameTime = Engine::GetGameTime();

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

	if (param == AsheSpellsSettings::rTapKey->Key) {
		switch (msg) {
			case WM_KEYDOWN: Modes::AimR(); break;
			case WM_KEYUP: break;
		}
	}

}

void Modes::AimR() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (isTimeToCastAsheR()) {
		switch (AsheSpellsSettings::targetMode->Value)
		{
			case 0: //Inherit
				if (const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetMaxRRange()); rTarget != nullptr)
					Functions::UseR(rTarget);
				break;
			case 1: //NearMouse
				if (const auto rTarget2 = TargetSelector::FindBestTarget(Engine::GetMouseWorldPos(), 300.0f); rTarget2 != nullptr)
					Functions::UseR(rTarget2);
				break;
			case 2: //NearChampion
				if (const auto rTarget3 = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), 500.0f); rTarget3 != nullptr)
					Functions::UseR(rTarget3);
			break;
		}
	}
}

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	//Usare questo
	if (AsheCombo::UseR->Value == true && isTimeToCastAsheR()
		&& ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetMinRRange()) >= AsheCombo::GetMinimumREnemies())
	{
		if (const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetMinRRange()))
			Functions::UseR(rTarget);
	}

}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetWRange());
	if (minionsInRange > 0) {
		if (AsheClear::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

		if (AsheClear::UseQ->Value && isTimeToCastAsheQ()) {
			const auto qTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange(), Alliance::Enemy);
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}

		
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetWRange());
		if (jungleMonstersInRange > 0) {
			if (AsheJungle::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

			

			if (AsheJungle::UseQ->Value && isTimeToCastAsheQ()) {
				const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
				if (qMonster != nullptr) {
					Functions::UseQ(qMonster);
				}
			}

			
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (AsheHarass::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

	

	if (AsheHarass::UseQ->Value && isTimeToCastAsheQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
		if (qTarget != nullptr) {
			Functions::UseQ(qTarget);
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > AsheSpellsSettings::GetWRange() + hero->GetBoundingRadius() / 2) continue;
		if (hero->IsInvulnerable()) continue;

		const float heroHealth = hero->GetHealth() + hero->GetShield();
		if (AsheKillsteal::UseR->Value && isTimeToCastAsheR() && heroHealth < Damages::RSpell::GetDamage(hero)) {
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
		if (target->GetDistanceTo(ObjectManager::GetLocalPlayer()) > AsheSpellsSettings::GetWRange()) continue;
		if (!Engine::MenuItemContains(AsheAntiGapCloser::whitelist, target->GetName().c_str())) continue;
		if (!target->GetAiManager()->IsDashing()) continue;
		if (target->GetBuffByName("rocketgrab2")) continue;

		if (target != nullptr) {
			if (AsheAntiGapCloser::UseW->Value && isTimeToCastAsheW())
				Functions::UseW(target);

			if (AsheAntiGapCloser::UseR->Value && isTimeToCastAsheR())
				Functions::UseR(target);
		}
	}
}

void Modes::AntiMelee()
{
	if (AsheAntiGapCloser::UseW->Value == true && isTimeToCastAsheW())
	{
		for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
		{
			if (!target) continue;
			if (target->GetDistanceTo(ObjectManager::GetLocalPlayer()) > AsheSpellsSettings::GetWRange()) continue;
			if (!Engine::MenuItemContains(AsheAntiMelee::whitelist, target->GetName().c_str())) continue;

			if (target != nullptr) {
				Functions::UseW(target);
			}
		}
	}
}

void Events::OnBeforeAttack() {
	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack) {
		if (AsheCombo::UseQ->Value && isTimeToCastAsheQ()) {
			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}

	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear) {
		const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
		if (minionsInRange > 0) {
			if (AsheClear::UseQ->Value && isTimeToCastAsheQ()) {
				const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
				if (qTarget != nullptr) {
					Functions::UseQ(qTarget);
				}
			}
		}
		else {
			const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
			if (jungleMonstersInRange > 0) {
				if (AsheJungle::UseQ->Value && isTimeToCastAsheQ()) {
					const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
					if (qTarget != nullptr) {
						Functions::UseQ(qTarget);
					}
				}
			}
		}
	}

	if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass) {
		if (AsheHarass::UseQ->Value && isTimeToCastAsheQ()) {
			const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), AsheSpellsSettings::GetQRange());
			if (qTarget != nullptr) {
				Functions::UseQ(qTarget);
			}
		}
	}
}

void Events::OnCastSpell(SpellCast* spellCastInfo) {
	if (Engine::GetSpellState(SpellIndex::E) != 0) return;
	if (spellCastInfo == nullptr) return;
	if (spellCastInfo->GetCasterHandle() != ObjectManager::GetLocalPlayer()->GetHandle()) return;
	if (spellCastInfo->GetSpellId() != SpellIndex::W) return;

	
}