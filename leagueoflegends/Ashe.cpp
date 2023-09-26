#include "Ashe.h"
#include "stdafx.h"
#include "TargetSelector.h"
#include "Orbwalker.h"

class AsheModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Ashe");
private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

private:
    bool Ashe_IsQReady()
    {
        if (globals::localPlayer->GetBuffByName("asheqcastready"))
            return true;
        
        return false;
    }

    bool Ashe_IsCastingQ()
    {
        if (globals::localPlayer->GetBuffByName("AsheQAttack"))
            return true;

        return false;
    }

    bool Ashe_TargetSlowed(Object* pEnemy)
    {
        if (pEnemy->GetBuffByName("ashepassiveslow"))
            return true;

        return false;
    }

    float Ashe_speedR(float distance)
    {
        if (!database.AsheR.IsCastable())
            return 0;

        float travelingTime, kekwTime;
        float startingSpeed = 1500.0f;
        float evolvingSpeed = 2100.0f;
        float accellerationDistance = 1350.0f;

        if (distance <= accellerationDistance)
            return distance / startingSpeed;

        travelingTime = accellerationDistance / startingSpeed;
        kekwTime = (distance - accellerationDistance) / evolvingSpeed;
        return travelingTime + kekwTime;
    }

public:

    AsheModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Init() override
    {
        auto asheMenu = Menu::CreateMenu("vezAshe", "vez.Ashe");

        auto miscMenu = asheMenu->AddMenu("Misc Settings", "Misc Settings");
        AsheConfig::AsheMisc::UseQOnObjectives = miscMenu->AddCheckBox("UseQOnObjectives", "Use Q On Objectives", true);

        auto combo = asheMenu->AddMenu("Combo Settings", "Combo Settings");
        AsheConfig::AsheCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheCombo::UseWIfInAARange = combo->AddCheckBox("Use W In AA Range", "Use SpellSlot W In AA Range", true);
        AsheConfig::AsheCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        //AsheConfig::AsheCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        auto harassMenu = asheMenu->AddMenu("Harass Settings", "Harass Settings");
        AsheConfig::AsheHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        auto clearMenu = asheMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        AsheConfig::AsheClear::UseQ = clearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheClear::UseW = clearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheClear::minMana = clearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        auto jungleMenu = asheMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        AsheConfig::AsheJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        auto ksMenu = asheMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        AsheConfig::AsheKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        auto drawMenu = asheMenu->AddMenu("Drawings Settings", "Drawings Settings");
        AsheConfig::AsheDrawings::DrawW = drawMenu->AddCheckBox("Draw W", "Draw SpellSlot W", true);
    }


    float Ashe_dmgW(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.AsheW.IsCastable())
            return -9999;

        int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();

        float attackDamage = globals::localPlayer->GetAttackDamage();
        float enemyHP = pEnemy->GetEffectiveHealth(Physical);
        float skillDamage = AsheDamages::WSpell::dmgSkillArray[levelSpell];
        float additionalADDamage = AsheDamages::WSpell::additionalPercentageAD * attackDamage;
        float totalDamage = skillDamage + additionalADDamage;

        return totalDamage;
    }

    float Ashe_dmgR(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.AsheR.IsCastable())
            return -9999;

        int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();

        float attackDamage = globals::localPlayer->GetAttackDamage();
        float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        float enemyHP = pEnemy->GetEffectiveHealth(Physical);
        float skillDamage = AsheDamages::RSpell::dmgSkillArray[levelSpell];
        float additionalADDamage = AsheDamages::RSpell::additionalPercentageAD * attackDamage;
        float additionalApDamage = AsheDamages::RSpell::additionalPercentageAP * abilityPowerDamage;

        float totalDamage = skillDamage + additionalADDamage + additionalApDamage;

        return totalDamage;
    }

    void Ashe_UseQ(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !Ashe_IsQReady())
            return;

        if (pEnemy && gameTime > QCastedTime + 0.25f)
        {
            if (pEnemy->IsInAARange())
            {
                functions::CastSpell(SpellIndex::Q, globals::localPlayer);
                QCastedTime = gameTime;
            }
        }
    }

    void Ashe_UseW(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.AsheW.IsCastable())
            return;

        if (pEnemy && gameTime > WCastedTime + database.AsheW.GetCastTime())
        {
            prediction::PredictionOutput wPrediction;

            if (GetPrediction(database.AsheW, wPrediction))
            {
                functions::CastSpell(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
            
        }
    }

    void Ashe_UseR(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.AsheR.IsCastable())
            return;

        if (pEnemy && gameTime > RCastedTime + database.AsheR.GetCastTime())
        {
            prediction::PredictionOutput rPrediction;

            if (GetPrediction(database.AsheR, rPrediction))
            {
                functions::CastSpell(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }

        }
    }

    void Update() override
    {
        gameTime = functions::GetGameTime();
        /*InventorySlot* item2 = globals::localPlayer->GetInventorySlotById(1);
        if (item2 != nullptr)
        {
            LOG("Item Name: %d", item2->GetId());
        }*/
    }

    void Attack() override
    {
        if (AsheConfig::AsheCombo::UseQ->Value == true)
        {
            auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetRealAttackRange());
            if (qTarget)
            {
                Ashe_UseQ(qTarget);
            }
        }

        if (AsheConfig::AsheCombo::UseW->Value == true)
        {
            auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.AsheW.GetMaxRange());
            if (wTarget)
            {
                if (AsheConfig::AsheCombo::UseWIfInAARange->Value == true && wTarget->IsInAARange()
                    || AsheConfig::AsheCombo::UseWIfInAARange->Value == false && !wTarget->IsInAARange())
                {
                    Ashe_UseW(wTarget);
                }
            }
        }

        if (AsheConfig::AsheCombo::UseR->Value == true)
        {
            auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(3000.0f);
            if (rTarget)
            {
                Ashe_UseR(rTarget);
            }
        }
    }

    void Clear() override
    {
        if (AsheConfig::AsheClear::UseQ->Value == true)
        {
            if (Ashe_IsQReady())
            {
                auto qMonster = TargetSelector::Functions::GetObjectInRange(
                    globals::localPlayer->GetRealAttackRange(), "",
                    { ObjectType::Minion_Lane, ObjectType::Monster },
                    { ObjectType::Monster_Epic, ObjectType::Monster_Dragon }, false);
                if (qMonster)
                {
                    Orbwalker::Functions::Actions::CastSpell(SpellIndex::Q, qMonster->GetPosition());
                    return;
                }
            }
        }

        if (AsheConfig::AsheClear::UseW->Value == true)
        {
            if (database.AsheW.IsCastable())
            {
                auto wMonster = TargetSelector::Functions::GetObjectInRange(
                    database.AsheW.GetMaxRange(), "",
                    { ObjectType::Minion_Lane, ObjectType::Monster },
                    { ObjectType::Monster_Epic, ObjectType::Monster_Dragon }, false);

                if (wMonster)
                {
                    Orbwalker::Functions::Actions::CastSpell(SpellIndex::W, wMonster->GetPosition());
                    return;
                }
            }
        }
    }

    void Harass() override
    {

    }
    void Lasthit() override
    {

    }
    void Killsteal()
	{

    }

    void Render() override
    {


    }
};

AsheModule module;