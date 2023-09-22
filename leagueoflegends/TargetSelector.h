#ifndef TARGET_SELECTOR_FUNCTIONS_H
#define TARGET_SELECTOR_FUNCTIONS_H

#include "../NewMenu.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "./Damage.h"

namespace UPasta::SDK::TargetSelector
{
    namespace Configs
    {
        void Initialize();
        extern Menu* TSMenu;
        void PopulateComboTargets();

        extern std::vector<Object*> Enemies;
        extern std::unordered_map<std::string, unsigned int> TargetPriorities;
        extern Menu* TargetPrioritiesMenu;
        void InitializeComboTargets();

        extern Menu* ClearPrioritiesMenu;
        void InitializeClearTargets();

        extern Menu* LasthitPrioritiesMenu;
        void InitializeLastHitTargets();

        extern Menu* UnderTowerPrioritiesMenu;
        void InitializeUnderTowerTargets();

        extern Menu* PrioritiesDrawMenu;
        void InitializeDrawSettings();

        enum class TargetSelectorMode
        {
            Auto,
            LeastHealth,
            MostAttackDamage,
            MostAbilityPower,
            Closest,
            HighestPriority,
            LessAttack,
            LessCast,
            NearMouse
        };
        extern TargetSelectorMode ActiveMode;

        enum class TargetType {
            TSTARGET_HEROES,
            TSTARGET_MINION
        };

        namespace TSModes
        {
            namespace Combo
            {
                inline CheckBox* resetPriorities;
                inline List* attackPriorities;

                namespace Advanced {
                    inline CheckBox* attackSelectedTarget;
                    inline CheckBox* attackOnlySelectedTarget;
                    inline CheckBox* attackClones;
                };

            };

            namespace Laneclear
            {
                inline Slider* wardPriority;
                inline Slider* superMinionPriority;
                inline Slider* meleeMinionPriority;
                inline Slider* rangedMinionPriority;
                inline Slider* siegeMinionPriority;
                inline Slider* voidMinionPriority;
                inline Slider* specialObjectsPriority;
                inline CheckBox* attackSpecialObjects;
                inline CheckBox* focusHarassOverClear;
            };

            namespace Lasthit
            {
                inline Slider* superMinionPriority;
                inline Slider* meleeMinionPriority;
                inline Slider* rangedMinionPriority;
                inline Slider* siegeMinionPriority;
                inline Slider* voidMinionPriority;
                inline Slider* specialObjectsPriority;
                inline CheckBox* attackSpecialObjects;
            };

            namespace UnderTower
            {
                inline Slider* superMinionPriority;
                inline Slider* meleeMinionPriority;
                inline Slider* rangedMinionPriority;
                inline Slider* siegeMinionPriority;
                inline Slider* voidMinionPriority;
                inline Slider* specialObjectsPriority;
                inline CheckBox* attackSpecialObjects;
            };
        };

        namespace TSDrawings {
            inline CheckBox* drawCurrentTarget;
            inline CheckBox* drawSelectedChampion;
            inline CheckBox* drawKillableMinion;
            inline CheckBox* drawWard;
            inline CheckBox* drawSpecialObjects;
            inline CheckBox* drawSuperMinion;
            inline CheckBox* drawMeleeMinion;
            inline CheckBox* drawRangedMinion;
            inline CheckBox* drawSiegeMinion;
            inline CheckBox* drawVoidMinion;
        };

    }

    namespace Functions
	{
        std::vector<Object*> GetMinionsInRange(float range);
        int GetMinionPriority(Object* minion);
        float GetMinionReducedPriority(Object* minion);
        Object* GetMinion(std::vector<Object*> targets, DamageType damageType);
        Object* GetEnemyMinionInRange(float radius);

        std::vector<Object*> GetTargetsInRange(float range);
        extern Object* SelectedTarget;
        int GetPriority(Object* hero);

        std::vector<Object*> GetJungleMonstersInRange(float range);
        Object* GetJungleInRange(float range);

        std::vector<Object*> GetJungleRespawnInRange(float range);


        Object* GetTarget(Configs::TargetType tsType, float range, DamageType damageType, Vector3 source = Vector3(0, 0, 0), bool addBoundingRadius = false);
        Object* GetTarget(std::vector<Object*> targets, DamageType damageType);

        
        float GetReducedPriority(Object* hero);

        bool ChooseSelectedObject(Object* selectedObject, Object* checkObj);
        Object* GetEnemyChampionInRange(float range, int damageType, Skillshot skillshot);
        Object* GetEnemyChampionInRange(float range);
        Object* GetEnemyChampionInRange(float range, Skillshot skillshot);
        Object* GetEnemyNexusInRange(float range);
        Object* GetEnemyInhibitorInRange(float range);
        Object* GetEnemyTurretInRange(float range);
        Object* GetMinionInRange(float range);
        Object* GetEnemyObjectInRange(float range);
        Object* GetObjectInRange(float range, std::string name = "", std::vector<QWORD> includefilterTypeHashes = {}, std::vector<QWORD> excludeFilterTypeHashesDetailed = {}, bool isSpecial = false);
    }

    namespace Drawings
    {
        void DrawSelectedTarget();
        void DrawMinionsPriority();
        
    }
}

#endif // TARGET_SELECTOR_FUNCTIONS_H