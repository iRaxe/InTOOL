#include "Awareness.h"
#include "Orbwalker.h"

#include "LoadImages.h"
#include "TargetSelector.h"
#include "zoom.h"
#include "../stdafx.h"
#include "Geometry.h"
namespace UPasta
{
	namespace SDK
	{
		namespace Awareness
		{
			namespace Resources
			{
				namespace EnemySidebar
				{
					namespace DX9
					{

						namespace HudDesign
						{
							static bool hudImagesLoaded = false;
							void InitializeHudImages()
							{
								if (!hudImagesLoaded)
								{
									const std::string awarenessFolder = "C:\\UPorn\\Media\\Awareness";

									static bool hudHpBarLoaded = false;
									const std::string hudHpBarFile = awarenessFolder + "\\hud_champ_hpbar.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(hudHpBarFile.c_str(), hudHpBarLoaded, hudHpBar);

									static bool hudChampPortraitLoaded = false;
									const std::string hudChampPortraitFile = awarenessFolder + "\\hud_champ_portrait.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(hudChampPortraitFile.c_str(), hudChampPortraitLoaded, hudChampPortrait);

									hudImagesLoaded = true;
								}
							}
						}

						namespace HeroIcons
						{
							static bool heroImagesLoaded = false;
							void InitializeHeroImages()
							{
								for (int i = 0; i < globals::heroManager->GetListSize(); i++)
								{
									auto obj = globals::heroManager->GetIndex(i);
									const std::string heroIconFile = "C:\\UPorn\\Media\\Champions\\" + str_tolower(obj->GetName()) + ".png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(heroIconFile.c_str(), false, textureArray[i]);
								}

								heroImagesLoaded = true;
							}
						}
						
						namespace SummonerSpells
						{
							static bool summSpellsImagesLoaded = false;
							void InitializeSummSpellsImages()
							{
								if (!summSpellsImagesLoaded)
								{
									const std::string awarenessFolder = "C:\\UPorn\\Media\\Champions";

									static bool summSpellBarrierLoaded = false;
									const std::string summSpellBarrierFile = awarenessFolder + "\\Barrier.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellBarrierFile.c_str(), summSpellBarrierLoaded, summSpellBarrier);

									static bool summSpellFlashLoaded = false;
									const std::string summSpellFlashFile = awarenessFolder + "\\Flash.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellFlashFile.c_str(), summSpellFlashLoaded, summSpellFlash);

									static bool summSpellCleanseLoaded = false;
									const std::string summSpellCleanseFile = awarenessFolder + "\\Cleanse.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellCleanseFile.c_str(), summSpellCleanseLoaded, summSpellCleanse);

									static bool summSpellExhaustLoaded = false;
									const std::string summSpellExhaustFile = awarenessFolder + "\\Exhaust.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellExhaustFile.c_str(), summSpellExhaustLoaded, summSpellExhaust);

									static bool summSpellGhostLoaded = false;
									const std::string summSpellGhostFile = awarenessFolder + "\\Ghost.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellGhostFile.c_str(), summSpellGhostLoaded, summSpellGhost);

									static bool summSpellHealLoaded = false;
									const std::string summSpellHealFile = awarenessFolder + "\\Heal.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellHealFile.c_str(), summSpellHealLoaded, summSpellHeal);

									static bool summSpellIgniteLoaded = false;
									const std::string summSpellIgniteFile = awarenessFolder + "\\Ignite.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellIgniteFile.c_str(), summSpellIgniteLoaded, summSpellIgnite);

									static bool summSpellSmiteLoaded = false;
									const std::string summSpellSmiteFile = awarenessFolder + "\\Smite.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellSmiteFile.c_str(), summSpellSmiteLoaded, summSpellSmite);

									static bool summSpellTeleportLoaded = false;
									const std::string summSpellTeleportFile = awarenessFolder + "\\Teleport.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(summSpellTeleportFile.c_str(), summSpellTeleportLoaded, summSpellTeleport);

									summSpellsImagesLoaded = true;
								}
							}
						}

					}

					namespace DX11
					{

						namespace HudDesign
						{
							static bool hudImagesLoaded = false;
							void InitializeHudImages()
							{
								if (!hudImagesLoaded)
								{
									const std::string awarenessFolder = "C:\\UPorn\\Media\\Awareness";

									static bool hudHpBarLoaded = false;
									const std::string hudHpBarFile = awarenessFolder + "\\hud_champ_hpbar.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(hudHpBarFile.c_str(), hudHpBarLoaded, &hudHpBar);

									static bool hudChampPortraitLoaded = false;
									const std::string hudChampPortraitFile = awarenessFolder + "\\hud_champ_portrait.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(hudChampPortraitFile.c_str(), hudChampPortraitLoaded, &hudChampPortrait);

									hudImagesLoaded = true;
								}
							}
						}

						namespace HeroIcons
						{
							static bool heroImagesLoaded = false;
							void InitializeHeroImages()
							{
								for (int i = 0; i < globals::heroManager->GetListSize(); i++)
								{
									auto obj = globals::heroManager->GetIndex(i);
									const std::string heroIconFile = "C:\\UPorn\\Media\\Champions\\" + str_tolower(obj->GetName()) + ".png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(heroIconFile.c_str(), false, &textureArray[i]);
								}

								heroImagesLoaded = true;
							}
						}
						
