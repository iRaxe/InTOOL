#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Twitch.h"

using namespace UPasta::SDK;


class TwitchModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Twitch");

    float gameTime = 0.0f;
    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

    [[nodiscard]] bool isTimeToCastQ() const {
        return gameTime > QCastedTime + database.TwitchQ.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::Q) && Engine::GetSpellState(Q) == 0;
    }

    [[nodiscard]] bool isTimeToCastW() const {
        return gameTime > WCastedTime + database.TwitchW.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::W) && Engine::GetSpellState(W) == 0;
    }

    [[nodiscard]] bool isTimeToCastE() const {
        return gameTime > ECastedTime + database.TwitchE.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::E) && Engine::GetSpellState(E) == 0;
    }

    [[nodiscard]] bool isTimeToCastR() const {
        return gameTime > RCastedTime + database.TwitchR.GetCastTime() && globals::localPlayer->CanCastSpell(SpellIndex::R) && Engine::GetSpellState(R) == 0;
    }

    static float qRange() {
        return static_cast<float>(TwitchConfig::TwitchSpellsSettings::qRange->Value);
    }

    static float wRange() {
        return static_cast<float>(TwitchConfig::TwitchSpellsSettings::wRange->Value);
    }

    static float eRange() {
        return static_cast<float>(TwitchConfig::TwitchSpellsSettings::eRange->Value);
    }

    static float rRange() {
        return globals::localPlayer->GetRealAttackRange() + 300.0f;
    }

    static bool HasEnoughMana(OrbwalkState mode) {
        float minManaThreshold = 0.0f;

        switch (mode) {
        case OrbwalkState::Clear:
            if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(TwitchConfig::TwitchClear::minMana->Value);
            else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
                minManaThreshold = static_cast<float>(TwitchConfig::TwitchJungle::minMana->Value);
            break;
        case OrbwalkState::Harass:
            minManaThreshold = static_cast<float>(TwitchConfig::TwitchHarass::minMana->Value);
            break;
        default:
            return false;
        }

        return globals::localPlayer->GetPercentMana() > minManaThreshold;
    }

    static bool hasE(Object* pEnemy) {
        return pEnemy->GetBuffByName("TwitchDeadlyVenom");
    }

    static int getEStacks(Object* pEnemy) {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !hasE(pEnemy))
            return -9999;

        return pEnemy->GetBuffByName("TwitchDeadlyVenom")->GetStacks();
    }

    static bool hasEnoughStacks(Object* pEnemy, int stacksAmount) {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !hasE(pEnemy))
            return -9999;

        return getEStacks(pEnemy) >= stacksAmount;
    }

