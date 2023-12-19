#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Amumu.h"

using namespace UPasta::SDK;

class AmumuModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Amumu");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;


    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + database.AmumuQ.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.AmumuR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static bool HasAuraOfDespairBuff() {
        if (ObjectManager::GetLocalPlayer() == nullptr) {
            // Handle the case where localPlayer is null (optional)
            return false;
        }

        // Assume that localPlayer has a GetBuffByName method
        Buff* auraOfDespairBuff = ObjectManager::GetLocalPlayer()->GetBuffByName("AuraofDespair");
        return auraOfDespairBuff != nullptr;
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(AmumuConfig::AmumuJungle::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(AmumuConfig::AmumuHarass::minMana->Value);
            break;
        default:
            return false;
        }

        return ObjectManager::GetLocalPlayer()->GetPercentMana() > minManaThreshold;
    }

    static float qRange() {
        return static_cast<float>(AmumuConfig::AmumuSpellsSettings::qRange->Value);
    }

    static float wRange() {
        return static_cast<float>(AmumuConfig::AmumuSpellsSettings::wRange->Value);
    }

    static float eRange() {
        return static_cast<float>(AmumuConfig::AmumuSpellsSettings::eRange->Value);
    }

    static float rRange() {
        return static_cast<float>(AmumuConfig::AmumuSpellsSettings::rRange->Value);
    }

    static float AmumuQDamage(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q))
            return -9999;

        const int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = AmumuDamages::QSpell::dmgSkillArray[level];
        const float abilityPowerModifier = ObjectManager::GetLocalPlayer()->GetAbilityPower() * AmumuDamages::QSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), target, damage);
    }

    static float AmumuRDamage(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R))
            return -9999;
        const int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = AmumuDamages::RSpell::dmgSkillArray[level];
        const float abilityPowerModifier = ObjectManager::GetLocalPlayer()->GetAbilityPower() * AmumuDamages::RSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), target, damage);
    }


