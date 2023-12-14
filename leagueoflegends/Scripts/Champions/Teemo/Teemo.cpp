#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Teemo.h"

using namespace UPasta::SDK;

class TeemoModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Teemo");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;


    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + 0.25f && globals::localPlayer->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime + 0.00f && globals::localPlayer->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime + 0.00f && globals::localPlayer->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + 0.25f && globals::localPlayer->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(TeemoConfig::TeemoJungle::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(TeemoConfig::TeemoHarass::minMana->Value);
            break;
        default:
            return false;
        }

        return globals::localPlayer->GetPercentMana() > minManaThreshold;
    }

    static float qRange() {
        return globals::localPlayer->GetRealAttackRange();
    }

    static float wRange() {
        return static_cast<float>(TeemoConfig::TeemoSpellsSettings::wRange->Value);
    }

    static float rRange() {
        return static_cast<float>(TeemoConfig::TeemoSpellsSettings::rRange->Value);
    }

    static float TeemoQDamage(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::Q))
            return -9999;

        const int level = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = TeemoDamages::QSpell::dmgSkillArray[level];
        const float abilityPowerModifier = globals::localPlayer->ReadClientStat(Object::AbilityPower) * TeemoDamages::QSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(globals::localPlayer, target, damage);
    }

    static float TeemoRDamage(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::R))
            return -9999;
        const int level = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = TeemoDamages::RSpell::dmgSkillArray[level];
        const float abilityPowerModifier = globals::localPlayer->ReadClientStat(Object::AbilityPower) * TeemoDamages::RSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(globals::localPlayer, target, damage);
    }


