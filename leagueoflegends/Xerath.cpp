#include "Xerath.h"

#include "Awareness.h"
#include "stdafx.h"
#include "TargetSelector.h"

class XerathModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Xerath");
private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.XerathQ.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastW() const
    {
        return gameTime > WCastedTime + database.XerathW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + database.XerathE.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.XerathR.GetCastTime() + XerathConfig::XerathUltimate::castDelay->Value/1000;
    }

public:

    XerathModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto XerathMenu = Menu::CreateMenu("vezXerath", "vez.Xerath");

        const auto combo = XerathMenu->AddMenu("Combo Settings", "Combo Settings");
        XerathConfig::XerathCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        XerathConfig::XerathCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        XerathConfig::XerathCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
        XerathConfig::XerathCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        XerathConfig::XerathCombo::UseR->AddTooltip("Be sure to check Additional settings -> Ultimate Settings\nFor correct ultimate usage");


        const auto harassMenu = XerathMenu->AddMenu("Harass Settings", "Harass Settings");
        XerathConfig::XerathHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        XerathConfig::XerathHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
        XerathConfig::XerathHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        XerathConfig::XerathHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = XerathMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        XerathConfig::XerathJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        XerathConfig::XerathJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        XerathConfig::XerathJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        XerathConfig::XerathJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto laneMenu = XerathMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        XerathConfig::XerathLaneClear::UseQ = laneMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        XerathConfig::XerathLaneClear::minQMinions = laneMenu->AddSlider("minQMinions", "Minimum Minions in Q Width", 3, 1, 6, 1);
        XerathConfig::XerathLaneClear::UseW = laneMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        XerathConfig::XerathLaneClear::minWMinions = laneMenu->AddSlider("minWMinions", "Minimum Minions in W Width", 3, 1, 6, 1);
    	XerathConfig::XerathLaneClear::minMana = laneMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

    	const auto lasthitMenu = XerathMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        XerathConfig::XerathLastHit::UseW = lasthitMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        XerathConfig::XerathLastHit::UseE = lasthitMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        XerathConfig::XerathLastHit::minMana = lasthitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = XerathMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ultimateMenu = additionalMenu->AddMenu("Ultimate Settings", "Ultimate Settings");
        XerathConfig::XerathUltimate::targetMode = ultimateMenu->AddList("targetMode", "Target Selector Mode", std::vector<std::string>{"Inherit", "Near Mouse"}, 0);
        XerathConfig::XerathUltimate::manualCastUltimateKey = ultimateMenu->AddKeyBind("manualCastUltimateKey", "Aim SpellSlot R Key", VK_CONTROL, false, false);
        XerathConfig::XerathUltimate::castDelay = ultimateMenu->AddSlider("castDelay", "Minimum Delay", 1000, 700, 2000, 100);

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        XerathConfig::XerathKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        XerathConfig::XerathKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        XerathConfig::XerathKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        XerathConfig::XerathKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        XerathConfig::XerathKillsteal::UseR->AddTooltip("Be sure to check Ultimate Settings\nFor correct ultimate usage");
        XerathConfig::XerathKillsteal::enemiesKillable = combo->AddSlider("enemiesKillable", "Minimum enemies killable to use R", 2, 1, 5, 1);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        XerathConfig::XerathAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto interrupterMenu = additionalMenu->AddMenu("Interrupter Settings", "Interrupter Settings");
        XerathConfig::XerathInterrupter::UseE = interrupterMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        XerathConfig::XerathFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto drawMenu = XerathMenu->AddMenu("Drawings Settings", "Drawings Settings");
        XerathConfig::XerathDrawings::DrawQ = drawMenu->AddCheckBox("Draw Q", "Draw SpellSlot Q", true);
        XerathConfig::XerathDrawings::DrawW = drawMenu->AddCheckBox("Draw W", "Draw SpellSlot W", true);
        XerathConfig::XerathDrawings::DrawE = drawMenu->AddCheckBox("Draw E", "Draw SpellSlot E", true);
        XerathConfig::XerathDrawings::DrawR = drawMenu->AddCheckBox("Draw R", "Draw SpellSlot R", true);
        XerathConfig::XerathDrawings::DrawIfReady = drawMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Xerath_dmgQ(const Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathQ.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = XerathDamages::QSpell::dmgSkillQ[levelSpell - 1];

        const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
        const float additionalSkillDamage = XerathDamages::QSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    static float Xerath_dmgW(const Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathW.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillDamage = XerathDamages::WSpell::dmgSkillW[levelSpell - 1];

        const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
        const float additionalSkillDamage = XerathDamages::WSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    static float Xerath_dmgE(const Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathE.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetLevel();
        const float skillDamage = XerathDamages::ESpell::dmgSkillE[levelSpell - 1];

        const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
        const float additionalSkillDamage = XerathDamages::ESpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    float Xerath_dmgR(const Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathR.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float skillDamage = XerathDamages::RSpell::dmgSkillR[levelSpell - 1];

        const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
        const float additionalSkillDamage = XerathDamages::RSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage * Xerath_GetRStacks();
    }

    int Xerath_GetRStacks()
    {
	    const auto rSkill = globals::localPlayer->GetSpellBySlotId(SpellIndex::R);
        if (rSkill)
        {
	        const int Rstack[4] = { 0, 3, 4, 5 };
            return Rstack[rSkill->GetLevel()];
        }
        return 0;
    }

    bool IsCastingQ()
    {
        SpellCast* spellCast = globals::localPlayer->GetActiveSpellCast();
        return spellCast && spellCast->GetSpellInfo()->GetSpellData()->GetName() == "XerathArcanopulseChargeUp";
    }

    bool IsCastingR()
    {
        SpellCast* spellCast = globals::localPlayer->GetActiveSpellCast();
        return spellCast && spellCast->GetSpellInfo()->GetSpellData()->GetName() == "XerathLocusOfPower2";
    }

    float Xerath_QRange()
    {
        if (!IsCastingQ()) return 0.0f;

        float castTime = gameTime - QCastedTime;
        float deltaCurrentRange = min(1.0f, castTime / 1.750);
        float deltaRange = 1450.0f - 700.0f;
        float currentRange = 700.0f + deltaCurrentRange * deltaRange;

        return currentRange;
    }

    void Xerath_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || IsCastingQ())
        	return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= database.XerathQ.GetRange() && isTimeToCastQ())
        {
            Engine::CastSpell(SpellIndex::Q);
            QCastedTime = gameTime;
        }
    }

    void Xerath_UseQ2(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !IsCastingQ())
        	return;

    	if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= database.XerathQ.GetRange() && isTimeToCastQ())
        {
            float rangeCharged = Xerath_QRange();

            float moveSpeed = (pEnemy->ReadClientStat(Object::MovementSpeed) > 1) ? pEnemy->ReadClientStat(Object::MovementSpeed) : 350.0f;
            if (pEnemy->GetDistanceTo(globals::localPlayer) <= 750.0f - moveSpeed * database.XerathQ.GetCastTime() + 50.0f)
            {
	            if (pEnemy->IsHero())
	            {
                    Modules::prediction::PredictionOutput qPrediction;
                    if (GetPrediction(database.XerathQ, qPrediction))
                        Engine::ReleaseSpell(SpellIndex::Q, qPrediction.position);
                    QCastedTime = 0.0f;
	            }
	            else
                    Engine::ReleaseSpell(SpellIndex::Q, pEnemy->GetPosition());
                QCastedTime = 0.0f;
            }
            else
            {
                float rangeCheck = rangeCharged - moveSpeed * database.XerathQ.GetCastTime() + 50.0f;

                if (rangeCharged > 1499) rangeCheck = rangeCharged;

                if (pEnemy->GetDistanceTo(globals::localPlayer) <= rangeCheck)
                {
                    if (pEnemy->IsHero())
                    {
                        Modules::prediction::PredictionOutput qPrediction;
                        if (GetPrediction(database.XerathQ, qPrediction))
                            Engine::ReleaseSpell(SpellIndex::Q, qPrediction.position);
                        QCastedTime = 0.0f;
                    }
                    else
                        Engine::ReleaseSpell(SpellIndex::Q, pEnemy->GetPosition());
						QCastedTime = 0.0f;
                }
            }

        }
    }

    void Xerath_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathW.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= database.XerathW.GetRange() && isTimeToCastW())
        {
            if (pEnemy->IsHero())
            {
                Modules::prediction::PredictionOutput wPrediction;

                if (GetPrediction(database.XerathW, wPrediction))
                {
                    Engine::CastSpell(SpellIndex::W, wPrediction.position);
                    WCastedTime = gameTime;
                }
            }
            else
            {
                Engine::CastSpell(SpellIndex::W, pEnemy->GetPosition());
                WCastedTime = gameTime;
            }
        }
    }

    void Xerath_UseE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathE.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= database.XerathE.GetRange() && isTimeToCastE())
        {
            if (pEnemy->IsHero())
            {
                Modules::prediction::PredictionOutput ePrediction;

                if (GetPrediction(database.XerathE, ePrediction))
                {
                    Engine::CastSpell(SpellIndex::E, ePrediction.position);
                    ECastedTime = gameTime;
                }
            }
            else
            {
                Engine::CastSpell(SpellIndex::E, pEnemy->GetPosition());
                ECastedTime = gameTime;
            }
        }
    }

    void Xerath_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.XerathR.IsCastable())
            return;
       
        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= database.XerathR.GetRange() && isTimeToCastR())
        {
            Modules::prediction::PredictionOutput rPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy,database.XerathR, rPrediction))
            {
                Engine::CastSpell(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }
        }
    }

	void Update() override
    {
        gameTime = Engine::GetGameTime();

        Killsteal();
        AntiGapCloser();
        if (XerathConfig::XerathUltimate::manualCastUltimateKey->Value == true)
        {
            switch (XerathConfig::XerathUltimate::targetMode->Value)
            {
            case 0: //Inherit
                if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathR.GetRange()))
                    Xerath_UseR(rTarget);
            	break;
            case 1: //NearMouse
                if (const auto rTarget2 = TargetSelector::FindBestTarget(Engine::GetMouseWorldPos(), 300.0f))
                {
	                Xerath_UseR(rTarget2);
                }
            	break;
            }
        }
    }

    void Combo() override
    {
	    if (!IsCastingR())
	    {
            if (XerathConfig::XerathCombo::UseQ->Value == true && database.XerathQ.IsCastable())
            {
                if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathQ.GetRange()))
                {
                    if (IsCastingQ())
                    {
                        Xerath_UseQ2(qTarget);
                    }
                    else
                    {
                        Xerath_UseQ(qTarget);
                    }
                }
            }

            if (!database.XerathQ.IsCastable())
            {
                if (XerathConfig::XerathCombo::UseW->Value == true && database.XerathW.IsCastable())
                {
                    if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathW.GetRange()))
                    {
                        Xerath_UseW(wTarget);
                    }
                }

                if (XerathConfig::XerathCombo::UseE->Value == true && database.XerathE.IsCastable())
                {
                    if (const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathE.GetRange()))
                    {
                        Xerath_UseE(eTarget);
                    }
                }
            }
	    }
	    else
	    {
            if (XerathConfig::XerathCombo::UseR->Value == true && database.XerathR.IsCastable())
            {
                if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathR.GetRange()))
                {
                    Xerath_UseR(rTarget);
                }
            }
	    }
        
    }

    void Clear() override
    {
        const auto minion = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(),database.XerathQ.GetRange(), Alliance::Enemy);
        if (minion && globals::localPlayer->GetPercentMana() >= XerathConfig::XerathLaneClear::minMana->Value)
        {
            if (XerathConfig::XerathLaneClear::UseQ->Value == true && database.XerathQ.IsCastable() 
                && ObjectManager::CountMinionsInRange(Alliance::Enemy, minion->GetPosition(), 300.0f) >= XerathConfig::XerathLaneClear::minQMinions->Value)
            {
                if (!IsCastingQ())
                    Xerath_UseQ(minion);
                else
                    Xerath_UseQ2(minion);
            }

            if (XerathConfig::XerathLaneClear::UseW->Value == true && database.XerathW.IsCastable() 
                && ObjectManager::CountMinionsInRange(Alliance::Enemy, minion->GetPosition(), database.XerathW.GetRadius()) >= XerathConfig::XerathLaneClear::minWMinions->Value)
            {
                Xerath_UseW(minion);
            }
        }

        const auto monster = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), database.XerathQ.GetRange());
        if (monster && globals::localPlayer->GetPercentMana() >= XerathConfig::XerathJungle::minMana->Value)
        {
            if (XerathConfig::XerathJungle::UseQ->Value == true && database.XerathQ.IsCastable())
            {
                if (!IsCastingQ())
                    Xerath_UseQ(monster);
                else
                    Xerath_UseQ2(monster);
            }

            if (XerathConfig::XerathJungle::UseW->Value == true && database.XerathW.IsCastable())
            {
                Xerath_UseW(monster);
            }

            if (XerathConfig::XerathJungle::UseE->Value == true && database.XerathE.IsCastable())
            {
                Xerath_UseE(monster);
            }
        }
    }

    void Harass() override
    {
        if (globals::localPlayer->GetPercentMana() < XerathConfig::XerathHarass::minMana->Value)
            return;

        if (XerathConfig::XerathHarass::UseQ->Value == true && database.XerathQ.IsCastable())
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathQ.GetRange()))
            {
                if (!IsCastingQ())
                    Xerath_UseQ(qTarget);
                else
                    Xerath_UseQ2(qTarget);
            }
        }

        if (XerathConfig::XerathHarass::UseW->Value == true && database.XerathW.IsCastable())
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathW.GetRange()))
            {
                Xerath_UseW(wTarget);
            }
        }

        if (XerathConfig::XerathHarass::UseE->Value == true && database.XerathE.IsCastable())
        {
            if (const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathE.GetRange()))
            {
                Xerath_UseE(eTarget);
            }
        }
    }

    void Lasthit() override
    {
        if (globals::localPlayer->GetPercentMana() < XerathConfig::XerathLastHit::minMana->Value)
            return;

        if (XerathConfig::XerathLastHit::UseW->Value == true && database.XerathW.IsCastable())
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathW.GetRange()); Xerath_dmgW(wTarget) > wTarget->ReadClientStat(Object::Health))
                Xerath_UseW(wTarget);
        }

        if (XerathConfig::XerathLastHit::UseE->Value == true && database.XerathE.IsCastable())
        {
            if (const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathE.GetRange()); Xerath_dmgE(eTarget) > eTarget->ReadClientStat(Object::Health))
                Xerath_UseE(eTarget);
        }
    }

    void Flee() override
    {
        if (XerathConfig::XerathFlee::UseE->Value == true && database.XerathE.IsCastable())
        {
            if (const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathE.GetRange()))
            {
                Xerath_UseE(eTarget);
            }
        }
    }

    void Killsteal()
    {
	    if (!IsCastingR())
	    {
            if (XerathConfig::XerathKillsteal::UseQ->Value == true && database.XerathQ.IsCastable())
            {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathQ.GetRange());
                if (qTarget && qTarget->ReadClientStat(Object::Health) < Xerath_dmgQ(qTarget))
                {
                    if (!IsCastingQ())
                    {
                        Xerath_UseQ(qTarget);
                    }
                    else
                    {
                        Xerath_UseQ2(qTarget);
                    }
                }
            }

            if (XerathConfig::XerathKillsteal::UseE->Value == true && database.XerathE.IsCastable())
            {
                const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathE.GetRange());
                if (eTarget && eTarget->ReadClientStat(Object::Health) < Xerath_dmgE(eTarget))
                {
                    Xerath_UseE(eTarget);
                }
            }

            if (XerathConfig::XerathKillsteal::UseW->Value == true && database.XerathW.IsCastable())
            {
                const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),database.XerathW.GetRange());
                if (wTarget && wTarget->ReadClientStat(Object::Health) < Xerath_dmgW(wTarget))
                {
                    Xerath_UseW(wTarget);
                }
            }
	    }
    }

    void AntiGapCloser()
    {
        if (XerathConfig::XerathAntiGapCloser::UseE->Value == true && database.XerathE.IsCastable())
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > database.XerathE.GetRange()) continue;
         
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, database.XerathE.GetRange()))
                        Xerath_UseE(target);
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

    void OnBeforeAttack() override {
        return;
    }

    void OnAfterAttack() override {
        return;
    }

    void Render() override
    {
        if (XerathConfig::XerathDrawings::DrawQ->Value == true && (XerathConfig::XerathDrawings::DrawIfReady->Value == true && database.XerathQ.IsCastable() || XerathConfig::XerathDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), max(database.XerathQ.GetRange(),Xerath_QRange()), COLOR_WHITE, 1.0f);
        if (XerathConfig::XerathDrawings::DrawE->Value == true && (XerathConfig::XerathDrawings::DrawIfReady->Value == true && database.XerathE.IsCastable() || XerathConfig::XerathDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.XerathE.GetRange(), COLOR_WHITE, 1.0f);
        if (XerathConfig::XerathDrawings::DrawR->Value == true && (XerathConfig::XerathDrawings::DrawIfReady->Value == true && database.XerathR.IsCastable() || XerathConfig::XerathDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), database.XerathR.GetRange(), COLOR_WHITE, 1.0f);
        if (IsCastingR() && XerathConfig::XerathUltimate::targetMode->Value == 1)
            Awareness::Functions::Radius::DrawRadius(Engine::GetMouseWorldPos(), 300.0f, COLOR_WHITE, 1.0f);

    }
};

XerathModule module;