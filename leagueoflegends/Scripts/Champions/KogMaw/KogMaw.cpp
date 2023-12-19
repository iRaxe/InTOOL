#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Kogmaw.h"

using namespace UPasta::SDK;

class KogMawModule : public ChampionModule
{
private:
    std::string name = SP_STRING("KogMaw");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;


    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + database.KogMawQ.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime + database.KogMawW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime + database.KogMawE.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.KogMawR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(KogMawConfig::KogMawClear::minMana->Value);  // NOLINT(bugprone-branch-clone)
            else if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(KogMawConfig::KogMawClear::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(KogMawConfig::KogMawClear::minMana->Value);
            break;
        default:
            return false;
        }

        return ObjectManager::GetLocalPlayer()->GetPercentMana() > minManaThreshold;
    }

    static float qRange() {
        return static_cast<float>(KogMawConfig::KogMawSpellsSettings::qRange->Value);
    }

    static float wRange() {
        return ObjectManager::GetLocalPlayer()->GetRealAttackRange() + KogMawDamages::WSpell::addtionalRangeArray[ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::W)->GetLevel()] + static_cast<float>(KogMawConfig::KogMawSpellsSettings::wRangeOffset->Value);
    }

    static float eRange() {
        return static_cast<float>(KogMawConfig::KogMawSpellsSettings::eRange->Value);
    }

    static float rRange() {
        return static_cast<float>(KogMawConfig::KogMawSpellsSettings::rRange->Value);
    }

    static float qMinArmor() {
        return static_cast<float>(KogMawConfig::KogMawSpellsSettings::qMinArmor->Value);
    }

    static float KogQDamage(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q))
            return -9999;

        const int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = KogMawDamages::QSpell::dmgSkillArray[level];
        const float abilityPowerModifier = ObjectManager::GetLocalPlayer()->GetAbilityPower() * KogMawDamages::QSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), target, damage);
    }


    static float KogEDamage(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E))
            return -9999;

        const int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::E)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = KogMawDamages::ESpell::dmgSkillArray[level];
        const float abilityPowerModifier = ObjectManager::GetLocalPlayer()->GetAbilityPower() * KogMawDamages::ESpell::additionalPercentageAP;
        const float attackDamageModifier = ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * KogMawDamages::ESpell::additionalPercentageAD;
        const float damage = dmgSkill + abilityPowerModifier + attackDamageModifier;

        return Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), target, damage);
    }

    static float KogRDamage(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !target->IsAlive())
            return 0.0f;
        const int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = KogMawDamages::RSpell::dmgSkillArray[level];
        const float abilityPowerModifier = ObjectManager::GetLocalPlayer()->GetAbilityPower() * KogMawDamages::RSpell::additionalPercentageAP;
        const float attackDamageModifier = ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * KogMawDamages::RSpell::additionalPercentageAD;
        float damage = dmgSkill + abilityPowerModifier + attackDamageModifier;
        const float targetHPPercent = target->GetHealth() / target->GetMaxHealth() * 100;

        if (targetHPPercent > 40.0f) {
            damage = damage + (damage * (targetHPPercent * KogMawDamages::RSpell::multiplicationFactor) / 100);
        }
        else {
            damage = damage * 2;
        }

        return Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), target, damage);
    }