						namespace SummonerSpells
						{
							static bool summSpellsImagesLoaded = false;
							void InitializeSummSpellsImages()
							{
								if (!summSpellsImagesLoaded)
								{
									const std::string awarenessFolder = "C:\\UPorn\\Media\\Champions";

									static bool summSpellBarrierLoaded = false;
									const std::string summSpellBarrierFile = awarenessFolder + "\\Barrier.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellBarrierFile.c_str(), summSpellBarrierLoaded, &summSpellBarrier);

									static bool summSpellFlashLoaded = false;
									const std::string summSpellFlashFile = awarenessFolder + "\\Flash.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellFlashFile.c_str(), summSpellFlashLoaded, &summSpellFlash);

									static bool summSpellCleanseLoaded = false;
									const std::string summSpellCleanseFile = awarenessFolder + "\\Cleanse.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellCleanseFile.c_str(), summSpellCleanseLoaded, &summSpellCleanse);

									static bool summSpellExhaustLoaded = false;
									const std::string summSpellExhaustFile = awarenessFolder + "\\Exhaust.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellExhaustFile.c_str(), summSpellExhaustLoaded, &summSpellExhaust);

									static bool summSpellGhostLoaded = false;
									const std::string summSpellGhostFile = awarenessFolder + "\\Ghost.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellGhostFile.c_str(), summSpellGhostLoaded, &summSpellGhost);

									static bool summSpellHealLoaded = false;
									const std::string summSpellHealFile = awarenessFolder + "\\Heal.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellHealFile.c_str(), summSpellHealLoaded, &summSpellHeal);

									static bool summSpellIgniteLoaded = false;
									const std::string summSpellIgniteFile = awarenessFolder + "\\Ignite.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellIgniteFile.c_str(), summSpellIgniteLoaded, &summSpellIgnite);

									static bool summSpellSmiteLoaded = false;
									const std::string summSpellSmiteFile = awarenessFolder + "\\Smite.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellSmiteFile.c_str(), summSpellSmiteLoaded, &summSpellSmite);

									static bool summSpellTeleportLoaded = false;
									const std::string summSpellTeleportFile = awarenessFolder + "\\Teleport.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(summSpellTeleportFile.c_str(), summSpellTeleportLoaded, &summSpellTeleport);

									summSpellsImagesLoaded = true;
								}
							}
						}

					}

					void InitializeTextures()
					{
						__try
						{
							if (globals::renderType == "D3D9")
							{
								__try { DX9::HeroIcons::InitializeHeroImages(); }
								__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing hero images"); }

								__try { DX9::HudDesign::InitializeHudImages(); }
								__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing hud images"); }

								__try { DX9::SummonerSpells::InitializeSummSpellsImages(); }
								__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing summspells images"); }
								initializedTextures = true;
							}
							else if (globals::renderType == "D3D11")
							{
								__try { DX11::HeroIcons::InitializeHeroImages(); }
								__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing hero images"); }

								__try { DX11::HudDesign::InitializeHudImages(); }
								__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing hud images"); }

								__try { DX11::SummonerSpells::InitializeSummSpellsImages(); }
								__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing summspells images"); }
								initializedTextures = true;
							}
						}
						__except (1)
						{
							 LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing textures"); 
						}
					}
				}

				namespace JungleTracker
				{
					namespace DX9
					{

						namespace HudDesign
						{
							static bool hudImagesLoaded = false;
							void InitializeHudImages()
							{
								if (!hudImagesLoaded)
								{
									const std::string awarenessFolder = "C:\\UPorn\\Media\\Awareness";

									static bool hudJunglePortraitLoaded = false;
									const std::string hudJunglePortraitFile = awarenessFolder + "\\hud_jung_portrait.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(hudJunglePortraitFile.c_str(), hudJunglePortraitLoaded, hudJunglePortrait);
									hudImagesLoaded = true;
								}
							}
						}

						namespace JungleIcons
						{
							static bool JungleImagesLoaded = false;
							void InitializeJungleImages()
							{
								if (!JungleImagesLoaded)
								{
									const std::string jungleFolder = "C:\\UPorn\\Media\\Jungle";

									static bool baronIconLoaded = false;
									const std::string baronIconFile = jungleFolder + "\\SRU_Baron.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(baronIconFile.c_str(), baronIconLoaded, baronTextureIcon);

									static bool blueIconLoaded = false;
									const std::string blueIconFile = jungleFolder + "\\SRU_Blue.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(blueIconFile.c_str(), blueIconLoaded, blueTextureIcon);

									static bool crabIconLoaded = false;
									const std::string crabIconFile = jungleFolder + "\\Sru_Crab.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(crabIconFile.c_str(), crabIconLoaded, crabTextureIcon);

									static bool dragonIconLoaded = false;
									const std::string dragonIconFile = jungleFolder + "\\SRU_Dragon.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonIconFile.c_str(), dragonIconLoaded, dragonTextureIcon);

									static bool dragonairIconLoaded = false;
									const std::string dragonairIconFile = jungleFolder + "\\SRU_Dragon_Air.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonairIconFile.c_str(), dragonairIconLoaded, dragonairTextureIcon);

									static bool dragonchemtechIconLoaded = false;
									const std::string dragonchemtechIconFile = jungleFolder + "\\SRU_Dragon_Chemtech.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonchemtechIconFile.c_str(), dragonchemtechIconLoaded, dragonchemtechTextureIcon);

									static bool dragonearthIconLoaded = false;
									const std::string dragonearthIconFile = jungleFolder + "\\SRU_Dragon_Earth.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonearthIconFile.c_str(), dragonearthIconLoaded, dragonearthTextureIcon);

									static bool dragonelderIconLoaded = false;
									const std::string dragonelderIconFile = jungleFolder + "\\SRU_Dragon_Elder.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonelderIconFile.c_str(), dragonelderIconLoaded, dragonelderTextureIcon);

									static bool dragonfireIconLoaded = false;
									const std::string dragonfireIconFile = jungleFolder + "\\SRU_Dragon_Fire.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonfireIconFile.c_str(), dragonfireIconLoaded, dragonfireTextureIcon);

									static bool dragonhextechIconLoaded = false;
									const std::string dragonhextechIconFile = jungleFolder + "\\SRU_Dragon_Hextech.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonhextechIconFile.c_str(), dragonhextechIconLoaded, dragonhextechTextureIcon);

									static bool dragonwaterIconLoaded = false;
									const std::string dragonwaterIconFile = jungleFolder + "\\SRU_Dragon_Water.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(dragonwaterIconFile.c_str(), dragonwaterIconLoaded, dragonwaterTextureIcon);

									static bool grompIconLoaded = false;
									const std::string grompIconFile = jungleFolder + "\\SRU_Gromp.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(grompIconFile.c_str(), grompIconLoaded, grompTextureIcon);

									static bool krugIconLoaded = false;
									const std::string krugIconFile = jungleFolder + "\\SRU_Krug.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(krugIconFile.c_str(), krugIconLoaded, krugTextureIcon);

									static bool murkwolfIconLoaded = false;
									const std::string murkwolfIconFile = jungleFolder + "\\SRU_Murkwolf.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(murkwolfIconFile.c_str(), murkwolfIconLoaded, murkwolfTextureIcon);

									static bool razorbeakIconLoaded = false;
									const std::string razorbeakIconFile = jungleFolder + "\\SRU_Razorbeak.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(razorbeakIconFile.c_str(), razorbeakIconLoaded, razorbeakTextureIcon);

									static bool redIconLoaded = false;
									const std::string redIconFile = jungleFolder + "\\SRU_Red.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(redIconFile.c_str(), redIconLoaded, redTextureIcon);

									static bool riftheraldIconLoaded = false;
									const std::string riftheraldIconFile = jungleFolder + "\\SRU_RiftHerald.png";
									Functions::EnemySidebar::LoadDX9ImageIfNeeded(riftheraldIconFile.c_str(), riftheraldIconLoaded, riftheraldTextureIcon);
									JungleImagesLoaded = true;
								}
							}
						}

					}

