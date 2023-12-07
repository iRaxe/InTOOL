#include "Vayne.h"
#include "Awareness.h"
#include "Damage.h"
#include "stdafx.h"
#include "TargetSelector.h"

class VayneModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Vayne");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.VayneQ.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + database.VayneE.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.VayneR.GetCastTime();
    }

    static float aaRange()
    {
        return globals::localPlayer->GetRealAttackRange();
    }

    static float qRange()
    {
        return VayneConfig::VayneSpellsSettings::qRange->Value;
    }

    static float eRange()
    {
        return VayneConfig::VayneSpellsSettings::eRange->Value;
    }

    static float rRange()
    {
        return VayneConfig::VayneSpellsSettings::maxRDistance->Value;
    }

public:

    VayneModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto VayneMenu = Menu::CreateMenu("vezVayne", "vez.Vayne");

        const auto combo = VayneMenu->AddMenu("Combo Settings", "Combo Settings");
        VayneConfig::VayneCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        VayneConfig::VayneCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
        VayneConfig::VayneCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        VayneConfig::VayneCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = VayneMenu->AddMenu("Harass Settings", "Harass Settings");
        VayneConfig::VayneHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        VayneConfig::VayneHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        VayneConfig::VayneHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = VayneMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        VayneConfig::VayneClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        VayneConfig::VayneClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        VayneConfig::VayneJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        VayneConfig::VayneJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        VayneConfig::VayneJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = VayneMenu->AddMenu("Additional Settings", "Additional Settings");
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        VayneConfig::VayneKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
        VayneConfig::VayneKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        VayneConfig::VayneAntiGapCloser::UseQ = antiGapMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        VayneConfig::VayneAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E", false);
        VayneConfig::VayneAntiGapCloser::UseR = antiGapMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

        const auto antiGapwhitelistMenu = antiGapMenu->AddMenu("Whitelist Settings", "Whitelist");

        const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
        VayneConfig::VayneAntiMelee::UseQ = antiMeleeMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
        VayneConfig::VayneAntiMelee::UseE = antiMeleeMenu->AddCheckBox("Use E", "Use SpellSlot E", false);

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
                        if (self->Value == false && !VayneConfig::VayneAntiGapCloser::whitelist.empty())
                        {
                            const auto it = std::ranges::find(VayneConfig::VayneAntiGapCloser::whitelist, obj);
                            VayneConfig::VayneAntiGapCloser::whitelist.erase(it);
                        }
                        else
                        {
                            VayneConfig::VayneAntiGapCloser::whitelist.push_back(obj);
                        }
                    });

                if (antiGap_checkbox->Value == true)
                {
                    VayneConfig::VayneAntiGapCloser::whitelist.push_back(obj);
                }

                if (!obj->IsMelee()) continue;
                const auto antiMelee_checkbox = antiMeleewhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !VayneConfig::VayneAntiMelee::whitelist.empty())
                        {
                            const auto it = std::ranges::find(VayneConfig::VayneAntiMelee::whitelist, obj);
                            VayneConfig::VayneAntiMelee::whitelist.erase(it);
                        }
                        else
                        {
                            VayneConfig::VayneAntiMelee::whitelist.push_back(obj);
                        }
                    });

                if (antiMelee_checkbox->Value == true)
                {
                    VayneConfig::VayneAntiMelee::whitelist.push_back(obj);
                }
            }
        }

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        VayneConfig::VayneFlee::UseQ = fleeMenu->AddCheckBox("Use Q", "Use SpellSlot Q To Mouse", false);
        VayneConfig::VayneFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E", false);

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");
        VayneConfig::VayneSpellsSettings::saveMana = spellsMenu->AddCheckBox("saveManaForCombo", "Save Mana For A Full Combo Rotation", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        VayneConfig::VayneSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        VayneConfig::VayneSpellsSettings::wStacksRequired = qSpellMenu->AddSlider("wStacksRequired", "Use Spellslot Q Only At Minimum stacks", 2, 0, 3, 1);
        VayneConfig::VayneSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.VayneQ.GetRange(), 100, database.VayneQ.GetRange(), 50);
        VayneConfig::VayneSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
        VayneConfig::VayneSpellsSettings::eCastMode = eSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        VayneConfig::VayneSpellsSettings::UseEOnlyStun = eSpellMenu->AddCheckBox("Use E Only Stun", "Use SpellSlot E Only if Stun", true);
        VayneConfig::VayneSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.VayneE.GetRange(), 100, database.VayneE.GetRange(), 50);
        VayneConfig::VayneSpellsSettings::wallCheckRange = eSpellMenu->AddSlider("wallCheckRange", "Maximum Wall Check Range", 475, 0, 1000, 10);
        VayneConfig::VayneSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        VayneConfig::VayneSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "SpellSlot R Minimum Fire Distance", qRange(), 100, database.VayneR.GetRange(), 100);
        VayneConfig::VayneSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "SpellSlot R Maximum Fire Distance", aaRange(), 100, database.VayneR.GetRange(), 100);

        VayneConfig::VayneSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    bool Vayne_IsCondemnable(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.VayneE.IsCastable())
            return false;

        auto pushDistance = VayneConfig::VayneSpellsSettings::wallCheckRange->Value;
        auto targetPosition = pEnemy->GetPosition();
        float checkDistance = pushDistance / 40.0f;
        auto pushDirection = (targetPosition - globals::localPlayer->GetPosition()).Normalized();

        for (int i = 0; i < 40; i++)
        {
            Vector3 finalPosition = targetPosition + (pushDirection * checkDistance * i);

            if (Engine::IsWall(finalPosition))
            {
                /*render::RenderLine(Engine::WorldToScreen(pEnemy->GetPosition()).ToImVec(), Engine::WorldToScreen(finalPosition).ToImVec(),COLOR_WHITE, 1.0f);
                Awareness::Functions::Radius::DrawRadius(finalPosition, 50, COLOR_RED, 1.0f);*/
                return true;
            }
        }
        return false;
    }

    bool Vayne_CanCastQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || Engine::GetSpellState(SpellIndex::Q) != 0)
            return false;

        auto wBuff = pEnemy->GetBuffByName("VayneSilveredDebuff");
        if (pEnemy != nullptr && wBuff != nullptr && wBuff->GetStacksAlt() >= VayneConfig::VayneSpellsSettings::wStacksRequired->Value)
            return true;
        
        return false;
    }

    static float Vayne_dmgQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || Engine::GetSpellState(SpellIndex::Q) != 0)
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float skillDamage = VayneDamages::QSpell::additionalPercentageAD[levelSpell - 1];
        const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
        const float additionalAbilityPowerSkillDamage = VayneDamages::QSpell::additionalPercentageAP;

        const float totalDamage = Damage::CalculateAutoAttackDamage(globals::localPlayer, pEnemy) + (100 * skillDamage) + (abilityPowerDamage * additionalAbilityPowerSkillDamage);
        return Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, totalDamage);
    }

    float Vayne_dmgE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.VayneE.IsCastable())
            return -9999;

        float damage = 0;
        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetLevel();
        const float skillDamage = VayneDamages::ESpell::dmgSkillArray[levelSpell];

        const float attackDamage = globals::localPlayer->ReadClientStat(Object::BonusAttackDamage);
        const float additionalAttackDamageSkillDamage = VayneDamages::ESpell::additionalPercentageAD;

        const float bonusSkillDamage = VayneDamages::ESpell::bonusdmgSkillArray[levelSpell];
        const float bonusadditionalAttackDamageSkillDamage = VayneDamages::ESpell::bonusadditionalPercentageAd;
        float test = skillDamage + (attackDamage * additionalAttackDamageSkillDamage);
        float test1 = bonusSkillDamage + (attackDamage * bonusadditionalAttackDamageSkillDamage);
        damage += Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, test);
        if (Vayne_IsCondemnable(pEnemy))
            damage += Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, test1);

        return Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, damage);
    }

    static bool Vayne_HasEnoughComboMana()
    {
        if (globals::localPlayer == nullptr)
            return false;

        const float qSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetManaCost();
        const float eSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetManaCost();
        const float rSpellManaCost = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetManaCost();

        if (globals::localPlayer->GetLevel() >= 6)
            return globals::localPlayer->GetMana() > qSpellManaCost + eSpellManaCost + rSpellManaCost;

        return globals::localPlayer->GetMana() > qSpellManaCost + eSpellManaCost;
    }

    static bool Vayne_HasEnoughMana(float minValue)
    {
        if (globals::localPlayer == nullptr || VayneConfig::VayneSpellsSettings::saveMana->Value && !Vayne_HasEnoughComboMana())
            return false;

        return globals::localPlayer->GetPercentMana() > minValue;
    }

    void Vayne_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || Engine::GetSpellState(SpellIndex::Q) != 0)
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < aaRange() && isTimeToCastQ())
        {
            auto mousePos = globals::localPlayer->GetPosition().Extend(Engine::GetMouseWorldPos(), 300);

            Engine::CastSpell(SpellIndex::Q, mousePos);
            QCastedTime = gameTime;
        }
    }

    void Vayne_UseE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.VayneE.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < eRange() && isTimeToCastE())
        {
            Engine::CastSpell(SpellIndex::E, pEnemy);
            ECastedTime = gameTime;
        }
    }

    void Vayne_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.VayneR.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < rRange() && isTimeToCastR())
        {
            Engine::CastSpell(SpellIndex::R);
            RCastedTime = gameTime;
        }
    }

    void Update() override
    {
        gameTime = Engine::GetGameTime();

        Killsteal();
        AntiGapCloser();
        AntiMelee();
        
    }

    void Combo() override
    {
        if (VayneConfig::VayneCombo::UseR->Value == true && database.VayneR.IsCastable() && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), VayneConfig::VayneSpellsSettings::minRDistance->Value) >= VayneConfig::VayneCombo::enemiesInRange->Value)
        {
            if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange()))
                Vayne_UseR(rTarget);
        }

        if (VayneConfig::VayneCombo::UseE->Value == true && database.VayneE.IsCastable() && VayneConfig::VayneSpellsSettings::eCastMode->Value == 0)
        {
            if (const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),eRange()))
                if (VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == true && Vayne_IsCondemnable(eTarget) || VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == false && !Vayne_IsCondemnable(eTarget))
                    Vayne_UseE(eTarget);
        }

        if (VayneConfig::VayneCombo::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 0)
        {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),aaRange());
            if (qTarget != nullptr && Vayne_CanCastQ(qTarget))
                Vayne_UseQ(qTarget);
        }
    }

    void Clear() override
    {
        //Laneclear
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), aaRange()) > 0)
        {
            if (!Vayne_HasEnoughMana(VayneConfig::VayneClear::minMana->Value)) return;

            if (VayneConfig::VayneClear::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 0)
            {
                const auto qTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(),aaRange(), Alliance::Enemy);
                if (qTarget != nullptr && qTarget->ReadClientStat(Object::Health) < Vayne_dmgQ(qTarget))
                    Vayne_UseQ(qTarget);
            }
        }

        //Jungleclear
        else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), aaRange()) > 0)
        {
            if (!Vayne_HasEnoughMana(VayneConfig::VayneJungle::minMana->Value)) return;

            if (VayneConfig::VayneJungle::UseE->Value == true && database.VayneE.IsCastable() && VayneConfig::VayneSpellsSettings::eCastMode->Value == 0)
            {
                if (const auto wTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), eRange()))
                    Vayne_UseE(wTarget);
            }

            if (VayneConfig::VayneJungle::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 0)
            {
                const auto qTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), aaRange());
                if (qTarget != nullptr && Vayne_CanCastQ(qTarget))
                    Vayne_UseQ(qTarget);
            }
        }
    }


    void Harass() override
    {
        if (!Vayne_HasEnoughMana(VayneConfig::VayneHarass::minMana->Value))
            return;

        if (VayneConfig::VayneHarass::UseE->Value == true && database.VayneE.IsCastable() && VayneConfig::VayneSpellsSettings::eCastMode->Value == 0)
        {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),eRange());
            if (eTarget != nullptr && (VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == true && Vayne_IsCondemnable(eTarget) || VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == false && !Vayne_IsCondemnable(eTarget)))
                Vayne_UseE(eTarget);
        }

        if (VayneConfig::VayneHarass::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 0)
        {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),aaRange());
            if (qTarget != nullptr && Vayne_CanCastQ(qTarget))
                Vayne_UseQ(qTarget);
        }
    }

    void Lasthit() override    {
        return;
    }

    void Flee() override
    {
        if (VayneConfig::VayneFlee::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0)
        {
            auto pathEnd = globals::localPlayer->GetPosition().Extend(Engine::GetMouseWorldPos(), 300);
            if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, aaRange()) && isTimeToCastQ())
            {
                Engine::CastSpell(SpellIndex::Q, pathEnd);
                QCastedTime = gameTime;
            }
        }

        if (VayneConfig::VayneFlee::UseE->Value == true && database.VayneE.IsCastable())
        {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),eRange());
            if (eTarget != nullptr && (VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == true && Vayne_IsCondemnable(eTarget) || VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == false && !Vayne_IsCondemnable(eTarget)))
                Vayne_UseE(eTarget);
        }
    }

    void Killsteal()
    {
        __try {
            if (VayneConfig::VayneKillsteal::UseE->Value == true && database.VayneE.IsCastable())
            {
                const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),eRange());
                if (eTarget != nullptr && eTarget->ReadClientStat(Object::Health) < Vayne_dmgE(eTarget))
                {
                    Vayne_UseE(eTarget);
                }
            }

            if (VayneConfig::VayneKillsteal::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0)
            {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),aaRange());
                if (qTarget != nullptr && qTarget->ReadClientStat(Object::Health) < Vayne_dmgQ(qTarget))
                {
                    Vayne_UseQ(qTarget);
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
        for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
        {
            if (!target) continue;
            if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > eRange()) continue;
            if (!Engine::MenuItemContains(VayneConfig::VayneAntiGapCloser::whitelist, target->GetName().c_str())) continue;
            if (!target->GetAiManager()->IsDashing()) continue;
            if (target->GetBuffByName("rocketgrab2")) continue;

            if (target != nullptr)
            {
                if (VayneConfig::VayneAntiGapCloser::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0)
                    Vayne_UseQ(target);

                if (VayneConfig::VayneAntiGapCloser::UseE->Value == true && database.VayneE.IsCastable())
                    Vayne_UseE(target);

                if (VayneConfig::VayneAntiGapCloser::UseR->Value == true && database.VayneR.IsCastable())
                    Vayne_UseR(target);
            }
        }
    }

    void AntiMelee()
    {
        for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
        {
            if (!target) continue;
            if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > eRange()) continue;
            if (!Engine::MenuItemContains(VayneConfig::VayneAntiMelee::whitelist, target->GetName().c_str())) continue;

            if (VayneConfig::VayneAntiMelee::UseE->Value == true && database.VayneE.IsCastable())
                Vayne_UseE(target);

            if (VayneConfig::VayneAntiMelee::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0)
                Vayne_UseQ(target);

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
                    if (VayneConfig::VayneCombo::UseE->Value == true && database.VayneE.IsCastable() && VayneConfig::VayneSpellsSettings::eCastMode->Value == 1)
                    {
                        if (VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == true && Vayne_IsCondemnable(object) || VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == false && !Vayne_IsCondemnable(object))
                            Vayne_UseE(object);
                    }

                    if (VayneConfig::VayneCombo::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 1)
                    {
                        if (Vayne_CanCastQ(object))
                            Vayne_UseQ(object);
                    }
                }
            }
            //Laneclear mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr)
                {
                    if (object->IsMinion())
                    {
                        if (VayneConfig::VayneClear::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 1 && object->ReadClientStat(Object::Health) < Vayne_dmgQ(object))
                        {
                            if (!Vayne_HasEnoughMana(VayneConfig::VayneClear::minMana->Value)) return;

                            Vayne_UseQ(object);
                        }
                    }
                    else if (object->IsJungle())
                    {
                        if (!Vayne_HasEnoughMana(VayneConfig::VayneJungle::minMana->Value)) return;

                        if (VayneConfig::VayneJungle::UseE->Value == true && database.VayneE.IsCastable() && VayneConfig::VayneSpellsSettings::eCastMode->Value == 1)
                            Vayne_UseE(object);

                        if (VayneConfig::VayneJungle::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && VayneConfig::VayneSpellsSettings::qCastMode->Value == 1)
                        {
                            if (Vayne_CanCastQ(object))
                                Vayne_UseQ(object);
                        }
                    }
                }

            }

            //Harass mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (!Vayne_HasEnoughMana(VayneConfig::VayneHarass::minMana->Value))
                        return;

                    if (VayneConfig::VayneHarass::UseE->Value == true && database.VayneE.IsCastable() && VayneConfig::VayneSpellsSettings::eCastMode->Value == 1)
                    {
                        if (VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == true && Vayne_IsCondemnable(object) || VayneConfig::VayneSpellsSettings::UseEOnlyStun->Value == false && !Vayne_IsCondemnable(object))
                            Vayne_UseE(object);
                    }

                    if (VayneConfig::VayneHarass::UseQ->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 && Vayne_CanCastQ(object) && VayneConfig::VayneSpellsSettings::qCastMode->Value == 1)
                    {
                        Vayne_UseQ(object);
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
        if (VayneConfig::VayneSpellsSettings::DrawQ->Value == true && (VayneConfig::VayneSpellsSettings::DrawIfReady->Value == true && Engine::GetSpellState(SpellIndex::Q) == 0 || VayneConfig::VayneSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);
        if (VayneConfig::VayneSpellsSettings::DrawE->Value == true && (VayneConfig::VayneSpellsSettings::DrawIfReady->Value == true && database.VayneE.IsCastable() || VayneConfig::VayneSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), eRange(), COLOR_WHITE, 1.0f);
    }
};

VayneModule module;