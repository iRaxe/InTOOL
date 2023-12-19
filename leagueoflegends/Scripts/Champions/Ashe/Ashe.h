#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Ashe {
            namespace Config
            {

                using namespace UPasta::SDK;

                namespace AsheCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;

                    inline Slider* enemiesInRange;
                    static float GetMinimumREnemies() { return static_cast<float>(enemiesInRange->Value); }

                }

                namespace AsheHarass {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace AsheClear {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline Slider* minMinions;
                    static int GetMinimumMinions() { return minMinions->Value; }

                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace AsheJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace AsheLastHit {
                    inline CheckBox* UseW;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace AsheKillsteal {
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;
                }

                namespace AsheFlee {
                    inline CheckBox* UseW;
                }

                namespace AsheAntiGapCloser {
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;
                    inline std::vector<Object*> whitelist;
                }

                namespace AsheAntiMelee {
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;
                    inline std::vector<Object*> whitelist;
                }

                namespace AsheSpellsSettings {
                    static float GetQRange() { return ObjectManager::GetLocalPlayer()->GetRealAttackRange(); }
                                        
                    inline CheckBox* UseWIfInAARange;
                    inline CheckBox* UseWIfFullAASpeed;

                    inline Slider* wRange;
                    inline CheckBox* DrawW;
                    static float GetWRange() { return static_cast<float>(wRange->Value); }

                    inline List* targetMode;
                    inline KeyBind* rTapKey;

                    inline Slider* minRDistance;
                    static float GetMinRRange() { return static_cast<float>(minRDistance->Value); }

                    inline Slider* maxRDistance;
                    static float GetMaxRRange() { return static_cast<float>(maxRDistance->Value); }

                    inline CheckBox* DrawHPDamage;
                    inline CheckBox* DrawPosDamage;

                    inline CheckBox* DrawIfReady;
                    static bool ShouldDrawOnlyIfReady() { return DrawIfReady->Value; }

                }
            }

            namespace Events {
                void Initialize();
                void Subscribe();
                void Unsubscribe();
                void OnGameUpdate();
                void OnWndProc(UINT msg, WPARAM param);
                void OnBeforeAttack();
                void OnCastSpell(SpellCast* spellCastInfo);

                void OnDraw();
                void OnCreateMissile(Object* unit);
                void OnDeleteMissile(Object* unit);
                void OnCreateObject(Object* unit);
                void OnDeleteObject(Object* unit);
                void OnIssueOrder(Object* unit, OrbwalkState order, Vector3* position, Object* target);
                //void OnCastSpell(void* spellbook, SpellInfo* pSpellInfo, SpellIndex slot, Vector3* _end_position, Vector3* _start_position, DWORD netId);
                void OnSpellCast(SpellIndex slot);
                void OnDoCast(SpellInfo* castInfo, SpellData* spellData);
                void OnDoCastDelayed(SpellInfo* castInfo, SpellData* spellData);
                void OnProcessSpell(uintptr_t* castInfo, SpellCast* spellData);
                void OnPlayAnimation(Object* ptr, char* name, float animationTime);
                void OnFinishCast(SpellInfo* castInfo, Object* object);
                void OnGapCloserSpell(SpellInfo* castInfo, SpellData* spellData);
                void OnInterruptibleSpell(SpellInfo* castInfo, SpellData* spellData);
            }

            namespace Modes {
                void AimR();
                void Combo();
                void Clear();
                void Harass();
                void Killsteal();
                void AntiGapCloser();
                void AntiMelee();
            }

            namespace Functions {
                void InitializeMenu();

                void UseQ(Object* obj);
                void UseW(Object* obj);
                void UseE(Object* obj);
                void UseR(Object* obj);

                void DrawSpellRadius(float range);
                void DrawDamageOnHPBar(Object* obj);
                void DrawDamageOnPos(Object* obj);

                bool HasAsheQ();
            }

            namespace Damages
            {
                namespace WSpell {
                    inline float dmgSkillArray[6] = { 0, 95, 145, 195, 245, 295 };
                    inline float additionalPercentageAP = 0.50f;

                    static float GetDamage(Object* obj) {
	                    return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::W, obj, dmgSkillArray, additionalPercentageAP);
                    }
                }

                namespace ESpell {
                    inline float dmgAPSkillArray[6] = { 0, 55, 80, 105, 130, 155 };
                    inline float additionalDmgAPPercentageAP = 0.25f;

                    inline float dmgADSkillArray[6] = { 0, 70, 80, 90, 100, 110 };
                    inline float additionalDmgADPercentageAD[6] = { 0, 0.50f, 0.75f, 1.0f, 1.25f, 1.50f };
                    inline float additionalDmgADPercentageAP = 0.50f;

                    inline float dmgStackADSkillArray[6] = { 0, 21, 24, 27, 30, 33 };
                    inline float additionalDmgStackPercentageAD[6] = { 0, 0.15f, 0.225f, 0.30f, 0.375f, 0.45f };
                    inline float additionalDmgStackPercentageAP = 0.15f;

                    inline float dmgMaxSkillArray[6] = { 0, 205, 235, 264, 293, 323 };
                    inline float additionalDmgMaxPercentageAD[6] = { 0, 1.46f, 2.199f, 2.933f, 3.666f, 4.399f };
                    inline float additionalDmgMaxPercentageAP = 1.456f;

                    static float GetDamage(Object* obj)
                    {
	                    const float magicDamage = SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::E, obj, dmgAPSkillArray, additionalDmgAPPercentageAP);
                        const float minDamage = SDK::Damage::CalculateSlotMixedDamage<float*, float, float*>(SpellIndex::E, obj, dmgADSkillArray, additionalDmgADPercentageAP, additionalDmgADPercentageAD);
                        const float stackDamage = SDK::Damage::CalculateSlotMixedDamage<float*, float, float*>(SpellIndex::E, obj, dmgStackADSkillArray, additionalDmgStackPercentageAP, additionalDmgStackPercentageAD);
                        const float stacksDamage = 0;
                        const float defMinDamage = magicDamage + minDamage + stacksDamage;
                        const float maxDamage = SDK::Damage::CalculateSlotMixedDamage<float*, float, float*>(SpellIndex::E, obj, dmgMaxSkillArray, additionalDmgMaxPercentageAP, additionalDmgMaxPercentageAD);
                        const float defDamage = defMinDamage > maxDamage ? maxDamage : minDamage;
	                    return defDamage;
                    }
                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { 0, 300, 400, 500 };
                    inline float additionalPercentageAP = 1.00f;
                    inline float knockbackDistance[4] = { 0, 600, 800, 1000 };

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAP); }
                }
            }
        }
    }
}

