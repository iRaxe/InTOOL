#include "Nautilus.h"

#include "Awareness.h"
#include "stdafx.h"
#include "TargetSelector.h"
#include "Orbwalker.h"

class NautilusModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Nautilus");

	Skillshot q = SkillshotManager::RegisterSpell(
        name,
        SpellIndex::Q,
        Skillshot(
            NautilusDamages::QSpell::range,
            NautilusDamages::QSpell::width,
            NautilusDamages::QSpell::speed,
            NautilusDamages::QSpell::delay,
            SkillshotType::SkillshotLine,
            { CollidableObjects::Objects, CollidableObjects::Walls }));

    Skillshot w = SkillshotManager::RegisterSpell(
        name,
        SpellIndex::W,
        Skillshot(
            NautilusDamages::ESpell::range,
            NautilusDamages::ESpell::range,
            NautilusDamages::ESpell::speed,
            NautilusDamages::ESpell::delay,
            SkillshotType::SkillshotNone));

    Skillshot e = SkillshotManager::RegisterSpell(
        name,
        SpellIndex::E,
        Skillshot(
            NautilusDamages::ESpell::range,
            NautilusDamages::ESpell::range,
            NautilusDamages::ESpell::speed,
            NautilusDamages::ESpell::delay,
            SkillshotType::SkillshotNone));

    Skillshot r = SkillshotManager::RegisterSpell(
        name,
        SpellIndex::R,
        Skillshot(
            NautilusDamages::RSpell::range,
            NautilusDamages::RSpell::range,
            NautilusDamages::RSpell::speed,
            NautilusDamages::RSpell::delay,
            SkillshotType::SkillshotNone));

private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

private:
    /*NautilusPassive for passivemanager
	nautiluspassivecheck once hit
	nautiluspassiveroot while rooted
	--Q--
	nautilusanchordragroot
	nautilusanchordragglobalroot
	--W--
	nautiluspiercinggazeshield 
	--E--
	nautiliussplashzoneslow
	--R--
	nautiliusgrandlinetarget once targetted
	nautiliusknockup*/

    bool TargettedByR(Object* pEnemy)
    {
        if (pEnemy->GetBuffByName("nautiliusgrandlinetarget"))
            return true;

        return false;
    }

