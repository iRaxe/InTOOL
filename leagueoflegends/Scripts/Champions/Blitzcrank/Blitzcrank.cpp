#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../../../Orbwalker.h"
#include "Blitzcrank.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Blitzcrank;
using namespace UPasta::Plugins::Blitzcrank::Config;

float BlitzcrankGameTime = 0.0f;

float BlitzcrankQCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastBlitzcrankQ() {
	return BlitzcrankGameTime > BlitzcrankQCastedTime + database.BlitzcrankQ.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float BlitzcrankWCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastBlitzcrankW() {
	return BlitzcrankGameTime > BlitzcrankWCastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float BlitzcrankECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastBlitzcrankE() {
	return BlitzcrankGameTime > BlitzcrankECastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float BlitzcrankRCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastBlitzcrankR() {
	return BlitzcrankGameTime > BlitzcrankRCastedTime + database.BlitzcrankR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

void Functions::InitializeMenu()
{
	const auto BlitzcrankMenu = Menu::CreateMenu("vezBlitzcrank", "Champion Settings");

	const auto comboMenu = BlitzcrankMenu->AddMenu("Combo Settings", "Combo Settings");
	BlitzcrankCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BlitzcrankCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	BlitzcrankCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	BlitzcrankCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	BlitzcrankCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = BlitzcrankMenu->AddMenu("Harass Settings", "Harass Settings");
	BlitzcrankHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BlitzcrankHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

	const auto clearMenu = BlitzcrankMenu->AddMenu("Clear Settings", "Clear Settings");
	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	BlitzcrankJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
	BlitzcrankJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
	BlitzcrankJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	BlitzcrankJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto additionalMenu = BlitzcrankMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	BlitzcrankKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	BlitzcrankKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

	const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
	BlitzcrankAntiGapCloser::UseR = antiGapMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

	for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++) {
		auto obj = ObjectManager::GetHeroList()->GetIndex(i);
		if (obj != nullptr && obj->IsEnemy()) {
			const auto antiGap_checkbox = antiGapMenu->AddCheckBox(obj->GetName().c_str(), obj->GetName().c_str(), true,
				[obj](const CheckBox* self, bool newValue) {
					if (self->Value == false && !BlitzcrankAntiGapCloser::whitelist.empty()) {
						const auto it = std::ranges::find(BlitzcrankAntiGapCloser::whitelist, obj);
						BlitzcrankAntiGapCloser::whitelist.erase(it);
					}
					else {
						BlitzcrankAntiGapCloser::whitelist.push_back(obj);
					}
				});

			if (antiGap_checkbox->Value == true) {
				BlitzcrankAntiGapCloser::whitelist.push_back(obj);
			}
		}
	}

	const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
	BlitzcrankFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	BlitzcrankSpellsSettings::qRange = qSpellMenu->AddSlider("QRange", "Maximum Range", database.BlitzcrankQ.GetRange(), 0, 1150, 50);
	BlitzcrankSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Q Draw", "Draw Q", true);


	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	BlitzcrankSpellsSettings::wRange = wSpellMenu->AddSlider("WRange", "Maximum Range", database.BlitzcrankQ.GetRange(), 100, 1150, 50);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	BlitzcrankSpellsSettings::rRange = rSpellMenu->AddSlider("RRange", "Maximum Range", 600, 100, 600, 50);
	BlitzcrankSpellsSettings::DrawR = rSpellMenu->AddCheckBox("RDraw", "Draw R", true);

	BlitzcrankSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	BlitzcrankSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw Damage over HealthBar", false);
	BlitzcrankSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
}

void Events::Initialize() {
	TryCatch(Functions::InitializeMenu(), "Error initializing the menu");
	TryCatch(Subscribe(), "Error subscribing to events");
}

void Events::Subscribe() {
	TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnDraw event");
	TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameTick event");
	TryCatch(Event::Subscribe(Event::OnAfterAttack, &OnAfterAttack), "Error subscribing to OnAfterAttack event");
}

void Events::Unsubscribe() {
	TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
	TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
	TryCatch(Event::UnSubscribe(Event::OnAfterAttack, &OnAfterAttack), "Error unsubscribing to OnAfterAttack event");
}

void Functions::UseQ(Object* obj) {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastBlitzcrankQ()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::Q, obj->GetPosition());
		BlitzcrankQCastedTime = BlitzcrankGameTime;
	}
	else {
		Modules::prediction::PredictionOutput qPrediction;
		if (GetPrediction(ObjectManager::GetLocalPlayer(), obj, database.BlitzcrankQ, qPrediction)) {
			Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
			BlitzcrankQCastedTime = BlitzcrankGameTime;
		}
	}

}

void Functions::UseW(Object* obj) {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastBlitzcrankW()) return;

	Engine::CastSelf(SpellIndex::W);
	BlitzcrankWCastedTime = BlitzcrankGameTime;
}

void Functions::UseE(Object* obj) {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastBlitzcrankE()) return;

	Engine::CastSelf(SpellIndex::E);
	BlitzcrankECastedTime = BlitzcrankGameTime;
}

void Functions::UseR(Object* obj) {
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange())) return;

	Engine::CastSelf(SpellIndex::R);
	BlitzcrankRCastedTime = BlitzcrankGameTime;
}

void Functions::DrawSpellRadius(float range) {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (!ObjectManager::GetLocalPlayer()->IsTargetable()) return;

	Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), range, COLOR_WHITE, 1.0f);
	return;
}

