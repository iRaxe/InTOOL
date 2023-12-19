#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Jinx.h"

using namespace UPasta::SDK;


class JinxModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Jinx");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + 1.0f && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime + database.JinxW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime + database.JinxE.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.JinxR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static float qRange() {
        return static_cast<float>(JinxConfig::JinxSpellsSettings::qRange->Value);
    }

    static float wRange() {
        return static_cast<float>(JinxConfig::JinxSpellsSettings::wRange->Value);
    }

    static float eRange() {
        return static_cast<float>(JinxConfig::JinxSpellsSettings::eRange->Value);
    }

    static float rMinRange() {
        return static_cast<float>(JinxConfig::JinxSpellsSettings::minRDistance->Value);
    }

    static float rMaxRange() {
        return static_cast<float>(JinxConfig::JinxSpellsSettings::maxRDistance->Value);
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
        	minManaThreshold = static_cast<float>(JinxConfig::JinxJungle::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(JinxConfig::JinxHarass::minMana->Value);
            break;
        default:
            return false;
        }

        return ObjectManager::GetLocalPlayer()->GetPercentMana() > minManaThreshold;
    }

    static bool hasE(Object* pEnemy) {
        return pEnemy->GetBuffByName("JinxDeadlyVenom");
    }

public:

    JinxModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override {
        const auto JinxMenu = Menu::CreateMenu("vez.Jinx", "Champion Settings");

        const auto comboMenu = JinxMenu->AddMenu("Combo Settings", "Combo Settings");
        JinxConfig::JinxCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        JinxConfig::JinxCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        JinxConfig::JinxCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        JinxConfig::JinxCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = JinxMenu->AddMenu("Harass Settings", "Harass Settings");
        JinxConfig::JinxHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", false);
        JinxConfig::JinxHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = JinxMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        JinxConfig::JinxJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = JinxMenu->AddMenu("Additional Settings", "Additional Settings");
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        JinxConfig::JinxKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        JinxConfig::JinxKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        JinxConfig::JinxAntiGapCloser::UseW = antiGapMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        const auto antiGapwhitelistMenu = antiGapMenu->AddMenu("Whitelist Settings", "Whitelist");

        const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
        JinxConfig::JinxAntiMelee::UseW = antiMeleeMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxAntiMelee::UseE = antiMeleeMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        const auto antiMeleewhitelistMenu = antiMeleeMenu->AddMenu("Whitelist Settings", "Whitelist");
        for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++)
        {
            auto obj = ObjectManager::GetHeroList()->GetIndex(i);
            if (obj != nullptr && obj->IsEnemy())
            {
                const auto antiGap_checkbox = antiGapwhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj](const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !JinxConfig::JinxAntiGapCloser::whitelist.empty()) {
                            const auto it = std::ranges::find(JinxConfig::JinxAntiGapCloser::whitelist, obj);
                            JinxConfig::JinxAntiGapCloser::whitelist.erase(it);
                        }
                        else {
                            JinxConfig::JinxAntiGapCloser::whitelist.push_back(obj);
                        }
                    });

                if (antiGap_checkbox->Value == true) {
                    JinxConfig::JinxAntiGapCloser::whitelist.push_back(obj);
                }

                if (!obj->IsMelee()) continue;
                const auto antiMelee_checkbox = antiMeleewhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !JinxConfig::JinxAntiMelee::whitelist.empty()) {
                            const auto it = std::ranges::find(JinxConfig::JinxAntiMelee::whitelist, obj);
                            JinxConfig::JinxAntiMelee::whitelist.erase(it);
                        }
                        else {
                            JinxConfig::JinxAntiMelee::whitelist.push_back(obj);
                        }
                    });

                if (antiMelee_checkbox->Value == true)
                {
                    JinxConfig::JinxAntiMelee::whitelist.push_back(obj);
                }
            }
        }

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        JinxConfig::JinxFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        JinxConfig::JinxFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");
        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        JinxConfig::JinxSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        JinxConfig::JinxSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange",
            "Maximum Range",
            static_cast<float>(ObjectManager::GetLocalPlayer()->GetRealAttackRange()) * 1.5f,
            100,
            static_cast<float>(ObjectManager::GetLocalPlayer()->GetRealAttackRange()) * 2.0f, 50);
        JinxConfig::JinxSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        JinxConfig::JinxSpellsSettings::wCastMode = wSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        JinxConfig::JinxSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.JinxW.GetRange(), 100, database.JinxW.GetRange(), 50);
        JinxConfig::JinxSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
        JinxConfig::JinxSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.JinxE.GetRange(), 100, database.JinxE.GetRange(), 50);
        JinxConfig::JinxSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        JinxConfig::JinxSpellsSettings::targetMode = rSpellMenu->AddList("targetMode", "Target Selector Mode", std::vector<std::string>{"Inherit", "Near Mouse", "Near Champion"}, 0);
        JinxConfig::JinxSpellsSettings::rTapKey = rSpellMenu->AddKeyBind("rTapKey", "Aim SpellSlot R Key", VK_CONTROL, false, false);
        JinxConfig::JinxSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "SpellSlot R Minimum Fire Distance", 800, 100, database.JinxR.GetRange(), 100);
        JinxConfig::JinxSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "SpellSlot R Maximum Fire Distance", 3000, 100, database.JinxR.GetRange(), 100);

        JinxConfig::JinxSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Jinx_dmgE(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E))
            return -9999;

        if (hasE(pEnemy))
        {
            const int stacks = pEnemy->GetBuffByName("JinxDeadlyVenom")->GetStacks();
            if (stacks > 0)
            {
                const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::E)->GetLevel();
                const int eDMG = levelSpell * 5 + 10;
                const float sDMG = (eDMG + (ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * .35) + ObjectManager::GetLocalPlayer()->GetAbilityPower() * .30) * stacks;

                return Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), pEnemy, sDMG);
            }
        }


        return -9999;
    }

    void Jinx_UseQ(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !isTimeToCastQ())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= qRange())
        {
            Engine::CastSelf(SpellIndex::Q);
            QCastedTime = gameTime;
        }
    }

    void Jinx_UseW(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !isTimeToCastW())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= wRange())
        {
            Modules::prediction::PredictionOutput wPrediction;

            if (GetPrediction(database.JinxW, wPrediction))
            {
                Engine::CastToPosition(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void Jinx_UseE(Object* pEnemy) {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !isTimeToCastE())
            return;

        if (pEnemy->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= eRange())
        {
            Engine::CastSelf(SpellIndex::E);
            ECastedTime = gameTime;
        }
    }

    void Jinx_UseR() {
        if (ObjectManager::GetLocalPlayer() == nullptr || !isTimeToCastR())
            return;

        Engine::CastSelf(SpellIndex::R);
        RCastedTime = gameTime;
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
    }

    void Combo() override {

        if (JinxConfig::JinxCombo::UseW->Value == true
            && JinxConfig::JinxSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),wRange());
            if (wTarget != nullptr)
            {
                Jinx_UseW(wTarget);
            }
        }

        if (JinxConfig::JinxCombo::UseQ->Value == true
            && JinxConfig::JinxSpellsSettings::qCastMode->Value == 0
            && isTimeToCastQ())
        {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),qRange());
            if (qTarget != nullptr)
            {
                Jinx_UseQ(qTarget);
            }
        }

        if (JinxConfig::JinxCombo::UseR->Value == true
            && ObjectManager::CountHeroesInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), rMinRange()) >= JinxConfig::JinxCombo::enemiesInRange->Value
            && isTimeToCastR())
        {
            Jinx_UseR();
        }
    }

    void Clear() override {
		if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), qRange()) > 0)
        {
            if (!HasEnoughMana(OrbwalkState::Clear)) return;

            if (JinxConfig::JinxJungle::UseW->Value == true && isTimeToCastW()) {
                const auto wTarget = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
                if (wTarget != nullptr) {
                    Jinx_UseW(wTarget);
                }
            }
        }
    }

    void Harass() override {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;

        if (JinxConfig::JinxHarass::UseW->Value
            && JinxConfig::JinxSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),wRange());
            if (wTarget != nullptr) {
                Jinx_UseW(wTarget);
            }
        }

        if (JinxConfig::JinxHarass::UseE->Value == true
            && JinxConfig::JinxSpellsSettings::qCastMode->Value == 0
            && isTimeToCastE())
        {
            const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),qRange());
            if (eTarget != nullptr && eTarget->GetHealth() < Jinx_dmgE(eTarget)) {
                Jinx_UseE(eTarget);
            }
        }
    }

    void Lasthit() override {
        return;
    }

    void Flee() override {
        return;
    }

    void Killsteal()
    {
        __try {
            if (JinxConfig::JinxKillsteal::UseE->Value && isTimeToCastE())
            {
                const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),eRange());
                if (eTarget != nullptr && eTarget->GetHealth() < Jinx_dmgE(eTarget))
                {
                    Jinx_UseE(eTarget);
                }
            }

        }
        __except (1)
        {
            LOG("ERROR IN KILLSTEAL MODE");
        }
    }


    //Events
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
            if (JinxConfig::JinxSpellsSettings::DrawW->Value == true && (JinxConfig::JinxSpellsSettings::DrawIfReady->Value == true && database.JinxW.IsCastable() || JinxConfig::JinxSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), wRange(), COLOR_WHITE, 1.0f);

            if (JinxConfig::JinxSpellsSettings::DrawE->Value == true && (JinxConfig::JinxSpellsSettings::DrawIfReady->Value == true && database.JinxE.IsCastable() || JinxConfig::JinxSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), eRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("ERROR IN RENDER MODE");
        }
    }
};

JinxModule module;