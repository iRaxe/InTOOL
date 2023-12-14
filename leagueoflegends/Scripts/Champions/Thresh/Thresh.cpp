#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Thresh.h"

class ThreshModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Thresh");

private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

private:
    static int threshSouls()
    {
        return globals::localPlayer->GetBuffByName("ThreshPassiveSoulsGain")->GetStacks();
    }

    static bool hasQ2()
    {
        return globals::localPlayer->GetSpellBySlotId(0)->GetName() == "ThreshQLeap";
    }

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.ThreshQ.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastW() const
    {
        return gameTime > WCastedTime + database.ThreshW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + database.ThreshE.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.ThreshR.GetCastTime();
    }

    static float qRange()
    {
        return ThreshConfig::ThreshSpellsSettings::qRange->Value;
    }

    static float wRange()
    {
        return ThreshConfig::ThreshSpellsSettings::wRange->Value;
    }

    static float eRange()
    {
        return ThreshConfig::ThreshSpellsSettings::eRange->Value;
    }

    static float rRange()
    {
        return ThreshConfig::ThreshSpellsSettings::rRange->Value;
    }

public:

    ThreshModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto ThreshMenu = Menu::CreateMenu("vezThresh", "vez.Thresh");

        const auto combo = ThreshMenu->AddMenu("Combo Settings", "Combo Settings");
        ThreshConfig::ThreshCombo::UseQ = combo->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        ThreshConfig::ThreshCombo::UseW = combo->AddCheckBox("Use W", "Use SpellSlot W", false);
        ThreshConfig::ThreshCombo::UseE = combo->AddCheckBox("Use E", "Use SpellSlot E", true);
        ThreshConfig::ThreshCombo::UseR = combo->AddCheckBox("Use R", "Use SpellSlot R", true);
        ThreshConfig::ThreshCombo::enemiesInRange = combo->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = ThreshMenu->AddMenu("Harass Settings", "Harass Settings");
        ThreshConfig::ThreshHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        ThreshConfig::ThreshHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        ThreshConfig::ThreshHarass::eMode = harassMenu->AddList("eMode", "E Mode", std::vector<std::string>{"Pull", "Push"}, 0);

        ThreshConfig::ThreshHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = ThreshMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        ThreshConfig::ThreshClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        ThreshConfig::ThreshClear::minionsInRange = laneClearMenu->AddSlider("minMinionsInRange", "Minimum minions to use E", 2, 1, 6, 1);
        ThreshConfig::ThreshClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        ThreshConfig::ThreshJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        ThreshConfig::ThreshJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        ThreshConfig::ThreshJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        ThreshConfig::ThreshLastHit::UseQ = lastHitMenu->AddCheckBox("Use Q", "Use SpellSlot Q if out of AA Range", true);
        ThreshConfig::ThreshLastHit::UseE = lastHitMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        ThreshConfig::ThreshLastHit::minionsInRange = lastHitMenu->AddSlider("minMinionsInRange", "Minimum minions to use E", 2, 1, 6, 1);
        ThreshConfig::ThreshLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = ThreshMenu->AddMenu("Additional Settings", "Additional Settings");
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        ThreshConfig::ThreshKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        ThreshConfig::ThreshKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W for Assist", true);
        ThreshConfig::ThreshKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        ThreshConfig::ThreshKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", false);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        ThreshConfig::ThreshAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        const auto antiGapwhitelistMenu = antiGapMenu->AddMenu("Whitelist Settings", "Whitelist");

        const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
        ThreshConfig::ThreshAntiMelee::UseE = antiMeleeMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
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
                        if (self->Value == false && !ThreshConfig::ThreshAntiGapCloser::whitelist.empty())
                        {
                            const auto it = std::ranges::find(ThreshConfig::ThreshAntiGapCloser::whitelist, obj);
                            ThreshConfig::ThreshAntiGapCloser::whitelist.erase(it);
                        }
                        else
                        {
                            ThreshConfig::ThreshAntiGapCloser::whitelist.push_back(obj);
                        }
                    });

                if (antiGap_checkbox->Value == true)
                {
                    ThreshConfig::ThreshAntiGapCloser::whitelist.push_back(obj);
                }

                if (!obj->IsMelee()) continue;
                const auto antiMelee_checkbox = antiMeleewhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !ThreshConfig::ThreshAntiMelee::whitelist.empty())
                        {
                            const auto it = std::ranges::find(ThreshConfig::ThreshAntiMelee::whitelist, obj);
                            ThreshConfig::ThreshAntiMelee::whitelist.erase(it);
                        }
                        else
                        {
                            ThreshConfig::ThreshAntiMelee::whitelist.push_back(obj);
                        }
                    });

                if (antiMelee_checkbox->Value == true)
                {
                    ThreshConfig::ThreshAntiMelee::whitelist.push_back(obj);
                }
            }
        }

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        ThreshConfig::ThreshFlee::UseQ = fleeMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        ThreshConfig::ThreshFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use SpellSlot E", false);

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");
        ThreshConfig::ThreshSpellsSettings::saveMana = spellsMenu->AddCheckBox("saveManaForCombo", "Save Mana For A Full Combo Rotation", true);

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        ThreshConfig::ThreshSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        ThreshConfig::ThreshSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.ThreshQ.GetRange(), 100, database.ThreshQ.GetRange(), 50);
        ThreshConfig::ThreshSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        ThreshConfig::ThreshSpellsSettings::wCastMode = wSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        ThreshConfig::ThreshSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.ThreshW.GetRange(), 100, database.ThreshW.GetRange(), 50);
        ThreshConfig::ThreshSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
        ThreshConfig::ThreshSpellsSettings::eCastMode = eSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        ThreshConfig::ThreshSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.ThreshE.GetRange(), 100, database.ThreshE.GetRange(), 50);
        ThreshConfig::ThreshSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        ThreshConfig::ThreshSpellsSettings::rRange = rSpellMenu->AddSlider("maxRRange", "Maximum Range", database.ThreshR.GetRange(), 100, database.ThreshR.GetRange(), 50);
        ThreshConfig::ThreshSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);

        ThreshConfig::ThreshSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    static float Thresh_dmgQ(Object* pEnemy) {
        return Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::Q, pEnemy, ThreshDamages::QSpell::dmgSkillArray, ThreshDamages::QSpell::additionalPercentageAP);
    }

    static float Thresh_dmgE(Object* pEnemy) {
        return Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::E, pEnemy, ThreshDamages::ESpell::dmgSkillArray, ThreshDamages::ESpell::additionalPercentageAP);
    }

    static float Thresh_dmgR(Object* pEnemy) {
        return Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::R, pEnemy, ThreshDamages::RSpell::dmgSkillArray, ThreshDamages::RSpell::additionalPercentageAP);
    }

    static float Thresh_shieldW()
    {
        if (globals::localPlayer == nullptr || !database.ThreshW.IsCastable())
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float skillShield = ThreshDamages::WSpell::shieldSkillArray[levelSpell];
        const float totalShield = skillShield + threshSouls();

        return totalShield;
    }

    static bool Thresh_HasEnoughComboMana()
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

    static bool Thresh_HasEnoughMana(float minValue)
    {
        if (globals::localPlayer == nullptr || ThreshConfig::ThreshSpellsSettings::saveMana->Value && !Thresh_HasEnoughComboMana())
            return false;

        return globals::localPlayer->GetPercentMana() > minValue;
    }


    void Thresh_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.ThreshQ.IsCastable() || hasQ2())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < qRange() && isTimeToCastQ())
        {
            Modules::prediction::PredictionOutput qPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.ThreshQ, qPrediction))
            {
                Engine::CastSpell(SpellIndex::Q, qPrediction.position);
                QCastedTime = gameTime;
            }
        }
    }

    void Thresh_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.ThreshW.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < wRange() && isTimeToCastW())
        {
            Modules::prediction::PredictionOutput wPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.ThreshW, wPrediction))
            {
                Engine::CastSpell(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void Thresh_UseE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.ThreshE.IsCastable() || globals::localPlayer->GetAiManager()->IsDashing())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < eRange() && isTimeToCastE())
        {
            Modules::prediction::PredictionOutput ePrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.ThreshE, ePrediction))
            {
                Engine::CastSpell(SpellIndex::E, ePrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void Thresh_UseE2(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.ThreshE.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < eRange() && isTimeToCastE())
        {
            //Use for push away
            auto enemyPos = pEnemy->GetPosition();


            const auto ally = ObjectManager::GetHeroAs(Alliance::Ally, globals::localPlayer->GetPosition(), 1100.0f);;
            if (ally != nullptr)
            {
                const auto allyPos = enemyPos.Extend(ally->GetPosition(), 800);
                if (Modules::prediction::IsSpecificObjectInWay(ally->GetPosition(), enemyPos, globals::localPlayer, database.ThreshE.GetRadius()))
                    Engine::CastSpell(SpellIndex::E, allyPos);
                else if (pEnemy->GetDistanceTo(ally) < globals::localPlayer->GetDistanceTo(ally))
                    Engine::CastSpell(SpellIndex::E, enemyPos);
            }
            else
            {
                const auto allyTower = TargetSelector::FindTurret(globals::localPlayer->GetPosition(),1100.0f, Alliance::Ally);
                if (allyTower != nullptr)
                {
                    const auto allyPos = enemyPos.Extend(allyTower->GetPosition(), 800);
                    if (Modules::prediction::IsSpecificObjectInWay(allyTower->GetPosition(), enemyPos, globals::localPlayer, database.ThreshE.GetRadius()))
                        Engine::CastSpell(SpellIndex::E, allyPos);
                    else if (pEnemy->GetDistanceTo(allyTower) < globals::localPlayer->GetDistanceTo(allyTower))
                        Engine::CastSpell(SpellIndex::E, enemyPos);
                }
                else
                {

                    if (Thresh_dmgQ(pEnemy) + Thresh_dmgE(pEnemy) + Thresh_dmgR(pEnemy) > pEnemy->ReadClientStat(Object::Health))
                    {
                        //Use for pull to thresh
                        const auto pullPos = enemyPos.Extend(globals::localPlayer->GetPosition(), 800);
                        Engine::CastSpell(SpellIndex::E, pullPos);
                    }
                    else
                        Engine::CastSpell(SpellIndex::E, enemyPos);
                }
            }
            ECastedTime = gameTime;
        }
    }

    void Thresh_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.ThreshR.IsCastable())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < rRange() && isTimeToCastR())
        {
            Modules::prediction::PredictionOutput rPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.ThreshR, rPrediction))
            {
                Engine::CastSpell(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }
        }
    }

    void Update() override {
        gameTime = Engine::GetGameTime();

        Killsteal();
        AntiGapCloser();
        AntiMelee();
    }

    void Combo()  override {
        if (ThreshConfig::ThreshCombo::UseR->Value == true && database.ThreshR.IsCastable() && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(),rRange()) >= ThreshConfig::ThreshCombo::enemiesInRange->Value)
        {
            if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange()))
                Thresh_UseR(rTarget);
        }

        if (ThreshConfig::ThreshCombo::UseW->Value == true && database.ThreshW.IsCastable() && ThreshConfig::ThreshSpellsSettings::wCastMode->Value == 0)
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
                Thresh_UseW(wTarget);
        }

        if (ThreshConfig::ThreshCombo::UseQ->Value == true && database.ThreshQ.IsCastable() && ThreshConfig::ThreshSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Thresh_UseQ(qTarget);
        }
    }

    void Clear() override {
        //Laneclear
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) > 0)
        {

            if (!Thresh_HasEnoughMana(ThreshConfig::ThreshClear::minMana->Value)) return;

            /*if (ThreshConfig::ThreshClear::UseQ->Value == true && database.ThreshQ.IsCastable())
            {
                const auto qTarget = TargetSelector::Functions::GetEnemyMinionInRange(qRange());
                if (qTarget != nullptr && qTarget->ReadClientStat(Object::Health) < Thresh_dmgQ(qTarget))
                    Thresh_UseQ(qTarget);
            }*/
        }

        //Jungleclear
        else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
        {
            if (!Thresh_HasEnoughMana(ThreshConfig::ThreshJungle::minMana->Value)) return;

            if (ThreshConfig::ThreshJungle::UseW->Value == true && database.ThreshW.IsCastable())
            {
                const auto wTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), wRange());
                if (wTarget != nullptr && (wTarget->GetName().contains("Dragon") || wTarget->GetName().contains("Baron") || wTarget->GetName().contains("Herald")))
                    Thresh_UseW(wTarget);
            }

            if (ThreshConfig::ThreshJungle::UseQ->Value == true && database.ThreshQ.IsCastable())
            {
                if (const auto qTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), qRange()))
                    Thresh_UseQ(qTarget);
            }
        }
    }


    void Harass() override {
        if (!Thresh_HasEnoughMana(ThreshConfig::ThreshHarass::minMana->Value))
            return;

        /*if (ThreshConfig::ThreshHarass::UseW->Value == true && database.ThreshW.IsCastable() && ThreshConfig::ThreshSpellsSettings::wCastMode->Value == 0)
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
                Thresh_UseW(wTarget);
        }*/

        if (ThreshConfig::ThreshHarass::UseQ->Value == true && database.ThreshQ.IsCastable() && ThreshConfig::ThreshSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Thresh_UseQ(qTarget);
        }
    }

    void Lasthit() override {
        if (!Thresh_HasEnoughMana(ThreshConfig::ThreshLastHit::minMana->Value))
            return;

        if (ThreshConfig::ThreshLastHit::UseQ->Value == true && database.ThreshQ.IsCastable())
        {
            const auto minion = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(),qRange(), Alliance::Enemy);
            if (minion != nullptr && minion->ReadClientStat(Object::Health) < Thresh_dmgQ(minion))
                Thresh_UseQ(minion);
        }
    }

    void Flee() override {
        if (ThreshConfig::ThreshFlee::UseE->Value == true && database.ThreshE.IsCastable())
        {
            const Vector3 pathEnd = Engine::GetMouseWorldPos();
            if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, 750.0f) && isTimeToCastE())
            {
                Engine::CastSpell(SpellIndex::E, Engine::GetMouseWorldPos());
                ECastedTime = gameTime;
            }
        }
    }

    void Killsteal()
    {
        __try {
            if (ThreshConfig::ThreshKillsteal::UseQ->Value == true && database.ThreshQ.IsCastable())
            {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange());
                if (qTarget != nullptr && qTarget->ReadClientStat(Object::Health) < Thresh_dmgQ(qTarget))
                {
                    Thresh_UseQ(qTarget);
                }
            }

            if (ThreshConfig::ThreshKillsteal::UseR->Value == true && database.ThreshR.IsCastable())
            {
                const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange());
                if (rTarget != nullptr && rTarget->ReadClientStat(Object::Health) < Thresh_dmgR(rTarget))
                {
                    Thresh_UseR(rTarget);
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
        if (ThreshConfig::ThreshAntiGapCloser::UseE->Value == true && Engine::GetSpellState(SpellIndex::E) == 0)
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > 750.0f) continue;
                if (!Engine::MenuItemContains(ThreshConfig::ThreshAntiGapCloser::whitelist, target->GetName().c_str())) continue;
                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target != nullptr)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, 750.0f))
                    {
                        Thresh_UseE(target);
                    }
                }
            }
        }
    }

    void AntiMelee()
    {
        if (ThreshConfig::ThreshAntiMelee::UseE->Value == true && Engine::GetSpellState(SpellIndex::E) == 0)
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > 750.0f) continue;
                if (!Engine::MenuItemContains(ThreshConfig::ThreshAntiMelee::whitelist, target->GetName().c_str())) continue;

                if (target != nullptr && target->IsInRange(globals::localPlayer->GetPosition(), target->GetRealAttackRange()))
                {
                    const Vector3 pathEnd = Engine::GetMouseWorldPos();
                    if (pathEnd.IsValid() && globals::localPlayer->IsInRange(pathEnd, 750.0f))
                    {
                        Thresh_UseE(target);
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

    void OnBeforeAttack() override {
        __try {
            //Combo mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (ThreshConfig::ThreshCombo::UseW->Value == true && database.ThreshW.IsCastable() && ThreshConfig::ThreshSpellsSettings::wCastMode->Value == 1)
                    {
                        Thresh_UseW(object);
                    }

                    if (ThreshConfig::ThreshCombo::UseQ->Value == true && database.ThreshQ.IsCastable() && ThreshConfig::ThreshSpellsSettings::qCastMode->Value == 1)
                    {
                        Thresh_UseQ(object);
                    }
                }
            }
            //Laneclear mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsBuilding())
                {
                   /* if (ThreshConfig::ThreshClear::UseW->Value == true && database.ThreshW.IsCastable() && Thresh_HasEnoughMana(ThreshConfig::ThreshClear::minMana->Value))
                    {
                        Thresh_UseW(object);
                    }*/
                }
            }
            //Harass mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Harass)
            {
                const auto object = Engine::GetSelectedObject();
                if (object != nullptr && object->IsHero())
                {
                    if (!Thresh_HasEnoughMana(ThreshConfig::ThreshHarass::minMana->Value))
                        return;

                   /* if (ThreshConfig::ThreshHarass::UseW->Value == true && database.ThreshW.IsCastable() && ThreshConfig::ThreshSpellsSettings::wCastMode->Value == 1)
                    {
                        Thresh_UseW(object);
                    }*/

                    if (ThreshConfig::ThreshHarass::UseQ->Value == true && database.ThreshQ.IsCastable() && ThreshConfig::ThreshSpellsSettings::qCastMode->Value == 1)
                    {
                        Thresh_UseQ(object);
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

    void Render() override {
        __try {

            if (ThreshConfig::ThreshSpellsSettings::DrawQ->Value == true && (ThreshConfig::ThreshSpellsSettings::DrawIfReady->Value == true && database.ThreshQ.IsCastable() || ThreshConfig::ThreshSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);

            if (ThreshConfig::ThreshSpellsSettings::DrawW->Value == true && (ThreshConfig::ThreshSpellsSettings::DrawIfReady->Value == true && database.ThreshW.IsCastable() || ThreshConfig::ThreshSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), wRange(), COLOR_WHITE, 1.0f);

            if (ThreshConfig::ThreshSpellsSettings::DrawE->Value == true && (ThreshConfig::ThreshSpellsSettings::DrawIfReady->Value == true && database.ThreshE.IsCastable() || ThreshConfig::ThreshSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), eRange(), COLOR_WHITE, 1.0f);
        }
        __except (1)
        {
            LOG("ERROR IN RENDER MODE");
        }
    }
};

ThreshModule module;