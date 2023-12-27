#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"
namespace UPasta {
    namespace Plugins {
        namespace Twitch {
            namespace Config {

                using namespace UPasta::SDK;
                namespace TwitchCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseEWithStacks;
                    inline Slider* minStacks;
                    static float GetMinimumEStacks() { return static_cast<float>(minStacks->Value); }

                    inline CheckBox* UseR;

                    inline Slider* enemiesInRange;
                    static float GetMinimumREnemies() { return static_cast<float>(enemiesInRange->Value); }

                }

                namespace TwitchHarass {
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseEWithStacks;
                    inline Slider* minStacks;
                    static float GetMinimumEStacks() { return static_cast<float>(minStacks->Value); }

                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace TwitchClear {
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline Slider* minMinions;
                    static int GetMinimumMinions() { return minMinions->Value; }

                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace TwitchJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseEWithStacks;
                    inline Slider* minStacks;
                    static float GetMinimumEStacks() { return static_cast<float>(minStacks->Value); }

                    inline Slider* minMana;
                    static float GetMinimumMana() { return static_cast<float>(minMana->Value); }
                }

                namespace TwitchKillsteal {
                    inline CheckBox* UseE;
                }

                namespace TwitchFlee {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                }

                namespace TwitchAntiGapCloser {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseW;
                    inline std::vector<Object*> whitelist;
                }

                namespace TwitchSpellsSettings {
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
                void OnAfterAttack();
                void OnDraw();
            }

            namespace Modes {
                void Combo();
                void Clear();
                void Harass();
                void Killsteal();
                void AntiGapCloser();
                void Flee();

            }

            namespace Functions {
                bool HasTwitchE(Object* obj);
                int GetDeadlyVenomStacks(Object* obj);
                float GetTwitchEDamage(Object* obj);

                void InitializeMenu();

                void UseQ();
                void UseW(Object* obj);
                void UseE(Object* obj);
                void UseR(Object* obj);

                void DrawSpellRadius(float range);
                void DrawDamageOnHPBar(Object* obj);
                void DrawDamageOnPos(Object* obj);
            }

            namespace Damages
            {
                namespace ESpell {
                    inline float dmgSkillArray[6] = { 0, 20, 30, 40, 50, 60 };
                    inline float perStackDamage[6] = { 0, 15, 20, 25, 30, 35 };
                    inline float additionalPercentageAD = 0.35f;
                }
            }
        }
    }
}