public:

    TwitchModule() {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        //Creating main script menu
        const auto TwitchMenu = Menu::CreateMenu("JaySwitch", "jay.Twitch");

        //Creating first parent menu && assigning it to the champion menu
        const auto comboMenu = TwitchMenu->AddMenu("Combo Settings", "Combo Settings");

        //Assigning to UseQ the checkbox value && at the same time, the function will draw it in menu once needed
        TwitchConfig::TwitchCombo::UseQ = comboMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        TwitchConfig::TwitchCombo::UseW = comboMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        TwitchConfig::TwitchCombo::UseE = comboMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        TwitchConfig::TwitchCombo::UseEWithStacks = comboMenu->AddCheckBox("Use E Stack For Damage", "Use SpellSlot E For Stacks", true);
        TwitchConfig::TwitchCombo::minStacks = comboMenu->AddSlider("minStacks", "Minimum Stacks", 6, 1, 6, 1);

        TwitchConfig::TwitchCombo::UseR = comboMenu->AddCheckBox("Use R", "Use SpellSlot R", true);

        //Assigning to enemiesInRange the slider value && at the same time, the function will draw it in menu once needed
        TwitchConfig::TwitchCombo::enemiesInRange = comboMenu->AddSlider("minEnemiesInRange", "Minimum enemies to use R", 2, 1, 5, 1);

        //Creating second parent menu && assigning it to the champion menu
        const auto harassMenu = TwitchMenu->AddMenu("Harass Settings", "Harass Settings");
        TwitchConfig::TwitchHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        TwitchConfig::TwitchHarass::UseE = harassMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        TwitchConfig::TwitchHarass::UseEWithStacks = harassMenu->AddCheckBox("Use E Stack For Damage", "Use SpellSlot E For Damage", true);
        TwitchConfig::TwitchHarass::minStacks = harassMenu->AddSlider("minStacks", "Minimum Stacks", 6, 1, 6, 1);
        TwitchConfig::TwitchHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        //Creating third parent menu && assigning it to the champion menu
        const auto clearMenu = TwitchMenu->AddMenu("Clear Settings", "Clear Settings");

        //Creating first parent menu that is itself a child of clearMenu
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        TwitchConfig::TwitchClear::UseW = laneClearMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        TwitchConfig::TwitchClear::UseE = laneClearMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        TwitchConfig::TwitchClear::minMinion = laneClearMenu->AddSlider("minMinion", "Minimum Minions To Clear", 3, 1, 7, 1);
        TwitchConfig::TwitchClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        //Creating second parent menu that is itself a child of clearMenu
        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        TwitchConfig::TwitchJungle::UseW = jungleMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        TwitchConfig::TwitchJungle::UseE = jungleMenu->AddCheckBox("Use E", "Use SpellSlot E", true);
        TwitchConfig::TwitchJungle::UseEWithStacks = jungleMenu->AddCheckBox("Use E Stack For Damage", "Use SpellSlot E For Damage", true);
        TwitchConfig::TwitchJungle::minStacks = jungleMenu->AddSlider("minStacks", "Minimum Stacks", 6, 1, 6, 1);
        TwitchConfig::TwitchJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        //Creating fourth parent menu && assigning it to the champion menu
        const auto additionalMenu = TwitchMenu->AddMenu("Additional Settings", "Additional Settings");

        //Creating first parent menu that is itself a child of additionalMenu
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        TwitchConfig::TwitchKillsteal::UseE = ksMenu->AddCheckBox("Use E", "Use SpellSlot E", true);

        //Creating second parent menu that is itself a child of additionalMenu
        const auto fleeMenu = additionalMenu->AddMenu("Flee Settings", "Flee Settings");
        TwitchConfig::TwitchFlee::UseQ = fleeMenu->AddCheckBox("Use Q", "Use Q To Flee", false);

        //Creating first parent menu that is itself a child of additionalMenu
        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        //Creating first parent menu that is itself a child of spellsMenu
        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        TwitchConfig::TwitchSpellsSettings::qCastMode = qSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        TwitchConfig::TwitchSpellsSettings::qRange = qSpellMenu->AddSlider("maxQRange",
            "Maximum Range",
            globals::localPlayer->GetRealAttackRange() * 1.5f,
            100,
            globals::localPlayer->GetRealAttackRange() * 2.0f, 50);
        TwitchConfig::TwitchSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        //Creating second parent menu that is itself a child of spellsMenu
        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        TwitchConfig::TwitchSpellsSettings::wCastMode = wSpellMenu->AddList("castMode", "Cast Mode", std::vector<std::string>{"Doesn't Matter", "While attacking"}, 0);
        TwitchConfig::TwitchSpellsSettings::wRange = wSpellMenu->AddSlider("maxWRange", "Maximum Range", database.TwitchW.GetRange(), 100, database.TwitchW.GetRange(), 50);
        TwitchConfig::TwitchSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);

        //Creating third parent menu that is itself a child of spellsMenu
        const auto eSpellMenu = spellsMenu->AddMenu("SpellSlot E Settings", "SpellSlot E");
        TwitchConfig::TwitchSpellsSettings::eRange = eSpellMenu->AddSlider("maxERange", "Maximum Range", database.TwitchE.GetRange(), 100, database.TwitchE.GetRange(), 50);
        TwitchConfig::TwitchSpellsSettings::DrawE = eSpellMenu->AddCheckBox("Draw E", "Draw Range", true);

        //Creating fourth parent menu that is itself a child of spellsMenu
        const auto rSpellMenu = spellsMenu->AddMenu("SpellSlot R Settings", "SpellSlot R");
        TwitchConfig::TwitchSpellsSettings::DrawR = rSpellMenu->AddCheckBox("Draw R", "Draw Range", true);

        TwitchConfig::TwitchSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);

        const auto miscMenu = additionalMenu->AddMenu("Hp bar", "Damage Drawings");
        TwitchConfig::TwitchHPBAR::DrawEDamage = miscMenu->AddCheckBox("DrawRDamage", "Draw E Damage", true);
    }

    static float Twitch_dmgE(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !globals::localPlayer->CanCastSpell(SpellIndex::E))
            return -9999;

        if (hasE(pEnemy))
        {
            const int stacks = getEStacks(pEnemy);
            if (stacks > 0)
            {
                const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::E)->GetLevel();
                float base = levelSpell * 10 + 10;
                const int eDMG = 15 + (levelSpell - 1) * 5;
                float attackDamage = base + (eDMG + (globals::localPlayer->GetBonusAttackDamage() * 0.35f) * stacks);
                const float pDMG = Damage::CalculatePhysicalDamage(globals::localPlayer, pEnemy, attackDamage);
                float abilityPowerDamage = (globals::localPlayer->GetAbilityPower() * .30) * stacks;

                const float aDMG = Damage::CalculateMagicalDamage(globals::localPlayer, pEnemy, abilityPowerDamage);
                return pDMG + aDMG;
            }
        }

        return -9999;
    }

    static ImVec2 CalculateTopLeft(const Vector2& basePos) {
        return { basePos.x, basePos.y - TwitchDamages::yOffset };
    }

    static ImVec2 CalculateBottomRight(const Vector2& basePos, float widthLimit) {
        return { basePos.x + widthLimit, basePos.y - TwitchDamages::yOffset + 10.0f };
    }

    void DrawDamage(Object* pEnemy) const
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastE() || !hasE(pEnemy))
            return;

        const Vector2 screenPos = Engine::GetHpBarPosition(pEnemy);

        const float endOffset2 = TwitchDamages::xOffset + pEnemy->GetHealth() / pEnemy->GetMaxHealth() * TwitchDamages::widthMultiplier;
        const float damage = Twitch_dmgE(pEnemy);
        const float startOffset2 = max(endOffset2 - (damage / pEnemy->GetMaxHealth() * TwitchDamages::widthMultiplier), TwitchDamages::xOffset);

        const ImVec2 topLeft = CalculateTopLeft(Vector2(screenPos.x + startOffset2, screenPos.y));
        const ImVec2 bottomRight = CalculateBottomRight(screenPos, endOffset2);

        const float targetHealth = pEnemy->GetHealth();
        const bool canKill = damage > targetHealth;
        const auto drawColor = canKill ? COLOR_GREEN : COLOR_RED;
        render::RenderRect(topLeft, bottomRight, drawColor, 0.0f, 0, 1.0f, true);
    }

    void Twitch_UseQ(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastQ())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= qRange())
        {
            Engine::CastSelf(SpellIndex::Q);
            QCastedTime = gameTime;
        }
    }

    void Twitch_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastW())
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) <= wRange())
        {

            if (pEnemy->IsMinion() || pEnemy->IsJungle()) {
                Engine::CastToPosition(SpellIndex::W, pEnemy->GetPosition());
                WCastedTime = gameTime;
                return;
            }
            Modules::prediction::PredictionOutput wPrediction;
            if (GetPrediction(database.TwitchW, wPrediction))
            {
                Engine::CastToPosition(SpellIndex::W, wPrediction.position);
                WCastedTime = gameTime;
            }
        }
    }

    void Twitch_UseE(Object* pEnemy) {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !isTimeToCastE())
            return;

        if (pEnemy->GetDistanceTo(globals::localPlayer) <= eRange())
        {
            Engine::CastSelf(SpellIndex::E);
            ECastedTime = gameTime;
        }
    }

    void Twitch_UseR() {
        if (globals::localPlayer == nullptr || !isTimeToCastR())
            return;

        Engine::CastSelf(SpellIndex::R);
        RCastedTime = gameTime;
    }

    void Update() override {
        gameTime = Engine::GetGameTime();
        Killsteal();
    }

    void Combo() override {

        if (TwitchConfig::TwitchCombo::UseE->Value == true
            && isTimeToCastE())
        {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), eRange());
            if (eTarget != nullptr)
            {
                const bool canKillTarget = eTarget->GetHealth() + eTarget->GetShield() < Twitch_dmgE(eTarget);
                const bool shouldUseE = TwitchConfig::TwitchCombo::UseEWithStacks->Value && hasEnoughStacks(eTarget, TwitchConfig::TwitchCombo::minStacks->Value);

                if (canKillTarget || shouldUseE) {
                    Twitch_UseE(eTarget);
                }
            }
        }

        if (TwitchConfig::TwitchCombo::UseW->Value == true
            && TwitchConfig::TwitchSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr)
            {
                Twitch_UseW(wTarget);
            }
        }

        if (TwitchConfig::TwitchCombo::UseQ->Value == true
            && TwitchConfig::TwitchSpellsSettings::qCastMode->Value == 0
            && isTimeToCastQ())
        {
            const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), qRange());
            if (qTarget != nullptr)
            {
                Twitch_UseQ(qTarget);
            }
        }

        if (TwitchConfig::TwitchCombo::UseR->Value == true
            && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), rRange()) >= TwitchConfig::TwitchCombo::enemiesInRange->Value
            && isTimeToCastR())
        {
            Twitch_UseR();
        }
    }
    void Clear() override {
        if (!HasEnoughMana(OrbwalkState::Clear)) return;

        const auto minionsInRange = ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), eRange());

        if (minionsInRange > 0) {
            if (TwitchConfig::TwitchClear::UseE->Value && isTimeToCastE()) {
                int kill_count = 0;
                for (auto minion : ObjectManager::GetHeroesAs(Alliance::Enemy))
                {
                    if (!minion) continue;
                    if (minion->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > eRange()) continue;
                    if (minion->GetHealth() < Twitch_dmgE(minion)) {
                        kill_count++;
                        if (kill_count >= TwitchConfig::TwitchClear::minMinion->Value) {
                            Twitch_UseE(minion);
                            break;
                        }
                    }
                }
            }

            if (TwitchConfig::TwitchClear::UseW->Value && isTimeToCastW()) {
                const auto wTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), wRange(), Alliance::Enemy);
                if (wTarget != nullptr) {
                    Twitch_UseW(wTarget);
                }
            }
        }
        else {
            const auto jungleMonstersInRange = ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), wRange());
            if (jungleMonstersInRange > 0) {
                Object* highestHpMonster = nullptr;
                float highestHp = 0.0f;
                int totalAAsNeeded = 0;

                for (auto monster : ObjectManager::GetJungleMonsters())
                {
                    if (!monster) continue;
                    if (monster->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > wRange()) continue;

                    float AAdamage = Damage::CalculateAutoAttackDamage(globals::localPlayer, monster);
                    totalAAsNeeded += std::ceil(monster->GetHealth() / AAdamage);

                    if (monster->GetHealth() > highestHp) {
                        highestHp = monster->GetHealth();
                        highestHpMonster = monster;
                    }

                }

                if (TwitchConfig::TwitchJungle::UseW->Value && isTimeToCastW() && totalAAsNeeded > 4 && highestHpMonster != nullptr) {
                    Twitch_UseW(highestHpMonster);
                }

                if (TwitchConfig::TwitchJungle::UseE->Value && isTimeToCastE()) {
                    for (const auto& monster : ObjectManager::GetJungleMonsters()) {
                        if (monster != nullptr && (monster->GetHealth() + monster->GetShield() < Twitch_dmgE(monster) ||
                            (TwitchConfig::TwitchJungle::UseEWithStacks->Value && hasEnoughStacks(monster, TwitchConfig::TwitchJungle::minStacks->Value)))) {
                            Twitch_UseE(monster);
                            break;
                        }
                    }
                }
            }
        }
    }

    void Harass() override {
        if (!HasEnoughMana(OrbwalkState::Harass)) return;

        if (TwitchConfig::TwitchHarass::UseW->Value
            && TwitchConfig::TwitchSpellsSettings::wCastMode->Value == 0
            && isTimeToCastW())
        {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr) {
                Twitch_UseW(wTarget);
            }
        }

        if (TwitchConfig::TwitchHarass::UseE->Value == true
            && isTimeToCastE())
        {
            const auto eTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), eRange());
            if (eTarget != nullptr)
            {
                const bool canKillTarget = eTarget->GetHealth() + eTarget->GetShield() < Twitch_dmgE(eTarget);
                const bool shouldUseE = TwitchConfig::TwitchCombo::UseEWithStacks->Value && hasEnoughStacks(eTarget, TwitchConfig::TwitchHarass::minStacks->Value);

                if (canKillTarget || shouldUseE) {
                    Twitch_UseE(eTarget);
                }
            }
        }
    }

    void Lasthit() override {
        return;
    }

    void Flee() override {
        if (TwitchConfig::TwitchFlee::UseQ->Value && isTimeToCastQ())
        {
            Engine::CastSelf(SpellIndex::Q);
            ECastedTime = gameTime;
        }
    }

    void Killsteal()
    {
        if (TwitchConfig::TwitchKillsteal::UseE->Value && isTimeToCastE())
        {
            for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
                if (!hero) continue;
                if (hero->GetPosition().Distance(globals::localPlayer->GetPosition()) > eRange() + hero->GetBoundingRadius() / 2) continue;
                if (hero->GetHealth() + hero->GetShield() < Twitch_dmgE(hero)) {
                    Twitch_UseE(hero);
                    break;
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

    void Render() override {
        if (TwitchConfig::TwitchSpellsSettings::DrawQ->Value == true && (TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == true && database.TwitchQ.IsCastable() || TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);

        if (TwitchConfig::TwitchSpellsSettings::DrawW->Value == true && (TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == true && database.TwitchW.IsCastable() || TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), wRange(), COLOR_WHITE, 1.0f);

        if (TwitchConfig::TwitchSpellsSettings::DrawE->Value == true && (TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == true && database.TwitchE.IsCastable() || TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), eRange(), COLOR_WHITE, 1.0f);

        if (TwitchConfig::TwitchSpellsSettings::DrawR->Value == true && (TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == true && database.TwitchR.IsCastable() || TwitchConfig::TwitchSpellsSettings::DrawIfReady->Value == false))
            Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), rRange(), COLOR_WHITE, 1.0f);

        if (TwitchConfig::TwitchHPBAR::DrawEDamage->Value == true) {
            for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
                if (!hero) continue;
                if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable())
                    DrawDamage(hero);
            }
        }
    }
};

TwitchModule module;