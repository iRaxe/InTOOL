#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Ezreal.h"

class EzrealModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Ezreal");

private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

private:
    static bool hasW(Object* pEnemy)
    {
        return pEnemy->GetBuffByName("ezrealwattach");
    }

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.EzrealQ.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastW() const
    {
        return gameTime > WCastedTime + database.EzrealW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + 0.25f;
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.EzrealR.GetCastTime();
    }

    static float qRange()
    {
        return EzrealConfig::EzrealSpellsSettings::qRange->Value;
    }

    static float wRange()
    {
        return EzrealConfig::EzrealSpellsSettings::wRange->Value;
    }

    static float eRange()
    {
        return EzrealConfig::EzrealSpellsSettings::eRange->Value;
    }

    static float rRange()
    {
        return EzrealConfig::EzrealSpellsSettings::maxRDistance->Value;
    }

public:

    EzrealModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {

        const auto EzrealMenu = Menu::CreateMenu("vezEzreal", "vez.Ezreal");

        const auto combo = EzrealMenu->AddMenu("Combo Settings", "Combo Settings");
        EzrealConfig::EzrealCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        EzrealConfig::EzrealCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        EzrealConfig::EzrealCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = EzrealMenu->AddMenu("Harass Settings", "Harass Settings");
        EzrealConfig::EzrealHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        EzrealConfig::EzrealHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = EzrealMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        EzrealConfig::EzrealClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W on turret", true);
        EzrealConfig::EzrealClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        EzrealConfig::EzrealJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        EzrealConfig::EzrealJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        EzrealConfig::EzrealLastHit::UseQ = lastHitMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = EzrealMenu->AddMenu("Additional Settings", "Additional Settings");
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        EzrealConfig::EzrealKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        EzrealConfig::EzrealKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        EzrealConfig::EzrealAntiGapCloser::eMode = antiGapMenu->AddList("eMode", "Dash To", std::vector<std::string>{"Near Mouse", "Extend Enemy Dash Position"}, 0);
        EzrealConfig::EzrealAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E To Mouse", false);
        const auto antiGapwhitelistMenu = antiGapMenu->AddMenu("Whitelist Settings", "Whitelist");

        const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
        EzrealConfig::EzrealAntiMelee::eMode = antiMeleeMenu->AddList("eMode", "Dash To", std::vector<std::string>{"Near Mouse", "Extend Enemy Dash Position"}, 0);
        EzrealConfig::EzrealAntiMelee::UseE = antiMeleeMenu->AddCheckBox("Use E", "Use SpellSlot E To Mouse", false);
        const auto antiMeleewhitelistMenu = antiMeleeMenu->AddMenu("Whitelist Settings", "Whitelist");
        for (int i = 0; i < globals::heroManager->GetListSize(); i++)
        {
            auto obj = globals::heroManager->GetIndex(i);
            if (obj != nullptr && obj->IsEnemy())
            {
                const auto antiGap_checkbox = antiGapwhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj](const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !EzrealConfig::EzrealAntiGapCloser::whitelist.empty())
                        {
                            const auto it = std::ranges::find(EzrealConfig::EzrealAntiGapCloser::whitelist, obj);
                            EzrealConfig::EzrealAntiGapCloser::whitelist.erase(it);
                        }
                        else
                        {
                            EzrealConfig::EzrealAntiGapCloser::whitelist.push_back(obj);
                        }
                    });

                if (antiGap_checkbox->Value == true)
                {
                    EzrealConfig::EzrealAntiGapCloser::whitelist.push_back(obj);
                }

                if (!obj->IsMelee()) continue;
                const auto antiMelee_checkbox = antiMeleewhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !EzrealConfig::EzrealAntiMelee::whitelist.empty())
                        {
                            const auto it = std::ranges::find(EzrealConfig::EzrealAntiMelee::whitelist, obj);
                            EzrealConfig::EzrealAntiMelee::whitelist.erase(it);
                        }
                        else
                        {
                            EzrealConfig::EzrealAntiMelee::whitelist.push_back(obj);
                        }
                    });

                if (antiMelee_checkbox->Value == true)
                {
                    EzrealConfig::EzrealAntiMelee::whitelist.push_back(obj);
                }
            }
        }

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        EzrealConfig::EzrealFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E To Mouse", false);

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");
        EzrealConfig::EzrealSpellsSettings::saveMana = spellsMenu->AddCheckBox("saveManaForCombo", "Save Mana For A Full Combo Rotation", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        EzrealConfig::EzrealSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        EzrealConfig::EzrealSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.EzrealQ.GetRange(), 100, database.EzrealQ.GetRange(), 50);
        EzrealConfig::EzrealSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        EzrealConfig::EzrealSpellsSettings::wCastMode = wSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        EzrealConfig::EzrealSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.EzrealW.GetRange(), 100, database.EzrealW.GetRange(), 50);
        EzrealConfig::EzrealSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
        EzrealConfig::EzrealSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.EzrealE.GetRange(), 100, database.EzrealE.GetRange(), 50);
        EzrealConfig::EzrealSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        EzrealConfig::EzrealSpellsSettings::targetMode = rSpellMenu->AddList("targetMode", "Target Selector Mode", std::vector<std::string>{"Inherit", "Near Mouse", "Near Champion"}, 0);
        EzrealConfig::EzrealSpellsSettings::rTapKey = rSpellMenu->AddKeyBind("rTapKey", "Aim SpellSlot R Key", VK_CONTROL, false, false);
        EzrealConfig::EzrealSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "SpellSlot R Minimum Fire Distance", 1000, 100, database.EzrealR.GetRange(), 100);
        EzrealConfig::EzrealSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "SpellSlot R Maximum Fire Distance", 3000, 100, database.EzrealR.GetRange(), 100);

        EzrealConfig::EzrealSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Ezreal_dmgQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealQ.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = EzrealDamages::QSpell::dmgSkillArray[levelSpell - 1];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = EzrealDamages::QSpell::additionalPercentageAD;

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = EzrealDamages::QSpell::additionalPercentageAP;

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage) + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, totalDamage);
    }

    static float Ezreal_dmgW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealW.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillDamage = EzrealDamages::WSpell::dmgSkillArray[levelSpell - 1];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = EzrealDamages::WSpell::additionalPercentageAD;

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = EzrealDamages::WSpell::additionalPercentageAP[levelSpell - 1];

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage) + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return Damage::CalculateMagicalDamage(globals::localPlayer, pEnemy, totalDamage);
    }

    static float Ezreal_dmgR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealR.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float skillDamage = EzrealDamages::RSpell::dmgSkillArray[levelSpell - 1];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = EzrealDamages::RSpell::additionalPercentageAD;

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = EzrealDamages::RSpell::additionalPercentageAP;

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage) + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return Damage::CalculateMagicalDamage(globals::localPlayer, pEnemy, totalDamage);
    }

    static bool Ezreal_HasEnoughComboMana()
    {
        if (globals::localPlayer == nullptr)
            return false;

        const float qSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetManaCost();
        const float wSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetManaCost();
        const float eSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetManaCost();
        const float rSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetManaCost();

        if (globals::localPlayer->GetLevel() >= 6)
            return globals::localPlayer->GetMana() > qSpellManaCost + wSpellManaCost + eSpellManaCost + rSpellManaCost;

        return globals::localPlayer->GetMana() > qSpellManaCost + wSpellManaCost + eSpellManaCost;
    }

    static bool Ezreal_HasEnoughMana(float minValue)
    {
        if (globals::localPlayer == nullptr || EzrealConfig::EzrealSpellsSettings::saveMana->Value && !Ezreal_HasEnoughComboMana())
            return false;

        return globals::localPlayer->GetPercentMana() > minValue;
    }

    void Ezreal_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealQ.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < qRange() && isTimeToCastQ())
        {
            Modules::prediction::PredictionOutput qPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.EzrealQ, qPrediction))
            {
                Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
                QCastedTime = gameTime;
            }
        }
    }

    void Ezreal_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealW.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < wRange() && isTimeToCastW())
        {
            Modules::prediction::PredictionOutput wPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.EzrealW, wPrediction))
            {
                Engine::CastToPosition(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void Ezreal_UseE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealE.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < eRange() && isTimeToCastE())
        {
            Engine::CastToPosition(SpellIndex::E, Engine::GetMouseWorldPos());
            ECastedTime = gameTime;
        }
    }

    void Ezreal_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.EzrealR.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < rRange() && isTimeToCastR())
        {
            Modules::prediction::PredictionOutput rPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.EzrealR, rPrediction))
            {
                Engine::CastToPosition(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }
        }
    }

    void Update() override
    {
        gameTime = Engine::GetGameTime();

        Killsteal();
        AntiGapCloser();
        AntiMelee();
        if (EzrealConfig::EzrealSpellsSettings::rTapKey->Value == true && database.EzrealR.IsCastable())
        {
            switch (EzrealConfig::EzrealSpellsSettings::targetMode->Value)
            {
            case 0: //Inherit
                if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange()))
                    Ezreal_UseR(rTarget);
                break;
            case 1: //NearMouse
                if (const auto rTarget2 = TargetSelector::FindBestTarget(Engine::GetMouseWorldPos(), 300.0f))
                    Ezreal_UseR(rTarget2);
                break;
            case 2: //NearChampion
                if (const auto rTarget3 = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), 500.0f))
                    Ezreal_UseR(rTarget3);
                break;
            }
        }

    }

    void Combo() override
    {
        if (EzrealConfig::EzrealCombo::UseR->Value == true && database.EzrealR.IsCastable() && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), EzrealConfig::EzrealSpellsSettings::minRDistance->Value) >= EzrealConfig::EzrealCombo::enemiesInRange->Value)
        {
            if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange()))
                Ezreal_UseR(rTarget);
        }

        if (EzrealConfig::EzrealCombo::UseW->Value == true && database.EzrealW.IsCastable() && EzrealConfig::EzrealSpellsSettings::wCastMode->Value == 0)
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
                Ezreal_UseW(wTarget);
        }

        if (EzrealConfig::EzrealCombo::UseQ->Value == true && database.EzrealQ.IsCastable() && EzrealConfig::EzrealSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Ezreal_UseQ(qTarget);
        }
    }

    void Clear() override
    {
        //Laneclear
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) > 0)
        {

            if (!Ezreal_HasEnoughMana(EzrealConfig::EzrealClear::minMana->Value)) return;

            if (EzrealConfig::EzrealClear::UseQ->Value == true && database.EzrealQ.IsCastable())
            {
                const auto qTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), qRange(), Alliance::Enemy);
                if (qTarget != nullptr && qTarget->GetHealth() < Ezreal_dmgQ(qTarget))
                    Ezreal_UseQ(qTarget);
            }
        }

        //Jungleclear
        else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
        {
            if (!Ezreal_HasEnoughMana(EzrealConfig::EzrealJungle::minMana->Value)) return;

            if (EzrealConfig::EzrealJungle::UseW->Value == true && database.EzrealW.IsCastable())
            {
                const auto wTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), wRange());
                if (wTarget != nullptr && (wTarget->GetName().contains("Dragon") || wTarget->GetName().contains("Baron") || wTarget->GetName().contains("Herald")))
                    Ezreal_UseW(wTarget);
            }

            if (EzrealConfig::EzrealJungle::UseQ->Value == true && database.EzrealQ.IsCastable())
            {
                if (const auto qTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), qRange()))
                    Ezreal_UseQ(qTarget);
            }
        }
    }


    void Harass() override
    {
        if (!Ezreal_HasEnoughMana(EzrealConfig::EzrealHarass::minMana->Value))
            return;

        if (EzrealConfig::EzrealHarass::UseW->Value == true && database.EzrealW.IsCastable() && EzrealConfig::EzrealSpellsSettings::wCastMode->Value == 0)
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
                Ezreal_UseW(wTarget);
        }

        if (EzrealConfig::EzrealHarass::UseQ->Value == true && database.EzrealQ.IsCastable() && EzrealConfig::EzrealSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Ezreal_UseQ(qTarget);
        }
    }

    void Lasthit() override
    {
        if (!Ezreal_HasEnoughMana(EzrealConfig::EzrealLastHit::minMana->Value))
            return;

        if (EzrealConfig::EzrealLastHit::UseQ->Value == true && database.EzrealQ.IsCastable())
        {
            const auto minion = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), qRange(), Alliance::Enemy);
            if (minion != nullptr && minion->GetHealth() < Ezreal_dmgQ(minion))
                Ezreal_UseQ(minion);
        }
    }

    void Flee() override
    {
        if (EzrealConfig::EzrealFlee::UseE->Value == true && database.EzrealE.IsCastable())
        {
            const Vector3 pathEnd = Engine::GetMouseWorldPos();
            if (pathEnd.IsValid() && globals::localPlayer->GetDistanceTo(globals::localPlayer) < 750.0f && isTimeToCastE())
            {
                Engine::CastToPosition(SpellIndex::E, Engine::GetMouseWorldPos());
                ECastedTime = gameTime;
            }
        }
    }

    void Killsteal()
    {
        __try {
            if (EzrealConfig::EzrealKillsteal::UseQ->Value == true && database.EzrealQ.IsCastable())
            {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange());
                if (qTarget != nullptr && qTarget->GetHealth() < Ezreal_dmgQ(qTarget))
                {
                    Ezreal_UseQ(qTarget);
                }
            }

            if (EzrealConfig::EzrealKillsteal::UseR->Value == true && database.EzrealR.IsCastable())
            {
                const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange());
                if (rTarget != nullptr
                    && rTarget->GetDistanceTo(globals::localPlayer) > EzrealConfig::EzrealSpellsSettings::minRDistance->Value
                    && rTarget->GetDistanceTo(globals::localPlayer) < EzrealConfig::EzrealSpellsSettings::maxRDistance->Value
                    && rTarget->GetHealth() < Ezreal_dmgR(rTarget))
                {
                    Ezreal_UseR(rTarget);
                }
            }
        }
        __except (1)
        {
            LOG("ERROR IN KILLSTEAL MODE");
        }
    }

    void AntiGapCloser()
    {
        if (EzrealConfig::EzrealAntiGapCloser::UseE->Value == true && Engine::GetSpellState(SpellIndex::E) == 0)
        {

            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > 750.0f) continue;
                if (!Engine::MenuItemContains(EzrealConfig::EzrealAntiGapCloser::whitelist, target->GetName().c_str())) continue;
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target != nullptr)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->GetPosition().distanceTo(pathEnd) < 750.0f)
                    {
                        Ezreal_UseE(target);
                    }
                }
            }
        }
    }

    void AntiMelee() 
    {
        if (EzrealConfig::EzrealAntiMelee::UseE->Value == true && Engine::GetSpellState(SpellIndex::E) == 0)
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > 750.0f) continue;
                if (!Engine::MenuItemContains(EzrealConfig::EzrealAntiMelee::whitelist, target->GetName().c_str())) continue;

                if (target != nullptr && target->GetDistanceTo(globals::localPlayer) < target->GetRealAttackRange())
                {
                    const Vector3 pathEnd = Engine::GetMouseWorldPos();
                    if (pathEnd.IsValid() && globals::localPlayer->GetPosition().distanceTo(pathEnd) < 750.0f)
                    {
                        Ezreal_UseE(target);
                    }
                }
            }
        }
    }

    //Events
    void OnCreateMissile() override {
        return;
    }

    void OnDeleteMissile() override {
        return;
    }

    void OnBeforeAttack() override
    {
        __try {
            //Combo mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (EzrealConfig::EzrealCombo::UseW->Value == true && database.EzrealW.IsCastable() && EzrealConfig::EzrealSpellsSettings::wCastMode->Value == 1)
                    {
                        Ezreal_UseW(object);
                    }

                    if (EzrealConfig::EzrealCombo::UseQ->Value == true && database.EzrealQ.IsCastable() && EzrealConfig::EzrealSpellsSettings::qCastMode->Value == 1)
                    {
                        Ezreal_UseQ(object);
                    }
                }
            }
            //Laneclear mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsTurret())
                {
                    if (EzrealConfig::EzrealClear::UseW->Value == true && database.EzrealW.IsCastable() && Ezreal_HasEnoughMana(EzrealConfig::EzrealClear::minMana->Value))
                    {
                        Ezreal_UseW(object);
                    }
                }
            }
            //Harass mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (!Ezreal_HasEnoughMana(EzrealConfig::EzrealHarass::minMana->Value))
                        return;

                    if (EzrealConfig::EzrealHarass::UseW->Value == true && database.EzrealW.IsCastable() && EzrealConfig::EzrealSpellsSettings::wCastMode->Value == 1)
                    {
                        Ezreal_UseW(object);
                    }

                    if (EzrealConfig::EzrealHarass::UseQ->Value == true && database.EzrealQ.IsCastable() && EzrealConfig::EzrealSpellsSettings::qCastMode->Value == 1)
                    {
                        Ezreal_UseQ(object);
                    }
                }
            }

        }
        __except (1)
        {
            LOG("ERROR IN ONBEFOREATTACK MODE");
        }
    }

    void OnAfterAttack() override {
        return;
    }

    void Render() override
    {
        __try {
            if (EzrealConfig::EzrealSpellsSettings::DrawQ->Value == true && (EzrealConfig::EzrealSpellsSettings::DrawIfReady->Value == true && database.EzrealQ.IsCastable() || EzrealConfig::EzrealSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);

            if (EzrealConfig::EzrealSpellsSettings::DrawW->Value == true && (EzrealConfig::EzrealSpellsSettings::DrawIfReady->Value == true && database.EzrealW.IsCastable() || EzrealConfig::EzrealSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), wRange(), COLOR_WHITE, 1.0f);

            if (EzrealConfig::EzrealSpellsSettings::DrawE->Value == true && (EzrealConfig::EzrealSpellsSettings::DrawIfReady->Value == true && database.EzrealE.IsCastable() || EzrealConfig::EzrealSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), eRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("ERROR IN RENDER MODE");
        }
    }
};

EzrealModule module;