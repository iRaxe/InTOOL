#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Yone {
            namespace Config
            {
                using namespace UPasta::SDK;

                namespace YoneCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;
                }

                namespace YoneHarass {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                }

                namespace YoneClear {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseW;
                }

                namespace YoneJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseW;
                }
                namespace YoneKillsteal {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;
                }


                namespace YoneSpellsSettings {
                    inline Slider* qRange;
                    inline CheckBox* DrawQ;
                    static float GetQRange() { return static_cast<float>(qRange->Value); }

                    inline Slider* q3Range;
                    inline CheckBox* DrawQ3;
                    inline CheckBox* useQ3Tower;
                    static float GetQ3Range() { return static_cast<float>(q3Range->Value); }

                    inline Slider* wRange;
                    inline CheckBox* DrawW;
                    static float GetWRange() { return static_cast<float>(wRange->Value); }

                    inline Slider* eRange;
                    inline CheckBox* DrawE;
                    inline CheckBox* eBack;
                    inline CheckBox* eBackEnemy;;
                    inline CheckBox* eBackAtHp;;
                    inline Slider* eBackEnemySlider;
                    inline Slider* eBackHealthPercentSlider;
                    inline CheckBox* useETower;
                    static float GetERange() { return static_cast<float>(eRange->Value); }
                    static float GetE2Range() { return static_cast<float>(eBackEnemySlider->Value); }

                    inline Slider* rRange;
                    inline CheckBox* DrawR;
                    inline CheckBox* useRTower;
                    inline Slider* rEnemyCount;
                    inline CheckBox* rEnemyKnockUpOnly;
                    inline CheckBox* rKillable;

                    static float GetRRange() { return static_cast<float>(rRange->Value); }

                    inline CheckBox* DrawHPDamage;
                    inline CheckBox* DrawPosDamage;

                    inline CheckBox* DrawIfReady;
                    static bool ShouldDrawOnlyIfReady() { return DrawIfReady->Value; }

                    

                }
            }

            namespace Interuptables {
                inline std::unordered_map<std::string, std::vector<bool>> selectedSpells;
                inline SDK::CheckBox* interruptibleMenuCheck;
                enum DangerLevel {
                    Low,
                    Medium,
                    High
                };
                

                struct Spell {
                    DangerLevel dangerLevel;
                    bool movementInterrupts;
                    std::string slot;

                    Spell(DangerLevel level, bool interrupts, std::string slotName)
                        : dangerLevel(level), movementInterrupts(interrupts), slot(std::move(slotName)) {}
                };

                inline std::unordered_map<std::string, std::vector<Spell>> createSpellDictionary() {
                    std::unordered_map<std::string, std::vector<Spell>> spellDictionary;

                    spellDictionary["Caitlyn"] = { {High, true, "R"} };
                    spellDictionary["Fiddlesticks"] = { {Medium, true, "W"}, {High, true, "R"} };
                    spellDictionary["Galio"] = { {High, true, "R"} };
                    spellDictionary["Janna"] = { {Medium, true, "R"} };
                    spellDictionary["Jhin"] = { {High, true, "R"} };
                    spellDictionary["Karthus"] = { {High, true, "R"} };
                    spellDictionary["Katarina"] = { {High, true, "R"} };
                    spellDictionary["Lucian"] = { {High, false, "R"} };
                    spellDictionary["Malzahar"] = { {High, true, "R"} };
                    spellDictionary["MasterYi"] = { {Low, true, "W"} };
                    spellDictionary["MissFortune"] = { {High, true, "R"} };
                    spellDictionary["Nunu"] = { {High, true, "R"} };
                    spellDictionary["Pantheon"] = { {Low, true, "E"}, {High, true, "R"} };
                    spellDictionary["Quinn"] = { {Medium, true, "R"} };
                    spellDictionary["RekSai"] = { {Low, true, "R"} };
                    spellDictionary["Sion"] = { {High, true, "Q"}, {Low, true, "R"} };
                    spellDictionary["Shen"] = { {Low, true, "R"} };
                    spellDictionary["TahmKench"] = { {Medium, true, "R"} };
                    spellDictionary["TwistedFate"] = { {Medium, true, "R"} };
                    spellDictionary["Urgot"] = { {High, true, "R"} };
                    spellDictionary["Velkoz"] = { {High, true, "R"} };
                    spellDictionary["Warwick"] = { {High, true, "R"} };
                    spellDictionary["Xerath"] = { {Medium, false, "Q"}, {High, true, "R"} };
                    spellDictionary["Varus"] = { {Medium, false, "Q"} };
                    spellDictionary["Vi"] = { {Medium, false, "Q"} };
                    spellDictionary["Vladimir"] = { {Low, false, "E"} };
                    spellDictionary["Zilean"] = { {Low, true, "52"} };

                    return spellDictionary;
                }

            }
        

            namespace Events {
                void Initialize();
                void Subscribe();
                void Unsubscribe();
                void OnGameUpdate();
                void OnWndProc(UINT msg, WPARAM param);
                void OnBeforeAttack();

                void OnDraw();
                void OnCreateMissile(Object* unit);
                void OnDeleteMissile(Object* unit);
                void OnCreateObject(Object* unit);
                void OnDeleteObject(Object* unit);
                void OnIssueOrder(Object* unit, OrbwalkState order, Vector3* position, Object* target);
                void OnCastSpell(void* spellbook, SpellInfo* pSpellInfo, SpellIndex slot, Vector3* _end_position, Vector3* _start_position, DWORD netId);
                void OnSpellCast(SpellIndex slot);
                void OnDoCast(SpellInfo* castInfo, SpellData* spellData);
                void OnCreateSpell(SpellCast* spellCastInfo);
                void OnDoCastDelayed(SpellInfo* castInfo, SpellData* spellData);
                void OnProcessSpell(uintptr_t* castInfo, SpellCast* spellData);
                void OnPlayAnimation(Object* ptr, char* name, float animationTime);
                void OnFinishCast(SpellInfo* castInfo, Object* object);
                void OnGapCloserSpell(SpellInfo* castInfo, SpellData* spellData);
                void OnInterruptibleSpell(SpellInfo* castInfo, SpellData* spellData);
                static void MyTThing(int state, SpellCast* spellCastInfo);
            }

            namespace Modes {
                void Combo();
                void Clear();
                void Harass();
                void Killsteal();
                void Auto();;
            }

            namespace Functions {
                void InitializeMenu();

                void UseQ(Object* obj);
                void UseQ3(Object* obj);
                void UseW(Object* obj);
                void UseE(Object* obj);
                void UseR(Object* obj, bool override);

                void DrawSpellRadius(float range);
                void DrawDamageOnHPBar(Object* obj);
                void DrawDamageOnPos(Object* obj);
                void EOnMark(Object* target);
                bool EKills(Object* target);

                Object* GetRTarget(float range); // Code from gay boy
               

        

                

            }

            namespace Damages
            {

                namespace QSpell {
                    inline float dmgSkillArray[6] = { 0, 20, 40, 60, 80, 199 };
                    inline float additionalPercentageAD = 1.05f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::Q, obj, dmgSkillArray, additionalPercentageAD); }


                }


                namespace WSpell {
                    inline float baseDamageArray[6] = { 0, 10, 20, 30, 40, 50 };

                    inline float healthPercentageArray[6] = { 0, 0.11, 0.12, 0.13, 0.14, 0.15 };

                    static float GetDamage(Object* obj) {
                        if (globals::localPlayer == nullptr) {
                            return 0;
                        }
                        const int levelSpell = globals::localPlayer->GetSpellBySlotId(SpellIndex::W)->GetLevel();


                        float maxHealth = obj->GetMaxHealth();

                        float healthDamagePercentage = healthPercentageArray[levelSpell] / 2.0f;
                        
                        float healthDamage = healthDamagePercentage * maxHealth;

                        // Create an array for enhanced base damage
                        float enhancedBaseDamageArray[6];
                        for (int i = 0; i < 6; ++i) {
                            float healthDamage = (healthPercentageArray[i] / 2.0f) * maxHealth;
                            enhancedBaseDamageArray[i] = baseDamageArray[i] / 2 + healthDamage;
                        }

                        // Calculate physical and magical damage separately using the enhanced base damage array
                        float physicalDamage = SDK::Damage::CalculateSlotPhysicalDamage(SpellIndex::W, obj, enhancedBaseDamageArray, 0);
                        float magicalDamage = SDK::Damage::CalculateSlotMagicalDamage(SpellIndex::W, obj, enhancedBaseDamageArray, 0);

                        float totalPhysicalDamage = physicalDamage + magicalDamage;;

                        return totalPhysicalDamage;
                    }
                }


                namespace ESpell {
                    inline float getPercentStored[6] = { 0, .25, .275, .30, .325, .35 };

                    static float GetDamage(Object* obj, float damage)
                    {
                        auto spell = globals::localPlayer->GetSpellBySlotId(SpellIndex::E);
                        int level = spell->GetLevel();

                        return damage * getPercentStored[level];
                    }
                }
                

                namespace RSpell {
                    inline float dmgSkillArray[4] = { 0, 200, 400, 600 };
                    inline float additionalPercentageAD = .8f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAD); }
                }

               
            }
        }
    }
}

