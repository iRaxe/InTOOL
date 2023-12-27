#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"
namespace UPasta {
    namespace Plugins {
        namespace Blitzcrank {
            namespace Config  {

                using namespace UPasta::SDK;
                namespace BlitzcrankCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;

                    inline Slider* enemiesInRange;
                    static float GetMinimumREnemies() { return static_cast<float>(enemiesInRange->Value); }

                }

                namespace BlitzcrankHarass {
                    inline CheckBox* UseQ;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace BlitzcrankJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace BlitzcrankKillsteal {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseR;
                }

                namespace BlitzcrankFlee {
                    inline CheckBox* UseW;
                }

                namespace BlitzcrankAntiGapCloser {
                    inline CheckBox* UseR;
                    inline std::vector<Object*> whitelist;
                }

                namespace BlitzcrankSpellsSettings {
                    inline Slider* qRange;
                    inline CheckBox* DrawQ;
                    static float GetQRange() { return static_cast<float>(qRange->Value); }

                    inline Slider* wRange;
                    static float GetWRange() { return static_cast<float>(wRange->Value); }

                    static float GetERange() { return ObjectManager::GetLocalPlayer()->GetRealAttackRange(); }

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
                void OnAfterAttack();
                void OnDraw();
            }

            namespace Modes {
                void Combo();
                void Clear();
                void Harass();
                void Killsteal();
                void AntiGapCloser();
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
            }

            namespace Damages
            {
                namespace QSpell {
                    inline float dmgSkillArray[6] = { 0, 105, 150, 195, 240, 285 };
                    inline float additionalPercentageAP = 1.20f;

                    static float GetDamage(Object* obj) {
                        return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::Q, obj, dmgSkillArray, additionalPercentageAP);
                    }
                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { 0, 275, 400, 525 };
                    inline float additionalPercentageAP = 1.0f;

                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotMagicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAP); }
                }
            }
        }
    }
}

