#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Blitzcrank.h"

using namespace UPasta::SDK;

class BlitzcrankModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Blitzcrank");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;


    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + database.BlitzcrankQ.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime && globals::localPlayer->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime && globals::localPlayer->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.BlitzcrankR.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(BlitzcrankConfig::BlitzcrankJungle::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(BlitzcrankConfig::BlitzcrankHarass::minMana->Value);
            break;
        default:
            return false;
        }

        return globals::localPlayer->GetPercentMana() > minManaThreshold;
    }

    static float qRange() {
        return static_cast<float>(BlitzcrankConfig::BlitzcrankSpellsSettings::qRange->Value);
    }

    static float wRange() {
        return static_cast<float>(BlitzcrankConfig::BlitzcrankSpellsSettings::wRange->Value);
    }

    static float eRange() {
        return static_cast<float>(BlitzcrankConfig::BlitzcrankSpellsSettings::eRange->Value);
    }

    static float rRange() {
        return static_cast<float>(BlitzcrankConfig::BlitzcrankSpellsSettings::rRange->Value);
    }

    static float BlitzcrankQDamage(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::Q))
            return -9999;

        const int level = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = BlitzcrankDamages::QSpell::dmgSkillArray[level];
        const float abilityPowerModifier = globals::localPlayer->ReadClientStat(Object::AbilityPower) * BlitzcrankDamages::QSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(globals::localPlayer, target, damage);
    }

    static float BlitzcrankRDamage(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::R))
            return -9999;
        const int level = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        if (level == 0)
            return 0.0f;

        const float dmgSkill = BlitzcrankDamages::RSpell::dmgSkillArray[level];
        const float abilityPowerModifier = globals::localPlayer->ReadClientStat(Object::AbilityPower) * BlitzcrankDamages::RSpell::additionalPercentageAP;
        const float damage = dmgSkill + abilityPowerModifier;

        return Damage::CalculateMagicalDamage(globals::localPlayer, target, damage);
    }


