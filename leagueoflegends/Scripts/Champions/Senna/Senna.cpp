#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../Orbwalker.h"
#include "Senna.h"


using namespace UPasta::SDK;


class SennaModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Senna");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + database.SennaQ.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime + database.SennaW.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime + database.SennaE.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.SennaR.GetCastTime() && ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static float qRange() {
        return ObjectManager::GetLocalPlayer()->GetRealAttackRange();
    }

    static float wRange() {
        return static_cast<float>(SennaConfig::SennaSpellsSettings::wRange->Value);
    }

    static float eRange() {
        return ObjectManager::GetLocalPlayer()->GetRealAttackRange() * 1.5f;
    }

    static float rRange() {
        return static_cast<float>(SennaConfig::SennaSpellsSettings::maxRDistance->Value);
    }



    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(SennaConfig::SennaClear::minMana->Value);
            else if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(SennaConfig::SennaJungle::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(SennaConfig::SennaHarass::minMana->Value);
            break;
        case OrbwalkState::Lasthit:
            minManaThreshold = static_cast<float>(SennaConfig::SennaLastHit::minMana->Value);
            break;
        default:
            return false;
        }

        return ObjectManager::GetLocalPlayer()->GetPercentMana() > minManaThreshold;
    }


    static float Senna_dmgQ(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::Q))
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float AttackDMG = SennaDamages::QSpell::additionalPercentageAD;
        const float DMG = SennaDamages::QSpell::dmgSkillArray[levelSpell];
        const float pDMG = (DMG + (ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * AttackDMG));
        const float PhsyDMG = Damage::CalculatePhysicalDamage(ObjectManager::GetLocalPlayer(), pEnemy, pDMG);
        return PhsyDMG;

    }

    static float Senna_dmgW(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::W))
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float AttackDMG = SennaDamages::WSpell::additionalPercentageAD;
        const float DMG = SennaDamages::WSpell::dmgSkillArray[levelSpell];
        const float pDMG = (DMG + (ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * AttackDMG));
        const float PhsyDMG = Damage::CalculatePhysicalDamage(ObjectManager::GetLocalPlayer(), pEnemy, pDMG);
        return PhsyDMG;

    }

    static float Senna_dmgR(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !ObjectManager::GetLocalPlayer()->CanCastSpell(SpellIndex::R))
            return -9999;

        const int levelSpell = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float AttackDMG = SennaDamages::RSpell::additionalPercentageAD;
        const float APDMG = SennaDamages::RSpell::additionalPercentageAP;
        const float DMG = SennaDamages::RSpell::dmgSkillArray[levelSpell];
        const float pDMG = (DMG + (ObjectManager::GetLocalPlayer()->GetBonusAttackDamage() * AttackDMG));
        const float aDMG = ((ObjectManager::GetLocalPlayer()->GetAbilityPower() * APDMG));
        const float PhsyDMG = Damage::CalculatePhysicalDamage(ObjectManager::GetLocalPlayer(), pEnemy, pDMG);
        const float APPDMG = Damage::CalculateMagicalDamage(ObjectManager::GetLocalPlayer(), pEnemy, aDMG);
        return PhsyDMG + APPDMG;

    }

    static bool CanKill(Object* target, float damageCalculation)
    {
        if (target == nullptr) return false;
        const float targetHealth = target->GetHealth();
        const float targetShield = target->GetShield();
        return targetHealth + targetShield < damageCalculation;
    }

    static bool IsControlled(Object* target) {
        if (target == nullptr) return false;
        return target->GetBuffByType(BuffType::Stun) or target->GetBuffByType(BuffType::Snare) or target->GetBuffByType(BuffType::Suppression);
    }

    static std::vector<Object*> GetHeroesThatNeedsHeal()
    {
        std::vector<Object*> possible_targets;
        for (auto hero : ObjectManager::GetHeroesAs(Alliance::Ally)) {
            if (!hero) continue;
            if (hero->GetDistanceTo(ObjectManager::GetLocalPlayer()) > qRange()) continue;

            possible_targets.push_back(hero);
        }

        if (possible_targets.size() > 1)
            std::sort(possible_targets.begin(), possible_targets.end(),
                [](Object* pFirst, Object* pSecond) -> bool
                {
                    auto health_first = pFirst->GetHealth();
                    auto max_health_first = pFirst->GetMaxHealth();

                    auto health_second = pSecond->GetHealth();
                    auto max_health_second = pSecond->GetMaxHealth();

                    auto autos_first = health_first / max_health_first;
                    auto autos_second = health_second / max_health_second;

                    return autos_first < autos_second;

                });

        return possible_targets;
    }

    static Object* GetHeroThatNeedsHeal()
    {
        Object* heroToReturn = nullptr;
        for (auto hero : GetHeroesThatNeedsHeal()) {
            if (!hero) continue;
            const float heroHPPercent = hero->GetHealth() / hero->GetMaxHealth() * 100;

            if (!heroToReturn && heroHPPercent <= SennaConfig::SennaAuto::MinHealth->Value and hero->IsAlive() and hero->IsTargetable()) {
                heroToReturn = hero;
                break;
            }
        }

        if (heroToReturn)
            return heroToReturn;

        return nullptr;
    }

    static ImVec2 CalculateTopLeft(const Vector2& basePos) {
        return { basePos.x, basePos.y - SennaDamages::yOffset };
    }

    static ImVec2 CalculateBottomRight(const Vector2& basePos, float widthLimit) {
        return { basePos.x + widthLimit, basePos.y - SennaDamages::yOffset + 10.0f };
    }