public:

    NautilusModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Init() override
    {
        auto NautilusMenu = Menu::CreateMenu("vezNautilus", "vez.Nautilus");

        auto combo = NautilusMenu->AddMenu("Combo Settings", "Combo Settings");
        NautilusConfig::NautilusCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        NautilusConfig::NautilusCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
        NautilusConfig::NautilusCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);

        NautilusConfig::NautilusCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);
        NautilusConfig::NautilusCombo::engageModeKey = combo->AddKeyBind("engageModeKey", "Engage Mode Key", VK_CONTROL, false, true);

    	auto harassMenu = NautilusMenu->AddMenu("Harass Settings", "Harass Settings");
        NautilusConfig::NautilusHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusHarass::UseQDive = harassMenu->AddCheckBox("Use Q Dive", "Use SpellSlot Q Under Enemy Turret", false);
        NautilusConfig::NautilusHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        NautilusConfig::NautilusHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        auto jungleMenu = NautilusMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        NautilusConfig::NautilusJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        NautilusConfig::NautilusJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        auto ksMenu = NautilusMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        NautilusConfig::NautilusKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        NautilusConfig::NautilusKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

    	auto antiGapMenu = NautilusMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        NautilusConfig::NautilusAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        auto interrupterMenu = NautilusMenu->AddMenu("Interrupter Settings", "Interrupter Settings");
        NautilusConfig::NautilusInterrupter::UseQ = interrupterMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusInterrupter::UseR = interrupterMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        auto fleeMenu = NautilusMenu->AddMenu("Flee Settings", "Flee Settings");
        NautilusConfig::NautilusFlee::UseQ = fleeMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);

    	auto drawMenu = NautilusMenu->AddMenu("Drawings Settings", "Drawings Settings");
        NautilusConfig::NautilusDrawings::DrawQ = drawMenu->AddCheckBox("Draw Q", "Draw SpellSlot Q", true);
        NautilusConfig::NautilusDrawings::DrawE = drawMenu->AddCheckBox("Draw E", "Draw SpellSlot E", true);
        NautilusConfig::NautilusDrawings::DrawR = drawMenu->AddCheckBox("Draw R", "Draw SpellSlot R", true);
        NautilusConfig::NautilusDrawings::DrawIfReady = drawMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    float Nautilus_dmgQ(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !q.IsCastable())
            return -9999;

        int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = NautilusDamages::QSpell::dmgSkillQ[levelSpell];

        float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::QSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    float Nautilus_dmgW(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !w.IsCastable())
            return -9999;

        int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillDamage = NautilusDamages::WSpell::dmgSkillW[levelSpell];

        float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::WSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    float Nautilus_dmgE(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !e.IsCastable())
            return -9999;

        int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetLevel();
        const float skillDamage = NautilusDamages::ESpell::dmgSkillE[levelSpell];

        float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::ESpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    float Nautilus_dmgR(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy || !r.IsCastable())
            return -9999;

        int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float skillDamage = NautilusDamages::RSpell::dmgSkillR[levelSpell];

        float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::RSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    void Nautilus_UseQ(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && gameTime > QCastedTime + q.GetCastTime())
        {
            prediction::PredictionOutput qPrediction;

            if (GetPrediction(q, qPrediction))
            {
                functions::CastSpell(SpellIndex::Q, qPrediction.position);
                QCastedTime = gameTime;
            }
        }
    }

    void Nautilus_UseW(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && gameTime > WCastedTime + w.GetCastTime())
        {
        	functions::CastSpell(SpellIndex::W);
            WCastedTime = gameTime;
        }
    }

    void Nautilus_UseE(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && gameTime > ECastedTime + e.GetCastTime())
        {
            functions::CastSpell(SpellIndex::E);
            ECastedTime = gameTime;
        }
    }

    void Nautilus_UseR(Object* pEnemy)
    {
        if (!globals::localPlayer || !pEnemy)
            return;

        if (pEnemy && gameTime > RCastedTime + r.GetCastTime())
        {
            functions::CastSpell(SpellIndex::R, pEnemy);
            RCastedTime = gameTime;
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
        if (NautilusConfig::NautilusCombo::engageModeKey->Value == true)
        {
            if (NautilusConfig::NautilusCombo::UseR->Value == true && r.IsCastable())
            {
                auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(r.GetRange());
                if (rTarget)
                {
                    Nautilus_UseR(rTarget);
                }
            }

            auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(q.GetRange());
            if (qTarget)
            {
                if (!qTarget->CanMove() && q.IsCastable())
                {
                    Nautilus_UseQ(qTarget);
                }

                if (qTarget->IsInAARange())
                {
	                if (w.IsCastable())
                        Nautilus_UseW(qTarget);

                    if (e.IsCastable())
                        Nautilus_UseE(qTarget);
                }
            }
        }
        else
        {
            if (NautilusConfig::NautilusCombo::UseQ->Value == true && q.IsCastable())
            {
                auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(q.GetRange());
                if (qTarget)
                {
                    Nautilus_UseQ(qTarget);
                }
            }

            if (!q.IsCastable() )
            {
                if (NautilusConfig::NautilusCombo::UseW->Value == true && w.IsCastable())
                {
                    auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(w.GetRange());
                    if (wTarget)
                    {
                        Nautilus_UseW(wTarget);
                    }
                }

                if (NautilusConfig::NautilusCombo::UseE->Value == true && e.IsCastable())
                {
                    auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(e.GetRange());
                    if (eTarget)
                    {
                        Nautilus_UseE(eTarget);
                    }
                }

                if (NautilusConfig::NautilusCombo::UseR->Value == true && r.IsCastable())
                {
                    auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(r.GetRange());
                    if (rTarget)
                    {
                        Nautilus_UseR(rTarget);
                    }
                }
            }
            
        }
        
    }

    void Clear() override
    {
        if (globals::localPlayer->GetPercentMana() < NautilusConfig::NautilusJungle::minMana->Value)
            return;

        if (NautilusConfig::NautilusJungle::UseQ->Value == true && q.IsCastable())
        {
            auto qMonster = TargetSelector::Functions::GetJungleInRange(q.GetRange());
            if (qMonster && gameTime > QCastedTime + q.GetCastTime())
            {
                functions::CastSpell(SpellIndex::Q, qMonster->GetPosition());
                QCastedTime = gameTime;
            }
        }
        
        if (NautilusConfig::NautilusJungle::UseW->Value == true && w.IsCastable())
        {
            auto wMonster = TargetSelector::Functions::GetJungleInRange(w.GetRange());
            if (wMonster)
                Nautilus_UseW(wMonster);
        }
    }

    void Harass() override
    {
        if (globals::localPlayer->GetPercentMana() < NautilusConfig::NautilusHarass::minMana->Value)
            return;
        

        if (NautilusConfig::NautilusHarass::UseQ->Value == true && q.IsCastable())
        {
            auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(q.GetRange());
            if (qTarget)
            {
                if (NautilusConfig::NautilusHarass::UseQDive->Value == true && qTarget->IsUnderEnemyTower() || !qTarget->IsUnderEnemyTower())
					Nautilus_UseQ(qTarget);
            }
        }

        if (NautilusConfig::NautilusHarass::UseE->Value == true && e.IsCastable())
        {
            auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(e.GetRange());
            if (eTarget)
            {
                Nautilus_UseE(eTarget);
            }
        }
    }

    void Lasthit() override
    {

    }

    void Killsteal()
    {

        if (NautilusConfig::NautilusKillsteal::UseQ->Value == true && q.IsCastable())
        {
            auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(q.GetRange());
            if (qTarget && qTarget->GetHealth() < Nautilus_dmgQ(qTarget))
            {
                Nautilus_UseQ(qTarget);
            }
        }

        if (NautilusConfig::NautilusKillsteal::UseE->Value == true && e.IsCastable())
        {
            auto eTarget = TargetSelector::Functions::GetEnemyChampionInRange(e.GetRange());
            if (eTarget && eTarget->GetHealth() < Nautilus_dmgE(eTarget))
            {
                Nautilus_UseE(eTarget);
            }
        }

        if (NautilusConfig::NautilusKillsteal::UseR->Value == true && r.IsCastable())
        {
            auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(r.GetRange());
            if (rTarget && rTarget->GetHealth() < Nautilus_dmgR(rTarget))
            {
                Nautilus_UseR(rTarget);
            }
        }
    }

    void AntiGapCloser()
    {
        if (NautilusConfig::NautilusAntiGapCloser::UseE->Value == true && e.IsCastable())
        {
            for (auto target : TargetSelector::Functions::GetTargetsInRange(e.GetRange()))
            {
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, e.GetRange()))
                        Nautilus_UseE(target);
                }
            }
        }
    }

    void Render() override
    {
        if (NautilusConfig::NautilusCombo::engageModeKey->Value == true)
        {
            Vector2 screenPos = functions::WorldToScreen(globals::localPlayer->GetPosition());
            render::RenderText("Engage Mode: ON", screenPos.ToImVec(), 18.0f, COLOR_RED, true);
        }

        if (NautilusConfig::NautilusDrawings::DrawQ->Value == true && (NautilusConfig::NautilusDrawings::DrawIfReady->Value == true && q.IsCastable() || NautilusConfig::NautilusDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), q.GetRange(), COLOR_WHITE, 1.0f);
        if (NautilusConfig::NautilusDrawings::DrawE->Value == true && (NautilusConfig::NautilusDrawings::DrawIfReady->Value == true && e.IsCastable() || NautilusConfig::NautilusDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), e.GetRange(), COLOR_WHITE, 1.0f);
        if (NautilusConfig::NautilusDrawings::DrawR->Value == true && (NautilusConfig::NautilusDrawings::DrawIfReady->Value == true && r.IsCastable() || NautilusConfig::NautilusDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), r.GetRange(), COLOR_WHITE, 1.0f);
    }
};

NautilusModule module;