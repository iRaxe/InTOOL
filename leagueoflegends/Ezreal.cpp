#include "Ezreal.h"

#include "Awareness.h"
#include "stdafx.h"
#include "TargetSelector.h"

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

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.EzrealR.GetCastTime();
    }
    

public:

    EzrealModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Init() override
    {
	    const auto EzrealMenu = Menu::CreateMenu("vezEzreal", "vez.Ezreal");

        const auto combo = EzrealMenu->AddMenu("Combo Settings", "Combo Settings");
        EzrealConfig::EzrealCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        EzrealConfig::EzrealCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        EzrealConfig::EzrealCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        EzrealConfig::EzrealCombo::manualCastUltimateKey = combo->AddKeyBind("manualCastUltimateKey", "Aim SpellSlot R Key", VK_CONTROL, false, false);

        EzrealConfig::EzrealCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);
        EzrealConfig::EzrealCombo::maxTargetDistance = combo->AddSlider("maxTargetDistance", "Maximum distance between you and target", 2000, 100, 5000, 100);

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
        EzrealConfig::EzrealAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E To Mouse", false);

	    const auto drawMenu = EzrealMenu->AddMenu("Drawings Settings", "Drawings Settings");
        EzrealConfig::EzrealDrawings::DrawQ = drawMenu->AddCheckBox("Draw Q", "Draw SpellSlot Q", true);
        EzrealConfig::EzrealDrawings::DrawW = drawMenu->AddCheckBox("Draw W", "Draw SpellSlot W", true);
        EzrealConfig::EzrealDrawings::DrawIfReady = drawMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Ezreal_dmgQ(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.EzrealQ.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = EzrealDamages::QSpell::dmgSkillArray[levelSpell];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = EzrealDamages::QSpell::additionalPercentageAD;

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = EzrealDamages::QSpell::additionalPercentageAP;

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage) + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return totalDamage;
    }

    static float Ezreal_dmgW(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.EzrealW.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillDamage = EzrealDamages::WSpell::dmgSkillArray[levelSpell];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = EzrealDamages::WSpell::additionalPercentageAD;

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = EzrealDamages::WSpell::additionalPercentageAP[levelSpell];

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage) + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return totalDamage;
    }

    static float Ezreal_dmgR(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !database.EzrealR.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float skillDamage = EzrealDamages::RSpell::dmgSkillArray[levelSpell];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = EzrealDamages::RSpell::additionalPercentageAD;

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = EzrealDamages::RSpell::additionalPercentageAP;

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage) + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return totalDamage;
    }

    void Ezreal_UseQ(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && isTimeToCastQ())
        {
            if (pEnemy->IsHero())
            {
                prediction::PredictionOutput qPrediction;
                if (GetPrediction(database.EzrealQ, qPrediction))
                {
                    functions::CastSpell(SpellIndex::Q, qPrediction.position);
                    QCastedTime = gameTime;
                }
            }
            else
            {
                functions::CastSpell(SpellIndex::Q, pEnemy->GetPosition());
                QCastedTime = gameTime;
            }

        }
    }

    void Ezreal_UseW(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && isTimeToCastW())
        {
	        if (pEnemy->IsHero())
	        {
                prediction::PredictionOutput wPrediction;
                if (GetPrediction(database.EzrealW, wPrediction))
                {
                    functions::CastSpell(SpellIndex::W, wPrediction.position);
                    WCastedTime = gameTime;
                }
	        }
	        else
	        {
                functions::CastSpell(SpellIndex::W, pEnemy->GetPosition());
                WCastedTime = gameTime;
	        }
            
        }
    }

    void Ezreal_UseE(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && gameTime > ECastedTime + 0.25f)
        {
            functions::CastSpell(SpellIndex::E, functions::GetMouseWorldPos());
            ECastedTime = gameTime;
        }
    }

    void Ezreal_UseR(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && isTimeToCastR())
        {
            prediction::PredictionOutput rPrediction;
            if (GetPrediction(database.EzrealR, rPrediction))
            {
                functions::CastSpell(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }
        }
    }

    void Update() override
    {
        gameTime = functions::GetGameTime();

        Killsteal();
        AntiGapCloser();

        if (EzrealConfig::EzrealCombo::manualCastUltimateKey->Value == true)
        {
            if (const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(EzrealConfig::EzrealCombo::maxTargetDistance->Value))
                Ezreal_UseR(rTarget);
        }

        /*InventorySlot* item2 = globals::localPlayer->GetInventorySlotById(0);
        if (item2 != nullptr)
        {
            LOG("Item Name: %d", item2->GetId());
        }*/
    }

    void Attack() override
    {
        if (database.EzrealR.IsCastable() 
            && EzrealConfig::EzrealCombo::UseR->Value == true 
            && TargetSelector::Functions::GetTargetsInRange(EzrealConfig::EzrealCombo::maxTargetDistance->Value).size() > EzrealConfig::EzrealCombo::enemiesInRange->Value)
        {
	        if (const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(EzrealConfig::EzrealCombo::maxTargetDistance->Value))
                Ezreal_UseR(rTarget);
        }

        if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.EzrealQ.GetRange()))
        {
            if (EzrealConfig::EzrealCombo::UseW->Value == true && database.EzrealW.IsCastable())
                Ezreal_UseW(qTarget);

            if (EzrealConfig::EzrealCombo::UseQ->Value == true && database.EzrealQ.IsCastable())
                Ezreal_UseQ(qTarget);            
        }
    }

    void Clear() override
    {
        if (globals::localPlayer->GetPercentMana() < static_cast<float>(EzrealConfig::EzrealJungle::minMana->Value))
            return;

        if (database.EzrealQ.IsCastable())
        {
	        if (const auto minion = TargetSelector::Functions::GetMinionInRange(database.EzrealQ.GetRange());
                EzrealConfig::EzrealClear::UseQ->Value == true 
                && minion)
	        {
                Ezreal_UseQ(minion);
	        }

	        if (const auto monster = TargetSelector::Functions::GetJungleInRange(database.EzrealQ.GetRange());
                EzrealConfig::EzrealJungle::UseQ->Value == true 
                && monster)
            {
                Ezreal_UseQ(monster);
            }
        }

        if (database.EzrealW.IsCastable())
        {
            if (const auto monster = TargetSelector::Functions::GetJungleInRange(database.EzrealW.GetRange());
                EzrealConfig::EzrealJungle::UseW->Value == true
                && monster
                && (monster->GetName().contains("Dragon") || monster->GetName().contains("Baron") || monster->GetName().contains("Herald")))
            {
                Ezreal_UseW(monster);
            }

	        if (EzrealConfig::EzrealClear::UseW->Value == true)
	        {
		        if (const auto turret = TargetSelector::Functions::GetEnemyTurretInRange(database.EzrealW.GetRange()))
                    Ezreal_UseW(turret);

                if (const auto inhibitor = TargetSelector::Functions::GetEnemyInhibitorInRange(database.EzrealW.GetRange()))
                    Ezreal_UseW(inhibitor);

                if (const auto nexus = TargetSelector::Functions::GetEnemyNexusInRange(database.EzrealW.GetRange()))
                	Ezreal_UseW(nexus);
	        }            
        }
    }

    void Harass() override
    {
        if (globals::localPlayer->GetPercentMana() < static_cast<float>(EzrealConfig::EzrealHarass::minMana->Value))
            return;

        if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.EzrealQ.GetRange()))
        {
            if (EzrealConfig::EzrealHarass::UseW->Value == true && database.EzrealW.IsCastable())
                Ezreal_UseW(qTarget);

            if (EzrealConfig::EzrealHarass::UseQ->Value == true && database.EzrealQ.IsCastable())
                Ezreal_UseQ(qTarget);
        }
    }

    void Lasthit() override
    {
        if (globals::localPlayer->GetPercentMana() < static_cast<float>(EzrealConfig::EzrealLastHit::minMana->Value))
            return;

        if (EzrealConfig::EzrealLastHit::UseQ->Value == true && database.EzrealQ.IsCastable())
        {
	        if (const auto minion = TargetSelector::Functions::GetMinionInRange(database.EzrealQ.GetRange()); minion && minion->GetHealth() < Ezreal_dmgQ(minion))
            {
                Ezreal_UseQ(minion);
            }
        }

    }

    void Killsteal()
    {
        if (EzrealConfig::EzrealKillsteal::UseQ->Value == true && database.EzrealQ.IsCastable())
        {
	        if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.EzrealQ.GetRange()); 
                qTarget && qTarget->GetHealth() < Ezreal_dmgQ(qTarget))
            {
                Ezreal_UseQ(qTarget);
            }
        }

        if (EzrealConfig::EzrealKillsteal::UseR->Value == true && database.EzrealR.IsCastable())
        {
	        if (const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.EzrealR.GetRange());
                rTarget && rTarget->GetDistanceTo(globals::localPlayer) < EzrealConfig::EzrealCombo::maxTargetDistance->Value 
                && rTarget->GetHealth() < Ezreal_dmgR(rTarget))
            {
                Ezreal_UseR(rTarget);
            }
        }
    }

    void AntiGapCloser()
    {
        if (EzrealConfig::EzrealAntiGapCloser::UseE->Value == true && functions::GetSpellState(SpellIndex::E) == 0)
        {
            for (auto target : TargetSelector::Functions::GetTargetsInRange(750.0f))
            {
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, 750.0f))
                    {
	                    Ezreal_UseE(target);
                    }
                }
            }
        }
    }

    void Render() override
    {
        if (EzrealConfig::EzrealDrawings::DrawQ->Value == true && (EzrealConfig::EzrealDrawings::DrawIfReady->Value == true && database.EzrealQ.IsCastable() || EzrealConfig::EzrealDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.EzrealQ.GetRange(), COLOR_WHITE, 1.0f);

        if (EzrealConfig::EzrealDrawings::DrawW->Value == true && (EzrealConfig::EzrealDrawings::DrawIfReady->Value == true && database.EzrealW.IsCastable() || EzrealConfig::EzrealDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.EzrealW.GetRange(), COLOR_WHITE, 1.0f);
    }
};

EzrealModule module;