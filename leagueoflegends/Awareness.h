#pragma once
#include "../NewMenu.h"
#include "../stdafx.h"

namespace UPasta::SDK::Awareness
{
    namespace Resources
    {
        namespace EnemySidebar
        {
            namespace DX11
            {
                namespace HudDesign
                {
                    inline ID3D11ShaderResourceView* hudHpBar;
                    inline ID3D11ShaderResourceView* hudChampPortrait;

                    void InitializeHudImages();

                }

                namespace HeroIcons
                {
                    const int numTextures = 10;
                    inline ID3D11ShaderResourceView* textureArray[numTextures] = { NULL };

                    void InitializeHeroImages();
                }

                namespace SummonerSpells
                {
                    inline ID3D11ShaderResourceView* summSpellBarrier;
                    inline ID3D11ShaderResourceView* summSpellFlash;
                    inline ID3D11ShaderResourceView* summSpellCleanse;
                    inline ID3D11ShaderResourceView* summSpellExhaust;
                    inline ID3D11ShaderResourceView* summSpellGhost;
                    inline ID3D11ShaderResourceView* summSpellHeal;
                    inline ID3D11ShaderResourceView* summSpellIgnite;
                    inline ID3D11ShaderResourceView* summSpellSmite;
                    inline ID3D11ShaderResourceView* summSpellTeleport;

                    void InitializeSummSpellsImages();
                }

            }

            inline bool initializedTextures;
            void InitializeTextures();
        }

        namespace JungleTracker
        {
            namespace DX11
            {

                namespace HudDesign
                {

                    inline ID3D11ShaderResourceView* hudJunglePortrait;

                    void InitializeHudImages();

                }

                namespace JungleIcons
                {
                    inline ID3D11ShaderResourceView* baronTextureIcon;
                    inline ID3D11ShaderResourceView* blueTextureIcon;
                    inline ID3D11ShaderResourceView* crabTextureIcon;
                    inline ID3D11ShaderResourceView* dragonTextureIcon;
                    inline ID3D11ShaderResourceView* dragonairTextureIcon;
                    inline ID3D11ShaderResourceView* dragonchemtechTextureIcon;
                    inline ID3D11ShaderResourceView* dragonearthTextureIcon;
                    inline ID3D11ShaderResourceView* dragonelderTextureIcon;
                    inline ID3D11ShaderResourceView* dragonfireTextureIcon;
                    inline ID3D11ShaderResourceView* dragonhextechTextureIcon;
                    inline ID3D11ShaderResourceView* dragonwaterTextureIcon;
                    inline ID3D11ShaderResourceView* grompTextureIcon;
                    inline ID3D11ShaderResourceView* krugTextureIcon;
                    inline ID3D11ShaderResourceView* murkwolfTextureIcon;
                    inline ID3D11ShaderResourceView* razorbeakTextureIcon;
                    inline ID3D11ShaderResourceView* redTextureIcon;
                    inline ID3D11ShaderResourceView* riftheraldTextureIcon;

                    void InitializeJungleImages();
                }

            }

            inline bool initializedTextures;
            void InitializeTextures();
        }
    }


    namespace Configs
    {
        inline bool initializedAwarenessMenu;
        extern Menu* AwarenessMenu;

        namespace EnemyTracker
        {
            inline bool initializedTrackerMenu;
            void Initialize();
            void InitializeTrackerMenu();

            inline CheckBox* status;

            inline CheckBox* showDamagePrediction;
            inline CheckBox* showKillableStatus;

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
        	inline CheckBox* showLastPositionMiniMap;
        }

        namespace EnemySidebar
        {
            inline bool open_ptr = true;
            inline bool initializedSidebarMenu;
            void Initialize();
            void InitializeSidebarMenu();

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
            inline List* drawMode;
            inline CheckBox* showHeight;
            inline Slider* heightTollerance;

            inline Slider* qualityDraw;

        	inline CheckBox* showMissiles;
            inline CheckBox* showMissilesAnimation;
            inline CheckBox* showBoundingRadius;
            inline CheckBox* showAARadius;
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
        }

    }

    namespace Functions
    {
        void Initialize();
        void Update();

        namespace EnemyTracker
        {
            constexpr float levelExp[18] = { 0.f, 280.f, 660.f, 1140.f, 1720.f, 2400.f, 3180.f, 4060.f,
                         5040.f, 6120.f, 7300.f, 8580.f, 9960.f, 11440.f, 13020.f,
                         14700.f, 16480.f, 18360.f };

            constexpr float expNeeded[18] = { 0.f, 280.f, 380.f, 480.f, 580.f, 680.f, 780.f, 880.f,
                                                           980.f, 1080.f, 1180.f, 1280.f, 1380.f, 1480.f, 1580.f,
                                                           1680.f, 1780.f, 1880.f };

            void DrawTracker();
            void DrawCooldownBar(Object* obj);
            void DrawPlayerPaths(Object* obj);
            void ShowLastEnemyPosition(Object* obj, int index);
            void DrawLastHitDamage();


            void Initialize();
            void Update();
        }

        namespace EnemySidebar
        {
            ImTextureID GetSummonerSpellTexture(Object* obj, int spellIndex);
            void LoadDX11ImageIfNeeded(const char* filename, bool loaded, ID3D11ShaderResourceView** texture);
            static void ShowSidebar(bool* p_open);
            void Initialize();
            void Update();
        }

        namespace Radius
        {
            void DrawRadius(Vector3 worldPos, float radius, uintptr_t color, float thickness, bool takeHeightInConsideration = Configs::Radius::showHeight->Value, bool glow = false);
            static void ShowMissiles();
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
