#include "Ashe.h"

#include "Awareness.h"
#include "Orbwalker.h"
#include "stdafx.h"
#include "TargetSelector.h"

class AsheModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Ashe");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

    static bool Ashe_IsQReady()
    {
        return globals::localPlayer->GetBuffByName("asheqcastready");
    }

    static bool Ashe_IsCastingQ()
    {
        return globals::localPlayer->GetBuffByName("AsheQAttack");
    }

    static bool Ashe_TargetSlowed(Object* pEnemy)
    {
        return pEnemy != nullptr && pEnemy->GetBuffByName("ashepassiveslow");
    }

    static float Ashe_speedR(float distance)
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

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.AsheW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastW() const
    {
        return gameTime > WCastedTime + database.AsheW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + 0.25f;
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.AsheR.GetCastTime();
    }

    static float aaRange()
    {
        return globals::localPlayer->GetRealAttackRange();
    }

    static float wRange()
    {
        return AsheConfig::AsheSpellsSettings::wRange->Value;
    }

    static float rRange()
    {
        return AsheConfig::AsheSpellsSettings::maxRDistance->Value;
    }

public:

    AsheModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void OnPopuplateMenu() override
    {

    }

    void Init() override
    {
        const auto AsheMenu = Menu::CreateMenu("vezAshe", "Champion\n  Settings");

        const auto combo = AsheMenu->AddMenu("Combo Settings", "Combo Settings");
        AsheConfig::AsheCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        AsheConfig::AsheCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = AsheMenu->AddMenu("Harass Settings", "Harass Settings");
        AsheConfig::AsheHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = AsheMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        AsheConfig::AsheClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheClear::minWMinions = laneClearMenu->AddSlider("minWMinions", "Minimum Minions in W Width", 3, 1, 6, 1);
        AsheConfig::AsheClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        AsheConfig::AsheJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        AsheConfig::AsheJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        AsheConfig::AsheLastHit::UseW = lastHitMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = AsheMenu->AddMenu("Additional Settings", "Additional Settings");
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        AsheConfig::AsheKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        AsheConfig::AsheKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        AsheConfig::AsheAntiGapCloser::UseW = antiGapMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
        AsheConfig::AsheAntiGapCloser::UseR = antiGapMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

        const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
        AsheConfig::AsheAntiMelee::UseW = antiMeleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

        for (int i = 0; i < globals::heroManager->GetListSize(); i++)
        {
            auto obj = globals::heroManager->GetIndex(i);
            if (obj != nullptr && obj->IsEnemy())
            {
                const auto antiGap_checkbox = antiGapMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj](const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !AsheConfig::AsheAntiGapCloser::whitelist.empty())
                        {
                            const auto it = std::ranges::find(AsheConfig::AsheAntiGapCloser::whitelist, obj);
                            AsheConfig::AsheAntiGapCloser::whitelist.erase(it);
                        }
                        else
                        {
                            AsheConfig::AsheAntiGapCloser::whitelist.push_back(obj);
                        }
                    });

                if (antiGap_checkbox->Value == true)
                {
                    AsheConfig::AsheAntiGapCloser::whitelist.push_back(obj);
                }

                if (!obj->IsMelee()) continue;
                const auto antiMelee_checkbox = antiMeleeMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !AsheConfig::AsheAntiMelee::whitelist.empty())
                        {
                            const auto it = std::ranges::find(AsheConfig::AsheAntiMelee::whitelist, obj);
                            AsheConfig::AsheAntiMelee::whitelist.erase(it);
                        }
                        else
                        {
                            AsheConfig::AsheAntiMelee::whitelist.push_back(obj);
                        }
                    });

                if (antiMelee_checkbox->Value == true)
                {
                    AsheConfig::AsheAntiMelee::whitelist.push_back(obj);
                }
            }
        }

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        AsheConfig::AsheFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use SpellSlot W", false);

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");
        AsheConfig::AsheSpellsSettings::saveMana = spellsMenu->AddCheckBox("saveManaForCombo", "Save Mana For A Full Combo Rotation", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        AsheConfig::AsheSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
      
        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        AsheConfig::AsheSpellsSettings::wCastMode = wSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        AsheConfig::AsheSpellsSettings::UseWIfInAARange = wSpellMenu->AddCheckBox("Use W In AA Range", "Use SpellSlot W In AA Range", true);
        AsheConfig::AsheSpellsSettings::UseWIfFullAASpeed = wSpellMenu->AddCheckBox("UseWIfFullAASpeed", "Use SpellSlot W If Full AA Speed", true);

    	AsheConfig::AsheSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.AsheW.GetRange(), 100, database.AsheW.GetRange(), 50);
        AsheConfig::AsheSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        AsheConfig::AsheSpellsSettings::targetMode = rSpellMenu->AddList("targetMode", "Target Mode", std::vector<std::string>{"Inherit", "Near Mouse", "Near Champion"}, 0);
        AsheConfig::AsheSpellsSettings::rTapKey = rSpellMenu->AddKeyBind("rTapKey", "Aim SpellSlot R Key", VK_CONTROL, false, false);
        AsheConfig::AsheSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "Minimum Distance", 1000, 100, database.AsheR.GetRange(), 100);
        AsheConfig::AsheSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "Maximum Distance", 3000, 100, database.AsheR.GetRange(), 100);

        AsheConfig::AsheSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static bool Ashe_CanCastW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.AsheW.IsCastable())
            return false;

        return pEnemy != nullptr && (AsheConfig::AsheSpellsSettings::UseWIfInAARange->Value == true && pEnemy->IsInAARange() || AsheConfig::AsheSpellsSettings::UseWIfInAARange->Value == false && !pEnemy->IsInAARange());
    }

    static float Ashe_dmgW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.AsheW.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillDamage = AsheDamages::WSpell::dmgSkillArray[levelSpell - 1];

        const float attackDamage = globals::localPlayer->GetAttackDamage();
        const float additionalAttackDamageSkillDamage = AsheDamages::WSpell::additionalPercentageAD;

        const float totalDamage = skillDamage + (attackDamage * additionalAttackDamageSkillDamage);
        return Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, totalDamage);
    }

    static float Ashe_dmgR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.AsheR.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float skillDamage = AsheDamages::RSpell::dmgSkillArray[levelSpell - 1];

        const float abilityPowerDamage = globals::localPlayer->GetAbilityPower();
        const float additionalAbilityPowerSkillDamage = AsheDamages::RSpell::additionalPercentageAP;

        const float totalDamage = skillDamage + (additionalAbilityPowerSkillDamage * abilityPowerDamage);
        return Damage::CalculateMagicalDamage(globals::localPlayer, pEnemy, totalDamage);
    }

    static bool Ashe_HasEnoughComboMana()
    {
        if (globals::localPlayer == nullptr)
            return false;

        const float qSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetManaCost();
        const float wSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetManaCost();
        const float rSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetManaCost();

        if (globals::localPlayer->GetLevel() >= 6)
            return globals::localPlayer->GetMana() > qSpellManaCost + wSpellManaCost + rSpellManaCost;

        return globals::localPlayer->GetMana() > qSpellManaCost + wSpellManaCost;
    }

    static bool Ashe_HasEnoughMana(float minValue)
    {
        if (globals::localPlayer == nullptr || AsheConfig::AsheSpellsSettings::saveMana->Value && !Ashe_HasEnoughComboMana())
            return false;

        return globals::localPlayer->GetPercentMana() > minValue;
    }

    void Ashe_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !Ashe_IsQReady())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < aaRange() && isTimeToCastQ())
        {
            functions::CastSpell(SpellIndex::Q, globals::localPlayer);
            QCastedTime = gameTime;
        }
    }

    void Ashe_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.AsheW.IsCastable() || AsheConfig::AsheSpellsSettings::UseWIfFullAASpeed->Value && globals::localPlayer->GetAttackSpeed() > 2.0f)
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < wRange() && isTimeToCastW())
        {
            prediction::PredictionOutput wPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.AsheW, wPrediction))
            {
                functions::CastSpell(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void Ashe_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.AsheR.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < rRange() && isTimeToCastR())
        {
            prediction::PredictionOutput rPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.AsheR, rPrediction))
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
        AntiMelee();

        if (AsheConfig::AsheSpellsSettings::rTapKey->Value == true && database.AsheR.IsCastable())
        {
            switch (AsheConfig::AsheSpellsSettings::targetMode->Value)
            {
            case 0: //Inherit
                if (const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(rRange()))
                    Ashe_UseR(rTarget);
                break;
            case 1: //NearMouse
                if (const auto rTarget2 = TargetSelector::Functions::GetEnemyChampionInRange(functions::GetMouseWorldPos(), 300.0f))
                    Ashe_UseR(rTarget2);
                break;
            case 2: //NearChampion
                if (const auto rTarget3 = TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetPosition(), 500.0f))
                    Ashe_UseR(rTarget3);
                break;
            }
        }
    }

    void Attack() override
    {
        if (AsheConfig::AsheCombo::UseR->Value == true && database.AsheR.IsCastable() && TargetSelector::Functions::GetTargetsInRange(globals::localPlayer->GetPosition(), AsheConfig::AsheSpellsSettings::minRDistance->Value).size() >= AsheConfig::AsheCombo::enemiesInRange->Value)
        {
            if (const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(rRange()))
                Ashe_UseR(rTarget);
        }

        if (AsheConfig::AsheCombo::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
        {
            const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(wRange());
            if (wTarget != nullptr && Ashe_CanCastW(wTarget))
            	Ashe_UseW(wTarget);
        }

        if (AsheConfig::AsheCombo::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(aaRange()))
                Ashe_UseQ(qTarget);
        }
    }

    void Clear() override
    {
        //Laneclear
        if (TargetSelector::Functions::GetMinionsInRange(globals::localPlayer->GetPosition(), wRange()).size() > 0)
        {
            if (!Ashe_HasEnoughMana(AsheConfig::AsheClear::minMana->Value)) return;

            if (AsheConfig::AsheClear::UseW->Value == true && database.AsheW.IsCastable() && TargetSelector::Functions::GetMinionsInRange(globals::localPlayer->GetPosition(), wRange()).size() >= AsheConfig::AsheClear::minWMinions->Value && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
            {
                const auto wTarget = TargetSelector::Functions::GetEnemyMinionInRange(wRange());
                if (wTarget != nullptr && wTarget->GetHealth() < Ashe_dmgW(wTarget) && Ashe_CanCastW(wTarget))
                    Ashe_UseW(wTarget);
            }
        }

        //Jungleclear
        else if (TargetSelector::Functions::GetJungleMonstersInRange(wRange()).size() > 0)
        {
            if (!Ashe_HasEnoughMana(AsheConfig::AsheJungle::minMana->Value)) return;

            if (AsheConfig::AsheJungle::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
            {
                const auto wTarget = TargetSelector::Functions::GetJungleInRange(wRange());
                if (wTarget != nullptr && Ashe_CanCastW(wTarget))
                    Ashe_UseW(wTarget);
            }

            if (AsheConfig::AsheJungle::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 0)
            {
                if (const auto qTarget = TargetSelector::Functions::GetJungleInRange(aaRange()))
                    Ashe_UseQ(qTarget);
            }
        }
    }


    void Harass() override
    {
        if (!Ashe_HasEnoughMana(AsheConfig::AsheHarass::minMana->Value))
            return;

        if (AsheConfig::AsheHarass::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
        {
            const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(wRange());
            if (wTarget != nullptr && Ashe_CanCastW(wTarget))
                Ashe_UseW(wTarget);
        }

        if (AsheConfig::AsheHarass::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::Functions::GetEnemyChampionInRange(aaRange()))
                Ashe_UseQ(qTarget);
        }
    }

    void Lasthit() override
    {
        if (!Ashe_HasEnoughMana(AsheConfig::AsheLastHit::minMana->Value))
            return;

        if (AsheConfig::AsheLastHit::UseW->Value == true && database.AsheW.IsCastable())
        {
            const auto wTarget = TargetSelector::Functions::GetEnemyMinionInRange(wRange());
            if (wTarget != nullptr && wTarget->GetHealth() < Ashe_dmgW(wTarget))
                Ashe_UseW(wTarget);
        }
    }

    void Flee() override
    {
        if (AsheConfig::AsheFlee::UseW->Value == true && database.AsheW.IsCastable())
        {
            const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(wRange());
            if (wTarget != nullptr)
                Ashe_UseW(wTarget);
        }
    }

    void Killsteal()
    {
        __try {
            if (AsheConfig::AsheKillsteal::UseW->Value == true && database.AsheW.IsCastable())
            {
                const auto wTarget = TargetSelector::Functions::GetEnemyChampionInRange(wRange());
                if (wTarget != nullptr && wTarget->GetHealth() < Ashe_dmgW(wTarget))
                {
                    Ashe_UseW(wTarget);
                }
            }

            if (AsheConfig::AsheKillsteal::UseR->Value == true && database.AsheR.IsCastable())
            {
                const auto rTarget = TargetSelector::Functions::GetEnemyChampionInRange(rRange());
                if (rTarget != nullptr
                    && rTarget->GetDistanceTo(globals::localPlayer) > AsheConfig::AsheSpellsSettings::minRDistance->Value
                    && rTarget->GetDistanceTo(globals::localPlayer) < AsheConfig::AsheSpellsSettings::maxRDistance->Value
                    && rTarget->GetHealth() < Ashe_dmgR(rTarget))
                {
                    Ashe_UseR(rTarget);
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
        for (auto target : TargetSelector::Functions::GetTargetsInRange(globals::localPlayer->GetPosition(), wRange()))
        {
            if (!functions::MenuItemContains(AsheConfig::AsheAntiGapCloser::whitelist, target->GetName().c_str())) continue;
            if (!target->GetAiManager()->IsDashing()) continue;
            if (target->GetBuffByName("rocketgrab2")) continue;

            if (target != nullptr)
            {
                if (AsheConfig::AsheAntiGapCloser::UseW->Value == true && database.AsheW.IsCastable())
                    Ashe_UseW(target);

                if (AsheConfig::AsheAntiGapCloser::UseR->Value == true && database.AsheR.IsCastable())
                    Ashe_UseR(target);
            }
        }
    }

    void AntiMelee()
    {
        if (AsheConfig::AsheAntiMelee::UseW->Value == true && database.AsheW.IsCastable())
        {
            for (auto target : TargetSelector::Functions::GetTargetsInRange(globals::localPlayer->GetPosition(), wRange()))
            {
                if (!functions::MenuItemContains(AsheConfig::AsheAntiMelee::whitelist, target->GetName().c_str())) continue;

                if (target != nullptr && target->IsInRange(globals::localPlayer->GetPosition(), target->GetAttackRange()))
                {
                	Ashe_UseW(target);
                }
            }
        }
    }

    //Events
    void OnBeforeAttack() override
    {
        __try {
            //Combo mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack)
            {
                const auto object = functions::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (AsheConfig::AsheCombo::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 1)
                    {
                    	Ashe_UseW(object);
                    }

                    if (AsheConfig::AsheCombo::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 1)
                    {
                    	Ashe_UseQ(object);
                    }
                }
            }
            //Laneclear mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear)
            {
                const auto object = functions::GetSelectedObject();
                if (object != nullptr && object->IsMinion())
                {
                    if (AsheConfig::AsheClear::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 1  && Ashe_HasEnoughMana(AsheConfig::AsheClear::minMana->Value))
                    {
                    	Ashe_UseW(object);
                    }
                }
            }
            //Harass mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass)
            {
                const auto object = functions::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (!Ashe_HasEnoughMana(AsheConfig::AsheHarass::minMana->Value))
                        return;

                    if (AsheConfig::AsheHarass::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 1)
                    {
                    	Ashe_UseW(object);
                    }

                    if (AsheConfig::AsheHarass::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 1)
                    {
                    	Ashe_UseQ(object);
                    }
                }
            }
        }
        __except (1)
        {
            LOG("ERROR IN ONBEFOREATTACK MODE");
        }
    }

    void OnCastSpell() override
    {

    }

    void Render() override
    {
        __try {
            if (AsheConfig::AsheSpellsSettings::DrawW->Value == true && (AsheConfig::AsheSpellsSettings::DrawIfReady->Value == true && database.AsheW.IsCastable() || AsheConfig::AsheSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), wRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("ERROR IN RENDER MODE");
        }
    }
};

AsheModule module;