					namespace DX11
					{

						namespace HudDesign
						{
							static bool hudImagesLoaded = false;
							void InitializeHudImages()
							{
								if (!hudImagesLoaded)
								{
									const std::string awarenessFolder = "C:\\UPorn\\Media\\Awareness";

									static bool hudJunglePortraitLoaded = false;
									const std::string hudJunglePortraitFile = awarenessFolder + "\\hud_jung_portrait.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(hudJunglePortraitFile.c_str(), hudJunglePortraitLoaded, &hudJunglePortrait);

									hudImagesLoaded = true;
								}
							}
						}

						namespace JungleIcons
						{
							static bool JungleImagesLoaded = false;
							void InitializeJungleImages()
							{
								if (!JungleImagesLoaded)
								{
									const std::string jungleFolder = "C:\\UPorn\\Media\\Jungle";

									static bool baronIconLoaded = false;
									const std::string baronIconFile = jungleFolder + "\\SRU_Baron.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(baronIconFile.c_str(), baronIconLoaded, &baronTextureIcon);

									static bool blueIconLoaded = false;
									const std::string blueIconFile = jungleFolder + "\\SRU_Blue.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(blueIconFile.c_str(), blueIconLoaded, &blueTextureIcon);

									static bool crabIconLoaded = false;
									const std::string crabIconFile = jungleFolder + "\\Sru_Crab.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(crabIconFile.c_str(), crabIconLoaded, &crabTextureIcon);

									static bool krugIconLoaded = false;
									const std::string krugIconFile = jungleFolder + "\\SRU_Krug.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(krugIconFile.c_str(), krugIconLoaded, &krugTextureIcon);

									static bool dragonIconLoaded = false;
									const std::string dragonIconFile = jungleFolder + "\\SRU_Dragon.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonIconFile.c_str(), dragonIconLoaded, &dragonTextureIcon);

									static bool dragonairIconLoaded = false;
									const std::string dragonairIconFile = jungleFolder + "\\SRU_Dragon_Air.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonairIconFile.c_str(), dragonairIconLoaded, &dragonairTextureIcon);

									static bool dragonchemtechIconLoaded = false;
									const std::string dragonchemtechIconFile = jungleFolder + "\\SRU_Dragon_Chemtech.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonchemtechIconFile.c_str(), dragonchemtechIconLoaded, &dragonchemtechTextureIcon);

									static bool dragonearthIconLoaded = false;
									const std::string dragonearthIconFile = jungleFolder + "\\SRU_Dragon_Earth.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonearthIconFile.c_str(), dragonearthIconLoaded, &dragonearthTextureIcon);

									static bool dragonelderIconLoaded = false;
									const std::string dragonelderIconFile = jungleFolder + "\\SRU_Dragon_Elder.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonelderIconFile.c_str(), dragonelderIconLoaded, &dragonelderTextureIcon);

									static bool dragonfireIconLoaded = false;
									const std::string dragonfireIconFile = jungleFolder + "\\SRU_Dragon_Fire.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonfireIconFile.c_str(), dragonfireIconLoaded, &dragonfireTextureIcon);

									static bool dragonhextechIconLoaded = false;
									const std::string dragonhextechIconFile = jungleFolder + "\\SRU_Dragon_Hextech.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonhextechIconFile.c_str(), dragonhextechIconLoaded, &dragonhextechTextureIcon);

									static bool dragonwaterIconLoaded = false;
									const std::string dragonwaterIconFile = jungleFolder + "\\SRU_Dragon_Water.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(dragonwaterIconFile.c_str(), dragonwaterIconLoaded, &dragonwaterTextureIcon);

									static bool grompIconLoaded = false;
									const std::string grompIconFile = jungleFolder + "\\SRU_Gromp.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(grompIconFile.c_str(), grompIconLoaded, &grompTextureIcon);

									static bool murkwolfIconLoaded = false;
									const std::string murkwolfIconFile = jungleFolder + "\\SRU_Murkwolf.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(murkwolfIconFile.c_str(), murkwolfIconLoaded, &murkwolfTextureIcon);

									static bool razorbeakIconLoaded = false;
									const std::string razorbeakIconFile = jungleFolder + "\\SRU_Razorbeak.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(razorbeakIconFile.c_str(), razorbeakIconLoaded, &razorbeakTextureIcon);

									static bool redIconLoaded = false;
									const std::string redIconFile = jungleFolder + "\\SRU_Red.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(redIconFile.c_str(), redIconLoaded, &redTextureIcon);

									static bool riftheraldIconLoaded = false;
									const std::string riftheraldIconFile = jungleFolder + "\\SRU_RiftHerald.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(riftheraldIconFile.c_str(), riftheraldIconLoaded, &riftheraldTextureIcon);
									JungleImagesLoaded = true;
								}
							}
						}

					}

