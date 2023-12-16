#include "../Awareness.h"
#include "../Damage.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "Kaisa.h"

class KaisaModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Kaisa");

private:
    float gameTime = 0.0f;

    float QCastedTime = 0.0f;
    float WCastedTime = 0.0f;
    float ECastedTime = 0.0f;
    float RCastedTime = 0.0f;

private:
    static bool hasStacks(Object* pEnemy)
    {
        return pEnemy->GetBuffByName("kaisapassivemarker");
    }

    [[nodiscard]] bool isTimeToCastQ() const
    {
        return gameTime > QCastedTime + database.KaisaQ.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastW() const
    {
        return gameTime > WCastedTime + database.KaisaW.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastE() const
    {
        return gameTime > ECastedTime + database.KaisaE.GetCastTime();
    }

    [[nodiscard]] bool isTimeToCastR() const
    {
        return gameTime > RCastedTime + database.KaisaR.GetCastTime();
    }

    static float qRange() {
        return 660.0f;
    }

    static float wRange() {
        return database.KaisaW.GetMaxRange();
    }

    static float rRange() {
        const int Rlevel = globals::localPlayer->GetSpellBySlotId(SpellIndex::R)->GetLevel();
        return KaisaDamages::RSpell::Rrange[Rlevel];
    }

    float q_dmg(Object* pEnemy) const {
        return Damage::CalculateSlotMixedDamage<float*, float, float>(SpellIndex::Q, pEnemy, KaisaDamages::QSpell::dmgSkillArray, KaisaDamages::QSpell::additionalPercentageAP, KaisaDamages::QSpell::additionalPercentageAD);
    }

    float w_dmg(Object* pEnemy) const {
        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();
        return Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::W, pEnemy, KaisaDamages::WSpell::dmgSkillArray, KaisaDamages::QSpell::additionalPercentageAD);
    }

public:

    KaisaModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Initialize() override
    {
        const auto KaisaMenu = Menu::CreateMenu("m0sKaisa", "Kai'sa");

            const auto qSettings = KaisaMenu->AddMenu("Q Settings", "[Q] Icathian Rain");
        KaisaConfig::QConfig::AutoQ = qSettings->AddCheckBox("Auto Q", "Auto Q", true);
        KaisaConfig::QConfig::AutoQ->AddTooltip("Only Isolated Target");
        KaisaConfig::QConfig::ComboQ = qSettings->AddList("qMode", "Q Mode", std::vector<std::string>{"Isolated Target", "Always", "Never"}, 1);

        const auto wSettings = KaisaMenu->AddMenu("W Settings", "[W] Void Seeker");
        KaisaConfig::WConfig::ComboW = wSettings->AddList("wMode", "Use in Combo", std::vector<std::string>{"Only on CC", "Always", "Never"}, 1);
        KaisaConfig::WConfig::ComboW->AddTooltip("'Only on CC' ignores stack check \n 'Always' doesnt ignore stack check");
        KaisaConfig::WConfig::wRange = wSettings->AddSlider("maxWrange", "[Combo] Maximum range to check", 1000, 500, 2500, 100);
        KaisaConfig::WConfig::wstack = wSettings->AddSlider("minStacks", "Minimum stacks", 3, 0, 4, 1);

        const auto eSettings = KaisaMenu->AddMenu("E Settings", "[E] Supercharge");
        KaisaConfig::EConfig::ComboE = eSettings->AddList("eMode", "Use in Combo", std::vector<std::string>{"Always", "After Attack", "Never"}, 0);

        const auto rSettings = KaisaMenu->AddMenu("R Settings", "[R] Killer Instinct");
        KaisaConfig::RConfig::ComboR = rSettings->AddCheckBox("Combo R", "Use in combo", true);
        KaisaConfig::RConfig::Rcond1 = rSettings->AddSlider("r cond1", "Target < HP %", 30, 1, 100, 5);
        KaisaConfig::RConfig::Rcond2 = rSettings->AddSlider("r cond2", "Player > HP %% to use R", 60, 1, 100, 5);
        KaisaConfig::RConfig::Rcond3 = rSettings->AddCheckBox("r cond3", "Dont use R under tower", true);


        const auto harassMenu = KaisaMenu->AddMenu("Harass Settings", "Harass Settings");
        KaisaConfig::KaisaHarass::UseQ = harassMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KaisaConfig::KaisaHarass::UseW = harassMenu->AddCheckBox("Use W", "Use SpellSlot W", true);
        KaisaConfig::KaisaHarass::minMana = harassMenu->AddSlider("minHarassMana", "Minimum Mana", 60, 1, 100, 5);

        const auto clearMenu = KaisaMenu->AddMenu("Clear Settings", "Clear Settings");
        const auto laneClearMenu = clearMenu->AddMenu("Laneclear Settings", "Laneclear Settings");
        KaisaConfig::KaisaClear::UseQ = laneClearMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KaisaConfig::KaisaClear::Qmin = laneClearMenu->AddSlider("minminions", "Minimum # of minions to use q", 3, 0, 5, 1);
        KaisaConfig::KaisaClear::minMana = laneClearMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto jungleMenu = clearMenu->AddMenu("Jungleclear Settings", "Jungleclear Settings");
        KaisaConfig::KaisaJungle::UseQ = jungleMenu->AddCheckBox("Use Q", "Use SpellSlot Q", true);
        KaisaConfig::KaisaJungle::minMana = jungleMenu->AddSlider("minClearMana", "Minimum Mana", 60, 1, 100, 5);

        const auto additionalMenu = KaisaMenu->AddMenu("Additional Settings", "Additional Settings");
        const auto ksMenu = additionalMenu->AddMenu("Killsteal Settings", "Killsteal Settings");
        KaisaConfig::KaisaKillsteal::UseW = ksMenu->AddCheckBox("Use W", "Use W to Killsteal", true);
        KaisaConfig::KaisaKillsteal::UseW->AddTooltip("ignores stack check");
        KaisaConfig::KaisaKillsteal::wksRange = ksMenu->AddSlider("maxWksrange", "[Killsteal] Maximum range to check", 2000, 500, 2500, 100);

        const auto antiGapMenu = additionalMenu->AddMenu("AntiGapCloser Settings", "AntiGapCloser Settings");
        KaisaConfig::KaisaAntiGapCloser::eMode = antiGapMenu->AddList("eMode", "Dash To", std::vector<std::string>{"Near Mouse", "Extend Enemy Dash Position"}, 0);
        KaisaConfig::KaisaAntiGapCloser::UseE = antiGapMenu->AddCheckBox("Use E", "Use SpellSlot E To Mouse", false);
        const auto antiGapwhitelistMenu = antiGapMenu->AddMenu("Whitelist Settings", "Whitelist");

        const auto antiMeleeMenu = additionalMenu->AddMenu("AntiMelee Settings", "AntiMelee Settings");
        KaisaConfig::KaisaAntiMelee::eMode = antiMeleeMenu->AddList("eMode", "Dash To", std::vector<std::string>{"Near Mouse", "Extend Enemy Dash Position"}, 0);
        KaisaConfig::KaisaAntiMelee::UseE = antiMeleeMenu->AddCheckBox("Use E", "Use SpellSlot E To Mouse", false);
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
                        if (self->Value == false && !KaisaConfig::KaisaAntiGapCloser::whitelist.empty())
                        {
                            const auto it = std::ranges::find(KaisaConfig::KaisaAntiGapCloser::whitelist, obj);
                            KaisaConfig::KaisaAntiGapCloser::whitelist.erase(it);
                        }
                        else
                        {
                            KaisaConfig::KaisaAntiGapCloser::whitelist.push_back(obj);
                        }
                    });

                if (antiGap_checkbox->Value == true)
                {
                    KaisaConfig::KaisaAntiGapCloser::whitelist.push_back(obj);
                }

                if (!obj->IsMelee()) continue;
                const auto antiMelee_checkbox = antiMeleewhitelistMenu->AddCheckBox(obj->GetName().c_str(),
                    obj->GetName().c_str(),
                    true,
                    [obj]
                    (const CheckBox* self, bool newValue)
                    {
                        if (self->Value == false && !KaisaConfig::KaisaAntiMelee::whitelist.empty())
                        {
                            const auto it = std::ranges::find(KaisaConfig::KaisaAntiMelee::whitelist, obj);
                            KaisaConfig::KaisaAntiMelee::whitelist.erase(it);
                        }
                        else
                        {
                            KaisaConfig::KaisaAntiMelee::whitelist.push_back(obj);
                        }
                    });

                if (antiMelee_checkbox->Value == true)
                {
                    KaisaConfig::KaisaAntiMelee::whitelist.push_back(obj);
                }
            }
        }
        const auto spellsMenu = additionalMenu->AddMenu("Spells Settings", "Spells Settings");

        const auto qSpellMenu = spellsMenu->AddMenu("SpellSlot Q Settings", "SpellSlot Q");
        KaisaConfig::KaisaSpellsSettings::DrawQ = qSpellMenu->AddCheckBox("Draw Q", "Draw Range", true);

        const auto wSpellMenu = spellsMenu->AddMenu("SpellSlot W Settings", "SpellSlot W");
        KaisaConfig::KaisaSpellsSettings::DrawW = wSpellMenu->AddCheckBox("Draw W", "Draw Range", true);


        KaisaConfig::KaisaSpellsSettings::DrawIfReady = spellsMenu->AddCheckBox("DrawIfReady", "Draw SpellSlots Only If Ready", true);
    }


    void Kaisa_UseAbility(Object* pEnemy, SpellIndex spellID)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr) return;

        auto isCastable = [&]() -> bool {
            switch (spellID) {
            case Q: return database.KaisaQ.IsCastable();
            case W: return database.KaisaW.IsCastable();
            case E: return database.KaisaE.IsCastable();
            case R: return database.KaisaR.IsCastable();
            default: return false;
            }};

        if (!isCastable()) return;

        auto abilityRange = [&]() -> float {
            switch (spellID) {
            case Q: return qRange();
            case W: return wRange();
            case E: return database.KaisaE.GetMaxRange();
            case R: return database.KaisaR.GetMaxRange();
            default: return 0.0f;
            }};

        if (pEnemy->GetDistanceTo(globals::localPlayer) > abilityRange()) return;

        auto isTimeToCast = [&]() -> bool {
            switch (spellID) {
            case Q: return isTimeToCastQ();
            case W: return isTimeToCastW();
            case E: return isTimeToCastE();
            case R: return isTimeToCastR();
            default: return false;
            }};

        if (!isTimeToCast()) return;

        auto handleSpellCast = [&](Skillshot spellData) {
            Modules::prediction::PredictionOutput prediction;
            if (GetPrediction(globals::localPlayer, pEnemy, spellData, prediction))
                Engine::CastToPosition(spellID, prediction.position);
            };

        switch (spellID) {
        case Q:
            handleSpellCast(database.KaisaQ);
            QCastedTime = gameTime;

            break;
        case W:
            handleSpellCast(database.KaisaW);
            WCastedTime = gameTime;
            break;
        case E:
            Engine::CastToPosition(E, Engine::GetMouseWorldPos());
            ECastedTime = gameTime;
            break;
        case R:
            handleSpellCast(database.KaisaR);
            RCastedTime = gameTime;

            break;
        default:
            break;
        }

    }

    void Update() override
    {
        gameTime = Engine::GetGameTime();

        Killsteal();
        AntiGapCloser();
        AntiMelee();
        //ObjectManager::CountMinionsInRange(Alliance::Enemy, 
        if (KaisaConfig::QConfig::AutoQ->Value == true && database.KaisaQ.IsCastable())
        {
            if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) == 0 && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) == 1)
            {
                Engine::CastSelf(SpellIndex::Q);
            }
        }
    }

    void Combo() override
    {
        if (KaisaConfig::EConfig::ComboE->Value == 1 && database.KaisaE.IsCastable())
        {
            const auto attackedTarget = Engine::GetSelectedObject();
            if (attackedTarget != nullptr)
            {
                Engine::CastSelf(SpellIndex::E);
            }

        }

        if (KaisaConfig::QConfig::ComboQ->Value == 0 && database.KaisaQ.IsCastable())
        {
            if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange())== 0 && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) == 1)
            {
                Engine::CastSelf(SpellIndex::Q);
            }
        }

        if (KaisaConfig::QConfig::ComboQ->Value == 1 && database.KaisaQ.IsCastable())
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Kaisa_UseAbility(qTarget, Q);
        }
        if (KaisaConfig::EConfig::ComboE->Value == 1 && database.KaisaE.IsCastable())
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Engine::CastSelf(SpellIndex::E);
        }

        if (KaisaConfig::WConfig::ComboW->Value == 0 && database.KaisaW.IsCastable())
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
            {
                if (wTarget->GetBuffByType(BuffType::Stun))
                {
                    Kaisa_UseAbility(wTarget, W);
                }
            }

        }
        if (KaisaConfig::WConfig::ComboW->Value == 1 && database.KaisaW.IsCastable())
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
            {
                auto stacks = wTarget->GetBuffByName("kaisapassivemarker");
                if (stacks->GetStacks() >= KaisaConfig::WConfig::wstack->Value)
                {
                    Kaisa_UseAbility(wTarget, W);
                }

            }

        }
        if (KaisaConfig::RConfig::ComboR->Value == 1 && database.KaisaR.IsCastable())
        {
            if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange()))
            {
                int meHP = globals::localPlayer->GetPercentHealth();
                int targetHP = rTarget->GetPercentHealth();
                float R_Radius = 525;
                float highestDistance = 0;
                Vector3 bestPoint;
                if (rTarget->GetBuffByName("kaisapassivemarker") && meHP >= KaisaConfig::RConfig::Rcond2->Value && targetHP <= KaisaConfig::RConfig::Rcond1->Value)
                {
                    for (int point = 0; point < 360; point += 20)
                    {
                        float point_temp = M_PI / 180.0f * point;
                        float pX = R_Radius * cosf(point_temp) + rTarget->GetPosition().x;
                        float pY = rTarget->GetPosition().y;
                        float pZ = R_Radius * cosf(point_temp) + rTarget->GetPosition().z;


                        if (Vector3(pX, pY, pZ).distanceTo(rTarget->GetPosition()) > highestDistance)
                        {
                            if (KaisaConfig::RConfig::Rcond3->Value == 1)
                            {
                               // if (!Vector3(pX, pY, pZ).IsUnderEnemyTower())
                                {
                                    highestDistance = Vector3(pX, pY, pZ).distanceTo(rTarget->GetPosition());
                                    bestPoint = Vector3(pX, pY, pZ);
                                    Engine::CastToPosition(R, bestPoint);
                                    RCastedTime = gameTime;
                                }

                            }
                            else
                            {
                                highestDistance = Vector3(pX, pY, pZ).distanceTo(rTarget->GetPosition());
                                bestPoint = Vector3(pX, pY, pZ);

                                Engine::CastToPosition(R, bestPoint);
                                RCastedTime = gameTime;
                            }

                        }



                    }

                }

            }

        }



    }

    void Clear() override
    {
        //Laneclear
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), qRange()) >= KaisaConfig::KaisaClear::Qmin->Value)
        {

            if (KaisaConfig::KaisaClear::UseQ->Value == true && database.KaisaQ.IsCastable())
            {
                Engine::CastSelf(SpellIndex::Q);
            }
        }

        //Jungleclear
        else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), qRange()) > 0)
        {

            if (KaisaConfig::KaisaJungle::UseQ->Value == true && database.KaisaQ.IsCastable())
            {
                if (const auto qTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), qRange()))
                    Kaisa_UseAbility(qTarget, Q);
            }
        }
    }


    void Harass() override
    {

        if (KaisaConfig::KaisaHarass::UseW->Value == true && database.KaisaW.IsCastable())
        {
            if (const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange()))
                Kaisa_UseAbility(wTarget, W);
        }

        if (KaisaConfig::KaisaHarass::UseQ->Value == true && database.KaisaQ.IsCastable())
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),qRange()))
                Kaisa_UseAbility(qTarget, Q);
        }
    }

    void Killsteal()
    {
        __try {

            if (KaisaConfig::KaisaKillsteal::UseW->Value == true && database.KaisaW.IsCastable())
            {
                const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),KaisaConfig::KaisaKillsteal::wksRange->Value);
                if (wTarget != nullptr
                    && wTarget->GetDistanceTo(globals::localPlayer) <= KaisaConfig::KaisaKillsteal::wksRange->Value
                    && wTarget->GetHealth() < w_dmg(wTarget))
                {
                    Kaisa_UseAbility(wTarget, W);
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
        if (KaisaConfig::KaisaAntiGapCloser::UseE->Value == true && Engine::GetSpellState(SpellIndex::E) == 0)
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (!Engine::MenuItemContains(KaisaConfig::KaisaAntiGapCloser::whitelist, target->GetName().c_str())) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > 450.0f) continue;

                if (!target->GetAiManager()->IsDashing()) continue;
                if (target->GetBuffByName("rocketgrab2")) continue;

                if (target != nullptr)
                {
                    const Vector3 pathEnd = target->GetAiManager()->GetPathEnd();
                    if (pathEnd.IsValid() && globals::localPlayer->GetPosition().distanceTo(pathEnd) < 350.0f)
                    {
                        Engine::CastSelf(SpellIndex::E);
                    }
                }
            }
        }
    }

    void AntiMelee()
    {
        if (KaisaConfig::KaisaAntiMelee::UseE->Value == true && Engine::GetSpellState(SpellIndex::E) == 0)
        {
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetPosition().distanceTo(globals::localPlayer->GetPosition()) > 450.0f) continue;
                if (!Engine::MenuItemContains(KaisaConfig::KaisaAntiMelee::whitelist, target->GetName().c_str())) continue;

                if (target != nullptr && target->GetDistanceTo(globals::localPlayer) < target->GetRealAttackRange())
                {
                    const Vector3 pathEnd = Engine::GetMouseWorldPos();
                    if (pathEnd.IsValid() && globals::localPlayer->GetPosition().distanceTo(pathEnd) < 350.0f)
                    {
                        Engine::CastSelf(SpellIndex::E);
                    }
                }
            }
        }
    }

    void Lasthit() override {
        return;
    }

    void Flee() override {
        return;
    }

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
        __try {
            if (KaisaConfig::KaisaSpellsSettings::DrawQ->Value == true && (KaisaConfig::KaisaSpellsSettings::DrawIfReady->Value == true && database.KaisaQ.IsCastable() || KaisaConfig::KaisaSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), qRange(), COLOR_WHITE, 1.0f);

            if (KaisaConfig::KaisaSpellsSettings::DrawW->Value == true && (KaisaConfig::KaisaSpellsSettings::DrawIfReady->Value == true && database.KaisaW.IsCastable() || KaisaConfig::KaisaSpellsSettings::DrawIfReady->Value == false))
                Awareness::Functions::Radius::DrawRadius(globals::localPlayer->GetPosition(), wRange(), COLOR_PURPLE, 1.0f);

        }
        __except (1)
        {
            LOG("ERROR IN RENDER MODE");
        }
    }
};

KaisaModule module;