public:

    AmumuModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto AmumuMenu = Menu::CreateMenu("isi.Amumu", "isi.Amumu");

        const auto comboMenu = AmumuMenu->AddMenu("Combo Settings", "Combo Settings");
        AmumuConfig::AmumuCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AmumuConfig::AmumuCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AmumuConfig::AmumuCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        AmumuConfig::AmumuCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        AmumuConfig::AmumuCombo::rMinEnemies = comboMenu->AddSlider("rMinEnemies", "Minium Enemies To Hit", 2, 1, 5, 1);

        const auto harassMenu = AmumuMenu->AddMenu("Harass Settings", "Harass Settings");
        AmumuConfig::AmumuHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AmumuConfig::AmumuHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AmumuConfig::AmumuHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        AmumuConfig::AmumuHarass::minMana = harassMenu->AddSlider("minMana", "Minimum Mana", 60, 1, 100, 10);

        const auto clearMenu = AmumuMenu->AddMenu("Clear Settings", "Clear Settings");

        const auto jungleClearMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        AmumuConfig::AmumuJungle::UseQ = jungleClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AmumuConfig::AmumuJungle::UseW = jungleClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AmumuConfig::AmumuJungle::UseE = jungleClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        AmumuConfig::AmumuJungle::minMana = jungleClearMenu->AddSlider("minMana", "Minimum Mana", 60, 1, 100, 10);

        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        AmumuConfig::AmumuClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AmumuConfig::AmumuClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AmumuConfig::AmumuClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        AmumuConfig::AmumuClear::minMinion = laneClearMenu->AddSlider("minMinion", "Minimum Minions To Clear", 3, 1, 7, 1);
        AmumuConfig::AmumuClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = AmumuMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        AmumuConfig::AmumuKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
        AmumuConfig::AmumuKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

        // const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        // AmumuConfig::AmumuFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W");

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        AmumuConfig::AmumuSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        AmumuConfig::AmumuSpellsSettings::qRange = qSpellMenu->AddSlider("QRange", "Maximum Range", database.AmumuQ.GetRange(), 0, 1100, 50);
        AmumuConfig::AmumuSpellsSettings::qDraw = qSpellMenu->AddCheckBox("Q Draw", "Draw Q", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W Settings");
        AmumuConfig::AmumuSpellsSettings::wRange = wSpellMenu->AddSlider("WRange", "Maximum Range", 300, 0, 350, 50);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E Settings");
        AmumuConfig::AmumuSpellsSettings::eRange = eSpellMenu->AddSlider("ERange", "Maximum Range", 300, 0, 350, 50);
        AmumuConfig::AmumuSpellsSettings::eMode = comboMenu->AddList("eMode", "E Cast Mode", std::vector<std::string>{ "Before Attack", "After Attack" }, 0);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R Settings");
        AmumuConfig::AmumuSpellsSettings::rRange = rSpellMenu->AddSlider("RRange", "Maximum Range", database.AmumuR.GetRange(), 500, 550, 50);
        AmumuConfig::AmumuSpellsSettings::rDraw = rSpellMenu->AddCheckBox("RDraw", "Draw R", true);
    }


    void CastQSpell(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !isTimeToCastQ())
            return;

        if (target && target->GetDistanceTo(ObjectManager::GetLocalPlayer()) < qRange())
        {
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

            if (GetPrediction(ObjectManager::GetLocalPlayer(), target, database.AmumuQ, predOut)) {
                Engine::CastToPosition(SpellIndex::Q, predOut.position);
                QCastedTime = gameTime;
            }
        }
    }

    void CastWSpell() {
        if (ObjectManager::GetLocalPlayer() == nullptr || !isTimeToCastW())
            return;

        int enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), wRange());

        bool isAuraOfDespairActive = HasAuraOfDespairBuff();

        if (enemiesInRange > 0 && !isAuraOfDespairActive) {
            Engine::CastSelf(SpellIndex::W);
            WCastedTime = gameTime;
        }
        else if (enemiesInRange == 0 && isAuraOfDespairActive) {
            Engine::CastSelf(SpellIndex::W);
            WCastedTime = gameTime;
        }
    }

    void CastESpell(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !isTimeToCastE())
            return;

        if (target && target->GetDistanceTo(ObjectManager::GetLocalPlayer()) < eRange()) {
            Engine::CastSelf(SpellIndex::E);
            ECastedTime = gameTime;
        }
    }

    void CastRSpell(Object* target) {
        if (ObjectManager::GetLocalPlayer() == nullptr || target == nullptr || !isTimeToCastR())
            return;

        if (target && target->GetDistanceTo(ObjectManager::GetLocalPlayer()) < rRange())
        {
            Engine::CastSelf(SpellIndex::R);
            RCastedTime = gameTime;
        }
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
    }

    void Combo() override {
        if (AmumuConfig::AmumuCombo::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
            if (qTarget != nullptr) {
                CastQSpell(qTarget);
            }
        }

        if (AmumuConfig::AmumuCombo::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                CastWSpell();
            }
        }

        if (AmumuConfig::AmumuCombo::UseE->Value && isTimeToCastE() && AmumuConfig::AmumuSpellsSettings::eMode->Value == 0) {
            const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), eRange());
            if (eTarget != nullptr) {
                CastESpell(eTarget);
            }
        }

        if (AmumuConfig::AmumuCombo::UseR->Value && isTimeToCastR()) {
            if (AmumuConfig::AmumuCombo::rMinEnemies->Value <= ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), rRange())) {
                const auto target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), rRange());
                if (target != nullptr) {
                    CastRSpell(target);
                }
            }
        }
    }

    void Harass() override
    {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;
        if (AmumuConfig::AmumuHarass::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
            if (qTarget != nullptr) {
                CastQSpell(qTarget);
            }
        }

        if (AmumuConfig::AmumuHarass::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                CastWSpell();
            }
        }

        if (AmumuConfig::AmumuHarass::UseE->Value && isTimeToCastE()) {
            const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), eRange());
            if (eTarget != nullptr) {
                CastESpell(eTarget);
            }
        }

    }

    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;

        if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), wRange()) > 0)
        {
            if (AmumuConfig::AmumuJungle::UseQ->Value && isTimeToCastQ()) {
                const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
                if (qMonster != nullptr) {
                    CastQSpell(qMonster);
                    return;
                }
            }

            int enemiesInRange = ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
        	int jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());

            // Check if the "AuraofDespair" Buff aktiv ist
            bool isAuraOfDespairActive = HasAuraOfDespairBuff();

            // Toggle W basierend auf Bedingungen
            if ((enemiesInRange > 0 || jungleMonstersInRange > 0) && !isAuraOfDespairActive) {
                // Toggle W aktivieren
                Engine::CastSelf(SpellIndex::W);
                WCastedTime = gameTime;
            }
            else if ((enemiesInRange == 0 && jungleMonstersInRange == 0) && isAuraOfDespairActive) {
                // Toggle W deaktivieren, wenn keine Gegner in Reichweite sind und Aura aktiv ist
                Engine::CastSelf(SpellIndex::W);
                WCastedTime = gameTime;
            }

            if (AmumuConfig::AmumuJungle::UseE->Value && isTimeToCastE()) {
                const auto eMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), eRange());
                if (eMonster != nullptr) {
                    CastESpell(eMonster);
                    return;
                }
            }
        }
    }

    void Lasthit() override {
        return;
    }

    void Flee() override {
        return;
    }

    void Killsteal() {
        __try {
            if (AmumuConfig::AmumuKillsteal::UseQ->Value && isTimeToCastQ()) {
                const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
                if (qTarget != nullptr) {
                    if (qTarget->GetHealth() < AmumuQDamage(qTarget)) {
                        CastQSpell(qTarget);
                    }
                }
            }

            if (AmumuConfig::AmumuKillsteal::UseR->Value && isTimeToCastR()) {
                const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), rRange());
                if (rTarget != nullptr) {
                    if (rTarget->GetHealth() < AmumuRDamage(rTarget)) {
                        CastRSpell(rTarget);
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
        if (AmumuConfig::AmumuSpellsSettings::eMode->Value == 1 && isTimeToCastE()) {
            const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), eRange());
            if (eTarget != nullptr) {
                CastESpell(eTarget);
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
            if (AmumuConfig::AmumuSpellsSettings::qDraw->Value && (AmumuConfig::AmumuSpellsSettings::DrawIfReady->Value == true && database.AmumuQ.IsCastable() || AmumuConfig::AmumuSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), COLOR_WHITE, 1.0f);
            if (AmumuConfig::AmumuSpellsSettings::rDraw->Value && (AmumuConfig::AmumuSpellsSettings::DrawIfReady->Value == true && database.AmumuR.IsCastable() || AmumuConfig::AmumuSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), rRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("Render error");
        }
    }
};

AmumuModule module;