public:

    SennaModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto SennaMenu = Menu::CreateMenu("JaySwitch", "jay.Senna");

        const auto comboMenu = SennaMenu->AddMenu("Combo Settings", "Combo Settings");
        SennaConfig::SennaCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);        SennaConfig::SennaCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        SennaConfig::SennaCombo::UseQAlly = comboMenu->AddCheckBox("Use Q Ally", "Use SpellSlot Q for heal Ally", true);
        SennaConfig::SennaCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = SennaMenu->AddMenu("Harass Settings", "Harass Settings");
        SennaConfig::SennaHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        SennaConfig::SennaHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = SennaMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        SennaConfig::SennaClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", false);
        SennaConfig::SennaClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto lastHitMenu = clearMenu->AddMenu("Lasthit Settings", "Lasthit Settings");
        SennaConfig::SennaLastHit::UseQ = lastHitMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaLastHit::minMana = lastHitMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        SennaConfig::SennaJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        SennaConfig::SennaJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = SennaMenu->AddMenu("Additional Settings", "Additional Settings");

        const auto autoMenu = additionalMenu->AddMenu("Auto Settings", "Auto Settings");
        SennaConfig::SennaAuto::AutoSoul = autoMenu->AddCheckBox("Auto Grab Souls", "Auto Grab Souls", true);
        SennaConfig::SennaAuto::NoSoulInCombo = autoMenu->AddCheckBox("Dont Grab Souls In Combo", "Dont Grab Souls In Combo", true);
        SennaConfig::SennaAuto::AutoHeal = autoMenu->AddCheckBox("Auto Heal", "Auto Heal", true); //NoSoulInCombo
        SennaConfig::SennaAuto::MinManaHeal = autoMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);
        SennaConfig::SennaAuto::MinHealth = autoMenu->AddSlider("minHealth", "Health Percentage Minimum", 40, 1, 100, 5);
        SennaConfig::SennaAuto::UseW = autoMenu->AddCheckBox("Use W on CC Target", "Use W on CC Target", true);

        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        SennaConfig::SennaKillsteal::UseQ = ksMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        SennaConfig::SennaKillsteal::UseR = ksMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        SennaConfig::SennaFlee::UseW = fleeMenu->AddCheckBox("Use W", "Use W To Flee", false);
        SennaConfig::SennaFlee::UseE = fleeMenu->AddCheckBox("Use E", "Use E To Flee", false);

        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        SennaConfig::SennaSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        SennaConfig::SennaSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", qRange(), 100, qRange(), 50);
        SennaConfig::SennaSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        SennaConfig::SennaSpellsSettings::wCastMode = wSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        SennaConfig::SennaSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.SennaW.GetRange(), 100, database.SennaW.GetRange(), 50);
        SennaConfig::SennaSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
        SennaConfig::SennaSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.SennaE.GetRange(), 100, database.SennaE.GetRange(), 50);
        SennaConfig::SennaSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        SennaConfig::SennaSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);
        SennaConfig::SennaSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "SpellSlot R Minimum Fire Distance", 1000, 100, database.EzrealR.GetRange() * 2, 100);
        SennaConfig::SennaSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "SpellSlot R Maximum Fire Distance", 3000, 100, database.EzrealR.GetRange() * 2, 100);

        SennaConfig::SennaSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

        const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
        SennaConfig::SennaHPBAR::DrawRDamage = miscMenu->AddCheckBox("DrawRDamage", "Draw R Damage", true);
    }

    void DrawDamage(Object* pEnemy) const
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !isTimeToCastR())
            return;

        const Vector2 screenPos = Engine::GetHpBarPosition(pEnemy);

        const float endOffset2 = SennaDamages::xOffset + pEnemy->GetHealth() / pEnemy->GetMaxHealth() * SennaDamages::widthMultiplier;
        const float damage = Senna_dmgR(pEnemy);
        const float startOffset2 = max(endOffset2 - (damage / pEnemy->GetMaxHealth() * SennaDamages::widthMultiplier), SennaDamages::xOffset);

        const ImVec2 topLeft = CalculateTopLeft(Vector2(screenPos.x + startOffset2, screenPos.y));
        const ImVec2 bottomRight = CalculateBottomRight(screenPos, endOffset2);

        // const float targetHealth = pEnemy->GetHealth();
        const auto drawColor = CanKill(pEnemy, damage) ? COLOR_GREEN : COLOR_RED;
        render::RenderRect(topLeft, bottomRight, drawColor, 0.0f, 0, 1.0f, true);
    }

    void Senna_UseQ(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !isTimeToCastQ())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(ObjectManager::GetLocalPlayer()) < qRange() && isTimeToCastQ())
        {
            Engine::CastTargeted(SpellIndex::Q, pEnemy);
            QCastedTime = gameTime;
        }
    }

    void Senna_UseW(Object* pEnemy)
    {
        if (ObjectManager::GetLocalPlayer() == nullptr || pEnemy == nullptr || !isTimeToCastW())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(ObjectManager::GetLocalPlayer()) <= wRange())
        {
            if (pEnemy->IsMinion() || pEnemy->IsJungle()) {
                Engine::CastToPosition(SpellIndex::W, pEnemy->GetPosition());
                WCastedTime = gameTime;
                return;
            }

            Modules::prediction::PredictionOutput wPrediction;
            if (GetPrediction(database.SennaW, wPrediction))
            {
                Engine::CastToPosition(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void AttackOrb() {
        if (!SennaConfig::SennaAuto::AutoSoul->Value) {
            return;
        }

        if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack and SennaConfig::SennaAuto::NoSoulInCombo->Value) {
            return;
        }

        auto orbtarget = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange(), Alliance::Enemy);
        if (orbtarget != nullptr and orbtarget->GetName() == MINION_SENNA_SOUL) {
            Orbwalker::AttackTarget(orbtarget);
            return;
        }
    }

    void TryWOnControlledTarget() {
        if (SennaConfig::SennaAuto::UseW->Value && !isTimeToCastW()) return;

        auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
        if (wTarget != nullptr) {
            if (IsControlled(wTarget)) {
                Senna_UseW(wTarget);
            }
        }
    }

    void TryQHealAlly() {
        if (!SennaConfig::SennaAuto::AutoHeal->Value || !SennaConfig::SennaCombo::UseQAlly->Value && OrbwalkState::Attack) return;
        const float manaPercent = ObjectManager::GetLocalPlayer()->GetMana() / ObjectManager::GetLocalPlayer()->GetMaxMana() * 100;
        if (SennaConfig::SennaAuto::MinManaHeal->Value > manaPercent) return;
        if (!isTimeToCastQ()) return;

        auto qAlly = GetHeroThatNeedsHeal();
        if (qAlly != nullptr) {
            Senna_UseQ(qAlly);
        }
    }

    bool isEnemyCountSufficient(Object* hero, const Modules::prediction::PredictionOutput& rPrediction) {
        float distanceToPrediction = ObjectManager::GetLocalPlayer()->GetPosition().Distance(rPrediction.position);
        auto newPosition = rPrediction.position.Extend(rPrediction.position, distanceToPrediction + rRange());
        int enemyCount = Modules::prediction::CountObjectsInWay(ObjectManager::GetLocalPlayer()->GetPosition(), newPosition, hero, Alliance::Enemy, database.SennaR.GetRadius());

        return (enemyCount >= SennaConfig::SennaCombo::enemiesInRange->Value);
    }

    void Senna_UseR(bool check_count = false)
    {
        if (!isTimeToCastR())
            return;

        for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
            if (!hero) continue;
            float distanceToHero = hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition());
            if (distanceToHero > rRange() + hero->GetBoundingRadius() / 2 || distanceToHero < SennaConfig::SennaSpellsSettings::minRDistance->Value + hero->GetBoundingRadius() / 2)
                continue;

            Modules::prediction::PredictionOutput rPrediction;
            if (!GetPrediction(database.SennaR, rPrediction))
                continue;

            if (CanKill(hero, Senna_dmgR(hero)) || (check_count && isEnemyCountSufficient(hero, rPrediction))) {
                Engine::CastToPosition(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
                return;
            }
        }
    }


    void Update() override {
        gameTime = Engine::GetGameTime();
        AttackOrb();
        Killsteal();
        TryWOnControlledTarget();
        TryQHealAlly();
    }

    void Combo() override {
        if (SennaConfig::SennaCombo::UseR->Value == true
            && isTimeToCastR())
        {
            Senna_UseR(true);
        }

        if (SennaConfig::SennaCombo::UseW->Value == true
            && SennaConfig::SennaSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                Senna_UseW(wTarget);
            }
        }

        if (SennaConfig::SennaCombo::UseQ->Value == true
            && SennaConfig::SennaSpellsSettings::qCastMode->Value == 0
            && isTimeToCastQ())
        {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
            if (qTarget != nullptr) {
                Senna_UseQ(qTarget);
            }
        }

        TryQHealAlly();


    }
    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, ObjectManager::GetLocalPlayer()->GetPosition(), wRange()) > 0)
        {
            if (SennaConfig::SennaClear::UseQ->Value && isTimeToCastQ()) {
                const auto qMinion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), Alliance::Enemy);
                if (qMinion != nullptr) {
                    Senna_UseQ(qMinion);
                }
            }

            if (SennaConfig::SennaClear::UseW->Value && isTimeToCastW()) {
                const auto wMinion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), wRange(), Alliance::Enemy);
                if (wMinion != nullptr) {
                    float AAdamage = Damage::CalculateAutoAttackDamage(ObjectManager::GetLocalPlayer(), wMinion);
                    if (wMinion != nullptr && AAdamage * 2 < wMinion->GetHealth()) {
                        Senna_UseW(wMinion);
                    }
                }
            }
        }
        else if (ObjectManager::CountJungleMonstersInRange(ObjectManager::GetLocalPlayer()->GetPosition(), wRange()) > 0)
        {
            if (SennaConfig::SennaJungle::UseW->Value && isTimeToCastW()) {
                const auto wJungle = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
                if (wJungle != nullptr) {
                    const float AAdamage = Damage::CalculateAutoAttackDamage(ObjectManager::GetLocalPlayer(), wJungle);
                    if (AAdamage * 2 < wJungle->GetHealth()) {
                        Senna_UseW(wJungle);
                    }
                }

            }

            if (SennaConfig::SennaJungle::UseQ->Value && isTimeToCastQ()) {
                const auto qJungle = TargetSelector::FindBestJungle(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
                if (qJungle != nullptr) {
                    Senna_UseQ(qJungle);
                }
            }
        }
    }

    void Harass() override {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;

        if (SennaConfig::SennaHarass::UseW->Value
            && SennaConfig::SennaSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                Senna_UseW(wTarget);
            }
        }

        if (SennaConfig::SennaHarass::UseQ->Value == true
            && isTimeToCastQ())
        {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
            if (qTarget != nullptr)
            {
                Senna_UseQ(qTarget);
            }
        }
    }

    void Lasthit() override {
        if (SennaConfig::SennaLastHit::UseQ->Value && isTimeToCastQ()) {
            const auto minion = TargetSelector::FindBestMinion(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), Alliance::Enemy);
            if (minion != nullptr) {
                if (CanKill(minion, Senna_dmgQ(minion))) {
                    Senna_UseQ(minion);
                }
            }
        }
    }

    void Flee() override {
        const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
        if (wTarget != nullptr) {
            if (SennaConfig::SennaFlee::UseE->Value && isTimeToCastE())
            {
                Engine::CastSelf(SpellIndex::E);
                ECastedTime = gameTime;
                return;
            }
        }

        if (SennaConfig::SennaFlee::UseW->Value && isTimeToCastW()) {
            Senna_UseW(wTarget);
        }

    }

    void Killsteal()
    {


        if (SennaConfig::SennaKillsteal::UseQ->Value && isTimeToCastQ()) {
            const auto qTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), qRange());
            if (qTarget != nullptr) {
                if (CanKill(qTarget, Senna_dmgQ(qTarget))) {
                    Senna_UseQ(qTarget);
                }
            }
        }

        if (SennaConfig::SennaKillsteal::UseW->Value && isTimeToCastW()) {
            const auto wTarget = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), wRange());
            if (wTarget != nullptr) {
                if (CanKill(wTarget, Senna_dmgW(wTarget))) {
                    Senna_UseW(wTarget);
                }
            }

        }
        if (SennaConfig::SennaKillsteal::UseR->Value && isTimeToCastR()) {
            Senna_UseR();
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

    void Render() override {
        if (SennaConfig::SennaSpellsSettings::DrawQ->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaQ.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), qRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaSpellsSettings::DrawW->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaW.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), wRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaSpellsSettings::DrawE->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaE.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), eRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaSpellsSettings::DrawR->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaR.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(ObjectManager::GetLocalPlayer()->GetPosition(), rRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaHPBAR::DrawRDamage->Value == true) {
            for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
                if (!hero) continue;
                if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->GetPosition().Distance(ObjectManager::GetLocalPlayer()->GetPosition()) <= rRange() + hero->GetBoundingRadius() / 2)
                    DrawDamage(hero);
            }
        }
    }
};

SennaModule module;