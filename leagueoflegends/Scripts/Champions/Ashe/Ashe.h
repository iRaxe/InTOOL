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
                void UseR(Object* obj);

                void DrawSpellRadius(float range);
                void DrawDamageOnHPBar(Object* obj);
                void DrawDamageOnPos(Object* obj);

                bool HasAsheQ();
            }

            namespace Damages
            {
                namespace WSpell {

                    inline float dmgSkillArray[6] = { 0, 20, 35, 50, 65, 80 };
                    inline float additionalPercentageAD = 1.00f;

                    static float GetDamage(Object* obj) {
	                    return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::W, obj, dmgSkillArray, additionalPercentageAD);
                    }
                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { -9999, 200, 400, 600 };
                    inline float dmgSkillArrayMonster[6] = { -9999, 175, 250, 325 };
                    inline float additionalPercentageAP = 1.20f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAP); }
                }
            }
        }
    }
}

