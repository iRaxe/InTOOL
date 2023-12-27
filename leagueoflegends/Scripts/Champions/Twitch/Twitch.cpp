#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../../../Orbwalker.h"
#include "Twitch.h"

using namespace UPasta::SDK;
using namespace UPasta::Plugins::Twitch;
using namespace UPasta::Plugins::Twitch::Config;

float TwitchGameTime = 0.0f;

float TwitchQCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastTwitchQ() {
	return TwitchGameTime > TwitchQCastedTime + database.TwitchQ.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
}

float TwitchWCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastTwitchW() {
	return TwitchGameTime > TwitchWCastedTime + database.TwitchW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
}

float TwitchECastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastTwitchE() {
	return TwitchGameTime > TwitchECastedTime + database.TwitchE.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
}

float TwitchRCastedTime = 0.0f;
[[nodiscard]] bool isTimeToCastTwitchR() {
	return TwitchGameTime > TwitchRCastedTime + database.TwitchR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
}

inline bool Functions::HasTwitchE(Object* obj) {
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange())) 
		return false;

	const auto deadlyVenomBuff = obj->GetBuffByName("TwitchDeadlyVenom");
	if (deadlyVenomBuff == nullptr) return false;
	if (!deadlyVenomBuff->isActive()) return false;

	return true;
}

inline int Functions::GetDeadlyVenomStacks(Object* obj) {
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange())) 
		return 0;

	if (!HasTwitchE(obj)) return 0;

	const auto deadlyVenomBuff = obj->GetBuffByName("TwitchDeadlyVenom");
	if (deadlyVenomBuff == nullptr) return 0;
	if (!deadlyVenomBuff->isActive()) return 0;


	return deadlyVenomBuff->GetStacks();
}

inline float Functions::GetTwitchEDamage(Object* obj)
{
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange()))
		return 0;

	if (!HasTwitchE(obj)) return 0;

	float damage = 0.0f;
	const int stacks = GetDeadlyVenomStacks(obj);
	if (stacks > 0)
	{
		const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::E)->GetLevel();
		const float base = levelSpell * 10 + 10;
		const int eDMG = 15 + (levelSpell - 1) * 5;
		const float attackDamage = base + (eDMG + (ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * 0.35f) * stacks);
		const float pDMG = Damage::CalculatePhysicalDamage(ObjectManager::GetLocalPlayer(), obj, attackDamage);
		const float abilityPowerDamage = (ObjectManager::GetLocalPlayer()->GetAbilityPower() * .30) * stacks;

		const float aDMG = Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), obj, abilityPowerDamage);
		damage = pDMG + aDMG;
	}

	return damage;
}

