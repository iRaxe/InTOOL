#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Yasuo {
            namespace Config
            {
                using namespace UPasta::SDK;

                namespace YasuoCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;
                    inline Slider* enemiesInRange;
                    static int GetMinimumEnemies() { return enemiesInRange->Value; }
                }

                namespace YasuoHarass {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseE;
                }

                namespace YasuoClear {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ3;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                }

                namespace YasuoJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                }

                namespace YasuoKillsteal {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;
                }

                namespace YasuoGapClose {
                    inline CheckBox* GapCloseTarget;
                    inline List* GapCloseTargetMode;
                    
                }

                namespace YasuoAuto {
                    inline CheckBox* UseQ;
                }

                namespace YasuoSpellsSettings {
                    inline Slider* qRange;
                    inline CheckBox* DrawQ;
                    static float GetQRange() { return static_cast<float>(qRange->Value); }

                    inline Slider* Q3Range;
                    inline CheckBox* DrawQ3;
                    static float GetQ3Range() { return static_cast<float>(Q3Range->Value); }

                    inline Slider* wRange;
                    inline CheckBox* DrawW;
                    static float GetWRange() { return static_cast<float>(wRange->Value); }

                    inline Slider* eRange;
                    inline CheckBox* useETower;
                    inline CheckBox* DrawE;
                    static float GetERange() { return static_cast<float>(eRange->Value); }

                    inline Slider* rRange;
                    inline CheckBox* useRTower;
                    inline CheckBox* DrawR;
                    static float GetRRange() { return static_cast<float>(rRange->Value); }

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
                void OnDraw();
                void OnProcessSpell(int state, SpellCast* spellCastInfo);
                void OnFinishCast(SpellCast* spellCastInfo);
            }

            namespace Modes {
                void Combo();
                void Clear();
                void Harass();
                void Killsteal();
                void Auto();
            }

            namespace Functions {
                void InitializeMenu();

                void UseQ(Object* obj);
                void UseQ3(Object* obj);
                void UseQDash();
                void UseW(Object* obj);
                void UseE(Object* obj);
                void UseR();

                void DrawSpellRadius(float range);
                void DrawDamageOnHPBar(Object* obj);
                void DrawDamageOnPos(Object* obj);

            }

            namespace Damages
            {
                namespace QSpell {
                    inline float dmgSkillArray[6] = { 0.0f, 20.0f, 45.0f, 70.0f, 95.0f, 120.0f };
                    inline float additionalPercentageAD = 1.05f;
                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::Q, obj, dmgSkillArray, additionalPercentageAD); }
                }


                namespace ESpell {
                    inline float dmgSkillArray[6] = { 0.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f };
                    inline float additionalPercentageAP = 0.60f;
                    inline float additionalPercentageAD = .20f;
                    

                    static float GetDamage(Object* obj) {
                        auto me = ObjectManager::GetLocalPlayer();
                        int level = me->GetSpellBySlotId(SpellIndex::E)->GetLevel();
                        float physical = dmgSkillArray[level] + (me->GetAttackDamage() * .20f);
                        float magical = (me->GetAbilityPower() * .60f);
                    	return SDK::Damage::CalculatePhysicalDamage(me, obj, physical)
                        + SDK::Damage::CalculateMagicalDamage(me, obj, magical); }
                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { 0, 200.0f, 350.0f, 500.0f };
                    inline float additionalPercentageAD = 1.5f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAD); }
                }
            }
        }
    }
}