					void InitializeTextures()
					{
						__try
						{
							if (globals::renderType == "D3D9")
							{
								__try { DX9::HudDesign::InitializeHudImages(); }
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing hud images"); }

								__try { DX9::JungleIcons::InitializeJungleImages(); }
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing jungle images"); }

								initializedTextures = true;
							}
							else if (globals::renderType == "D3D11")
							{
								__try { DX11::HudDesign::InitializeHudImages(); }
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing hud images"); }

								__try { DX11::JungleIcons::InitializeJungleImages(); }
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing jungle images"); }

								initializedTextures = true;
							}
						}
						__except (1)
						{
							LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing textures");
						}
					}
				}
			}


			namespace Configs
			{
				Menu* AwarenessMenu;
				void Initialize()
				{
					AwarenessMenu = Menu::CreateMenu("Awareness", "vez.Awareness");
					initializedAwarenessMenu = true;
				}

				namespace EnemySidebar
				{
					Menu* EnemySidebarMenu;
					void InitializeSidebarMenu()
					{
						EnemySidebarMenu = AwarenessMenu->AddMenu("enemySidebar", "Enemies sidebar");
						status = EnemySidebarMenu->AddCheckBox("status", "Enable the sidebar", true);
						orientationHorizontal = EnemySidebarMenu->AddCheckBox("orientationHorizontal", "Rotate horizontal", false);
						hudSize = EnemySidebarMenu->AddSlider("hudSize", "Sidebar Size", 100, 50, 300, 50);
						initializedSidebarMenu = true;
					}
				}

				namespace Radius
				{
					Menu* RadiusMenu;
					void InitializeRadiusMenu()
					{
						RadiusMenu = AwarenessMenu->AddMenu("utilityRadius", "Radius drawings");
						status = RadiusMenu->AddCheckBox("status", "Enable radius drawings", true);
						showBoundingRadius = RadiusMenu->AddCheckBox("showBoundingRadius", "Enable bounding radius", true);
						showAARadius = RadiusMenu->AddCheckBox("showAARadius", "Enable autoattack radius", true);
						showAARadiusSelf = RadiusMenu->AddCheckBox("showAARadiusSelf", "Show your radius", true);
						showAARadiusAllies = RadiusMenu->AddCheckBox("showAARadiusAllies", "Show radius for allies", false);
						showAARadiusEnemies = RadiusMenu->AddCheckBox("showAARadiusEnemies", "Show radius for enemies", false);
						qualityDraw = RadiusMenu->AddSlider("qualityDraw", "Drawings quality", 50, 10, 100, 10);
						initializedRadiusMenu = true;
					}
				}

				namespace Zoom
				{
					Menu* ZoomMenu;
					void InitializeZoomMenu()
					{
						ZoomMenu = AwarenessMenu->AddMenu("zoomUtility", "Camera settings");
						status = ZoomMenu->AddCheckBox("status", "Enable camera tool", true);
						status3D = ZoomMenu->AddCheckBox("status3D", "Enable view 3D", false);
						statusKeyboard = ZoomMenu->AddCheckBox("statusKeyboard", "Enable Keyboard inputs", true);
						statusMouseWheel = ZoomMenu->AddCheckBox("statusMouseWheel", "Enable Mouse Wheel inputs", true);
						zoomValue = ZoomMenu->AddSlider("zoomValue", "Zoom Value", 12.81169772f, 7.80f, 27.0f, 1.0f);

						initializedZoomMenu = true;
					}
				}

				namespace JungleTracker
				{
					Menu* JungleTrackerMenu;
					void InitializeJungleTrackerMenu()
					{
						JungleTrackerMenu = AwarenessMenu->AddMenu("jungleUtility", "Jungle tracker settings");
						status = JungleTrackerMenu->AddCheckBox("status", "Enable jungle tracker tool", true);
						showTimer = JungleTrackerMenu->AddCheckBox("showTimer", "Show timers", true);
						showIcons = JungleTrackerMenu->AddCheckBox("showIcons", "Show icons", true);

						initializedJungleTrackerMenu = true;
					}
				}
			}

			namespace Functions
			{
				void Initialize()
				{
					if (!Configs::initializedAwarenessMenu)
					{
						Configs::Initialize();
					}

					if (Configs::initializedAwarenessMenu)
					{
						EnemySidebar::Initialize();
						Radius::Initialize();
						Zoom::Initialize();
						JungleTracker::Initialize();
					}
				}

