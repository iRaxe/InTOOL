#include "Yorick.h"

#include "Awareness.h"
#include "Orbwalker.h"
#include "stdafx.h"
#include "TargetSelector.h"

class YorickModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Yorick");

private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

private:
    static bool hasW(Object* pEnemy)
    {
        return pEnemy->GetBuffByName("Yorickwattach");
    }

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.YorickQ.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastW() const
    {
        return gameTime > WCastedTime + database.YorickW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + 0.25f;
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.YorickR.GetCastTime();
    }


public:

    YorickModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Init() override
    {
        const auto YorickMenu = Menu::CreateMenu("vezYorick", "vez.Yorick");

        const auto combo = YorickMenu->AddMenu("Combo Settings", "Combo Settings");
        YorickConfig::YorickCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        YorickConfig::YorickCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        YorickConfig::YorickCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
        YorickConfig::YorickCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto harassMenu = YorickMenu->AddMenu("Harass Settings", "Harass Settings");
        YorickConfig::YorickHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        YorickConfig::YorickHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
        YorickConfig::YorickHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        YorickConfig::YorickHarass::minGraves = harassMenu->AddSlider("minGraves", "Minimum Graves", 2, 1, 4, 1);

        YorickConfig::YorickHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = YorickMenu->AddMenu("Clear Settings", "Clear Settings");

        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        YorickConfig::YorickClear::UseQOnTowers = laneClearMenu->AddCheckBox("Use Q On Towers", "Use SpellSlot Q On Towers", true);
        YorickConfig::YorickClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        YorickConfig::YorickClear::minMinions = laneClearMenu->AddSlider("minWMinions", "Minimum Minions in E Width", 3, 1, 6, 1);

        YorickConfig::YorickClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        YorickConfig::YorickJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        YorickConfig::YorickJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        YorickConfig::YorickJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        YorickConfig::YorickJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        YorickConfig::YorickLastHit::UseQ = lastHitMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        YorickConfig::YorickLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = YorickMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        YorickConfig::YorickKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        YorickConfig::YorickKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        
        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        YorickConfig::YorickAntiGapCloser::UseW = antiGapMenu->AddCheckBox("Use W", "Use SpellSlot W", true);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        YorickConfig::YorickFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", true);

        YorickConfig::YorickFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto drawMenu = YorickMenu->AddMenu("Drawings Settings", "Drawings Settings");
        YorickConfig::YorickDrawings::DrawW = drawMenu->AddCheckBox("Draw W", "Draw SpellSlot W", true);
        YorickConfig::YorickDrawings::DrawE = drawMenu->AddCheckBox("Draw E", "Draw SpellSlot E", true);
        YorickConfig::YorickDrawings::DrawR = drawMenu->AddCheckBox("Draw R", "Draw SpellSlot R", true);

        YorickConfig::YorickDrawings::DrawIfReady = drawMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Yorick_dmgQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.YorickQ.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = YorickDamages::QSpell::dmgSkillArray[levelSpell - 1];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = YorickDamages::QSpell::additionalPercentageAD;

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage);
        return totalDamage;
    }

    static float Yorick_dmgE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.YorickE.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetLevel();
        const float skillDamage = YorickDamages::ESpell::dmgSkillArray[levelSpell - 1];

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = YorickDamages::ESpell::additionalPercentageAP;

        const float totalDamage = skillDamage + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return totalDamage;
    }

    bool IsCastingQ()
    {
        SpellCast* spellCast = globals::localPlayer->GetActiveSpellCast();
        return spellCast && spellCast->GetSpellInfo()->GetSpellData()->GetName() == "YorickQ2";
    }

    bool IsCastingR()
    {
        return globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetName() == "YorickR2";
    }

    bool IsMaidenAround()
    {
        auto monster = TargetSelector::Functions::GetObjectInRange("YorickBigGhoul", database.YorickR.GetMaxRange());
        return monster;
    }

    bool AreGhoulsAround()
    {
        auto monster = TargetSelector::Functions::GetObjectInRange("YorickGhoulMelee", database.YorickR.GetMaxRange());
        return monster;
    }

    bool AreGravesAround()
    {
        auto monster = TargetSelector::Functions::GetObjectInRange("TestCubeRender10Vision", database.YorickR.GetMaxRange());
        return monster;
    }

    int CountGravesAround()
    {
        return TargetSelector::Functions::GetObjectsInRange(globals::localPlayer->GetPosition(), "TestCubeRender10Vision", database.YorickR.GetMaxRange()).size();
    }

    void Yorick_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.YorickQ.IsCastable())
            return;

        if (pEnemy && isTimeToCastQ())
        {
            functions::CastSpell(SpellIndex::Q);
            QCastedTime = gameTime;
        }
    }

    void Yorick_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.YorickW.IsCastable())
            return;

        if (pEnemy && isTimeToCastW())
        {
            if (pEnemy->IsHero())
            {
                prediction::PredictionOutput wPrediction;
                if (GetPrediction(database.YorickW, wPrediction))
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

    void Yorick_UseE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.YorickE.IsCastable())
            return;

        if (pEnemy && isTimeToCastE())
        {
            if (pEnemy->IsHero())
            {
                prediction::PredictionOutput ePrediction;
                if (GetPrediction(database.YorickE, ePrediction))
                {
                    functions::CastSpell(SpellIndex::E, ePrediction.position);
                    ECastedTime = gameTime;
                }
            }
            else
            {
                functions::CastSpell(SpellIndex::E, pEnemy->GetPosition());
                ECastedTime = gameTime;
            }
        }
    }

    void Yorick_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || IsCastingR())
        	return;

        if (pEnemy && isTimeToCastR())
        {
            prediction::PredictionOutput rPrediction;
            if (GetPrediction(database.YorickR, rPrediction))
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
    }

    void Attack() override
    {
        if (database.YorickR.IsCastable() && YorickConfig::YorickCombo::UseR->Value == true)
        {
            if (const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickR.GetMaxRange()))
                Yorick_UseR(rTarget);
        }

        if (database.YorickW.IsCastable() && YorickConfig::YorickCombo::UseW->Value == true)
            if (const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickW.GetRange()))
                Yorick_UseW(wTarget);

        if (database.YorickE.IsCastable() && YorickConfig::YorickCombo::UseE->Value == true)
            if (const auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickE.GetRange() + 300.0f))
                Yorick_UseE(eTarget);

        if (database.YorickQ.IsCastable() && YorickConfig::YorickCombo::UseQ->Value == true)
			if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetRealAttackRange() + 50.0f))
				Yorick_UseQ(qTarget);
    }

    void Clear() override
    {
        if (globals::localPlayer->GetPercentMana() < static_cast<float>(YorickConfig::YorickJungle::minMana->Value))
            return;

        if (database.YorickQ.IsCastable())
        {
            if (const auto turret = TargetSelector::Functions::GetEnemyTurretInRange(globals::localPlayer->GetRealAttackRange() + 50.0f); YorickConfig::YorickClear::UseQOnTowers->Value == true)
                Yorick_UseQ(turret);

            if (const auto inhibitor = TargetSelector::Functions::GetEnemyInhibitorInRange(globals::localPlayer->GetRealAttackRange() + 50.0f); YorickConfig::YorickClear::UseQOnTowers->Value == true)
                Yorick_UseQ(inhibitor);

            if (const auto nexus = TargetSelector::Functions::GetEnemyNexusInRange(globals::localPlayer->GetRealAttackRange() + 50.0f); YorickConfig::YorickClear::UseQOnTowers->Value == true)
                Yorick_UseQ(nexus);

            if (const auto monster = TargetSelector::Functions::GetJungleInRange(globals::localPlayer->GetRealAttackRange() + 50.0f);
                YorickConfig::YorickJungle::UseQ->Value == true
                && monster)
            {
                Yorick_UseQ(monster);
            }

            if (const auto minion = TargetSelector::Functions::GetMinionInRange(globals::localPlayer->GetRealAttackRange() + 50.0f);
                YorickConfig::YorickLastHit::UseQ->Value == true
                && minion && minion->GetHealth() < Yorick_dmgQ(minion) + Damage::CalculateAutoAttackDamage(globals::localPlayer, minion))
            {
                Yorick_UseQ(minion);
            }
        }

        if (database.YorickW.IsCastable())
        {
            if (const auto monster = TargetSelector::Functions::GetJungleInRange(database.YorickW.GetRange());
                YorickConfig::YorickJungle::UseW->Value == true
                && monster)
            {
                Yorick_UseW(monster);
            }
        }

        if (database.YorickE.IsCastable())
        {
            if (const auto monster = TargetSelector::Functions::GetJungleInRange(database.YorickE.GetRange());
                YorickConfig::YorickJungle::UseE->Value == true
                && monster)
            {
                Yorick_UseE(monster);
            }

            if (const auto minion= TargetSelector::Functions::GetMinionInRange(database.YorickE.GetRange());
                YorickConfig::YorickClear::UseE->Value == true
                && minion 
                && TargetSelector::Functions::GetMinionsInRange(minion->GetPosition(), database.YorickE.GetRadius()).size() >= YorickConfig::YorickClear::minMinions->Value)
            {
                Yorick_UseE(minion);
            }
        }
    }

    void Harass() override
    {
        if (globals::localPlayer->GetPercentMana() < static_cast<float>(YorickConfig::YorickHarass::minMana->Value))
            return;

        if (database.YorickQ.IsCastable() && YorickConfig::YorickHarass::UseQ->Value == true)
            if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetRealAttackRange() + 50.0f))
                Yorick_UseQ(qTarget);

        if (database.YorickW.IsCastable() && YorickConfig::YorickHarass::UseW->Value == true)
            if (const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickW.GetMaxRange()))
            	Yorick_UseW(wTarget);

        if (database.YorickE.IsCastable() && YorickConfig::YorickHarass::UseE->Value == true)
            if (const auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickE.GetMaxRange()))
                if (AreGravesAround() && CountGravesAround() >= YorickConfig::YorickHarass::minGraves->Value)
					Yorick_UseE(eTarget);
        
    }

    void Lasthit() override
    {
        if (globals::localPlayer->GetPercentMana() < static_cast<float>(YorickConfig::YorickLastHit::minMana->Value))
            return;

        if (YorickConfig::YorickLastHit::UseQ->Value == true && database.YorickQ.IsCastable())
            if (const auto minion = TargetSelector::Functions::GetMinionInRange(database.YorickQ.GetRange()); minion->GetHealth() < Yorick_dmgQ(minion) + Damage::CalculateAutoAttackDamage(globals::localPlayer, minion))
                Yorick_UseQ(minion);
    }

    void Flee() override
    {
        if (YorickConfig::YorickFlee::UseE->Value == true && database.YorickE.IsCastable())
            if (const auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickE.GetRange()))
                Yorick_UseE(eTarget);

        if (YorickConfig::YorickFlee::UseW->Value == true && database.YorickW.IsCastable())
            if (const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickW.GetRange()))
                Yorick_UseW(wTarget);
    }

    void Killsteal()
    {
        if (YorickConfig::YorickKillsteal::UseQ->Value == true && database.YorickQ.IsCastable())
        {
            if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickQ.GetRange());
                qTarget && qTarget->GetHealth() < Yorick_dmgQ(qTarget))
            {
                Yorick_UseQ(qTarget);
            }
        }

        if (YorickConfig::YorickKillsteal::UseE->Value == true && database.YorickE.IsCastable())
        {
            if (const auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(database.YorickE.GetRange());
                eTarget && eTarget->GetHealth() < Yorick_dmgE(eTarget))
            {
                Yorick_UseE(eTarget);
            }
        }
    }

    void AntiGapCloser()
    {
        if (YorickConfig::YorickAntiGapCloser::UseW->Value == true && database.YorickW.IsCastable())
        {
            for (auto target : TargetSelector::Functions::GetTargetsInRange(globals::localPlayer->GetPosition(), database.YorickW.GetRange()))
            {
                if (!functions::MenuItemContains(YorickConfig::YorickAntiGapCloser::whitelist, target->GetName().c_str())) continue;
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, database.YorickW.GetRange()))
                    {
                        Yorick_UseW(target);
                    }
                }
            }
        }
    }

    //Events
    void OnBeforeAttack() override
    {

    }

    void OnCastSpell() override
    {

    }

    void Render() override
    {
       if (YorickConfig::YorickDrawings::DrawW->Value == true && (YorickConfig::YorickDrawings::DrawIfReady->Value == true && database.YorickW.IsCastable() || YorickConfig::YorickDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.YorickW.GetRange(), COLOR_WHITE, 1.0f);

       if (YorickConfig::YorickDrawings::DrawE->Value == true && (YorickConfig::YorickDrawings::DrawIfReady->Value == true && database.YorickE.IsCastable() || YorickConfig::YorickDrawings::DrawIfReady->Value == false))
           Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.YorickE.GetRange(), COLOR_WHITE, 1.0f);

       if (YorickConfig::YorickDrawings::DrawR->Value == true && (YorickConfig::YorickDrawings::DrawIfReady->Value == true && database.YorickR.IsCastable() || YorickConfig::YorickDrawings::DrawIfReady->Value == false))
           Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.YorickR.GetRange(), COLOR_WHITE, 1.0f);
    }
};

YorickModule module;