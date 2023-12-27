#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Brand {
            namespace Config
            {
                using namespace UPasta::SDK;

                namespace BrandCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQ2;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;
                    inline Slider* enemiesInRange;
                    static int GetMinimumEnemies() { return enemiesInRange->Value; }
                }

                namespace BrandHarass {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                	static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace BrandClear {
                    inline CheckBox* UseW;
                    inline Slider* minMinions;
                    static int GetMinimumMinions() { return minMinions->Value; }

                    inline CheckBox* UseE;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace BrandJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace BrandKillsteal {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;
                }

                namespace BrandSpellsSettings {
                    inline Slider* qRange;
                    inline CheckBox* DrawQ;
                    static float GetQRange() { return static_cast<float>(qRange->Value); }

                    inline Slider* wRange;
                    inline CheckBox* DrawW;
                    static float GetWRange() { return static_cast<float>(wRange->Value); }

                    inline Slider* eRange;
                    inline CheckBox* DrawE;
                    static float GetERange() { return static_cast<float>(eRange->Value); }

                    inline Slider* rRange;
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
                void OnDraw();
            }

            namespace Modes {
                void Combo();
                void Clear();
                void Harass();
                void Killsteal();
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

                Object* GetAblazedTarget(float range);

                bool HasBrandPassive(Object* obj);
            }

            namespace Damages
            {
                namespace QSpell {
                    inline float dmgSkillArray[6] = { 0, 80, 110, 140, 170, 200 };
                    inline float additionalPercentageAP = 0.65f;
                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::Q, obj, dmgSkillArray, additionalPercentageAP); }
                }

                namespace WSpell {
                    inline float dmgSkillArray[6] = { 0, 75, 120, 165, 210, 255 };
                    inline float additionalPercentageAP = 0.60f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::W, obj, dmgSkillArray, additionalPercentageAP); }

                    inline float dmgSkillArrayAblazed[6] = { 0, 93.75, 150, 206.25, 262.5, 318.75 };
                    inline float additionalPercentageAPAblazed = 0.75f;

                    static float GetDamageAblazed(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::W, obj, dmgSkillArray, additionalPercentageAPAblazed); }
                }

                namespace ESpell {
                    inline float dmgSkillArray[6] = { 0, 65, 90, 115, 140, 165 };
                    inline float additionalPercentageAP = 0.50f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::E, obj, dmgSkillArray, additionalPercentageAP); }
                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { 0, 100, 200, 300 };
                    inline float additionalPercentageAP = 0.25f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAP); }
                }
            }
        }
    }
}
	
