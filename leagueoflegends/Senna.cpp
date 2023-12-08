#include "Senna.h"

#include <vector>
#include "Awareness.h"
#include "Damage.h"
#include "LastHitDamage.h"
#include "TargetSelector.h"

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
        return gameTime > QCastedTime + database.SennaQ.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime + database.SennaW.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime + database.SennaE.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.SennaR.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static float qRange() {
        return static_cast<float>(SennaConfig::SennaSpellsSettings::qRange->Value);
    }

    static float wRange() {
        return static_cast<float>(SennaConfig::SennaSpellsSettings::wRange->Value);
    }

    static float eRange() {
        return globals::localPlayer->GetRealAttackRange() * 1.5f;
    }

    static float rRange() {
        return static_cast<float>(SennaConfig::SennaSpellsSettings::maxRDistance->Value);
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(SennaConfig::SennaClear::minMana->Value);
            else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
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

        return globals::localPlayer->GetPercentMana() > minManaThreshold;
    }


    static float Senna_dmgQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::Q))
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
        const float AttackDMG = SennaDamages::QSpell::additionalPercentageAD;
        const float DMG = SennaDamages::QSpell::dmgSkillArray[levelSpell];
        const float pDMG = (DMG + (globals::localPlayer->ReadClientStat(Object::BonusAttackDamage) * AttackDMG));
        const float PhsyDMG = Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, pDMG);
        return PhsyDMG;

    }

    static float Senna_dmgW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::W))
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        const float AttackDMG = SennaDamages::WSpell::additionalPercentageAD;
        const float DMG = SennaDamages::WSpell::dmgSkillArray[levelSpell];
        const float pDMG = (DMG + (globals::localPlayer->ReadClientStat(Object::BonusAttackDamage) * AttackDMG));
        const float PhsyDMG = Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, pDMG);
        return PhsyDMG;

    }

    static float Senna_dmgR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::R))
            return -9999;

        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        const float AttackDMG = SennaDamages::RSpell::additionalPercentageAD;
        const float APDMG = SennaDamages::RSpell::additionalPercentageAP;
        const float DMG = SennaDamages::RSpell::dmgSkillArray[levelSpell];
        const float pDMG = (DMG + (globals::localPlayer->ReadClientStat(Object::BonusAttackDamage) * AttackDMG));
        const float aDMG = ((globals::localPlayer->ReadClientStat(Object::AbilityPower) * APDMG));
        const float PhsyDMG = Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, pDMG);
        const float APPDMG = Damage::CalculateMagicalDamage(globals::localPlayer, pEnemy, aDMG);
        return PhsyDMG + APPDMG;

    }

    static bool CanKill(Object* target, float damageCalculation)
    {
        if (target == nullptr) return false;
        const float targetHealth = target->ReadClientStat(Object::Health);
        const float targetShield = target->ReadClientStat(Object::Shield);
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
            if (hero->GetDistanceTo(globals::localPlayer) > qRange()) continue;

            possible_targets.push_back(hero);
        }

        if (possible_targets.size() > 1)
            std::sort(possible_targets.begin(), possible_targets.end(),
                [](Object* pFirst, Object* pSecond) -> bool
                {
                    auto health_first = pFirst->ReadClientStat(Object::Health);
                    auto max_health_first = pFirst->ReadClientStat(Object::MaxHealth);

                    auto health_second = pSecond->ReadClientStat(Object::Health);
                    auto max_health_second = pSecond->ReadClientStat(Object::MaxHealth);

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

            if (!heroToReturn and hero->ReadClientStat(Object::Health) / hero->ReadClientStat(Object::Health) <= (SennaConfig::SennaAuto::MinHealth->Value / 100.0f)) {
                heroToReturn = hero;
                break;
            }
        }

        return heroToReturn;
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
        SennaConfig::SennaCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        SennaConfig::SennaCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        SennaConfig::SennaCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        SennaConfig::SennaCombo::UseQAlly = comboMenu->AddCheckBox("Use Q Ally", "Use SpellSlot Q for heal Ally", true);
        SennaConfig::SennaCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        const auto harassMenu = SennaMenu->AddMenu("Harass Settings", "Harass Settings");
        SennaConfig::SennaHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        SennaConfig::SennaHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = SennaMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        SennaConfig::SennaClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        SennaConfig::SennaClear::UseW = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", false);
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
        SennaConfig::SennaAuto::AutoHeal = autoMenu->AddCheckBox("Use R", "Use SpellSlot R", true);
        SennaConfig::SennaAuto::MinHealth = autoMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);
        SennaConfig::SennaAuto::MinManaHeal = autoMenu->AddSlider("minHealth", "Health Percentage Minimum", 40, 1, 100, 5);
        SennaConfig::SennaAuto::UseW = autoMenu->AddCheckBox("Use W on CC Target", "Use SpellSlot W", true);

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
        SennaConfig::SennaSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange", "Maximum Range", database.SennaQ.GetRange(), 100, database.SennaQ.GetRange(), 50);
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
        SennaConfig::SennaSpellsSettings::minRDistance = rSpellMenu->AddSlider("minRDistance", "SpellSlot R Minimum Fire Distance", 1000, 100, database.EzrealR.GetRange(), 100);
        SennaConfig::SennaSpellsSettings::maxRDistance = rSpellMenu->AddSlider("maxRDistance", "SpellSlot R Maximum Fire Distance", 3000, 100, database.EzrealR.GetRange(), 100);

        SennaConfig::SennaSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }

    void DrawDamage(Object* pEnemy) const
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastR())
            return;

        const Vector2 screenPos = Engine::GetHpBarPosition(pEnemy);

        const float endOffset2 = SennaDamages::xOffset + pEnemy->ReadClientStat(Object::Health) / pEnemy->ReadClientStat(Object::MaxHealth) * SennaDamages::widthMultiplier;
        const float damage = Senna_dmgR(pEnemy);
        const float startOffset2 = max(endOffset2 - (damage / pEnemy->ReadClientStat(Object::MaxHealth) * SennaDamages::widthMultiplier), SennaDamages::xOffset);

        const ImVec2 topLeft = CalculateTopLeft(Vector2(screenPos.x + startOffset2, screenPos.y));
        const ImVec2 bottomRight = CalculateBottomRight(screenPos, endOffset2);

        // const float targetHealth = pEnemy->ReadClientStat(Object::Health);
        const auto drawColor = CanKill(pEnemy, damage) ? COLOR_GREEN : COLOR_RED;
        render::RenderRect(topLeft, bottomRight, drawColor, 0.0f, 0, 1.0f, true);
    }

    void Senna_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastQ())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < qRange() && isTimeToCastQ())
        {
            Engine::CastSpell(SpellIndex::Q, pEnemy);
            QCastedTime = gameTime;
        }
    }

    void Senna_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastW())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= wRange())
        {
            if (pEnemy->IsMinion() || pEnemy->IsJungle()) {
                Engine::CastSpell(SpellIndex::W, pEnemy->GetPosition());
                WCastedTime = gameTime;
                return;
            }

            Modules::prediction::PredictionOutput wPrediction;
            if (GetPrediction(database.SennaW, wPrediction))
            {
                Engine::CastSpell(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void TryWOnControlledTarget() {
        if (SennaConfig::SennaAuto::UseW->Value && !isTimeToCastW()) return;

        auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
        if (wTarget != nullptr) {
            if (IsControlled(wTarget)) {
                Senna_UseW(wTarget);
            }
        }
    }

    void TryQHealAlly() {
        if (!SennaConfig::SennaAuto::AutoHeal->Value || !SennaConfig::SennaCombo::UseQAlly->Value && OrbwalkState::Attack) return;
        if (SennaConfig::SennaAuto::MinManaHeal->Value < globals::localPlayer->ReadClientStat(Object::Mana)) return;
        if (!isTimeToCastQ()) return;

        auto qAlly = GetHeroThatNeedsHeal();
        if (qAlly != nullptr) {
            if (SennaConfig::SennaAuto::MinHealth->Value > qAlly->ReadClientStat(Object::Health)) {
                Senna_UseQ(qAlly);
            }
        }
    }

    void Senna_UseR(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastR())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= rRange())
        {
            Modules::prediction::PredictionOutput rPrediction;
            if (GetPrediction(database.SennaR, rPrediction))
            {
                Engine::CastSpell(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }
        }
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
        TryWOnControlledTarget();
        TryQHealAlly();
    }

    void Combo() override {
        if (SennaConfig::SennaCombo::UseR->Value == true
            && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), SennaConfig::SennaSpellsSettings::minRDistance->Value) >= SennaConfig::SennaCombo::enemiesInRange->Value
            && isTimeToCastR())
        {
            const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
            if (rTarget != nullptr) {
                Senna_UseR(rTarget);
            }
        }

        if (SennaConfig::SennaCombo::UseW->Value == true
            && SennaConfig::SennaSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                Senna_UseW(wTarget);
            }
        }

        if (SennaConfig::SennaCombo::UseQ->Value == true
            && SennaConfig::SennaSpellsSettings::qCastMode->Value == 0
            && isTimeToCastQ())
        {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr) {
                Senna_UseQ(qTarget);
            }
        }

       
    }
    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), wRange()) > 0)
        {
            if (SennaConfig::SennaClear::UseQ->Value && isTimeToCastQ()) {
                const auto qMinion = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), qRange(), Alliance::Enemy);
                if (qMinion != nullptr) {
                    Senna_UseQ(qMinion);
                }
            }

            if (SennaConfig::SennaClear::UseW->Value && isTimeToCastW()) {
                const auto wMinion = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), wRange(), Alliance::Enemy);
                if (wMinion != nullptr) {
                    float AAdamage = Damage::CalculateAutoAttackDamage(globals::localPlayer, wMinion);
                    if (wMinion != nullptr && AAdamage * 2 < wMinion->ReadClientStat(Object::Health)) {
                        Senna_UseW(wMinion);
                    }
                }
            }
        }
        else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), wRange()) > 0)
        {
            if (SennaConfig::SennaJungle::UseW->Value && isTimeToCastW()) {
                const auto wJungle = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), wRange());
                if (wJungle != nullptr) {
                    const float AAdamage = Damage::CalculateAutoAttackDamage(globals::localPlayer, wJungle);
                    if (AAdamage * 4 > wJungle->ReadClientStat(Object::Health)) {
                        Senna_UseW(wJungle);
                    }
                }

            }

            if (SennaConfig::SennaJungle::UseQ->Value && isTimeToCastQ()) {
                const auto qJungle = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), qRange());
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
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                Senna_UseW(wTarget);
            }
        }

        if (SennaConfig::SennaHarass::UseQ->Value == true
            && isTimeToCastQ())
        {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr)
            {
                Senna_UseQ(qTarget);
            }
        }
    }

    void Lasthit() override {
        if (SennaConfig::SennaLastHit::UseQ->Value && isTimeToCastQ()) {
            const auto minion = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), qRange(), Alliance::Enemy);
            if (minion != nullptr) {
                if (CanKill(minion, Senna_dmgQ(minion))) {
                    Senna_UseQ(minion);
                }
            }
        }
    }

    void Flee() override {
        const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
        if (wTarget != nullptr) {
            if (SennaConfig::SennaFlee::UseE->Value && isTimeToCastE())
            {
                Engine::CastSpell(SpellIndex::E);
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
        __try
        {
            if (SennaConfig::SennaKillsteal::UseQ->Value && isTimeToCastQ()) {
                const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
                if (qTarget != nullptr) {
                    if (CanKill(qTarget, Senna_dmgQ(qTarget))) {
                        Senna_UseQ(qTarget);
                    }
                }

            }

            if (SennaConfig::SennaKillsteal::UseW->Value && isTimeToCastW()) {
                const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
                if (wTarget != nullptr) {
                    if (CanKill(wTarget, Senna_dmgW(wTarget))) {
                        Senna_UseW(wTarget);
                    }
                }

            }
            if (SennaConfig::SennaKillsteal::UseR->Value && isTimeToCastR()) {
                const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange());
                if (rTarget != nullptr) {
                    if (CanKill(rTarget, Senna_dmgR(rTarget))) {
                        Senna_UseR(rTarget);
                    }
                }

            }
        }
        __except (1)
        {
            LOG("ERROR IN KILLSTEAL MODE");
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
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaSpellsSettings::DrawW->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaW.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), wRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaSpellsSettings::DrawE->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaE.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), eRange(), COLOR_WHITE, 1.0f);

        if (SennaConfig::SennaSpellsSettings::DrawR->Value == true && (SennaConfig::SennaSpellsSettings::DrawIfReady->Value == true && database.SennaR.IsCastable() || SennaConfig::SennaSpellsSettings::DrawIfReady->Value == false))
			Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), rRange(), COLOR_WHITE, 1.0f);
        

        for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
            if (!hero) continue;
            if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->GetPosition().Distance(globals::localPlayer->GetPosition()) <= rRange() + hero->GetBoundingRadius() / 2)
                DrawDamage(hero);
        }
    }
};

SennaModule module;