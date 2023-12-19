#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Nautilus.h"

class NautilusModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Nautilus");
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

    void Initialize() override
    {
        const auto NautilusMenu = Menu::CreateMenu("vezNautilus", "vez.Nautilus");

        const auto combo = NautilusMenu->AddMenu("Combo Settings", "Combo Settings");
        NautilusConfig::NautilusCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        NautilusConfig::NautilusCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
        NautilusConfig::NautilusCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);

        NautilusConfig::NautilusCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);
        NautilusConfig::NautilusCombo::engageModeKey = combo->AddKeyBind("engageModeKey", "Engage Mode Key", VK_CONTROL, false, true);

        const auto harassMenu = NautilusMenu->AddMenu("Harass Settings", "Harass Settings");
        NautilusConfig::NautilusHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusHarass::UseQDive = harassMenu->AddCheckBox("Use Q Dive", "Use SpellSlot Q Under Enemy Turret", false);
        NautilusConfig::NautilusHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        NautilusConfig::NautilusHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = NautilusMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        NautilusConfig::NautilusJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        NautilusConfig::NautilusJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

    	const auto additionalMenu = NautilusMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        NautilusConfig::NautilusKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        NautilusConfig::NautilusKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        NautilusConfig::NautilusAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto whitelistMenu = additionalMenu->AddMenu("Whitelist Settings", "Whitelist Settings");
        const auto qWhitelistMenu = whitelistMenu->AddMenu("Q Whitelist Settings", "Q Whitelist Settings");
        const auto rWhitelistMenu = whitelistMenu->AddMenu("R Whitelist Settings", "R Whitelist Settings");

        for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++)
        {
            auto obj = ObjectManager::GetHeroList()->GetIndex(i);
            if (obj != nullptr && obj->IsEnemy())
            {
                const auto q_checkbox = qWhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj](const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !NautilusConfig::q_whitelist.empty())
                        {
	                        const auto it = std::ranges::find(NautilusConfig::q_whitelist, obj);
                            NautilusConfig::q_whitelist.erase(it);
                        }
                        else
                        {
                            NautilusConfig::q_whitelist.push_back(obj);
                        }
                    });

                if (q_checkbox->Value == true)
                {
                    NautilusConfig::q_whitelist.push_back(obj);
                }

                const auto r_checkbox = rWhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !NautilusConfig::r_whitelist.empty())
                        {
	                        const auto it = std::ranges::find(NautilusConfig::r_whitelist, obj);
                            NautilusConfig::r_whitelist.erase(it);
                        }
                        else
                        {
                            NautilusConfig::r_whitelist.push_back(obj);
                        }
                    });

                if (r_checkbox->Value == true)
                {
                    NautilusConfig::r_whitelist.push_back(obj);
                }
            }
        }
       
        const auto interrupterMenu = additionalMenu->AddMenu("Interrupter Settings", "Interrupter Settings");
        NautilusConfig::NautilusInterrupter::UseQ = interrupterMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        NautilusConfig::NautilusInterrupter::UseR = interrupterMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        NautilusConfig::NautilusFlee::UseQ = fleeMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);

        const auto drawMenu = NautilusMenu->AddMenu("Drawings Settings", "Drawings Settings");
        NautilusConfig::NautilusDrawings::DrawQ = drawMenu->AddCheckBox("Draw Q", "Draw SpellSlot Q", true);
        NautilusConfig::NautilusDrawings::DrawE = drawMenu->AddCheckBox("Draw E", "Draw SpellSlot E", true);
        NautilusConfig::NautilusDrawings::DrawR = drawMenu->AddCheckBox("Draw R", "Draw SpellSlot R", true);
        NautilusConfig::NautilusDrawings::DrawIfReady = drawMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Nautilus_dmgQ(const Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusQ.IsCastable())
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = NautilusDamages::QSpell::dmgSkillQ[levelSpell - 1];

        const float abilityPowerDamage = ObjectManager::GetLocalPlayer()->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::QSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    static float Nautilus_dmgW(const Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusW.IsCastable())
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillDamage = NautilusDamages::WSpell::dmgSkillW[levelSpell - 1];

        const float abilityPowerDamage = ObjectManager::GetLocalPlayer()->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::WSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    static float Nautilus_dmgE(const Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusE.IsCastable())
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::E)->GetLevel();
        const float skillDamage = NautilusDamages::ESpell::dmgSkillE[levelSpell - 1];

        const float abilityPowerDamage = ObjectManager::GetLocalPlayer()->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::ESpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    static float Nautilus_dmgR(const Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusR.IsCastable())
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float skillDamage = NautilusDamages::RSpell::dmgSkillR[levelSpell - 1];

        const float abilityPowerDamage = ObjectManager::GetLocalPlayer()->GetAbilityPower();
        const float additionalSkillDamage = NautilusDamages::RSpell::additionalPercentageAP;
        const float totalDamage = skillDamage + (additionalSkillDamage * abilityPowerDamage);

        return totalDamage;
    }

    void Nautilus_UseQ(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusQ.IsCastable())
            return;

        if (pEnemy && gameTime > QCastedTime + database.NautilusQ.GetCastTime())
        {
            if (Engine::MenuItemContains(NautilusConfig::q_whitelist, pEnemy->GetName().c_str()))
            {
                Modules::prediction::PredictionOutput qPrediction;

                if (GetPrediction(database.NautilusQ, qPrediction))
                {
                    Engine::CastToPosition(SpellIndex::Q, qPrediction.position);
                    QCastedTime = gameTime;
                }
            }
        }
    }

    void Nautilus_UseW(const Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusW.IsCastable())
            return;

        if (pEnemy && gameTime > WCastedTime + database.NautilusW.GetCastTime())
        {
        	Engine::CastSelf(SpellIndex::W);
            WCastedTime = gameTime;
        }
    }

    void Nautilus_UseE(const Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusE.IsCastable())
            return;

        if (pEnemy && gameTime > ECastedTime + database.NautilusE.GetCastTime())
        {
            Engine::CastSelf(SpellIndex::E);
            ECastedTime = gameTime;
        }
    }

    void Nautilus_UseR(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !database.NautilusR.IsCastable())
            return;

        if (pEnemy && gameTime > RCastedTime + database.NautilusR.GetCastTime())
        {
            if (Engine::MenuItemContains(NautilusConfig::r_whitelist, pEnemy->GetName().c_str()))
            {
                Engine::CastTargeted(SpellIndex::R, pEnemy);
                RCastedTime = gameTime;
            }
        }
    }

    void Update() override
    {
        gameTime = Engine::GetGameTime();

        Killsteal();
        AntiGapCloser();
    }

    void Combo() override
    {
        if (NautilusConfig::NautilusCombo::engageModeKey->Value == true)
        {
            if (NautilusConfig::NautilusCombo::UseR->Value == true && database.NautilusR.IsCastable())
            {
	            if (const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusR.GetRange()))
                {
                    Nautilus_UseR(rTarget);
                }
            }

            if (const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusQ.GetRange()))
            {
                if (!qTarget->CanMove() && database.NautilusQ.IsCastable())
                {
                    Nautilus_UseQ(qTarget);
                }

                if (qTarget->IsInAARange())
                {
	                if (database.NautilusW.IsCastable())
                        Nautilus_UseW(qTarget);

                    if (database.NautilusE.IsCastable())
                        Nautilus_UseE(qTarget);
                }
            }
        }
        else
        {
            if (NautilusConfig::NautilusCombo::UseQ->Value == true && database.NautilusQ.IsCastable())
            {
	            if (const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusQ.GetRange()))
                {
                    Nautilus_UseQ(qTarget);
                }
            }

            if (!database.NautilusQ.IsCastable() )
            {
                if (NautilusConfig::NautilusCombo::UseW->Value == true && database.NautilusW.IsCastable())
                {
	                if (const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusW.GetRange()))
                    {
                        Nautilus_UseW(wTarget);
                    }
                }

                if (NautilusConfig::NautilusCombo::UseE->Value == true && database.NautilusE.IsCastable())
                {
	                if (const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusE.GetRange()))
                    {
                        Nautilus_UseE(eTarget);
                    }
                }

                if (NautilusConfig::NautilusCombo::UseR->Value == true && database.NautilusR.IsCastable())
                {
	                if (const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusR.GetRange()))
                    {
                        Nautilus_UseR(rTarget);
                    }
                }
            }
            
        }
        
    }

    void Clear() override
    {
        if (ObjectManager::GetLocalPlayer()->GetPercentMana() < NautilusConfig::NautilusJungle::minMana->Value)
            return;

        if (NautilusConfig::NautilusJungle::UseQ->Value == true && database.NautilusQ.IsCastable())
        {
	        const auto qMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), database.NautilusQ.GetRange());
            if (qMonster && gameTime > QCastedTime + database.NautilusQ.GetCastTime())
            {
                Engine::CastToPosition(SpellIndex::Q, qMonster->GetPosition());
                QCastedTime = gameTime;
            }
        }
        
        if (NautilusConfig::NautilusJungle::UseW->Value == true && database.NautilusW.IsCastable())
        {
	        if (const auto wMonster = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), database.NautilusW.GetRange()))
                Nautilus_UseW(wMonster);
        }
    }

    void Harass() override
    {
        if (ObjectManager::GetLocalPlayer()->GetPercentMana() < NautilusConfig::NautilusHarass::minMana->Value)
            return;
        

        if (NautilusConfig::NautilusHarass::UseQ->Value == true && database.NautilusQ.IsCastable())
        {
	        if (const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusQ.GetRange()))
            {
                if (NautilusConfig::NautilusHarass::UseQDive->Value == true && qTarget->IsUnderTower(Alliance::Enemy) || !qTarget->IsUnderTower(Alliance::Enemy))
					Nautilus_UseQ(qTarget);
            }
        }

        if (NautilusConfig::NautilusHarass::UseE->Value == true && database.NautilusE.IsCastable())
        {
	        if (const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusE.GetRange()))
            {
                Nautilus_UseE(eTarget);
            }
        }
    }

    void Lasthit() override {
        return;
    }

    void Flee() override
    {
        if (NautilusConfig::NautilusFlee::UseQ->Value == true && database.NautilusQ.IsCastable())
        {
	        if (const auto mousePos = Engine::GetMouseWorldPos(); 
                Engine::IsWall(mousePos) 
                && mousePos.Distance(ObjectManager::GetLocalPlayer()->GetPosition()) <= database.NautilusQ.GetRange())
	        {
                Engine::CastToPosition(SpellIndex::Q, Engine::GetMouseWorldPos());
                QCastedTime = gameTime;
	        }
        }
    }

    void Killsteal()
    {

        if (NautilusConfig::NautilusKillsteal::UseQ->Value == true && database.NautilusQ.IsCastable())
        {
	        const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusQ.GetRange());
            if (qTarget && qTarget->GetHealth() < Nautilus_dmgQ(qTarget))
            {
                Nautilus_UseQ(qTarget);
            }
        }

        if (NautilusConfig::NautilusKillsteal::UseE->Value == true && database.NautilusE.IsCastable())
        {
	        const auto eTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusE.GetRange());
            if (eTarget && eTarget->GetHealth() < Nautilus_dmgE(eTarget))
            {
                Nautilus_UseE(eTarget);
            }
        }

        if (NautilusConfig::NautilusKillsteal::UseR->Value == true && database.NautilusR.IsCastable())
        {
	        const auto rTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(),database.NautilusR.GetRange());
            if (rTarget && rTarget->GetHealth() < Nautilus_dmgR(rTarget))
            {
                Nautilus_UseR(rTarget);
            }
        }
    }

    void AntiGapCloser()
    {
        if (NautilusConfig::NautilusAntiGapCloser::UseE->Value == true && database.NautilusE.IsCastable())
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(ObjectManager::GetLocalPlayer()->GetPosition()) > database.NautilusE.GetRange()) continue;
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && ObjectManager::GetLocalPlayer()->GetPosition().distanceTo(pathEnd) < database.NautilusE.GetRange())
                        Nautilus_UseE(target);
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
        if (NautilusConfig::NautilusCombo::engageModeKey->Value == true)
        {
            Vector2 screenPos = Engine::WorldToScreen(ObjectManager::GetLocalPlayer()->GetPosition());
            render::RenderText("Engage Mode: ON", screenPos.ToImVec(), 18.0f, COLOR_RED, true);
        }

        if (NautilusConfig::NautilusDrawings::DrawQ->Value == true && (NautilusConfig::NautilusDrawings::DrawIfReady->Value == true && database.NautilusQ.IsCastable() || NautilusConfig::NautilusDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), database.NautilusQ.GetRange(), COLOR_WHITE, 1.0f);
        if (NautilusConfig::NautilusDrawings::DrawE->Value == true && (NautilusConfig::NautilusDrawings::DrawIfReady->Value == true && database.NautilusE.IsCastable() || NautilusConfig::NautilusDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), database.NautilusE.GetRange(), COLOR_WHITE, 1.0f);
        if (NautilusConfig::NautilusDrawings::DrawR->Value == true && (NautilusConfig::NautilusDrawings::DrawIfReady->Value == true && database.NautilusR.IsCastable() || NautilusConfig::NautilusDrawings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), database.NautilusR.GetRange(), COLOR_WHITE, 1.0f);
    }
};

NautilusModule module;