public:

    KogMawModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto KogMawMenu = Menu::CreateMenu("vanKogMaw", "van.KogMaw");

        const auto comboMenu = KogMawMenu->AddMenu("Combo Settings", "Combo Settings");

        KogMawConfig::KogMawCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KogMawConfig::KogMawCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        KogMawConfig::KogMawCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        KogMawConfig::KogMawCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        KogMawConfig::KogMawCombo::RMode = comboMenu->AddList("rMode", "RCastMode", std::vector<std::string>{ "Can Kill", "Percent HP" }, 0);
        KogMawConfig::KogMawCombo::UseROnlyInBetweenAAS = comboMenu->AddCheckBox("rAA", "R Only Between AAs", true);

        const auto harassMenu = KogMawMenu->AddMenu("Harass Settings", "Harass Settings");
        KogMawConfig::KogMawHarass::minMana = harassMenu->AddSlider("minMana", "Minimum Mana", 140, 1, 500, 5);
        KogMawConfig::KogMawHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KogMawConfig::KogMawHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        KogMawConfig::KogMawHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        KogMawConfig::KogMawHarass::UseEOnlyWhenEnemyAttacking = harassMenu->AddCheckBox("Use E Attack", "E Only If Enemy Attacks", true);

        const auto clearMenu = KogMawMenu->AddMenu("Clear Settings", "Clear Settings");

        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        KogMawConfig::KogMawClear::minMana = laneClearMenu->AddSlider("minMana", "Minimum Mana", 140, 1, 500, 5);
        KogMawConfig::KogMawClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KogMawConfig::KogMawClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        //KogMawConfig::KogMawClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        //KogMawConfig::KogMawClear::eMinMinion = laneClearMenu->AddSlider("minMinions", "Minium Minions To Hit", 3, 1, 10, 1);

        const auto jungleClearMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        KogMawConfig::KogMawJungle::minMana = jungleClearMenu->AddSlider("minMana", "Minimum Mana", 140, 1, 500, 5);
        KogMawConfig::KogMawJungle::UseQ = jungleClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KogMawConfig::KogMawJungle::UseW = jungleClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        //KogMawConfig::KogMawJungle::UseE = jungleClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        //KogMawConfig::KogMawJungle::eMinMinion = jungleClearMenu->AddSlider("minMinions", "Minium Mobs To Hit", 4, 1, 10, 1);

        const auto additionalMenu = KogMawMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        KogMawConfig::KogMawKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        KogMawConfig::KogMawFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E");

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        KogMawConfig::KogMawSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        KogMawConfig::KogMawSpellsSettings::qMinArmor = qSpellMenu->AddSlider("QMinArmor", "Q Minimum Armor", 40, 0, 400, 5);
        KogMawConfig::KogMawSpellsSettings::qRange = qSpellMenu->AddSlider("QRange", "Q Range", 1175, 0, 1250, 5);
        KogMawConfig::KogMawSpellsSettings::qDraw = qSpellMenu->AddCheckBox("Q Draw", "Draw Q", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W Settings");
        KogMawConfig::KogMawSpellsSettings::wRangeOffset = wSpellMenu->AddSlider("wRangeOffset", "W Range Offset", 0, -200, 200, 5);
        KogMawConfig::KogMawSpellsSettings::wDraw = wSpellMenu->AddCheckBox("WDraw", "Draw W", true);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E Settings");
        KogMawConfig::KogMawSpellsSettings::eRange = eSpellMenu->AddSlider("ERange", "E Range", 1330, 0, 1400, 5);
        KogMawConfig::KogMawSpellsSettings::eDraw = eSpellMenu->AddCheckBox("EDraw", "Draw E", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R Settings");
        KogMawConfig::KogMawSpellsSettings::rRange = rSpellMenu->AddSlider("RRange", "R Range", 1300, 0, 1800, 5);
        KogMawConfig::KogMawSpellsSettings::rDraw = rSpellMenu->AddCheckBox("RDraw", "Draw R", true);


    }

    
    void CastQSpell(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !isTimeToCastQ())
            return;

        if (target->IsMinion()) {
            Engine::CastToPosition(SpellIndex::Q, target->GetPosition());
            QCastedTime = gameTime;
            return;
        }

        if (target->IsJungle() && !(target->GetName().find("Mini") != std::string::npos)) {
            Engine::CastToPosition(SpellIndex::Q, target->GetPosition());
            QCastedTime = gameTime;
            return;
        }

        Modules::prediction::PredictionOutput predOut;

        if (GetPrediction(ObjectManager::GetLocalPlayer(), target, database.KogMawQ, predOut)) {
            Engine::CastToPosition(SpellIndex::Q, predOut.position);
            QCastedTime = gameTime;
        }
    }

    void CastWSpell() {
        if (ObjectManager::GetLocalPlayer() == nullptr || !isTimeToCastW())
            return;

        Engine::CastSelf(SpellIndex::W);
        WCastedTime = gameTime;
    }

    void CastESpell(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !isTimeToCastE())
            return;

        if (target->IsMinion()) {
            Engine::CastToPosition(SpellIndex::E, target->GetPosition());
            ECastedTime = gameTime;
            return;
        }

        if (target->IsJungle()) {
            Engine::CastToPosition(SpellIndex::E, target->GetPosition());
            ECastedTime = gameTime;
            return;
        }

        Modules::prediction::PredictionOutput predOut;
        if (GetPrediction(ObjectManager::GetLocalPlayer(), target, database.KogMawE, predOut)) {
            Engine::CastToPosition(SpellIndex::E, predOut.position);
            ECastedTime = gameTime;
        }
    }

    void CastRSpell(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !isTimeToCastR())
            return;

        Modules::prediction::PredictionOutput predOut;

        if (GetPrediction(ObjectManager::GetLocalPlayer(), target, database.KogMawR, predOut)) {
            Engine::CastToPosition(SpellIndex::R, predOut.position);
            RCastedTime = gameTime;
        }
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
    }

    void Combo() override {

        if (KogMawConfig::KogMawCombo::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),qRange());
            if (qTarget != nullptr) {
	            if (qTarget->GetTotalArmor() >= qMinArmor()) {
	            	CastQSpell(qTarget);
	            }
            }
        }

        if (KogMawConfig::KogMawCombo::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                if (wTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= wRange()) {
                    CastWSpell();
                }
            }
        }

        if (KogMawConfig::KogMawCombo::UseE->Value && isTimeToCastE()) {
            const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), eRange());
            if (eTarget != nullptr) {
                CastESpell(eTarget);
            }
        }

        if (KogMawConfig::KogMawCombo::UseR->Value && !KogMawConfig::KogMawCombo::UseROnlyInBetweenAAS->Value && isTimeToCastR()) {
            const auto target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), rRange());
            if (target != nullptr) {
                CastRSpell(target);
            }
        }
    }

    void Harass() override
    {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;
        if (KogMawConfig::KogMawHarass::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
            if (qTarget != nullptr) {
                if (qTarget->GetTotalArmor() >= qMinArmor()) {
                    CastQSpell(qTarget);
                }
            }
        }
        if (KogMawConfig::KogMawHarass::UseE->Value && isTimeToCastE()) {
            const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), eRange());
            if (eTarget != nullptr && KogMawConfig::KogMawHarass::UseEOnlyWhenEnemyAttacking->Value) {
                if (eTarget->GetAiManager()->GetDirection() != ObjectManager::GetLocalPlayer()->GetPosition()) {
                    CastESpell(eTarget);
                }
            }
        }

        if (KogMawConfig::KogMawHarass::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                if (wTarget->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= wRange()) {
                    CastWSpell();
                }
            }
        }
    }
    

    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;

        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), wRange()) > 0)
        {
            if (KogMawConfig::KogMawClear::UseQ->Value && isTimeToCastQ()) {
                const auto qMinion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), Alliance::Enemy);
                if (qMinion != nullptr && qMinion->GetHealth() < KogQDamage(qMinion)) {
                    CastQSpell(qMinion);
                    return;
                }
            }

            if (KogMawConfig::KogMawClear::UseW->Value && isTimeToCastW()) {
                const auto wMinion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), wRange(), Alliance::Enemy);
                if (wMinion != nullptr) {
                    CastWSpell();
                    return;
                }
            }
        }


        if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), wRange()) > 0)
        {
            if (KogMawConfig::KogMawClear::UseQ->Value && isTimeToCastQ()) {
                const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(),qRange());
                if (qMonster != nullptr) {
                    CastQSpell(qMonster);
                    return;
                }
            }

            if (KogMawConfig::KogMawJungle::UseW->Value) {
                const auto wMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
                if (wMonster != nullptr) {
                    CastQSpell(wMonster);
                }
            }
        }
    }

    void Lasthit() override {
        if (!HasEnoughMana(OrbwalkState::Lasthit)) return;

        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), wRange()) > 0) {
            if (KogMawConfig::KogMawLastHit::UseQ->Value && isTimeToCastQ())
            {
                const auto qMinion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), Alliance::Enemy);
                if (qMinion != nullptr && qMinion->GetHealth() < KogQDamage(qMinion)) {
                    CastQSpell(qMinion);
                }
            }
        }
    }

    void Flee() override {
        if (KogMawConfig::KogMawFlee::UseE->Value && isTimeToCastE()) {
            const auto target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),eRange());
            if (target != nullptr) {
                CastESpell(target);
            }
        }
    }

    void Killsteal() {
        __try {
            if (KogMawConfig::KogMawKillsteal::UseR->Value && isTimeToCastR()) {
                const auto target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), rRange());
                if (target != nullptr) {
                    if (KogMawConfig::KogMawCombo::UseROnlyInBetweenAAS->Value && target->IsInAARange() 
                        || !KogMawConfig::KogMawCombo::UseROnlyInBetweenAAS->Value) {
	                    CastRSpell(target);
                    }
                }
            }
        }
        __except (1)
        {
            LOG("Killsteal error");
        }
    }

    void OnAfterAttack() override {
        if (KogMawConfig::KogMawCombo::UseROnlyInBetweenAAS->Value && isTimeToCastR())
        {
            const auto target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), rRange());
            if (target != nullptr) {
                CastRSpell(target);
            }
        }
    }

    void OnCreateMissile() override {
        return;
    }

    void OnDeleteMissile() override {
        return;
    }

    void OnBeforeAttack() override {
        return;
    }

    void Render() override {
        __try {
            if (KogMawConfig::KogMawSpellsSettings::qDraw->Value && (KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == true && database.KogMawQ.IsCastable() || KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), COLOR_WHITE, 1.0f);
            if (KogMawConfig::KogMawSpellsSettings::wDraw->Value && (KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == true && database.KogMawW.IsCastable() || KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), wRange(), COLOR_RED, 1.0f);
            if (KogMawConfig::KogMawSpellsSettings::eDraw->Value && (KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == true && database.KogMawE.IsCastable() || KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), eRange(), COLOR_WHITE, 1.0f);
            if (KogMawConfig::KogMawSpellsSettings::rDraw->Value && (KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == true && database.KogMawR.IsCastable() || KogMawConfig::KogMawSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), rRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("Render error");
        }
    }
};

KogMawModule module;