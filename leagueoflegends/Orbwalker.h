#pragma once
#include "../NewMenu.h"
#include "../stdafx.h"

namespace UPasta::SDK::Orbwalker
{
    namespace Configs
    {
        inline bool initializedOrbwalkerMenu;
        extern Menu* OrbwalkerMenu;
        inline CheckBox* status;

        void Initialize();

        namespace Humanizer
        {
            inline bool initializedHumanizerMenu;
            void InitializeHumanizerMenu();
            extern Menu* HumanizerMenu;

            inline CheckBox* status;

            inline CheckBox* randomizeDelay;
            inline Slider* clickDelay;

            inline Slider* windupDelay;
            inline Slider* beforeAttackDelay;
            inline Slider* farmAttackDelay;

        }

        namespace Status
        {
            inline bool initializedStatusMenu;

            void InitializeStatusMenu();

            extern Menu* StatusMenu;

            inline CheckBox* statusFollowMouse;
            inline CheckBox* statusComboMode;
            inline CheckBox* statusLaneClearMode;
            inline CheckBox* statusFastClearMode;
            inline CheckBox* statusLastHitMode;
            inline CheckBox* statusHarassMode;
            inline CheckBox* statusFleeMode;

        }

        namespace KeyBindings
        {
            inline bool initializedKeyBindingsMenu;

            void InitializeKeyBindingsMenu();

            extern Menu* KeyBindingsMenu;

            inline KeyBind* comboKey;
            inline KeyBind* laneClearKey;
            inline KeyBind* fastClearKey;
            inline KeyBind* lastHitKey;
            inline KeyBind* harassKey;
            inline KeyBind* fleeKey;

        }

    }

    namespace Functions
    {
        extern float nextRngBuffer;
        extern float gameTime;
        extern float lastAttackTime;
        extern QWORD lastSpellCastAddress;
        extern float lastActionTime;

        extern bool shouldWait;
        extern bool isReloading;
        extern bool canDoAction;

        void CheckActiveAttack();
        void StopOrbwalkCheck();
        bool CanDoAction();
        void IsReloadingCheck();

        namespace Actions
        {
            void Idle();
            void AttackObject(Object* obj);
            void AttackInhib(Object* obj);
            void CastSpell(int spellId, Object* target);
            void CastSpell(int spellId, Vector3 pos);
        }

		namespace States
		{
			inline Object* LastHitMinion;
            inline Object* AlmostLastHitMinion;
            inline Object* LaneClearMinion;

			void Attack();
			void Laneclear();
			void Harass();
			void Lasthit();
		}

        void Initialize();
        void Update();
        void KeyChecks();
    }

}
