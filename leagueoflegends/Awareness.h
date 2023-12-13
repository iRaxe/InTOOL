#pragma once
#include "../NewMenu.h"
#include "../stdafx.h"

namespace UPasta::SDK::Awareness
{
    namespace ChampionSpells
    {
        inline std::unordered_map<std::string, void*> _spell_map;

        void InitializeSpellsImages();
    }

    namespace Configs
    {
        inline bool initializedAwarenessMenu;
        extern Menu* AwarenessMenu;
        extern Menu* HealthbarOverlayMenu;


        namespace EnemyTracker
        {
            inline CheckBox* status;

            inline CheckBox* showDamagePrediction;
            inline CheckBox* showKillableStatus;

            inline CheckBox* showCooldowns;
            inline CheckBox* showCooldownsSelf;
            inline CheckBox* showCooldownsAllies;
            inline CheckBox* showCooldownsEnemies;

            inline CheckBox* showExperience;
            inline CheckBox* showExperienceSelf;
            inline CheckBox* showExperienceAllies;
            inline CheckBox* showExperienceEnemies;
            
            inline CheckBox* showPaths;
            inline CheckBox* showPathsSelf;
            inline CheckBox* showPathsAllies;
            inline CheckBox* showPathsEnemies;

            inline CheckBox* showLastPosition;
            inline CheckBox* showPosGuesser;
        	inline CheckBox* showLastPositionWorld;
        	inline CheckBox* showLastPositionMiniMap;

            inline CheckBox* showWardPosition;
            inline CheckBox* showWardTimer;
            inline CheckBox* showWardRange;
        }

        namespace EnemySidebar
        {
            inline bool open_ptr = true;
            inline bool initializedSidebarMenu;

            inline CheckBox* status;
            inline CheckBox* locked;

            inline List* orientation;
            inline Slider* hudSize;
        }

        namespace Radius
        {
            inline bool initializedRadiusMenu;
            void Initialize();
            void InitializeRadiusMenu();
            inline float color_edit0[4] = { 255 / 255.f, 77 / 255.f, 77 / 255.f, 200 / 255.f };

            inline CheckBox* status;
            inline CheckBox* rainbowMode;
            inline List* drawMode;
            inline CheckBox* showHeight;
            inline Slider* heightTollerance;

            inline Slider* qualityDraw;

            inline CheckBox* showBoundingRadius;
            inline CheckBox* showAARadius;
            inline CheckBox* showAARadiusTurrets;
            inline CheckBox* showAARadiusSelf;
            inline CheckBox* showAARadiusAllies;
            inline CheckBox* showAARadiusEnemies;

        }

        namespace Zoom
        {
            inline bool initializedZoomMenu;
            void Initialize();
            void InitializeZoomMenu();

            inline CheckBox* status;
            inline CheckBox* status3D;
            inline CheckBox* statusKeyboard;
            inline CheckBox* statusMouseWheel;
            inline Slider* zoomValue;

        }

        namespace JungleTracker
        {
            inline bool initializedJungleTrackerMenu;
            void Initialize();
            void InitializeJungleTrackerMenu();

            inline CheckBox* status;
            inline CheckBox* showTimer;
            inline CheckBox* showIcons;
            inline CheckBox* showNotifications;

        }

    }

    namespace Functions
    {
        void Initialize();
        void Update();

        namespace EnemyTracker
        {
            void DrawTracker();
            void DrawWardsTracker();
        	void Update();
        }

        namespace Radius
        {
            void DrawRadius(Vector3 worldPos, float radius, uintptr_t color, float thickness, bool takeHeightInConsideration = Configs::Radius::showHeight->Value, bool glow = Configs::Radius::rainbowMode->Value);
            void DrawSpell(SpellCast* spellCastInfo);
        	static void ShowBoundingRadius(Object* obj, int quality);
            static void ShowAARadius(Object* obj, int quality);
            void Initialize();
            void Update();
        }

        namespace Zoom
        {
            inline Vector2 pathEnd2DLast;
            void mouseLock();
            void resetZoom();
            void InitializeZoom();
            void Initialize();
            void Update();
        }

        namespace JungleTracker
        {
            void Initialize();
            void Update();
            void DrawJungleTracker();
            void DrawJungleNames();
            void DrawIcons(Vector3 objDrawPos, ImVec2 pos);
            void DrawTimers(Object* obj, ImVec2 pos);
        }
        
    }

}