void Functions::InitializeMenu()
{
	const auto TwitchMenu = Menu::CreateMenu("vezTwitch", "Champion Settings");

	const auto comboMenu = TwitchMenu->AddMenu("Combo Settings", "Combo Settings");
	TwitchCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
	TwitchCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);

	TwitchCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	TwitchCombo::UseEWithStacks = comboMenu->AddCheckBox("Use E Stack For Damage", "Use SpellSlot E For Stacks", true);
	TwitchCombo::minStacks = comboMenu->AddSlider("minStacks", "Minimum Stacks", 6, 1, 6, 1);

	TwitchCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
	TwitchCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

	const auto harassMenu = TwitchMenu->AddMenu("Harass Settings", "Harass Settings");
	TwitchHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);

	TwitchHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	TwitchHarass::UseEWithStacks = harassMenu->AddCheckBox("Use E Stack For Damage", "Use SpellSlot E For Damage", true);
	TwitchHarass::minStacks = harassMenu->AddSlider("minStacks", "Minimum Stacks", 6, 1, 6, 1);

	TwitchHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

	const auto clearMenu = TwitchMenu->AddMenu("Clear Settings", "Clear Settings");

	const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
	TwitchClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
	TwitchClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", false);
	TwitchClear::minMinions = laneClearMenu->AddSlider("minMinions", "Minimum Minions", 3, 1, 6, 1);
	TwitchClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

	const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
	TwitchJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
	TwitchJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);

	TwitchJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
	TwitchJungle::UseEWithStacks = jungleMenu->AddCheckBox("Use E Stack For Damage", "Use SpellSlot E For Damage", true);
	TwitchJungle::minStacks = jungleMenu->AddSlider("minStacks", "Minimum Stacks", 6, 1, 6, 1);

	const auto additionalMenu = TwitchMenu->AddMenu("Additional Settings", "Additional Settings");

	const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
	TwitchKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

	const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
	TwitchAntiGapCloser::UseQ = antiGapMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
	TwitchAntiGapCloser::UseW = antiGapMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

	for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++) {
		auto obj = ObjectManager::GetHeroList()->GetIndex(i);
		if (obj != nullptr && obj->IsEnemy()) {
			const auto antiGap_checkbox = antiGapMenu->AddCheckBox(obj->GetName().c_str(), obj->GetName().c_str(), true,
				[obj](const CheckBox* self, bool newValue) {
					if (self->Value == false && !TwitchAntiGapCloser::whitelist.empty()) {
						const auto it = std::ranges::find(TwitchAntiGapCloser::whitelist, obj);
						TwitchAntiGapCloser::whitelist.erase(it);
					}
					else {
						TwitchAntiGapCloser::whitelist.push_back(obj);
					}
				});

			if (antiGap_checkbox->Value == true) {
				TwitchAntiGapCloser::whitelist.push_back(obj);
			}
		}
	}

	const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
	TwitchFlee::UseQ = fleeMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
	TwitchFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

	const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

	const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
	TwitchSpellsSettings::qRange = qSpellMenu->AddSlider("QRange", "Maximum Range", database.TwitchQ.GetRange(), 0, 1150, 50);
	TwitchSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Q Draw", "Draw Q", true);

	const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
	TwitchSpellsSettings::wRange = wSpellMenu->AddSlider("WRange", "Maximum Range", database.TwitchW.GetRange(), 100, 950, 50);
	TwitchSpellsSettings::DrawW = wSpellMenu->AddCheckBox("W Draw", "Draw W", true);

	const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
	TwitchSpellsSettings::eRange = eSpellMenu->AddSlider("ERange", "Maximum Range", database.TwitchE.GetRange(), 100, 1200, 50);
	TwitchSpellsSettings::DrawW = eSpellMenu->AddCheckBox("E Draw", "Draw E", true);

	const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
	TwitchSpellsSettings::rRange = rSpellMenu->AddSlider("RRange", "Maximum Range", database.TwitchR.GetRange(), 100, 1100, 50);
	TwitchSpellsSettings::DrawR = rSpellMenu->AddCheckBox("RDraw", "Draw R", true);

	TwitchSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

	const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
	TwitchSpellsSettings::DrawHPDamage = miscMenu->AddCheckBox("DrawHPDamage", "Draw E Damage over HealthBar", false);
	TwitchSpellsSettings::DrawPosDamage = miscMenu->AddCheckBox("DrawPosDamage", "Draw Damage on target position", false);
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

void Functions::UseQ() {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastTwitchQ()) return;

	Engine::CastSelf(SpellIndex::Q);
	TwitchQCastedTime = TwitchGameTime;
}

void Functions::UseW(Object* obj) {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastTwitchW()) return;

	if (obj->IsMinion() || obj->IsJungle()) {
		Engine::CastToPosition(SpellIndex::W, obj->GetPosition());
		TwitchWCastedTime = TwitchGameTime;
	}
	else {
		Modules::prediction::PredictionOutput wPrediction;
		if (GetPrediction(ObjectManager::GetLocalPlayer(), obj, database.TwitchW, wPrediction)) {
			Engine::CastToPosition(SpellIndex::W, wPrediction.position);
			TwitchWCastedTime = TwitchGameTime;
		}
	}
}

void Functions::UseE(Object* obj) {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastTwitchE()) return;

	Engine::CastSelf(SpellIndex::E);
	TwitchECastedTime = TwitchGameTime;
}