				float startAngleCamera = 0.0f;  // Store the starting angle for interpolation
				float targetAngleCamera = 0.0f; // Store the target angle for interpolation
				float currentAngleCamera = 0.0f; // Store the current interpolated angle
				float transitionDuration = 2.0f; // Transition duration in seconds
				float gameTime = 0.0f;
				float lastMoveTime = 0.0f;
				float transitionTimer = 0.0f;    // Timer for tracking the transition progress
				bool refresh = false;
				float cameraVelocity = 0.0f;

				void Update()
				{
					EnemySidebar::Update();
					Radius::Update();
					Zoom::Update();
					JungleTracker::Update();
				}
				
				namespace EnemySidebar
				{
					ImTextureID GetSummonerSpellTexture(Object* obj, int spellIndex)
					{
						if (obj->HasBarrier(spellIndex)) {
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellBarrier;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellBarrier;
						}

						else if (obj->HasFlash(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellFlash;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellFlash;
						}

						else if (obj->HasCleanse(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellCleanse;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellCleanse;
						}

						else if (obj->HasExhaust(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellExhaust;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellExhaust;
						}

						else if (obj->HasGhost(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellGhost;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellGhost;
						}

						else if (obj->HasHeal(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellHeal;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellHeal;
						}

						else if (obj->HasIgnite(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellIgnite;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellIgnite;
						}

						else if (obj->HasSmite(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellSmite;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellSmite;
						}

						else if (obj->HasTeleport(spellIndex))
						{
							if (globals::renderType == "D3D9")
								return Resources::EnemySidebar::DX9::SummonerSpells::summSpellTeleport;
							else if (globals::renderType == "D3D11")
								return Resources::EnemySidebar::DX11::SummonerSpells::summSpellTeleport;
						}

						return nullptr;
					}
					
					void LoadDX9ImageIfNeeded(const char* filename, bool loaded, PDIRECT3DTEXTURE9 texture)
					{
						__try
						{
							if (!loaded)
							{
								int image_width = 0;
								int image_height = 0;
								bool success = LoadTextureFromFileDX9(filename, &texture, &image_width, &image_height);
								IM_ASSERT(success);
								loaded = true;
							}
						}
						__except (1)
						{
							LOG("[TEXTURE LOAD] Error in load texture by DX9");
						}
					}

					void LoadDX11ImageIfNeeded(const char* filename, bool loaded, ID3D11ShaderResourceView** texture)
					{
						__try
						{
							if (!loaded)
							{
								int image_width = 0;
								int image_height = 0;
								bool success = LoadTextureFromFile(filename, texture, &image_width, &image_height);
								IM_ASSERT(success);
								loaded = true;
							}
						}
						__except (1)
						{
							LOG("[TEXTURE LOAD] Error in load texture by DX11");
						}
					}

					static void ShowSidebar(bool* p_open)
					{
						float portrait_size = 80.0;
						float offsetX = 0.65;
						float offsetY = -0.3;

						float portrait_hsize = portrait_size / 2.0;
						ImVec2 portrait_vsize = ImVec2(portrait_size, portrait_size);
						ImVec2 champ_icon_size = ImVec2(portrait_size * 0.75, portrait_size * 0.75);
						ImVec2 spell1_offset = ImVec2(-portrait_size / 5, portrait_size / 3.2);
						ImVec2 spell1_size = ImVec2(portrait_size / 4.0, portrait_size / 4.0);
						ImVec2 spell2_offset = ImVec2(portrait_size / 5.3, portrait_size / 3.3);
						ImVec2 spell2_size = ImVec2(portrait_size / 4.0, portrait_size / 4.0);
						ImVec2 hpbar_size = ImVec2(50 * (portrait_size * 0.007), 122 * (portrait_size * 0.007));
						ImVec2 hpbar_offset = ImVec2(portrait_size * 0.3, -portrait_size * 0.05);

						ImGuiWindowFlags window_flags = 0;
						window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
						window_flags |= ImGuiWindowFlags_NoBackground;

						if (ImGui::Begin("Hero awareness", p_open, window_flags))
						{
							ImVec2 p2 = ImGui::GetCursorScreenPos();
							ImGui::BeginChild("Test", ImVec2(600, 600), false);
							float spacing = 0.0f;
							for (int i = 1; i < globals::heroManager->GetListSize(); i++)
							{
								auto obj = globals::heroManager->GetIndex(i);
								if (obj->GetName() == "PracticeTool_TargetDummy") continue;
								if (obj->IsAlly()) continue;
								ImVec2 p = ImVec2(0 ,0);
								if (Configs::EnemySidebar::orientationHorizontal->Value == true)
									p = ImVec2(p2.x + spacing, p2.y);
								else
									p = ImVec2(p2.x , p2.y + spacing);

								ImVec2 heroIconMin = ImVec2(p.x + 10, p.y + 10);
								ImVec2 heroIconMax = ImVec2(p.x + 80, p.y + 80);
								if (globals::renderType == "D3D9")
									ImGui::GetWindowDrawList()->AddImage(Resources::EnemySidebar::DX9::HeroIcons::textureArray[i], heroIconMin, heroIconMax, { 0.0f, 0.0f }, { 1.0f, 1.0f });
								else if (globals::renderType == "D3D11")
									ImGui::GetWindowDrawList()->AddImage(Resources::EnemySidebar::DX11::HeroIcons::textureArray[i], heroIconMin, heroIconMax, { 0.0f, 0.0f }, { 1.0f, 1.0f });

								if (obj->GetHealth() <= 0)
								{
									render::RenderCircleFilled(ImVec2(p.x + 45, p.y + 45), 35, COLOR_DARK_TRANSPARENT, 0);
									float respawnTime = functions::GetRespawnTimer(obj) + 1;
									render::RenderText(functions::ConvertTime(respawnTime), heroIconMin, 24, COLOR_RED, false);
								}

								float altezzaIntera = 80.00f;
								float altezzaDaTogliere = obj->GetPercentHealth();
								float altezzaTotale = altezzaIntera * (altezzaDaTogliere / 100);

								ImVec2 hudHpMin = ImVec2(p.x + 60, p.y + 5);
								ImVec2 hudHpMax = ImVec2(p.x + 95, p.y + altezzaTotale);
								if (globals::renderType == "D3D9")
									ImGui::GetWindowDrawList()->AddImage(Resources::EnemySidebar::DX9::HudDesign::hudHpBar, hudHpMin, hudHpMax, { 0.0f, 0.0f }, { 1.0f, altezzaDaTogliere / 100 }, COLOR_GREEN);
								else if (globals::renderType == "D3D11")
									ImGui::GetWindowDrawList()->AddImage(Resources::EnemySidebar::DX11::HudDesign::hudHpBar, hudHpMin, hudHpMax, { 0.0f, 0.0f }, { 1.0f, altezzaDaTogliere / 100 }, COLOR_GREEN);

								ImVec2 hudChampPortraitPosition = ImVec2(p.x + 100, p.y + 100);
								if (globals::renderType == "D3D9")
									ImGui::GetWindowDrawList()->AddImage(Resources::EnemySidebar::DX9::HudDesign::hudChampPortrait, p, hudChampPortraitPosition);
								else if (globals::renderType == "D3D11")
									ImGui::GetWindowDrawList()->AddImage(Resources::EnemySidebar::DX11::HudDesign::hudChampPortrait, p, hudChampPortraitPosition);

								ImVec2 SummSpell1Min = ImVec2(p.x + 20, p.y + 70);
								ImVec2 SummSpell1Max = ImVec2(p.x + 40, p.y + 90);
								if (globals::renderType == "D3D9")
									ImGui::GetWindowDrawList()->AddImageRounded(GetSummonerSpellTexture(obj,4), SummSpell1Min, SummSpell1Max, { 0.0f, 0.0f }, { 1.0f, 1.0f }, COLOR_WHITE, 200.0f);
								else if (globals::renderType == "D3D11")
									ImGui::GetWindowDrawList()->AddImageRounded(GetSummonerSpellTexture(obj, 4), SummSpell1Min, SummSpell1Max, { 0.0f, 0.0f }, { 1.0f, 1.0f }, COLOR_WHITE, 200.0f);

								ImVec2 SummSpell2Min = ImVec2(p.x + 58, p.y + 70);
								ImVec2 SummSpell2Max = ImVec2(p.x + 80, p.y + 90);

								if (globals::renderType == "D3D9")
									ImGui::GetWindowDrawList()->AddImageRounded(GetSummonerSpellTexture(obj, 5), SummSpell2Min, SummSpell2Max, { 0.0f, 0.0f }, { 1.0f, 1.0f }, COLOR_WHITE, 200.0f);
								else if (globals::renderType == "D3D11")
									ImGui::GetWindowDrawList()->AddImageRounded(GetSummonerSpellTexture(obj, 5), SummSpell2Min, SummSpell2Max, { 0.0f, 0.0f }, { 1.0f, 1.0f }, COLOR_WHITE, 200.0f);

								spacing += 120;
							}

							ImGui::EndChild();

						}

						ImGui::End();
					}

					void Initialize()
					{
						if (!Resources::EnemySidebar::initializedTextures)
						{
							Resources::EnemySidebar::InitializeTextures();
						}
						if (Resources::EnemySidebar::initializedTextures && !Configs::EnemySidebar::initializedSidebarMenu)
						{
							Configs::EnemySidebar::InitializeSidebarMenu();
						}
					}

					void Update()
					{
						if (Resources::EnemySidebar::initializedTextures && Configs::EnemySidebar::initializedSidebarMenu)
						{
							if (Configs::EnemySidebar::status->Value == true)
							{
								ShowSidebar(&Configs::EnemySidebar::status->Value);
							}
						}
					}
				}

				namespace Radius
				{
					static void ShowBoundingRadius(Object* obj, int quality)
					{
						if (obj->IsAlive() && obj->IsVisible())
						{
							if (obj->GetNetId() == globals::localPlayer->GetNetId())
							{
								if (Configs::Radius::showAARadiusSelf->Value == true)
								{
									render::RenderArcWorld(obj->GetPosition(), quality, obj->GetBoundingRadius(), COLOR_WHITE, 1.0f, PI / 3, functions::GetMouseWorldPos(), true);
								}
							}
							else
							{

								if (obj->IsEnemy() && Configs::Radius::showAARadiusEnemies->Value == true)
								{
									render::RenderArcWorld(obj->GetPosition(), quality, obj->GetBoundingRadius(), COLOR_WHITE, 1.0f, PI / 3, globals::localPlayer->GetPosition(), true);
								}
								if (obj->IsAlly() && Configs::Radius::showAARadiusAllies->Value == true)
									render::RenderArcWorld(obj->GetPosition(), quality, obj->GetBoundingRadius(), COLOR_WHITE, 1.0f, PI / 3, globals::localPlayer->GetPosition(), true);
							}
						}
					}
					static void ShowAARadius(Object* obj, int quality)
					{
						if (obj->IsAlive() && obj->IsVisible())
						{
							if (obj->GetNetId() == globals::localPlayer->GetNetId())
							{
								if (Configs::Radius::showAARadiusSelf->Value == true)
									render::RenderArcWorld(obj->GetPosition(), quality, obj->GetRealAttackRange(), COLOR_WHITE, 1.0f, PI / 2, functions::GetMouseWorldPos(), true);
							}
							else
							{
								if (obj->IsEnemy() && Configs::Radius::showAARadiusEnemies->Value == true)
									render::RenderArcWorld(obj->GetPosition(), quality, obj->GetRealAttackRange(), COLOR_WHITE, 1.0f, PI / 2, globals::localPlayer->GetPosition(), true);
								if (obj->IsAlly() && Configs::Radius::showAARadiusAllies->Value == true)
									render::RenderArcWorld(obj->GetPosition(), quality, obj->GetRealAttackRange(), COLOR_WHITE, 1.0f, PI / 2, globals::localPlayer->GetPosition(), true);
							}
						}
					}

					void Initialize()
					{
						if (!Configs::Radius::initializedRadiusMenu)
						{
							Configs::Radius::InitializeRadiusMenu();
						}
					}
					std::map<Object*, std::tuple<Missile*, Vector3, Vector3, Vector3>> missileMap;
					std::vector<Object*> objectsToRemove; // Lista degli oggetti da rimuovere

					std::vector<Object*> GetMissilesInRange2()
					{
						std::vector<Object*> validMissiles;
						for (int i = 0; i < 3; i++)
						{
							auto obj = globals::missileManager->GetIndex(i);
							if (obj && !missileMap.contains(obj))
							{
								auto missileTest = obj->GetMissileByIndex();
								if (missileTest)
								{
									/*Vector3 extended = missileTest->GetSpellStartPos().Extend
									(missileTest->GetSpellEndPos(), missileTest->GetSpellStartPos().Distance
									(missileTest->GetSpellEndPos()) + 100.0f);*/
									Vector3 startPosVector = missileTest->GetSpellStartPos();
									Vector3 posVector = missileTest->GetSpellPos();
									Vector3 endPosVector = missileTest->GetSpellEndPos();

									missileMap[obj] = std::make_tuple(missileTest, startPosVector, posVector, endPosVector);

									validMissiles.push_back(obj);
								}
							}
						}

						if (missileMap.size() > 10)
							missileMap.clear();

						if (validMissiles.size() > 0)
							return validMissiles;

						

						return validMissiles;
					}

					void Update()
					{
						if (Configs::Radius::initializedRadiusMenu)
						{
							GetMissilesInRange2();
							for (auto it = missileMap.begin(); it != missileMap.end(); ++it)
							{
								Object* correspondingObject = it->first;

								Missile* correspondingMissile = std::get<0>(it->second); // Missile*
								Vector3 startPosVector = std::get<1>(it->second); // Vector3
								Vector3 posVector = std::get<2>(it->second); // Vector3
								Vector3 endPosVector = std::get<3>(it->second); // Vector3

								//Vector3 extendedVector = std::get<1>(it->second); // Vector3
								if (IsValidPtr(correspondingMissile))
								{
									Geometry::Polygon poly = Geometry::Rectangle(startPosVector, endPosVector, 70.f).ToPolygon();
									//Geometry::Polygon poly2 = Geometry::Rectangle(startPosVector, posVector, 70.f).ToPolygon();

									render::RenderPolygon(poly, COLOR_WHITE, 1.0f);
									//render::RenderFilledPolygon(poly2, COLOR_WHITE);
									missileMap.erase(correspondingObject);
								}

							}



							if (Configs::Radius::status->Value == true)
							{
								for (int i = 0; i < globals::heroManager->GetListSize(); i++)
								{
									auto obj = globals::heroManager->GetIndex(i);
									if (!obj->IsAlive()) continue;
									if (!obj->IsVisible()) continue;

									if (obj)
									{
										if (Configs::Radius::showBoundingRadius->Value == true)
											ShowBoundingRadius(obj, Configs::Radius::qualityDraw->Value);

										if (Configs::Radius::showAARadius->Value == true)
											ShowAARadius(obj, Configs::Radius::qualityDraw->Value);
									}
								}
							}

						}
					}
				}

				namespace Zoom
				{
					
					void Initialize()
					{
						if (!Configs::Zoom::initializedZoomMenu)
						{
							Configs::Zoom::InitializeZoomMenu();
						}
					}

					void AssignValues()
					{
						zoomAddress = functions::GetZoomAddress();

						shouldInject = Configs::Zoom::status->Value;

						shouldWrite = Configs::Zoom::status3D->Value;

						useKeyboard = Configs::Zoom::statusKeyboard->Value;

						useMouseWheel = Configs::Zoom::statusMouseWheel->Value;

						isLeagueFocused = functions::IsGameFocused();

						zoomSlider = Configs::Zoom::zoomValue->Value / 10.0f;
					}

					void Update()
					{
						AssignValues();

						if (shouldWrite)
						{
							gameTime = functions::GetGameTime();

							const Vector3 pathEnd = globals::localPlayer->GetAiManager()->GetPathEnd();

							if (!globals::localPlayer->GetAiManager()->IsMoving()) //&& Orbwalker::Functions::Values::lastAttackTime + 5.0f < gameTime)
							{
								refresh = true;
								if (refresh)
								{
									pathEnd2DLast = Vector2(pathEnd.x, pathEnd.z);
									refresh = false;
								}
							}

							Vector2 pathEnd2D = pathEnd2DLast;

							const Vector3 worldPlayer = globals::localPlayer->GetPosition();
							const Vector2 worldPlayer2D = Vector2(worldPlayer.x, worldPlayer.z);
							Vector2 diffVec = worldPlayer2D - pathEnd2D;
							diffVec.Normalized();
							diffVec.Multiply(globals::localPlayer->GetMovementSpeed());

							const Vector2 destPos = diffVec;
							const float dX = destPos.x;
							const float dZ = destPos.y;

							targetAngleCamera = atan2f(dZ, dX) * (180 / PI) - 90.0f;

							if (targetAngleCamera != 90.0f)
							{
								angleCamera = targetAngleCamera;
							}
						}
					}
				}

				namespace JungleTracker
				{
					void Initialize()
					{
						if (!Resources::JungleTracker::initializedTextures)
						{
							Resources::JungleTracker::InitializeTextures();
						}

						if (Resources::JungleTracker::initializedTextures && !Configs::JungleTracker::initializedJungleTrackerMenu)
						{
							Configs::JungleTracker::InitializeJungleTrackerMenu();
						}
					}

					void Update()
					{
						if (Resources::JungleTracker::initializedTextures && Configs::JungleTracker::initializedJungleTrackerMenu)
						{
							if (Configs::JungleTracker::status->Value == true)
							{
								DrawJungleTracker();
							}
						}
					}

					void DrawJungleTracker()
					{

						for (auto obj : TargetSelector::Functions::GetJungleRespawnInRange(10000))
						{
							Vector3 objDrawPos = functions::GetBaseDrawPosition(obj);
							Vector2 objMinimapPos = functions::WorldToMinimap(obj);
							ImVec2 p = objMinimapPos.ToImVec();
							if (Configs::JungleTracker::showIcons->Value == true)
							{
								DrawIcons(objDrawPos, p, 30);
							}

							if (Configs::JungleTracker::showTimer->Value == true)
							{
								DrawTimers(obj, p);
							}
						}

					}

					void DrawIcons(Vector3 objDrawPos, ImVec2 pos, float size)
					{
						ImVec2 jungleIconMin = ImVec2(pos.x, pos.y);
						ImVec2 jungleIconMax = ImVec2(pos.x + size, pos.y + size);

						Vector3 bluePos1(11131.728516, 151.723694, 6990.844238);
						Vector3 bluePos2(3821.488525, 151.128738, 8101.054199);
						if (render::IsVectorEqual(objDrawPos, bluePos1) || render::IsVectorEqual(objDrawPos, bluePos2))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::blueTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 redPos1(7066.869141, 156.186646, 10975.546875);
						Vector3 redPos2(7762.243652, 153.967743, 4011.186768);
						if (render::IsVectorEqual(objDrawPos, redPos1) || render::IsVectorEqual(objDrawPos, redPos2))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::redTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 grompPos1(12703.628906, 151.690781, 6443.983887);
						Vector3 grompPos2(2288.018555, 151.777313, 8448.133789);
						if (render::IsVectorEqual(objDrawPos, grompPos1) || render::IsVectorEqual(objDrawPos, grompPos2))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::grompTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 wolvesPos1(11059.769531, 160.352585, 8419.830078);
						Vector3 wolvesPos2(3783.379883, 152.462723, 6495.560059);
						if (render::IsVectorEqual(objDrawPos, wolvesPos1) || render::IsVectorEqual(objDrawPos, wolvesPos2))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::murkwolfTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 razorbeaksPos1(7820.220215, 152.192017, 9644.450195);
						Vector3 razorbeaksPos2(7061.500000, 150.123642, 5325.509766);
						if (render::IsVectorEqual(objDrawPos, razorbeaksPos1) || render::IsVectorEqual(objDrawPos, razorbeaksPos2))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::razorbeakTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 golemsPos1(6499.490234, 156.476807, 12287.379883);
						Vector3 golemsPos2(8394.769531, 150.731064, 2641.590088);
						if (render::IsVectorEqual(objDrawPos, golemsPos1) || render::IsVectorEqual(objDrawPos, golemsPos2))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::krugTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 drakePos(9866.148438, 28.759399, 4414.014160);
						if (render::IsVectorEqual(objDrawPos, drakePos))
							render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::dragonTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);

						Vector3 heraldPos(5007.123535, 28.759399, 10471.446289);
						if (render::IsVectorEqual(objDrawPos, heraldPos))
						{
							if (functions::GetGameTime() >= 900)
								render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::baronTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);
							else
								render::RenderImage(Resources::JungleTracker::DX11::JungleIcons::riftheraldTextureIcon, jungleIconMin, jungleIconMax, COLOR_WHITE);
						}
						render::RenderImage(Resources::JungleTracker::DX11::HudDesign::hudJunglePortrait, pos, ImVec2(pos.x + 30, pos.y + 30), COLOR_WHITE);
					}

					void DrawTimers(Object* obj, ImVec2 pos)
					{
						float currentGameTime = functions::GetGameTime();

						if (obj->GetBuffByName("camprespawncountdownhidden"))
						{
							float respawnAtGameTime = obj->GetBuffByName("camprespawncountdownhidden")->GetEndTime() + 60;
							float timeToShow = respawnAtGameTime - currentGameTime;

							if (timeToShow > 0.0f)
							{
								render::RenderCircleFilled(ImVec2(pos.x + 15.0f, pos.y + 15.0f), 15, COLOR_DARK_TRANSPARENT, 0);
								render::RenderText(functions::ConvertTime(timeToShow), ImVec2(pos.x - 7.0f, pos.y - 7.0f), 14, COLOR_RED, false);
							}

						}
						else if (obj->GetBuffByName("camprespawncountdownvisible"))
						{
							float respawnAtGameTime = obj->GetBuffByName("camprespawncountdownvisible")->GetEndTime();
							float timeToShow = respawnAtGameTime - currentGameTime;

							if (timeToShow > 0.0f)
							{
								render::RenderCircleFilled(ImVec2(pos.x + 15.0f, pos.y + 15.0f), 15, COLOR_DARK_TRANSPARENT, 0);
								render::RenderText(functions::ConvertTime(timeToShow), ImVec2(pos.x - 7.0f, pos.y - 7.0f), 14, COLOR_RED, false);
							}
						}
					}
				}
			}
		}
	}
}