public:

    BlitzcrankModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto BlitzcrankMenu = Menu::CreateMenu("isi.Blitzcrank", "isi.Blitzcrank");

        const auto comboMenu = BlitzcrankMenu->AddMenu("Combo Settings", "Combo Settings");
        BlitzcrankConfig::BlitzcrankCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        BlitzcrankConfig::BlitzcrankCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        BlitzcrankConfig::BlitzcrankCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        BlitzcrankConfig::BlitzcrankCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        BlitzcrankConfig::BlitzcrankCombo::rMinEnemies = comboMenu->AddSlider("rMinEnemies", "Minium Enemies To Hit", 3, 1, 5, 1);

        const auto harassMenu = BlitzcrankMenu->AddMenu("Harass Settings", "Harass Settings");
        BlitzcrankConfig::BlitzcrankHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        BlitzcrankConfig::BlitzcrankHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        BlitzcrankConfig::BlitzcrankHarass::minMana = harassMenu->AddSlider("minMana", "Minimum Mana", 60, 1, 100, 10);

        const auto clearMenu = BlitzcrankMenu->AddMenu("Clear Settings", "Clear Settings");

        const auto jungleClearMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        BlitzcrankConfig::BlitzcrankJungle::UseQ = jungleClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
        BlitzcrankConfig::BlitzcrankJungle::UseW = jungleClearMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
        BlitzcrankConfig::BlitzcrankJungle::UseE = jungleClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        BlitzcrankConfig::BlitzcrankJungle::minMana = jungleClearMenu->AddSlider("minMana", "Minimum Mana", 60, 1, 100, 10);

        const auto additionalMenu = BlitzcrankMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        BlitzcrankConfig::BlitzcrankKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        BlitzcrankConfig::BlitzcrankKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        BlitzcrankConfig::BlitzcrankFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W");

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        BlitzcrankConfig::BlitzcrankSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        BlitzcrankConfig::BlitzcrankSpellsSettings::qRange = qSpellMenu->AddSlider("QRange", "Maximum Range", database.BlitzcrankQ.GetRange(), 0, 1150, 50);
        BlitzcrankConfig::BlitzcrankSpellsSettings::qDraw = qSpellMenu->AddCheckBox("Q Draw", "Draw Q", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W Settings");
        BlitzcrankConfig::BlitzcrankSpellsSettings::wRange = wSpellMenu->AddSlider("WRange", "Maximum Range", database.BlitzcrankQ.GetRange(), 100, 1150, 50);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E Settings");
        BlitzcrankConfig::BlitzcrankSpellsSettings::eRange = eSpellMenu->AddSlider("ERange", "Maximum Range", globals::localPlayer->GetRealAttackRange(), 0, 700, 50);
        BlitzcrankConfig::BlitzcrankSpellsSettings::eMode = comboMenu->AddList("eMode", "E Cast Mode", std::vector<std::string>{ "Before Attack", "After Attack" }, 0);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R Settings");
        BlitzcrankConfig::BlitzcrankSpellsSettings::rRange = rSpellMenu->AddSlider("RRange", "Maximum Range", database.BlitzcrankR.GetRange(), 100, 600, 50);
        BlitzcrankConfig::BlitzcrankSpellsSettings::rDraw = rSpellMenu->AddCheckBox("RDraw", "Draw R", true);
    }


    void CastQSpell(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !isTimeToCastQ())
            return;

        if (target && target->GetDistanceTo(globals::localPlayer) < qRange())
        {
            if (target->IsMinion()) {
                Engine::CastSpell(SpellIndex::Q, target->GetPosition());
                QCastedTime = gameTime;
                return;
            }

            if (target->IsJungle() && !(target->GetName().find("Mini") != std::string::npos)) {
                Engine::CastSpell(SpellIndex::Q, target->GetPosition());
                QCastedTime = gameTime;
                return;
            }

            Modules::prediction::PredictionOutput predOut;

            if (GetPrediction(globals::localPlayer, target, database.BlitzcrankQ, predOut)) {
                Engine::CastSpell(SpellIndex::Q, predOut.position);
                QCastedTime = gameTime;
            }
        }
    }

    void CastWSpell() {
        if (globals::localPlayer == nullptr || !isTimeToCastW())
            return;

        Engine::CastSpell(SpellIndex::W);
        WCastedTime = gameTime;
    }

    void CastESpell(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !isTimeToCastE())
            return;

        if (target && target->GetDistanceTo(globals::localPlayer) < eRange()) {
            Engine::CastSpell(SpellIndex::E);
            ECastedTime = gameTime;
        }
    }

    void CastRSpell(Object* target) {
        if (globals::localPlayer == nullptr || target == nullptr || !isTimeToCastR())
            return;

        if (target && target->GetDistanceTo(globals::localPlayer) < rRange())
        {
            Engine::CastSpell(SpellIndex::R);
            RCastedTime = gameTime;
        }
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
    }

    void Combo() override {
        if (BlitzcrankConfig::BlitzcrankCombo::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr) {
                CastQSpell(qTarget);
            }
        }

        if (BlitzcrankConfig::BlitzcrankCombo::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                CastWSpell();
            }
        }

        if (BlitzcrankConfig::BlitzcrankCombo::UseE->Value && isTimeToCastE() && BlitzcrankConfig::BlitzcrankSpellsSettings::eMode->Value == 0) {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), eRange());
            if (eTarget != nullptr) {
                CastESpell(eTarget);
            }
        }

        if (BlitzcrankConfig::BlitzcrankCombo::UseR->Value && isTimeToCastR()) {
            if (BlitzcrankConfig::BlitzcrankCombo::rMinEnemies->Value <= ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), rRange())) {
                const auto target = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
                if (target != nullptr) {
                    CastRSpell(target);
                }
            }
        }
    }

    void Harass() override
    {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;
        if (BlitzcrankConfig::BlitzcrankHarass::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr) {
                CastQSpell(qTarget);
            }
        }

        if (BlitzcrankConfig::BlitzcrankHarass::UseE->Value && isTimeToCastE()) {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), eRange());
            if (eTarget != nullptr) {
                CastESpell(eTarget);
            }
        }
    }


    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;

        if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), wRange()) > 0)
        {
            if (BlitzcrankConfig::BlitzcrankJungle::UseQ->Value && isTimeToCastQ()) {
                const auto qMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), qRange());
                if (qMonster != nullptr) {
                    CastQSpell(qMonster);
                    return;
                }
            }

            if (BlitzcrankConfig::BlitzcrankJungle::UseW->Value) {
                const auto wMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), wRange());
                if (wMonster != nullptr) {
                    CastQSpell(wMonster);
                }
            }

            if (BlitzcrankConfig::BlitzcrankJungle::UseE->Value && isTimeToCastE()) {
                const auto eMonster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), eRange());
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
        if (BlitzcrankConfig::BlitzcrankFlee::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                CastWSpell();
            }
        }
    }

    void Killsteal() {
        __try {
            if (BlitzcrankConfig::BlitzcrankKillsteal::UseQ->Value && isTimeToCastQ()) {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
                if (qTarget != nullptr) {
                    if (qTarget->ReadClientStat(Object::Health) < BlitzcrankQDamage(qTarget)) {
                        CastQSpell(qTarget);
                    }
                }
            }

            if (BlitzcrankConfig::BlitzcrankKillsteal::UseR->Value && isTimeToCastR()) {
                const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
                if (rTarget != nullptr) {
                    if (rTarget->ReadClientStat(Object::Health) < BlitzcrankRDamage(rTarget)) {
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
        if (BlitzcrankConfig::BlitzcrankSpellsSettings::eMode->Value == 1 && isTimeToCastE()) {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), eRange());
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
        if (BlitzcrankConfig::BlitzcrankSpellsSettings::qDraw->Value && (BlitzcrankConfig::BlitzcrankSpellsSettings::DrawIfReady->Value == true && database.BlitzcrankQ.IsCastable() || BlitzcrankConfig::BlitzcrankSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);
        if (BlitzcrankConfig::BlitzcrankSpellsSettings::rDraw->Value && (BlitzcrankConfig::BlitzcrankSpellsSettings::DrawIfReady->Value == true && database.BlitzcrankR.IsCastable() || BlitzcrankConfig::BlitzcrankSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), rRange(), COLOR_WHITE, 1.0f);

    	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
            if (!hero) continue;
            if (hero->GetDistanceTo(globals::localPlayer) > qRange()) continue;
            auto dmgPos = Engine::GetBaseDrawPosition(hero);

            if (isTimeToCastQ()) {
	            render::RenderTextWorld("Damage Q: " + std::to_string(ceil(BlitzcrankQDamage(hero))), dmgPos, 16, COLOR_WHITE, false);
            }

            if (isTimeToCastR()) {
	            render::RenderTextWorld("Damage R: " + std::to_string(ceil(BlitzcrankRDamage(hero))), Vector3(dmgPos.x, dmgPos.y - 32.0f, dmgPos.z), 16, COLOR_WHITE, false);
            }

        }
    }
};

BlitzcrankModule module;