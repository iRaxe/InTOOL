#pragma once
#include "../Damage.h"
#include "../PluginTemplate.h"

#define PI 3.14159265f
namespace UPasta {
    namespace Plugins {
        namespace Activator {
            namespace Config
            {
                using namespace UPasta::SDK;


                namespace ActivatorSettings {
                    inline CheckBox* Ignite;
                    inline CheckBox* IgniteKS;
                    inline CheckBox* DrawIgnite;
                    inline Slider* IgniteHP;
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
                void UseIgnite(Object* obj);
                void DrawSpellRadius(float range);
                


            }

            namespace Damages
            {
                namespace Ignite {

                    static float GetDamage()
                    {
                        auto me = ObjectManager::GetLocalPlayer();
                        if (me == nullptr) return 0;
                        int level = ObjectManager::GetLocalPlayer()->GetLevel();
	                    return 70.0f + (level * 20.0f);
                    }
                }


            }
        }
    }
}