void Functions::DrawDamageOnHPBar(Object* obj) {
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), 1500.0f)) return;

	const Vector2 objHPBarScreenPos = Engine::GetHpBarPosition(obj);
	if (!objHPBarScreenPos.IsValid()) return;

	const float qDamage = Damages::QSpell::GetDamage(obj);
	const float rDamage = Damages::RSpell::GetDamage(obj);
	const float comboDamage = qDamage + rDamage;

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
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), 1500.0f)) return;

	const auto dmgPos = Engine::GetBaseDrawPosition(obj);
	if (!dmgPos.IsValid()) return;

	float yOffset = 0;

	if (isTimeToCastBlitzcrankQ()) {
		const float qDamage = Damages::QSpell::GetDamage(obj);
		render::RenderTextWorld("Q: " + std::to_string(ceil(qDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false); yOffset += 20;
	}

	if (isTimeToCastBlitzcrankR()) {
		const float rDamage = Damages::RSpell::GetDamage(obj);
		render::RenderTextWorld("R: " + std::to_string(ceil(rDamage)), Vector3(dmgPos.x, dmgPos.y - yOffset, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {
	if (BlitzcrankSpellsSettings::DrawQ->Value == true && (BlitzcrankSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastBlitzcrankQ() || !BlitzcrankSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(BlitzcrankSpellsSettings::GetQRange());

	if (BlitzcrankSpellsSettings::DrawR->Value == true && (BlitzcrankSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastBlitzcrankR() || !BlitzcrankSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(BlitzcrankSpellsSettings::GetRRange());

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (BlitzcrankSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (BlitzcrankSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	BlitzcrankGameTime = Engine::GetGameTime();

	if (Orbwalker::Mode == Attack) {
		Modes::Combo();
	}
	if (Orbwalker::Mode == Harass) {
		Modes::Harass();
	}
	if (Orbwalker::Mode == Clear || Orbwalker::Mode == FastClear) {
		Modes::Clear();
	}

	Modes::Killsteal();
	Modes::AntiGapCloser();
}

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange()) > 0) return;

	if (BlitzcrankCombo::UseW->Value == true && isTimeToCastBlitzcrankW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetWRange());
		if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetWRange())) return;
		if (!ObjectManager::GetLocalPlayer()->IsFacing(wTarget, 90.0f)) return;

		Functions::UseW(wTarget);
	}

	if (BlitzcrankCombo::UseQ->Value == true && isTimeToCastBlitzcrankQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange());
		if (!TargetSelector::IsValid(qTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange())) return;

		Functions::UseQ(qTarget);
	}

	if (BlitzcrankCombo::UseR->Value == true && isTimeToCastBlitzcrankR()) {
		if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange()) < BlitzcrankCombo::GetMinimumREnemies()) return;

		const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange());
		if (!TargetSelector::IsValid(rTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange())) return;

		Functions::UseR(rTarget);
	}
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange());
	if (jungleMonstersInRange > 0) {
		if (BlitzcrankJungle::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

		if (BlitzcrankJungle::UseQ->Value && isTimeToCastBlitzcrankQ()) {
			const auto qTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange());
			if (!TargetSelector::IsValid(qTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange())) return;

			Functions::UseQ(qTarget);
		}

		if (BlitzcrankJungle::UseW->Value && isTimeToCastBlitzcrankW()) {
			const auto wTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetWRange());
			if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetWRange())) return;

			Functions::UseW(wTarget);
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (BlitzcrankHarass::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

	const auto enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange());
	if (enemiesInRange == 0) return;

	const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange());
	if (!TargetSelector::IsValid(qTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange())) return;

	Functions::UseQ(qTarget);
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange());
	if (enemiesInRange == 0) return;

	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!target) continue;
		if (!TargetSelector::IsValid(target, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetQRange())) continue;

		const float heroHealth = target->GetHealth() + target->GetShield();
		if (BlitzcrankKillsteal::UseQ->Value && isTimeToCastBlitzcrankQ() && heroHealth < Damages::QSpell::GetDamage(target)) {
			Functions::UseQ(target);
			break;
		}

		if (BlitzcrankKillsteal::UseR->Value && isTimeToCastBlitzcrankR() && heroHealth < Damages::RSpell::GetDamage(target)) {
			Functions::UseR(target);
			break;
		}
	}
}

void Modes::AntiGapCloser()
{
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange());
	if (enemiesInRange == 0) return;

	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
	{
		if (!target) continue;
		if (!TargetSelector::IsValid(target, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetRRange())) continue;
		if (!Engine::MenuItemContains(BlitzcrankAntiGapCloser::whitelist, target->GetName().c_str())) continue;

		if (!target->GetAiManager()->IsDashing()) continue;
		if (target->GetBuffByName("rocketgrab2")) continue;

		if (BlitzcrankAntiGapCloser::UseR->Value && isTimeToCastBlitzcrankR())
			Functions::UseR(target);
	}
}

void Events::OnAfterAttack() {
	if (Orbwalker::Mode == Attack) {
		if (BlitzcrankCombo::UseE->Value && isTimeToCastBlitzcrankE()) {
			const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetERange());
			if (!TargetSelector::IsValid(eTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetERange())) return;

			Functions::UseE(eTarget);
		}
	}

	if (Orbwalker::Mode == Clear) {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetERange());
		if (jungleMonstersInRange > 0) {
			if (BlitzcrankJungle::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;
			if (BlitzcrankJungle::UseE->Value == false) return;
			if (!isTimeToCastBlitzcrankE()) return;

			const auto eTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetERange());
			if (!TargetSelector::IsValid(eTarget, ObjectManager::GetLocalPlayer()->GetPosition(), BlitzcrankSpellsSettings::GetERange())) return;

			Functions::UseE(eTarget);
		}
	}
}