void Functions::UseR(Object* obj) {
	if (!Orbwalker::CanCastAfterAttack() || !isTimeToCastTwitchR()) return;

	Engine::CastSelf(SpellIndex::R);
	TwitchRCastedTime = TwitchGameTime;
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

	const float eDamage = GetTwitchEDamage(obj);

	static constexpr float yOffset = 23.5f;
	static constexpr float xOffset = -46.0f;
	static constexpr float widthMultiplier = 105;

	const float objHealth = obj->GetHealth();
	const float objMaxHealth = obj->GetMaxHealth();
	const float endOffset2 = xOffset + objHealth / objMaxHealth * widthMultiplier;
	const float startOffset2 = max(endOffset2 - (eDamage / objMaxHealth * widthMultiplier), xOffset);

	const ImVec2 topLeft = ImVec2(objHPBarScreenPos.x + startOffset2, objHPBarScreenPos.y - yOffset);
	const ImVec2 bottomRight = ImVec2(objHPBarScreenPos.x + endOffset2, objHPBarScreenPos.y - yOffset + 10.0f);

	const bool canKill = eDamage > objHealth;
	const auto drawColor = canKill ? COLOR_GREEN : COLOR_RED;
	render::RenderRect(topLeft, bottomRight, drawColor, 0.0f, 0, 1.0f, true);
}

void Functions::DrawDamageOnPos(Object* obj) {
	if (!TargetSelector::IsValid(obj, ObjectManager::GetLocalPlayer()->GetPosition(), 1500.0f)) return;

	const auto dmgPos = Engine::GetBaseDrawPosition(obj);
	if (!dmgPos.IsValid()) return;

	if (isTimeToCastTwitchE()) {
		const float eDamage = GetTwitchEDamage(obj);
		render::RenderTextWorld("E: " + std::to_string(ceil(eDamage)), Vector3(dmgPos.x, dmgPos.y, dmgPos.z), 16, COLOR_WHITE, false);
	}
}

void Events::OnDraw() {
	if (TwitchSpellsSettings::DrawQ->Value == true && (TwitchSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastTwitchQ() || !TwitchSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(TwitchSpellsSettings::GetQRange());

	if (TwitchSpellsSettings::DrawR->Value == true && (TwitchSpellsSettings::ShouldDrawOnlyIfReady() && isTimeToCastTwitchR() || !TwitchSpellsSettings::ShouldDrawOnlyIfReady()))
		Functions::DrawSpellRadius(TwitchSpellsSettings::GetRRange());

	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!hero) continue;
		if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > 1500.0f) continue;

		if (TwitchSpellsSettings::DrawPosDamage->Value == true) {
			Functions::DrawDamageOnPos(hero);
		}

		if (TwitchSpellsSettings::DrawHPDamage->Value == true) {
			Functions::DrawDamageOnHPBar(hero);
		}
	}
}

void Events::OnGameUpdate() {
	if (ObjectManager::GetLocalPlayer() == nullptr) return;
	if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;
	if (Orbwalker::State == Orbwalker::CHANNELING || Orbwalker::State == Orbwalker::DODGING) return;

	TwitchGameTime = Engine::GetGameTime();

	if (Orbwalker::Mode == Attack) {
		Modes::Combo();
	}
	if (Orbwalker::Mode == Harass) {
		Modes::Harass();
	}
	if (Orbwalker::Mode == Clear || Orbwalker::Mode == FastClear) {
		Modes::Clear();
	}
	if (Orbwalker::Mode == Flee) {
		Modes::Flee();
	}

	Modes::Killsteal();
	Modes::AntiGapCloser();
}

void Modes::Combo() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange()) > 0) return;

	if (TwitchCombo::UseQ->Value == true && isTimeToCastTwitchQ()) {
		const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetQRange());
		if (!TargetSelector::IsValid(qTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetQRange())) return;

		Functions::UseQ();
	}

	if (TwitchCombo::UseR->Value == true && isTimeToCastTwitchR()) {
		if (ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetRRange()) < TwitchCombo::GetMinimumREnemies()) return;

		const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetRRange());
		if (!TargetSelector::IsValid(rTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetRRange())) return;

		Functions::UseR(rTarget);
	}

	if (TwitchCombo::UseW->Value == true && isTimeToCastTwitchW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange());
		if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) return;

		Functions::UseW(wTarget);
	}
}