public:

    TeemoModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto TeemoMenu = Menu::CreateMenu("isi.Teemo", "isi.Teemo");

        const auto comboMenu = TeemoMenu->AddMenu("Combo Settings", "Combo Settings");
        TeemoConfig::TeemoCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TeemoConfig::TeemoCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        TeemoConfig::TeemoCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto harassMenu = TeemoMenu->AddMenu("Harass Settings", "Harass Settings");
        TeemoConfig::TeemoHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TeemoConfig::TeemoHarass::UseR = harassMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        TeemoConfig::TeemoHarass::minMana = harassMenu->AddSlider("minMana", "Minimum Mana", 60, 1, 100, 10);

        const auto clearMenu = TeemoMenu->AddMenu("Clear Settings", "Clear Settings");

        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        TeemoConfig::TeemoClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TeemoConfig::TeemoClear::UseR = laneClearMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        TeemoConfig::TeemoClear::minRMinions = laneClearMenu->AddSlider("minRMinions", "Minimum Minions in Lane for R", 3, 1, 6, 1);

        const auto jungleClearMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        TeemoConfig::TeemoJungle::UseQ = jungleClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TeemoConfig::TeemoJungle::UseR = jungleClearMenu->AddCheckBox("Use R", "Use SpellSlot R", false);
        TeemoConfig::TeemoJungle::minMana = jungleClearMenu->AddSlider("minMana", "Minimum Mana", 60, 1, 100, 10);

        const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        TeemoConfig::TeemoLastHit::UseQ = lastHitMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TeemoConfig::TeemoLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = TeemoMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        TeemoConfig::TeemoKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TeemoConfig::TeemoKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        TeemoConfig::TeemoFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W");

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        TeemoConfig::TeemoSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        TeemoConfig::TeemoSpellsSettings::qDraw = qSpellMenu->AddCheckBox("Q Draw", "Draw Q", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W Settings");
        TeemoConfig::TeemoSpellsSettings::wRange = wSpellMenu->AddSlider("WRange", "Maximum Range", globals::localPlayer->GetRealAttackRange(), 0, 1000, 50);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R Settings");
        TeemoConfig::TeemoSpellsSettings::rRange = rSpellMenu->AddSlider("RRange", "Maximum Range", 900, 600, 900, 150);
        TeemoConfig::TeemoSpellsSettings::rDraw = rSpellMenu->AddCheckBox("RDraw", "Draw R", true);
        TeemoConfig::TeemoSpellsSettings::rShowSpecialPlaces = rSpellMenu->AddCheckBox("rShowSpecialPlaces", "Show suggested places", true);

    }


    void CastQSpell(Object* pEnemy) {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastQ())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < qRange())
        {
            Engine::CastTargeted(SpellIndex::Q, pEnemy);
            QCastedTime = gameTime;
        }
    }

    void CastWSpell() {
        if (globals::localPlayer == nullptr || !isTimeToCastW())
            return;

        Engine::CastSelf(SpellIndex::W);
        WCastedTime = gameTime;
    }

    void CastRSpell(Object* pEnemy) {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastR())
            return;


        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= rRange() && isTimeToCastR())
        {
            if (pEnemy->IsHero())
            {
                Modules::prediction::PredictionOutput rPrediction;

                /*if (GetPrediction(database.TeemoR, rPrediction))
                {
                    Engine::CastSpell(SpellIndex::R, rPrediction.position);
                    RCastedTime = gameTime;
                }*/
            }
            else
            {
                Engine::CastToPosition(SpellIndex::R, pEnemy->GetPosition());
                RCastedTime = gameTime;
            }
        }
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
    }

    void Combo() override {
        if (TeemoConfig::TeemoCombo::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr) {
                CastQSpell(qTarget);
            }
        }

        if (TeemoConfig::TeemoCombo::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                CastWSpell();
            }
        }

        if (TeemoConfig::TeemoCombo::UseR->Value && isTimeToCastR()) {
            const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
            if (rTarget != nullptr) {
                CastRSpell(rTarget);
            }
        }


    }


    void Harass() override
    {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;
        if (TeemoConfig::TeemoHarass::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr) {
                CastQSpell(qTarget);
            }
        }

        if (TeemoConfig::TeemoHarass::UseR->Value && isTimeToCastR()) {
            const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
            if (rTarget != nullptr) {
                CastRSpell(rTarget);
            }
        }
    }


    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;

        if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), wRange()) > 0)
        {
            if (TeemoConfig::TeemoJungle::UseQ->Value && isTimeToCastQ()) {
                const auto qMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), qRange());
                if (qMonster != nullptr) {
                    CastQSpell(qMonster);
                    return;
                }
            }

            if (TeemoConfig::TeemoJungle::UseR->Value && isTimeToCastR()) {
                const auto rMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), rRange());
                if (rMonster != nullptr) {
                    CastRSpell(rMonster);
                    return;
                }
            }
        }
    }

    void Lasthit() override {
        return;
    }

    void Flee() override {
        if (TeemoConfig::TeemoFlee::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                CastWSpell();
            }
        }
    }

    void Killsteal() {
        __try {
            if (TeemoConfig::TeemoKillsteal::UseQ->Value && isTimeToCastQ()) {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
                if (qTarget != nullptr) {
                    if (qTarget->ReadClientStat(Object::Health) < TeemoQDamage(qTarget)) {
                        CastQSpell(qTarget);
                    }
                }
            }

            if (TeemoConfig::TeemoKillsteal::UseR->Value && isTimeToCastR()) {
                const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
                if (rTarget != nullptr) {
                    if (rTarget->ReadClientStat(Object::Health) < TeemoRDamage(rTarget)) {
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

    void OnCreateMissile() override {
        return;
    }

    void OnDeleteMissile() override {
        return;
    }

    void OnBeforeAttack() override {
        return;
    }

    void OnAfterAttack() override {
        return;
    }

    void Render() override {
        __try {
            if (TeemoConfig::TeemoSpellsSettings::qDraw->Value && (TeemoConfig::TeemoSpellsSettings::DrawIfReady->Value == true && isTimeToCastQ() || TeemoConfig::TeemoSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);
            if (TeemoConfig::TeemoSpellsSettings::rDraw->Value && (TeemoConfig::TeemoSpellsSettings::DrawIfReady->Value == true && isTimeToCastR() || TeemoConfig::TeemoSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), rRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("Render error");
        }
    }
};

TeemoModule module;