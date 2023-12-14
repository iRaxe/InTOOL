#include "Ashe.h"
#include "../Awareness.h"
#include "../imgui_notify.h"
#include "../ListManager.h"
#include "../stdafx.h"
#include "../TargetSelector.h"

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

    void Initialize() override
    {
        const auto AsheMenu = Menu::CreateMenu("vezAshe", "Champion Settings");

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

        const float attackDamage = globals::localPlayer->ReadClientStat(Object::TotalAttackDamage);
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

        const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
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
            Engine::CastSpell(SpellIndex::Q, globals::localPlayer);
            QCastedTime = gameTime;
        }
    }

    void Ashe_UseW(Object* pEnemy)
    {
        if (globals::localPlayer == nullptr || pEnemy == nullptr || !database.AsheW.IsCastable() || AsheConfig::AsheSpellsSettings::UseWIfFullAASpeed->Value && globals::localPlayer->ReadClientStat(Object::AttackSpeed) > 2.0f)
            return;

        if (pEnemy && pEnemy->GetDistanceTo(globals::localPlayer) < wRange() && isTimeToCastW())
        {
            Modules::prediction::PredictionOutput wPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.AsheW, wPrediction))
            {
                Engine::CastSpell(SpellIndex::W, wPrediction.position);
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
            Modules::prediction::PredictionOutput rPrediction;
            if (GetPrediction(globals::localPlayer, pEnemy, database.AsheR, rPrediction))
            {
                Engine::CastSpell(SpellIndex::R, rPrediction.position);
                RCastedTime = gameTime;
            }
        }
    }

    /*void test()
    {
        int spellSlotToFind = 1;
        std::string playerNameToFind = "Ashe";
        std::string spellNameToFind = "Volley";

        Object* ownerPointer = Engine::GetPlayerPointer("Ashe");
        if (ownerPointer != nullptr)
            LOG("%d", ownerPointer->GetNetId());

    	int cooldown = ListManager::Functions::playerSpells[playerNameToFind][spellSlotToFind][spellNameToFind][1];
        if (cooldown != 0)
			LOG("Trovato: Il cooldown è : %d", cooldown);

        std::vector<int> cooldowns = Engine::getCooldownData<std::vector<int>>(playerNameToFind, spellNameToFind);
        if (!cooldowns.empty())
        {
            LOG("Trovato: PlayerName = %s, SpellName = %s, Cooldowns: \n", playerNameToFind, spellNameToFind);
            for (int cooldown : cooldowns)
            {
                LOG("| %d |", cooldown);
            }
        }

        // Utilizza la funzione generica per ottenere il proprietario dell'abilità
        std::string owner = Engine::getCooldownData<std::string>("", spellNameToFind);
        if (!owner.empty()) 
            LOG("Trovato: Il proprietario di %s è : %s", spellNameToFind, owner);

        // Utilizza la funzione generica per ottenere lo slot dell'abilità
        int spellSlot = Engine::getCooldownData<int>(playerNameToFind, spellNameToFind);
        if (spellSlot != -1) 
            LOG("Trovato: Lo slot dell'abilità %s di %s è : %d", spellNameToFind, playerNameToFind, spellSlot);

    }*/

    void test2()
    {

            for (auto& obj : globals::heroManager->units_map)
            {
                uintptr_t network_id = obj.first;
                Object* object = obj.second;

                if (IsNotZeroPtr(object) && IsValidPtr(object) && object)
                {
                    LOG("%p", object);
                }
            }
    }


    void DrawStatsTest(Object* test) {
        float yOffset = 0;  // Inizializza l'offset

        render::RenderTextWorld("AttackRange: " + std::to_string(test->ReadClientStat(Object::AttackRange)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("Health: " + std::to_string(test->ReadClientStat(Object::Health)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("BonusHealth: " + std::to_string(test->ReadClientStat(Object::BonusHealth)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("MaxHealth: " + std::to_string(test->ReadClientStat(Object::MaxHealth)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("LifeRegeneration: " + std::to_string(test->ReadClientStat(Object::LifeRegeneration)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("Mana: " + std::to_string(test->ReadClientStat(Object::Mana)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("MaxMana: " + std::to_string(test->ReadClientStat(Object::MaxMana)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("BaseAttackDamage: " + std::to_string(test->ReadClientStat(Object::BaseAttackDamage)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("BonusAttackDamage: " + std::to_string(test->ReadClientStat(Object::BonusAttackDamage)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("AbilityHaste: " + std::to_string(test->ReadClientStat(Object::AbilityHaste)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("Lethality: " + std::to_string(test->ReadClientStat(Object::Lethality)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("ArmorPenetrationFlat: " + std::to_string(test->ReadClientStat(Object::ArmorPenetrationFlat)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("MagicPenetrationFlat: " + std::to_string(test->ReadClientStat(Object::MagicPenetrationFlat)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("MagicPenetrationMultiplier: " + std::to_string(test->ReadClientStat(Object::MagicPenetrationMultiplier)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("AbilityPower: " + std::to_string(test->ReadClientStat(Object::AbilityPower)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("BonusArmor: " + std::to_string(test->ReadClientStat(Object::BonusArmor)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("TotalArmor: " + std::to_string(test->ReadClientStat(Object::TotalArmor)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("Shield: " + std::to_string(test->ReadClientStat(Object::Shield)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("PhysicalShield: " + std::to_string(test->ReadClientStat(Object::PhysicalShield)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("MagicalShield: " + std::to_string(test->ReadClientStat(Object::MagicalShield)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("CritFlat: " + std::to_string(test->ReadClientStat(Object::CritFlat)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("CritMultiplier: " + std::to_string(test->ReadClientStat(Object::CritMultiplier)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
    	render::RenderTextWorld("AtackSpeedMulti: " + std::to_string(test->ReadClientStat(Object::AttackSpeed)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("BonusMagicResist: " + std::to_string(test->ReadClientStat(Object::BonusMagicResist)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("TotalMagicResist: " + std::to_string(test->ReadClientStat(Object::TotalMagicResist)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("MovementSpeed: " + std::to_string(test->ReadClientStat(Object::MovementSpeed)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("Ammo: " + std::to_string(test->ReadClientStat(Object::Ammo)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("MaxAmmo: " + std::to_string(test->ReadClientStat(Object::MaxAmmo)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

    	render::RenderTextWorld("ScaleMulti: " + std::to_string(test->ReadClientStat(Object::ScaleMulti)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

        render::RenderTextWorld("Experience: " + std::to_string(test->ReadClientStat(Object::Experience)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;
        render::RenderTextWorld("Level: " + std::to_string(test->ReadClientStat(Object::Level)), Vector3(test->GetPosition().x, test->GetPosition().y + yOffset, test->GetPosition().z), 16.0f, COLOR_WHITE, true); yOffset += 20;

    }

    void Update() override
    {
        gameTime = Engine::GetGameTime();
        Killsteal();
        AntiGapCloser(); 
        AntiMelee();
        //LOG("AAAA %s", globals::localPlayer->GetSpellBySlotId(0)->GetSpellInfo()->GetSpellData()->GetTexturePath().c_str());

        //LOG("%f", Functions::GetSpellRange(globals::localPlayer->GetSpellBySlotId(1)));
        //auto rune1 = globals::localPlayer->GetHeroPerks()->GetPerkByIndex(1);
        //auto spell = globals::localPlayer->GetSpellBySlotId(1)->GetSpellInfo();
        //LOG("%f", ListManager::Functions::GetCooldownFromChampSpellMap(globals::localPlayer->GetNetId(), 1));
        //LOG("%f", globals::localPlayer->GetSpellBySlotId(1)->GetSpellInfo()->GetSpellData()->GetCooldownArray()->GetArrayIndex(1)->GetBaseCooldown());

        /*auto reduction = 100 / (100 + globals::localPlayer->GetAbilityHaste());
        LOG("REDUCTION %f", reduction);
        LOG("TIME READY %f", 30 + 24 * reduction);*/

        if (AsheConfig::AsheSpellsSettings::rTapKey->Value == true && database.AsheR.IsCastable())
        {
            switch (AsheConfig::AsheSpellsSettings::targetMode->Value)
            {
            case 0: //Inherit
                if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange()))
                    Ashe_UseR(rTarget);
                break;
            case 1: //NearMouse
                if (const auto rTarget2 = TargetSelector::FindBestTarget(Engine::GetMouseWorldPos(), 300.0f))
                    Ashe_UseR(rTarget2);
                break;
            case 2: //NearChampion
                if (const auto rTarget3 = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), 500.0f))
                    Ashe_UseR(rTarget3);
                break;
            }
        }
    }

    void Combo() override
    {
        if (AsheConfig::AsheCombo::UseR->Value == true && database.AsheR.IsCastable()
            && ObjectManager::CountHeroesInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), AsheConfig::AsheSpellsSettings::minRDistance->Value) >= AsheConfig::AsheCombo::enemiesInRange->Value)
        {
            if (const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), rRange()))
                Ashe_UseR(rTarget);
        }

        if (AsheConfig::AsheCombo::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
        {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr && Ashe_CanCastW(wTarget))
            	Ashe_UseW(wTarget);
        }

        if (AsheConfig::AsheCombo::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), aaRange()))
                Ashe_UseQ(qTarget);
        }
    }

    void Clear() override
    {
        //Laneclear
        if (ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), wRange()) > 0)
        {
            if (!Ashe_HasEnoughMana(AsheConfig::AsheClear::minMana->Value)) return;

            if (AsheConfig::AsheClear::UseW->Value == true && database.AsheW.IsCastable() && ObjectManager::CountMinionsInRange(Alliance::Enemy, globals::localPlayer->GetPosition(), wRange()) >= AsheConfig::AsheClear::minWMinions->Value && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
            {
                const auto wTarget = TargetSelector::FindBestMinion(globals::localPlayer->GetPosition(), wRange(), Alliance::Enemy);
                if (wTarget != nullptr && wTarget->ReadClientStat(Object::Health) < Ashe_dmgW(wTarget) && Ashe_CanCastW(wTarget))
                    Ashe_UseW(wTarget);
            }
        }

        //Jungleclear
        else if (ObjectManager::CountJungleMonstersInRange(globals::localPlayer->GetPosition(), wRange()) > 0)
        {
            if (!Ashe_HasEnoughMana(AsheConfig::AsheJungle::minMana->Value)) return;

            if (AsheConfig::AsheJungle::UseW->Value == true && database.AsheW.IsCastable() && AsheConfig::AsheSpellsSettings::wCastMode->Value == 0)
            {
                const auto wTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), wRange());
                if (wTarget != nullptr && Ashe_CanCastW(wTarget))
                    Ashe_UseW(wTarget);
            }

            if (AsheConfig::AsheJungle::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 0)
            {
                if (const auto qTarget = TargetSelector::FindBestJungle(globals::localPlayer->GetPosition(), aaRange()))
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
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange());
            if (wTarget != nullptr && Ashe_CanCastW(wTarget))
                Ashe_UseW(wTarget);
        }

        if (AsheConfig::AsheHarass::UseQ->Value == true && Ashe_IsQReady() && AsheConfig::AsheSpellsSettings::qCastMode->Value == 0)
        {
            if (const auto qTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), aaRange()))
                Ashe_UseQ(qTarget);
        }
    }

    void Lasthit() override
    {
        if (!Ashe_HasEnoughMana(AsheConfig::AsheLastHit::minMana->Value))
            return;

        if (AsheConfig::AsheLastHit::UseW->Value == true && database.AsheW.IsCastable())
        {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr && wTarget->ReadClientStat(Object::Health) < Ashe_dmgW(wTarget))
                Ashe_UseW(wTarget);
        }
    }

    void Flee() override
    {
        if (AsheConfig::AsheFlee::UseW->Value == true && database.AsheW.IsCastable())
        {
            const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(), wRange());
            if (wTarget != nullptr)
                Ashe_UseW(wTarget);
        }
    }

    void Killsteal()
    {
        __try {
            if (AsheConfig::AsheKillsteal::UseW->Value == true && database.AsheW.IsCastable())
            {
                const auto wTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),wRange());
                if (wTarget != nullptr && wTarget->ReadClientStat(Object::Health) < Ashe_dmgW(wTarget))
                {
                    Ashe_UseW(wTarget);
                }
            }

            if (AsheConfig::AsheKillsteal::UseR->Value == true && database.AsheR.IsCastable())
            {
                const auto rTarget = TargetSelector::FindBestTarget(globals::localPlayer->GetPosition(),rRange());
                if (rTarget != nullptr
                    && rTarget->GetDistanceTo(globals::localPlayer) > AsheConfig::AsheSpellsSettings::minRDistance->Value
                    && rTarget->GetDistanceTo(globals::localPlayer) < AsheConfig::AsheSpellsSettings::maxRDistance->Value
                    && rTarget->ReadClientStat(Object::Health) < Ashe_dmgR(rTarget))
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
        for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
        {
            if (!target) continue;
            if (target->GetDistanceTo(globals::localPlayer) > wRange()) continue;
            if (!Engine::MenuItemContains(AsheConfig::AsheAntiGapCloser::whitelist, target->GetName().c_str())) continue;
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
            for (auto target : ObjectManager::GetHeroesAs(Alliance::Enemy))
            {
                if (!target) continue;
                if (target->GetDistanceTo(globals::localPlayer) > wRange()) continue;
                if (!Engine::MenuItemContains(AsheConfig::AsheAntiMelee::whitelist, target->GetName().c_str())) continue;

                if (target != nullptr && target->IsInRange(globals::localPlayer->GetPosition(), target->GetRealAttackRange()))
                {
                	Ashe_UseW(target);
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

    void OnBeforeAttack() override
    {
        __try {
            //Combo mode
            if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Attack)
            {
                const auto object = Engine::GetSelectedObject();
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
                const auto object = Engine::GetSelectedObject();
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
                const auto object = Engine::GetSelectedObject();
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
        
	void OnAfterAttack() override {
        return;
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