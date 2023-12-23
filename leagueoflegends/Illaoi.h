#pragma once
#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Illaoi {
            namespace Config
            {

                using namespace UPasta::SDK;

                namespace IllaoiCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;

                    inline Slider* enemiesInRange;
                    static float GetMinimumREnemies() { return static_cast<float>(enemiesInRange->Value); }

                }

                namespace IllaoiHarass {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace IllaoiClear {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline Slider* minMinions;
                    static int GetMinimumMinions() { return minMinions->Value; }

                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace IllaoiJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace IllaoiLastHit {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace IllaoiKillsteal {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;
                }

                namespace IllaoiFlee {
                    inline CheckBox* UseW;
                }




                namespace IllaoiSpellsSettings {
                    

                    inline Slider* qRange;
                    inline CheckBox* DrawQ;
                    static float GetQRange() { return static_cast<float>(qRange->Value); }

                    inline Slider* wRange;
                    inline CheckBox* DrawW;
                    static float GetWRange() { return ObjectManager::GetLocalPlayer()->GetRealAttackRange(); }

                    inline Slider* eRange;
                    inline CheckBox* DrawE;
                    static float GetERange() { return static_cast<float>(eRange->Value); }

                    inline Slider* rRange;
                    inline CheckBox* DrawR;
                    static float GetRRange() { return static_cast<float>(rRange->Value); }

                    inline List* targetMode;

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
                void OnAfterAttack();
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
                void LastHit();
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
                void UseR();

                void DrawSpellRadius(float range);
                void DrawDamageOnHPBar(Object* obj);
                void DrawDamageOnPos(Object* obj);
            }

            namespace Damages
            {

                namespace Tenticle
                {
                    inline float damageScalingArray[6] = { 0, 1.0, 1.08, 1.16, 1.24, 1.3 };

                    inline float baseDamageArray[19] = {
                        0, 10, 20, 30, 40, 50, 60, 70, 80, 90,
                        100, 110, 120, 130, 140, 150, 160, 170, 180
                    };

                    static float GetDamage(Object* obj) {
                        Object* localPlayer = ObjectManager::GetLocalPlayer();

                        int championLevel = localPlayer->GetLevel();
                        int abilityRank = localPlayer->GetSpellBySlotId(SpellIndex::Q)->GetLevel();

                        float baseDamage = baseDamageArray[championLevel];
                        float ADScaling = SDK::Damage::CalculatePhysicalDamage(localPlayer,obj,localPlayer->GetAttackDamage() * 1.20f);
                        float APScaling = SDK::Damage::CalculateMagicalDamage(localPlayer, obj,localPlayer->GetAbilityPower() * 0.40f);

                        // Total damage before scaling by ability rank
                        float totalDamage = baseDamage + ADScaling + APScaling;

                        // Adjust the total damage based on the ability rank
                        return totalDamage * damageScalingArray[abilityRank];
                    }

                }

                namespace QSpell {
                	inline float damageScalingArray[6] = { 0, .1f, .15f, .2f, .25f, .3f };

                	static float GetDamage(Object * obj) {
                        int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Q)->GetLevel();
                        float dmg = Tenticle::GetDamage(obj);
                		return dmg + (dmg * damageScalingArray[level]);

                    }
                }


                namespace WSpell {

                    inline float dmgSkillArray[6] = { 0, 0.03, .035, .04, .045, .05 };
                    inline float additionalPercentageAD = .04;
                    inline float minDamageArray[6] = { 0, 20, 30, 40, 50, 60 };



                    static float GetDamage(Object* obj) {
                        float PerAd = ObjectManager::GetLocalPlayer()->GetAttackDamage() / 100.0f;
                        float dmgPerStackScale = PerAd * additionalPercentageAD;
                        int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::W)->GetLevel();
                        float baseDamage = obj->GetMaxHealth() * dmgSkillArray[level];
                        float calculatedDamage = baseDamage + SDK::Damage::CalculatePhysicalDamage(ObjectManager::GetLocalPlayer(), obj, dmgPerStackScale);
                        float minDamage = minDamageArray[level];
                        return max(calculatedDamage, minDamage);
                    }
                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { -9999, 150, 250, 350 };
                    inline float additionalPercentageAD = .50f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAD); }
                }
            }
        }
    }
}