void Modes::Clear() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange());
	if (minionsInRange > 0) {
		if (TwitchClear::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;
		if (minionsInRange < TwitchClear::GetMinimumMinions()) return;

		if (TwitchClear::UseE->Value && isTimeToCastTwitchE()) {
			int kill_count = 0;
			for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy)) {
				if (!TargetSelector::IsValid(minion, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange())) continue;
				if (minion->GetHealth() < Functions::GetTwitchEDamage(minion)) {
					kill_count++;
					if (kill_count >= TwitchClear::GetMinimumMinions()) {
						Functions::UseE(minion);
						break;
					}
				}
			}
		}
		if (TwitchClear::UseW->Value && isTimeToCastTwitchW()) {
			const auto wTarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange(), Alliance::Enemy);
			if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) return;

			Functions::UseW(wTarget);
		}
	}
	else {
		const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetQRange());
		if (jungleMonstersInRange > 0) {
			if (TwitchJungle::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

			if (TwitchJungle::UseQ->Value && isTimeToCastTwitchQ()) {
				const auto qTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetQRange());
				if (!TargetSelector::IsValid(qTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetQRange())) return;

				Functions::UseQ();
			}

			if (TwitchJungle::UseW->Value && isTimeToCastTwitchW()) {
				const auto wTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange());
				if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) return;

				Functions::UseW(wTarget);
			}

			if (TwitchJungle::UseE->Value && isTimeToCastTwitchE()) {
				const auto eTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange());
				if (!TargetSelector::IsValid(eTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange())) return;
				if (TwitchJungle::GetMinimumEStacks() < Functions::GetDeadlyVenomStacks(eTarget) || Functions::GetTwitchEDamage(eTarget) > eTarget->GetHealth()) return;

				Functions::UseE(eTarget);
			}
		}
	}
}

void Modes::Harass() {
	if (!Orbwalker::CanCastAfterAttack()) return;
	if (TwitchHarass::GetMinimumMana() >= ObjectManager::GetLocalPlayer()->GetPercentMana()) return;

	const auto enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange());
	if (enemiesInRange == 0) return;

	if (TwitchHarass::UseW->Value && isTimeToCastTwitchW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange());
		if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) return;

		Functions::UseW(wTarget);
	}

	if (TwitchHarass::UseE->Value && isTimeToCastTwitchE()) {
		const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange());
		if (!TargetSelector::IsValid(eTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange())) return;
		const bool canKillTarget = eTarget->GetHealth() + eTarget->GetShield() < Functions::GetTwitchEDamage(eTarget);
		const bool shouldUseE = TwitchCombo::UseEWithStacks->Value && Functions::GetDeadlyVenomStacks(eTarget) >= TwitchHarass::GetMinimumEStacks();

		if (canKillTarget || shouldUseE) {
			Functions::UseE(eTarget);
		}
	}
}

void Modes::Killsteal() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange());
	if (enemiesInRange == 0) return;

	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
		if (!TargetSelector::IsValid(target, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetERange())) continue;

		const float heroHealth = target->GetHealth() + target->GetShield();
		if (TwitchKillsteal::UseE->Value && isTimeToCastTwitchE() && heroHealth < Functions::GetTwitchEDamage(target)) {
			Functions::UseE(target);
			break;
		}
	}
}

void Modes::AntiGapCloser() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	const auto enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange());
	if (enemiesInRange == 0) return;

	for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
	{
		if (!target) continue;
		if (!TargetSelector::IsValid(target, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) continue;
		if (!Engine::MenuItemContains(TwitchAntiGapCloser::whitelist, target->GetName().c_str())) continue;

		if (!target->GetAiManager()->IsDashing()) continue;
		if (target->GetBuffByName("rocketgrab2")) continue;

		if (TwitchAntiGapCloser::UseW->Value && isTimeToCastTwitchW())
			Functions::UseW(target);

		if (TwitchAntiGapCloser::UseQ->Value && isTimeToCastTwitchQ())
			Functions::UseQ();
	}
}

void Modes::Flee() {
	if (!Orbwalker::CanCastAfterAttack()) return;

	if (TwitchFlee::UseQ->Value && isTimeToCastTwitchQ())
		Functions::UseQ();

	if (TwitchFlee::UseW->Value && isTimeToCastTwitchW()) {
		const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange());
		if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) return;

		Functions::UseW(wTarget);
	}
}

void Events::OnAfterAttack() {
	if (Orbwalker::Mode == Attack) {
		if (TwitchCombo::UseW->Value == true && isTimeToCastTwitchW()) {
			const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange());
			if (!TargetSelector::IsValid(wTarget, ObjectManager::GetLocalPlayer()->GetPosition(), TwitchSpellsSettings::GetWRange())) return;
			if (!ObjectManager::GetLocalPlayer()->IsFacing(wTarget, 90.0f)) return;

			Functions::UseW(wTarget);
		}
	}
}