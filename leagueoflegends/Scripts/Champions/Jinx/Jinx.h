#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Jinx {
            namespace Config
            {
                using namespace UPasta::SDK;

                namespace JinxCombo {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQRocket;
                    inline CheckBox* UseW;
                    inline CheckBox* UseE;
                    inline CheckBox* UseR;
                    inline Slider* enemiesInRange;
                    static int GetMinimumEnemies() { return enemiesInRange->Value; }
                }

                namespace JinxHarass {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQRocket;
                    inline CheckBox* UseW;
                }

                namespace JinxClear {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQRocket;
                    inline Slider* minMana;
                }

                namespace JinxJungle {
                    inline CheckBox* UseQ;
                    inline CheckBox* UseQRocket;
                    inline Slider* minMana;
                }

                namespace JinxKillsteal {
                    inline CheckBox* UseW;
                    inline CheckBox* UseR;
                }


                namespace JinxAuto {
                    inline CheckBox* UseE;
                }

                namespace JinxSpellsSettings {
                    inline CheckBox* Weave;

                    inline Slider* wRange;
                    inline CheckBox* DrawW;
                    inline CheckBox* dontWinAARange;
                    static float GetWRange() { return static_cast<float>(wRange->Value); }

                    inline Slider* eRange;
                    inline CheckBox* useETower;
                    inline CheckBox* DrawE;
                    static float GetERange() { return static_cast<float>(eRange->Value); }

                    inline Slider* rRange;
                    inline Slider* rMinRange;
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



                namespace WSpell {
                    inline float dmgSkillArray[6] = { 0.0f, 10.0f, 60.0f, 110.0f, 160.0f, 210.0f };
                    inline float additionalPercentageAD = 1.6f;


                    static float GetDamage(Object* obj) { return SDK::Damage::CalculateSlotPhysicalDamage<float*, float>(SpellIndex::R, obj, dmgSkillArray, additionalPercentageAD); }

                }

                namespace RSpell {
                    inline float dmgSkillArray[4] = { 0, 325.0f, 475.0f, 625.0f };
                    inline float missingHealthArray[4] = { 0, .25f, .30f, .35f };
                    inline float additionalPercentageAD = 1.65f;
                    const float maxDistance = 1500.0f; // Max distance for scaling

                    static float GetDamage(Object* obj) {
                        int level = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::R)->GetLevel();
                        float distance = obj->GetDistanceTo(ObjectManager::GetLocalPlayer());
                        float percentFromDistance = 0.10f + (0.90f * min(distance, maxDistance) / maxDistance); // Scale from 10% to 100%
                        float missingHealth = (obj->GetMaxHealth() - obj->GetHealth()) / obj->GetMaxHealth();
                        float missingHealthDmg = missingHealth * missingHealthArray[level] * dmgSkillArray[level]; // Apply missing health percentage to base damage
                        float distanceDmg = dmgSkillArray[level] * percentFromDistance;
                        float bonusAD = ObjectManager::GetLocalPlayer()->GetBonusAttackDamage();
                        return SDK::Damage::CalculatePhysicalDamage(ObjectManager::GetLocalPlayer(), obj, distanceDmg + (additionalPercentageAD * bonusAD)) + missingHealthDmg;
                    }
                }

            }
        }
    }
}

