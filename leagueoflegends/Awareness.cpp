#include "Awareness.h"

#include <unordered_set>

#include "Orbwalker.h"

#include "LoadImages.h"
#include "TargetSelector.h"
#include "zoom.h"
#include "../stdafx.h"
#include "Geometry.h"
#include "imgui_notify.h"
#include "ListManager.h"

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
							if (globals::renderType == "D3D11")
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

							static bool JungleImagesAssigned = false;
							void AssignJungleIconsToMap()
							{
								if (!JungleImagesAssigned)
								{
									jungleIcons["Blue"] = Resources::JungleTracker::DX11::JungleIcons::blueTextureIcon;
									jungleIcons["Red"] = Resources::JungleTracker::DX11::JungleIcons::redTextureIcon;
									jungleIcons["Gromp"] = Resources::JungleTracker::DX11::JungleIcons::grompTextureIcon;
									jungleIcons["Wolves"] = Resources::JungleTracker::DX11::JungleIcons::murkwolfTextureIcon;
									jungleIcons["RazorBeaks"] = Resources::JungleTracker::DX11::JungleIcons::razorbeakTextureIcon;
									jungleIcons["Golems"] = Resources::JungleTracker::DX11::JungleIcons::krugTextureIcon;
									jungleIcons["Drake"] = Resources::JungleTracker::DX11::JungleIcons::dragonTextureIcon;
									jungleIcons["RiftHerald"] = Resources::JungleTracker::DX11::JungleIcons::riftheraldTextureIcon;
									jungleIcons["Baron"] = Resources::JungleTracker::DX11::JungleIcons::baronTextureIcon;
									JungleImagesAssigned = true;
								}
							}
						}

						namespace WardIcons
						{
							static bool WardImagesLoaded = false;
							void InitializeWardImages()
							{
								if (!WardImagesLoaded)
								{
									const std::string jungleFolder = "C:\\UPorn\\Media\\Troys";

									static bool blueWardIconLoaded = false;
									const std::string blueWardIconFile = jungleFolder + "\\BlueWard.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(blueWardIconFile.c_str(), blueWardIconLoaded, &blueWardTextureIcon);

									static bool controlWardIconLoaded = false;
									const std::string controlWardIconFile = jungleFolder + "\\ControlWard.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(controlWardIconFile.c_str(), controlWardIconLoaded, &controlWardTextureIcon);

									static bool yellowWardIconLoaded = false;
									const std::string yellowWardIconFile = jungleFolder + "\\YellowWard.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(yellowWardIconFile.c_str(), yellowWardIconLoaded, &yellowWardTextureIcon);

									static bool shacoBoxIconLoaded = false;
									const std::string shacoBoxIconFile = jungleFolder + "\\ShacoBox.png";
									Functions::EnemySidebar::LoadDX11ImageIfNeeded(shacoBoxIconFile.c_str(), shacoBoxIconLoaded, &shacoBoxTextureIcon);

									WardImagesLoaded = true;
								}
							}

							static bool WardImagesAssigned = false;
							void AssignWardIconsToMap()
							{
								if (!WardImagesAssigned) 
								{
									wardIcons["BlueTrinket"] = Resources::JungleTracker::DX11::WardIcons::blueWardTextureIcon;
									wardIcons["JammerDevice"] = Resources::JungleTracker::DX11::WardIcons::controlWardTextureIcon;
									wardIcons["YellowTrinket"] = Resources::JungleTracker::DX11::WardIcons::yellowWardTextureIcon;
									wardIcons["SightWard"] = Resources::JungleTracker::DX11::WardIcons::blueWardTextureIcon;
									wardIcons["ShacoBox"] = Resources::JungleTracker::DX11::WardIcons::shacoBoxTextureIcon;
									WardImagesAssigned = true;
								}
							}
						}

					}

					void InitializeTextures()
					{
						__try
						{
							if (globals::renderType == "D3D11")
							{
								__try { DX11::HudDesign::InitializeHudImages(); }
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing hud images"); }

								__try
								{
									DX11::JungleIcons::InitializeJungleImages();
									DX11::JungleIcons::AssignJungleIconsToMap();
								}
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing jungle images"); }

								__try
								{
									DX11::WardIcons::InitializeWardImages();
									DX11::WardIcons::AssignWardIconsToMap();
								}
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing ward images"); }
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
					AwarenessMenu = Menu::CreateMenu("Awareness", "Awareness");
					initializedAwarenessMenu = true;
				}

				Menu* TrackerMenu;
				namespace EnemyTracker
				{
					void InitializeTrackerMenu()
					{
						TrackerMenu = AwarenessMenu->AddMenu("TrackerDrawings", "Tracker Drawings");
						status = TrackerMenu->AddCheckBox("status", "Enable the Tracker", true);

						const auto MinionDrawings = TrackerMenu->AddMenu("MinionDrawings", "Minions Tracker");
						showDamagePrediction = MinionDrawings->AddCheckBox("showDamagePrediction", "Show damage prediction", true);
						showKillableStatus = MinionDrawings->AddCheckBox("showKillableStatus", "Show killable status", true);

						const auto ExperienceDrawingsMenu = TrackerMenu->AddMenu("ExperienceDrawingsMenu", "Experience Tracker");
						showExperience = ExperienceDrawingsMenu->AddCheckBox("showExperience", "Show experience", true);
						showExperienceSelf = ExperienceDrawingsMenu->AddCheckBox("showExperienceSelf", "Show your experience", false);
						showExperienceAllies = ExperienceDrawingsMenu->AddCheckBox("showExperienceAllies", "Show experience for allies", false);
						showExperienceEnemies = ExperienceDrawingsMenu->AddCheckBox("showExperienceEnemies", "Show experience for enemies", true);

						const auto PathDrawingsMenu = TrackerMenu->AddMenu("PathDrawingsMenu", "Path Tracker");
						showPaths = PathDrawingsMenu->AddCheckBox("showPaths", "Show paths", true);
						showPathsSelf = PathDrawingsMenu->AddCheckBox("showExperienceSelf", "Show your path", false);
						showPathsAllies = PathDrawingsMenu->AddCheckBox("showExperienceAllies", "Show path for allies", false);
						showPathsEnemies = PathDrawingsMenu->AddCheckBox("showExperienceEnemies", "Show path for enemies", true);

						const auto LastPosMenu = TrackerMenu->AddMenu("LastPosMenu", "Last Position Tracker");
						showLastPosition = LastPosMenu->AddCheckBox("showLastPosition", "Show last position", true);
						showPosGuesser = LastPosMenu->AddCheckBox("showPosGuesser", "Show position guesser circle", true);
						showLastPositionMiniMap = LastPosMenu->AddCheckBox("showLastPositionMiniMap", "Show last position on minimap", true);

						const auto WardPosMenu = TrackerMenu->AddMenu("WardMenu", "Ward Position Tracker");
						statusWardTracker = WardPosMenu->AddCheckBox("statusWardTracker", "Enable the ward tracker", true);
						showWardPosition = WardPosMenu->AddCheckBox("showWardPosition", "Show ward position", true);
						showWardTimer = WardPosMenu->AddCheckBox("showWardTimer", "Show ward timer", true);
						showWardRange = WardPosMenu->AddCheckBox("showWardRange", "Show ward range", true);

						initializedTrackerMenu = true;
					}
				}

				namespace EnemySidebar
				{

					void InitializeSidebarMenu()
					{
						const auto EnemySidebarMenu = TrackerMenu->AddMenu("enemySidebar", "Enemies Sidebar");
						status = EnemySidebarMenu->AddCheckBox("status", "Enable the sidebar", true);
						locked = EnemySidebarMenu->AddCheckBox("locked", "Block sidebar movements", false);
						locked->AddTooltip("Enable it if you want to avoid\nMoving the sidebar dragging it");
						orientation = EnemySidebarMenu->AddList("orientationMode", "Draw Mode", std::vector<std::string>{"Vertical", "Horizontal"}, 0);
						hudSize = EnemySidebarMenu->AddSlider("hudSize", "Sidebar Multiplier", 50.0f, 10.0f, 100.0f, 10.0f);
						initializedSidebarMenu = true;
					}
				}

				namespace Radius
				{
					void InitializeRadiusMenu()
					{
						const auto RadiusMenu = AwarenessMenu->AddMenu("utilityRadius", "Drawings Settings");
						status = RadiusMenu->AddCheckBox("status", "Enable radius drawings", true);
						drawMode = RadiusMenu->AddList("drawMode", "Draw Mode", std::vector<std::string>{"Circle", "Arc"}, 0);


						const auto DrawingsToShowMenu = RadiusMenu->AddMenu("DrawingsToShowMenu", "Drawings To Show");

						const auto MissilesMenu = RadiusMenu->AddMenu("MissilesMenu", "Missiles Drawings");
						showMissiles = MissilesMenu->AddCheckBox("showMissiles", "Enable missiles drawings", true);
						showMissilesAnimation = MissilesMenu->AddCheckBox("showMissilesAnimation", "Enable missiles drawings animation", false);

						showBoundingRadius = DrawingsToShowMenu->AddCheckBox("showBoundingRadius", "Enable bounding radius", true);
						showAARadius = DrawingsToShowMenu->AddCheckBox("showAARadius", "Enable autoattack radius", true);

						const auto EnableDrawingsForMenu = RadiusMenu->AddMenu("EnableDrawingsForMenu", "Whitelist");
						showAARadiusTurrets = EnableDrawingsForMenu->AddCheckBox("showAARadiusTurrets", "Show your radius", true);
						showAARadiusSelf = EnableDrawingsForMenu->AddCheckBox("showAARadiusSelf", "Show your radius", true);
						showAARadiusAllies = EnableDrawingsForMenu->AddCheckBox("showAARadiusAllies", "Show radius for allies", false);
						showAARadiusEnemies = EnableDrawingsForMenu->AddCheckBox("showAARadiusEnemies", "Show radius for enemies", false);

						const auto additionalMenu = RadiusMenu->AddMenu("Additional Settings", "Additional Settings");
						showHeight = additionalMenu->AddCheckBox("showHeight", "Consider terrain height", false);
						heightTollerance = additionalMenu->AddSlider("heightTollerance", "Terrain height tollerance", 20, 10, 60, 10);

						qualityDraw = additionalMenu->AddSlider("qualityDraw", "Drawings quality", 50, 10, 100, 10);
						initializedRadiusMenu = true;
					}
				}

				namespace Zoom
				{
					void InitializeZoomMenu()
					{
						const auto ZoomMenu = AwarenessMenu->AddMenu("zoomUtility", "Camera settings");
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
					void InitializeJungleTrackerMenu()
					{
						const auto JungleTrackerMenu = AwarenessMenu->AddMenu("jungleUtility", "Jungle tracker settings");
						status = JungleTrackerMenu->AddCheckBox("status", "Enable jungle tracker tool", true);
						showTimer = JungleTrackerMenu->AddCheckBox("showTimer", "Show timers", true);
						showIcons = JungleTrackerMenu->AddCheckBox("showIcons", "Show icons", true);
						showNotifications = JungleTrackerMenu->AddCheckBox("showNotifications", "Show notifications", true);

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
						EnemyTracker::Initialize();
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
					EnemyTracker::Update();
					EnemySidebar::Update();
					Radius::Update();
					Zoom::Update();
					JungleTracker::Update();
				}

				namespace EnemyTracker
				{
					void Initialize()
					{
						if (!Configs::EnemyTracker::initializedTrackerMenu)
						{
							Configs::EnemyTracker::InitializeTrackerMenu();
						}
					}

					void Update()
					{
						if (Configs::EnemyTracker::initializedTrackerMenu)
						{
							
							if (Configs::EnemyTracker::showDamagePrediction->Value == true)
								DrawLastHitDamage();

							if (Configs::EnemyTracker::status->Value == true)
								DrawTracker();

							if (Configs::EnemyTracker::statusWardTracker->Value == true)
								DrawWardsTracker();
						}
					}

					
					float calcolaProgresso(float expAttuale, int livelloCorrente)
					{
						float progresso = (expAttuale - levelExp[livelloCorrente - 1]) /
							(expNeeded[livelloCorrente]) * 100;

						return livelloCorrente == 18 ? 100.0f : progresso;
					}


					void DrawTracker()
					{
						for (int i = 0; i < globals::heroManager->GetListSize(); i++)
						{
							auto obj = globals::heroManager->GetIndex(i);
							if (obj->GetName() == "PracticeTool_TargetDummy") continue;

							if (obj != nullptr)
							{
								if (!obj->IsAlive()) continue;

								if (obj->IsVisible())
								{
									if (Configs::EnemyTracker::status->Value == true)
									{
										if (Configs::EnemyTracker::showExperienceSelf->Value == false && obj->GetNetId() == globals::localPlayer->GetNetId()) continue;
										if (Configs::EnemyTracker::showExperienceAllies->Value == false && obj->IsAlly() && obj->GetNetId() != globals::localPlayer->GetNetId()) continue;
										if (Configs::EnemyTracker::showExperienceEnemies->Value == false && obj->IsEnemy()) continue;

										DrawCooldownBar(obj);
									}

									if (Configs::EnemyTracker::showPaths->Value == true)
									{
										if (Configs::EnemyTracker::showPathsSelf->Value == false && obj->GetNetId() == globals::localPlayer->GetNetId()) continue;
										if (Configs::EnemyTracker::showPathsAllies->Value == false && obj->IsAlly() && obj->GetNetId() != globals::localPlayer->GetNetId()) continue;
										if (Configs::EnemyTracker::showPathsEnemies->Value == false && obj->IsEnemy()) continue;

										DrawPlayerPaths(obj);
									}
								}
								else if (!obj->IsAlly())
								{
									ShowLastEnemyPosition(obj, i);
								}
								
							}
						}
					}

					void DrawWardsTracker() {
						const auto& wardsList = TargetSelector::Functions::GetWardsInRange(globals::localPlayer->GetPosition(), 1100.0f);

						for (const auto& ward : wardsList) {
							if (!ward) continue;

							auto objDrawPos = functions::GetBaseDrawPosition(ward);
							auto objPos = functions::WorldToScreen(objDrawPos);
							ImVec2 objMapVec = objPos.ToImVec();

							bool showWardRange = Configs::EnemyTracker::showWardRange->Value;
							bool showWardPosition = Configs::EnemyTracker::showWardPosition->Value;
							bool showWardTimer = Configs::EnemyTracker::showWardTimer->Value;

							// Calculate colors and positions once, then use them in the conditionals below
							ImColor wardColor = COLOR_DARK_TRANSPARENT;
							ImVec2 wardIconMin, wardIconMax;
							if (showWardRange || showWardPosition) {
								auto wardName = ward->GetName();

								if (wardName == BLUEWARD) {
									wardColor = COLOR_BLUE;
								}
								else if (wardName == YELLOWWARD || wardName == SIGHTWWARD) {
									wardColor = COLOR_YELLOW;
								}
								else if (wardName == JAMMERDEVICE) {
									wardColor = COLOR_PURPLE;
								}

								wardIconMin = ImVec2(objMapVec.x - 25.0f, objMapVec.y + 10.0f);
								wardIconMax = ImVec2(objMapVec.x + 25.0f, objMapVec.y + 60.0f);
							}

							if (showWardRange) {
								render::RenderWardRange(objDrawPos.ToGround(), wardColor);
							}

							if (showWardPosition) {
								render::RenderImage(Resources::JungleTracker::DX11::WardIcons::wardIcons[ward->GetName()], wardIconMin, wardIconMax, COLOR_WHITE);
								render::RenderCircleFilled(ImVec2(objMapVec.x, objMapVec.y + 35.0f), 25, COLOR_DARK_TRANSPARENT, 0);
							}

							if (showWardTimer && ward->GetName() != JAMMERDEVICE && ward->GetMana() != 0.0f) 
							{
								auto remainingTime = max(0.0f, ward->GetMana());
								char timerBuffer[64];
								snprintf(timerBuffer, sizeof(timerBuffer), "%.1fs", remainingTime);
								render::RenderText(std::string(timerBuffer), ImVec2(objMapVec.x, objMapVec.y + 40.0f), 24, COLOR_WHITE, true);
							}
						}
					}

					void DrawCooldownBar(Object* obj)
					{
						const float BarWidth = 128.0f;
						const float CdWidth = 26.5f;
						const float CdHeight = 6.0f;
						const float YOffset = 3.0f;
						const float XOffset = -BarWidth / 2.0f + 18.0f;

						Vector2 screenPos = functions::GetHpBarPosition(obj);
						Vector2 basePos(screenPos.x + XOffset, screenPos.y - YOffset);

						if (Configs::EnemyTracker::showExperience->Value)
						{
							// Pre-computed constants based on the bar's dimensions
							const float ExpHeight = 30.0f;
							const float ExpYOffset = 1.0f;
							const float ExpXOffset = -BarWidth / 2.0f + 18.0f;
							float expProgress = calcolaProgresso(obj->GetExperience(), obj->GetLevel());

							float filledWidth = 108.0f * (expProgress / 100);

							render::RenderRectFilled(
								ImVec2(screenPos.x + ExpXOffset, screenPos.y - ExpYOffset - ExpHeight),
								ImVec2(screenPos.x + ExpXOffset + filledWidth, screenPos.y - ExpYOffset - ExpHeight + 2),
								COLOR_WHITE, 4.0f, ImDrawFlags_RoundCornersAll
							);
						}

						// Lambda function for drawing a spell cooldown
						auto drawSpellCooldown = [&](int slotId, float xStartOffset, float xEndOffset, float yStartOffset, float yEndOffset) {
							ImVec2 top_left(basePos.x + xStartOffset, basePos.y + yStartOffset);
							ImVec2 bottom_right(basePos.x + xEndOffset, basePos.y + yEndOffset);
							float relativeCooldown = obj->IsEnemy() ? (ListManager::Functions::GetCooldown(obj, slotId) / 10) : obj->GetSpellBySlotId(slotId)->GetRelativeCooldown();
							bottom_right.x -= 0.10 * CdWidth;
							auto color = (relativeCooldown == 0.0f) ? COLOR_GREEN : COLOR_RED;
							render::RenderRectFilled(top_left, bottom_right, color, 4.0f, ImDrawFlags_RoundCornersAll
							);
							};

						// Draw spell cooldown bars for slots 0 to 3
						for (int i = 0; i < 4; ++i) {
							drawSpellCooldown(i, i * (CdWidth + 1.0f) + 1.0f, (i + 1) * (CdWidth + 1.0f), 1.0f, CdHeight);
						}

						// Draw D and F spell cooldown bars
						drawSpellCooldown(4, -1.90f * (CdWidth + 1.0f) + 1.0f, (-0.90f) * (CdWidth + 1.0f), -24.0f, -1);
						drawSpellCooldown(5, 4 * (CdWidth + 1.0f) + 1.0f, (4 + 1) * (CdWidth + 1.0f), -24.0f, -1);
					}

					void DrawPlayerPaths(Object* obj)
					{
						const auto path = obj->GetAiManager()->GetFutureSegments();
						const int countSegments = static_cast<int>(path.size());
						if (countSegments <= 0)
							return;
						
						ImVec2 lastScreenPos = functions::WorldToScreen(obj->GetPosition()).ToImVec();

						for (int i = 0; i < countSegments; i++)
						{
							const ImVec2 currentScreenPos = functions::WorldToScreen(path[i]).ToImVec();

							render::RenderLine(lastScreenPos, currentScreenPos, COLOR_WHITE, 1.0f);

							lastScreenPos = currentScreenPos;
						}
					}

					float time_hidden(Object* hero,
						std::unordered_map<uint64_t, bool>& previous_visibility,
						std::unordered_map<uint64_t, Vector3>& previous_position,
						std::unordered_map<uint64_t, float>& hidden_time)
					{
						const auto netId = hero->GetNetId();
						const bool is_currently_visible = hero->IsValidTarget();

						// Only do this the first time the hero becomes invisible
						if (!is_currently_visible)
						{
							// Mark as not visible
							previous_visibility[netId] = false;
							previous_position[netId] = hero->GetPosition();
						}

						// If hero becomes visible again, reset the visibility status
						else
						{
							previous_visibility[netId] = true;
							hidden_time[netId] = functions::GetGameTime();
						}

						// Return the stored hidden time if the hero is not visible, otherwise return 0
						return !is_currently_visible ? hidden_time[netId] : 0.0f;
					}

					std::unordered_map<uint64_t, bool> previous_visibility;
					std::unordered_map<uint64_t, Vector3> previous_position;
					std::unordered_map<uint64_t, float> hidden_time;
					void ShowLastEnemyPosition(Object* obj, int index)
					{
						// Optimize by calling functions only once if their return value doesn't change.
						const uint64_t objNetId = obj->GetNetId();
						const auto itPos = previous_position.find(objNetId);
						const Vector3& wasPreviousPosition = (itPos != previous_position.end()) ? itPos->second : obj->GetPosition();

						
						const auto& heroIcons = Resources::EnemySidebar::DX11::HeroIcons::textureArray; // Use const reference

						// Minimize calculations by reusing results
						const float minimapSize = functions::GetMinimapSize();
						const float scaleFactor = 35 * ((minimapSize - 192) / 193);
						const float defScale = max(25.46f, scaleFactor);

						// These variables don't change and thus can be moved outside the conditionals
						const Vector2 objMinimapPos = functions::WorldToMinimap(obj->GetPosition());
						const ImVec2 pMiniMap = objMinimapPos.ToImVec();
						const ImVec2 heroIconCenter = ImVec2(pMiniMap.x + (defScale / 2), pMiniMap.y + (defScale / 2));

						if (Configs::EnemyTracker::showLastPosition->Value)
						{
							Vector2 objPos = functions::WorldToScreen(wasPreviousPosition); // Assuming this function doesn't change the state
							ImVec2 pWorldPos = objPos.ToImVec();
							ImVec2 heroWorldIconCenter = ImVec2(pWorldPos.x, pWorldPos.y);

							render::RenderImage(heroIcons[index], ImVec2(pWorldPos.x - 25, pWorldPos.y - 25), ImVec2(pWorldPos.x + 25, pWorldPos.y + 25), COLOR_WHITE);
							render::RenderCircleFilled(heroWorldIconCenter, 25, COLOR_DARK_TRANSPARENT, 0);
						}

						if (Configs::EnemyTracker::showLastPositionMiniMap->Value)
						{
							ImVec2 heroIconMin = ImVec2(pMiniMap.x, pMiniMap.y);
							ImVec2 heroIconMax = ImVec2(pMiniMap.x + defScale, pMiniMap.y + defScale);

							render::RenderImage(heroIcons[index], heroIconMin, heroIconMax, COLOR_WHITE);
							render::RenderCircleFilled(heroIconCenter, defScale / 2, COLOR_DARK_TRANSPARENT, 0);
							render::RenderCircle(heroIconCenter, defScale / 2, COLOR_RED, 1.0f, 0);

						}
					}
									

					void DrawLastHitDamage()
					{
						if (!Configs::EnemyTracker::showDamagePrediction->Value)
							return;

						const auto localPlayer = globals::localPlayer; // Assuming 'localPlayer' won't change during the loop.

						constexpr float barWidth = 100.0f;
						constexpr float yOffset = 5.5f;
						constexpr float xOffset = -barWidth / 3.3f;

						for (auto obj : TargetSelector::Functions::GetMinionsInRange(localPlayer->GetPosition(), localPlayer->GetRealAttackRange()))
						{
							const Vector2 screenPos = functions::GetHpBarPosition(obj);
							const float objHealthPercent = obj->GetPercentHealth();
							const float hpBarWidthLimit = screenPos.x + xOffset + (60 * objHealthPercent) / 100;

							const Vector2 outerBorderAngle3(screenPos.x + xOffset, screenPos.y - yOffset);
							const Vector2 outerBorderAngle4(hpBarWidthLimit, screenPos.y - yOffset + 3.5);

							const float damage = Damage::CalculateAutoAttackDamage(localPlayer, obj); // Call this function only once per minion.
							const float minionHealth = obj->GetHealth();
							const bool canKill = damage > minionHealth;

							auto drawColor = canKill ? COLOR_GREEN : COLOR_RED;
							render::RenderRectFilled(outerBorderAngle3.ToImVec(), outerBorderAngle4.ToImVec(), drawColor, 0.0f, 0);

							const float aaNeeded = ceil(minionHealth / damage);
							const float partWidth = (outerBorderAngle4.x - outerBorderAngle3.x) / aaNeeded;

							for (int j = 1; j < static_cast<int>(aaNeeded); ++j) // Using 'j' as 'i' is already used. Also, casting 'aaNeeded' only once.
							{
								float xPosition = outerBorderAngle3.x + j * partWidth;
								Vector2 startPoint = Vector2(xPosition, outerBorderAngle3.y);
								Vector2 endPoint = Vector2(xPosition, outerBorderAngle4.y);
								render::RenderLine(startPoint.ToImVec(), endPoint.ToImVec(), COLOR_BLACK, 1.0f);
							}

							if (Configs::EnemyTracker::showKillableStatus->Value)
							{
								// Draw radius is called only if the option is enabled.
								Radius::DrawRadius(obj->GetPosition(), 60.0f, drawColor, 1.0f);
							}
						}
					}
				}

				namespace EnemySidebar
				{
					ImTextureID GetSummonerSpellTexture(Object* obj, int spellIndex)
					{
						if (obj->HasBarrier(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellBarrier;


						else if (obj->HasFlash(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellFlash;


						else if (obj->HasCleanse(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellCleanse;


						else if (obj->HasExhaust(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellExhaust;

						else if (obj->HasGhost(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellGhost;


						else if (obj->HasHeal(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellHeal;

						else if (obj->HasIgnite(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellIgnite;

						else if (obj->HasSmite(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellSmite;

						else if (obj->HasTeleport(spellIndex))
							return Resources::EnemySidebar::DX11::SummonerSpells::summSpellTeleport;

						return nullptr;
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
						ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize;
						if (Configs::EnemySidebar::locked->Value) window_flags |= ImGuiWindowFlags_NoMove;

						if (ImGui::Begin("Hero awareness", p_open, window_flags))
						{
							const auto heroManager = globals::heroManager;
							const int champListSize = heroManager->GetListSize();
							const float size = Configs::EnemySidebar::hudSize->Value / 100.0f;
							float spacing = 0.0f;
							ImVec2 p2 = ImGui::GetCursorScreenPos();
							const auto& heroIcons = Resources::EnemySidebar::DX11::HeroIcons::textureArray;

							ImGui::BeginChildOld(
								"Test",
								Configs::EnemySidebar::orientation->Value ?
								ImVec2(champListSize * ((200 + spacing) * size), 200 * size) :
								ImVec2(200 * size, champListSize * ((200 + spacing) * size)),
								false, window_flags);

							for (int i = 1; i < champListSize; i++)
							{
								auto obj = heroManager->GetIndex(i);

								if (obj->GetName() == "PracticeTool_TargetDummy" || obj->IsAlly())
									continue;

								ImVec2 p = Configs::EnemySidebar::orientation->Value ? ImVec2(p2.x + (spacing * size), p2.y) : ImVec2(p2.x, p2.y + (spacing * size));
								ImVec2 backgroundMin = ImVec2(p.x + 10 * size, p.y + 10 * size);
								ImVec2 backgroundMax = ImVec2(backgroundMin.x + 180 * size, backgroundMin.y + 200 * size);

								ImVec2 heroIconMin = ImVec2(p.x + 40 * size, p.y + 30 * size);
								ImVec2 heroIconMax = ImVec2(p.x + 160 * size, p.y + 150 * size);
								render::RenderRectFilled(backgroundMin, backgroundMax, COLOR_DARK_TRANSPARENT, 4.f, ImDrawFlags_RoundCornersAll);
								render::RenderImage(heroIcons[i], heroIconMin, heroIconMax, COLOR_WHITE);

								if (!obj->IsAlive())
								{
									ImVec2 circleCenter = ImVec2(p.x + 100 * size, p.y + 90 * size);
									float circleRadius = 60 * size;
									render::RenderCircleFilled(circleCenter, circleRadius, COLOR_DARK_TRANSPARENT, 0);

									float respawnTime = functions::GetRespawnTimer(obj) + 1;
									ImVec2 respawnTextPos = ImVec2(p.x + 100 * size, p.y + 50 * size);
									render::RenderText(functions::ConvertTime(respawnTime), respawnTextPos, 24 * size, COLOR_RED, true);
								}

								const float larghezzaIntera = heroIconMax.x - heroIconMin.x;

								float larghezzaHpBar = obj->IsAlive() ? larghezzaIntera * (obj->GetPercentHealth() / 100) : larghezzaIntera;
								ImVec2 hudHpMin = ImVec2(heroIconMin.x, heroIconMax.y + 5 * size);
								ImVec2 hudHpMax = ImVec2(heroIconMin.x + larghezzaHpBar, hudHpMin.y + 10 * size);
								render::RenderRectFilled(hudHpMin, hudHpMax, obj->IsAlive() ? COLOR_GREEN : COLOR_RED, 4.f, ImDrawFlags_RoundCornersAll);

								if (obj->GetMana() > 0.0f)
								{
									float larghezzaManaBar = larghezzaIntera * (obj->GetPercentMana() / 100);
									ImVec2 hudManaMin = ImVec2(heroIconMin.x, hudHpMax.y + 2 * size);
									ImVec2 hudManaMax = ImVec2(heroIconMin.x + larghezzaManaBar, hudManaMin.y + 10 * size);
									render::RenderRectFilled(hudManaMin, hudManaMax, COLOR_BLUE, 4.f, ImDrawFlags_RoundCornersAll);
								}

								const float progresso = EnemyTracker::calcolaProgresso(obj->GetExperience(), obj->GetLevel());
								if (progresso > 0.0f)
								{
									float larghezzaExpBar = larghezzaIntera * (progresso / 100);
									ImVec2 hudExpMin = ImVec2(heroIconMin.x, hudHpMax.y + 14 * size);
									ImVec2 hudExpMax = ImVec2(heroIconMin.x + larghezzaExpBar, hudExpMin.y + 10 * size);
									render::RenderRectFilled(hudExpMin, hudExpMax, COLOR_WHITE, 4.f, ImDrawFlags_RoundCornersAll);
								}

								float iconSize = 40.0f * size;
								ImVec2 SummSpell1Min = ImVec2(heroIconMin.x, heroIconMax.y - iconSize);
								ImVec2 SummSpell1Center = ImVec2(heroIconMin.x + (iconSize / 2), heroIconMax.y - (iconSize / 2));
								ImVec2 SummSpell1Max = ImVec2(SummSpell1Min.x + iconSize, heroIconMax.y);
								render::RenderImageRounded(GetSummonerSpellTexture(obj, 4), SummSpell1Min, SummSpell1Max, COLOR_WHITE, 200.0f, ImDrawFlags_RoundCornersAll);
								const int cooldown1 = ceil(ListManager::Functions::GetCooldown(obj, 4));
								if (cooldown1 > 0)
								{
									render::RenderCircleFilled(SummSpell1Center, iconSize / 2, COLOR_DARK_TRANSPARENT, 0);
									render::RenderText(std::to_string(cooldown1), ImVec2(SummSpell1Center.x, SummSpell1Center.y - (iconSize / 1.30)), (iconSize / 2), COLOR_WHITE, true);
								}

								ImVec2 SummSpell2Min = ImVec2(heroIconMax.x - iconSize, heroIconMax.y - iconSize);
								ImVec2 SummSpell2Center = ImVec2(heroIconMax.x - (iconSize / 2), heroIconMax.y - (iconSize / 2));
								ImVec2 SummSpell2Max = ImVec2(heroIconMax.x, heroIconMax.y);
								render::RenderImageRounded(GetSummonerSpellTexture(obj, 5), SummSpell2Min, SummSpell2Max, COLOR_WHITE, 200.0f, ImDrawFlags_RoundCornersAll);
								const int cooldown2 = ceil(ListManager::Functions::GetCooldown(obj, 5));
								if (cooldown2 > 0)
								{
									render::RenderCircleFilled(SummSpell2Center, iconSize / 2, COLOR_DARK_TRANSPARENT, 0);
									render::RenderText(std::to_string(cooldown2), ImVec2(SummSpell2Center.x, SummSpell2Center.y - (iconSize / 1.30)), (iconSize / 2), COLOR_WHITE, true);
								}

								ImVec2 SummLevelPosCenter = ImVec2(heroIconMax.x - (iconSize / 2), heroIconMin.y + (iconSize / 2));
								render::RenderCircleFilled(SummLevelPosCenter, iconSize / 2, COLOR_DARK_TRANSPARENT, 0);
								render::RenderText(std::to_string(obj->GetLevel()), ImVec2(SummLevelPosCenter.x, SummLevelPosCenter.y - (iconSize / 1.30)), (iconSize / 2), COLOR_WHITE, true);

								if (obj->GetLevel() >= 6)
								{
									const int cooldownR = ceil(ListManager::Functions::GetCooldown(obj, 3));
									ImVec2 SummUltimatePosCenter = ImVec2(heroIconMin.x + (iconSize / 2), heroIconMin.y + (iconSize / 2));
									render::RenderCircleFilled(SummUltimatePosCenter, iconSize / 2, cooldownR > 0 ? COLOR_RED : COLOR_GREEN, 0);
									render::RenderText(cooldownR > 0 ? std::to_string(cooldownR) : "R", ImVec2(SummUltimatePosCenter.x, SummUltimatePosCenter.y - (iconSize / 1.30)), (iconSize / 2), COLOR_WHITE, true);
								}

								spacing += 200;
							}

							ImGui::EndChild();
						}

						ImGui::End();
					}

					void Initialize()
					{
						if (!Resources::EnemySidebar::initializedTextures)
							Resources::EnemySidebar::InitializeTextures();

						if (Resources::EnemySidebar::initializedTextures && !Configs::EnemySidebar::initializedSidebarMenu)
							Configs::EnemySidebar::InitializeSidebarMenu();
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
					void ShowMissiles()
					{
						for (auto& missileClient : globals::missileManager->missile_map)
						{
							uintptr_t network_id = missileClient.first;
							Missile* missile = missileClient.second;

							if (IsNotZeroPtr(missile) && IsValidPtr(missile) && missile && !missile->GetMissileData()->IsAutoAttack())
							{
								auto missilePos = missile->GetSpellPos().ToGround();
								if (missilePos.Distance(globals::localPlayer->GetPosition()) > 1500.0f)
									return;

								auto startPos = missile->GetSpellStartPos().ToGround();
								auto endPos = missile->GetSpellEndPos().ToGround();
								
								Geometry::Polygon poly = Geometry::Rectangle(startPos, endPos, 70.f).ToPolygon();
								render::RenderPolygon(poly, 0xFFFFFFFF, 1.0f);

								if (Configs::Radius::showMissilesAnimation->Value == true)
								{
									Geometry::Polygon poly2 = Geometry::Rectangle(startPos, missilePos, 70.f).ToPolygon();
									render::RenderFilledPolygon(poly2, 0x40FFFFFF);
								}
								
							}
						}
					}

					void DrawRadius(Vector3 worldPos, float radius, uintptr_t color, float thickness, bool takeHeightInConsideration, bool glow)
					{
						switch (Configs::Radius::drawMode->Value)
						{
						case 0: //Circle
							render::RenderCircleWorld(worldPos, Configs::Radius::qualityDraw->Value, radius, color, thickness, takeHeightInConsideration, glow);
							break;
						case 1: //Arc
							render::RenderArcWorld(worldPos, Configs::Radius::qualityDraw->Value, radius, color, thickness, PI / 3, functions::GetMouseWorldPos(), true);
							break;//34
						}

					}
					
					static void ShowBoundingRadius(Object* obj, int quality)
					{
						if (obj->IsAlive() && obj->IsVisible())
						{
							if (obj->GetNetId() == globals::localPlayer->GetNetId())
							{
								if (Configs::Radius::showAARadiusSelf->Value == true)
								{
									DrawRadius(obj->GetPosition(), obj->GetBoundingRadius(), COLOR_WHITE, 1.0f, false, true);
								}
							}
							else
							{
								if (obj->IsEnemy() && Configs::Radius::showAARadiusEnemies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetBoundingRadius(), COLOR_RED, 1.0f, false, true);
								if (obj->IsAlly() && Configs::Radius::showAARadiusAllies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetBoundingRadius(), COLOR_BLUE, 1.0f, false, true);
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
									DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_WHITE, 1.0f, false, true);
							}
							else
							{
								if (obj->IsEnemy() && Configs::Radius::showAARadiusEnemies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_RED, 1.0f, false, true);
								if (obj->IsAlly() && Configs::Radius::showAARadiusAllies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_BLUE, 1.0f, false, true);
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

					

					void Update()
					{
						if (Configs::Radius::initializedRadiusMenu)
						{
							if (Configs::Radius::status->Value == true)
							{
								if (Configs::Radius::showMissiles->Value == true)
								{
									ShowMissiles();
								}

								for (int i = 0; i < globals::heroManager->GetListSize(); i++)
								{
									auto obj = globals::heroManager->GetIndex(i);
									if (!obj->IsAlive()) continue;
									if (!obj->IsVisible()) continue;
									if (obj->GetDistanceTo(globals::localPlayer) > 2000.0f) continue;

									if (obj)
									{
										if (Configs::Radius::showBoundingRadius->Value == true)
											ShowBoundingRadius(obj, Configs::Radius::qualityDraw->Value);

										if (Configs::Radius::showAARadius->Value == true)
											ShowAARadius(obj, Configs::Radius::qualityDraw->Value);
									}
								}

								for (int i = 0; i < globals::turretManager->GetListSize(); i++)
								{
									auto obj = globals::turretManager->GetIndex(i);
									if (!obj->IsAlive()) continue;
									if (!obj->IsVisible()) continue;
									if (obj->GetDistanceTo(globals::localPlayer) > 2000.0f) continue;

									if (obj)
									{
										if (Configs::Radius::showAARadius->Value == true)
											DrawRadius(obj->GetPosition(), 850.0f, obj->IsAlly() ? COLOR_BLUE : COLOR_RED, 1.0f, false, false);
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

						shouldWrite = Configs::Zoom::status3D->Value ;

						useKeyboard = Configs::Zoom::statusKeyboard->Value;

						useMouseWheel = Configs::Zoom::statusMouseWheel->Value;

						isLeagueFocused = functions::IsGameFocused();

						zoomSlider = Configs::Zoom::zoomValue->Value / 10.0f;
					}

					void Update()
					{
						AssignValues();

						if (Configs::Zoom::status3D->Value && !globals::menuOpen)
						{
							gameTime = functions::GetGameTime();

							const Vector3 pathEnd = globals::localPlayer->GetAiManager()->GetPathEnd();

							if (!globals::localPlayer->GetAiManager()->IsMoving() && Orbwalker::Functions::lastActionTime + 5.0f < gameTime)
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
							Resources::JungleTracker::InitializeTextures();

						if (Resources::JungleTracker::initializedTextures && !Configs::JungleTracker::initializedJungleTrackerMenu)
							Configs::JungleTracker::InitializeJungleTrackerMenu();

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

					bool notificationShown = false;
					std::string notificationMessage;
					ID3D11ShaderResourceView* notificationIcon = nullptr;
					int notificationStartTime = 0;

					void ShowNotification(std::string iconToShow, std::string messageToShow)
					{
						notificationMessage = messageToShow;
						notificationStartTime = functions::GetGameTime();
						notificationIcon = Resources::JungleTracker::DX11::JungleIcons::jungleIcons[iconToShow];  // Ottieni l'icona dalla mappa

						notificationShown = true;
					}

					void RenderNotification()
					{
						if (notificationShown)
						{
							int currentTime = functions::GetGameTime();
							int elapsedTime = currentTime - notificationStartTime;

							if (elapsedTime >= 3)
							{
								notificationShown = false;
							}
							else
							{
								ImVec2 screenSize = ImGui::GetIO().DisplaySize;
								ImVec2 centerPosition = ImVec2(screenSize.x * 0.5f, screenSize.y * 0.15f);

								ImVec2 minPos = ImVec2(centerPosition.x - 300.0f, centerPosition.y - 50.0f);
								ImVec2 maxPos = ImVec2(centerPosition.x + 300.0f, centerPosition.y + 50.0f);

								render::RenderRectFilled(minPos, maxPos, COLOR_DARK_TRANSPARENT, 4.f, ImDrawFlags_RoundCornersAll);

								if (notificationIcon != nullptr)
								{
									ImVec2 iconMinPos = ImVec2(minPos.x + 100.0f, minPos.y + 10.0f);
									ImVec2 iconMaxPos = ImVec2(minPos.x + 190.0f, maxPos.y - 10.0f);
									render::RenderImage(notificationIcon, iconMinPos, iconMaxPos, COLOR_WHITE);
								}

								render::RenderText(notificationMessage, ImVec2(minPos.x + 300, minPos.y + 18.0f), 24, COLOR_WHITE, false);
							}
						}
					}

					void NotifyJungle(Vector3 objDrawPos)
					{
						const struct JungleInfo
						{
							Vector3 position;
							const char* name;
						}

						jungleInfos[] = {
						{ Vector3(11131.728516, 151.723694, 6990.844238), "Blue" }, { Vector3(3821.488525, 151.128738, 8101.054199), "Blue" },
						{ Vector3(7066.869141, 156.186646, 10975.546875), "Red" }, { Vector3(7762.243652, 153.967743, 4011.186768), "Red" },
						{ Vector3(12703.628906, 151.690781, 6443.983887), "Gromp" }, { Vector3(2288.018555, 151.777313, 8448.133789), "Gromp" },
						{ Vector3(11059.769531, 160.352585, 8419.830078), "Wolves" }, { Vector3(3783.379883, 152.462723, 6495.560059), "Wolves" },
						{ Vector3(7820.220215, 152.192017, 9644.450195), "RazorBeaks" }, { Vector3(7061.500000, 150.123642, 5325.509766), "RazorBeaks" },
						{ Vector3(6499.490234, 156.476807, 12287.379883), "Golems" }, { Vector3(8394.769531, 150.731064, 2641.590088), "Golems" },
						{ Vector3(9866.148438, 28.759399, 4414.014160), "Drake" }, { Vector3(5007.123535, 28.759399, 10471.446289), (functions::GetGameTime() >= 1000) ? "Baron" : "Herald" },
						};


						for (const auto& jungleInfo : jungleInfos)
						{
							if (render::IsVectorEqual(objDrawPos, jungleInfo.position))
							{
								if (jungleInfo.name == "Drake" && functions::GetGameTime() < 500 || jungleInfo.name == "Herald" && functions::GetGameTime() < 950)
									return;

								ShowNotification(std::string(jungleInfo.name), std::string(jungleInfo.name) + " Killed");
								break;
							}
						}
					}

					std::unordered_set<Object*> insertedObjects;
					void DrawJungleTracker()
					{
						if (functions::GetGameTime() < 100.0f) return;

						for (auto obj : TargetSelector::Functions::GetJungleRespawnInRange(12000.0f))
						{
							Vector3 objDrawPos = functions::GetBaseDrawPosition(obj);
							Vector2 objMinimapPos = functions::WorldToMinimap(obj);
							ImVec2 p = objMinimapPos.ToImVec();

							const Vector3 ignorePos1(2000.000000, 195.748108, 2000.000000);
							if (render::IsVectorEqual(objDrawPos, ignorePos1)) continue;


							if (Configs::JungleTracker::showIcons->Value == true)
							{
								DrawIcons(objDrawPos, p);
							}

							if (Configs::JungleTracker::showTimer->Value == true)
							{
								DrawTimers(obj, p);
							}

							if (Configs::JungleTracker::showNotifications->Value == true)
							{
								if (!insertedObjects.contains(obj))
								{
									insertedObjects.insert(obj);

									NotifyJungle(objDrawPos);
								}
							}
						}
						RenderNotification();
					}

					

					void DrawIcons(Vector3 objDrawPos, ImVec2 pos)
					{
						const float minimap_size = functions::GetMinimapSize();
						const float scale_factor = 30 * ((minimap_size - 192) / 193);
						const float def_scale = max(20.46, scale_factor);

						const struct JungleIconInfo
						{
							Vector3 position;
							ID3D11ShaderResourceView* icon;
						}

						jungleIconInfos[] = {	{ Vector3(11131.728516, 151.723694, 6990.844238), Resources::JungleTracker::DX11::JungleIcons::blueTextureIcon },
						{ Vector3(3821.488525, 151.128738, 8101.054199), Resources::JungleTracker::DX11::JungleIcons::blueTextureIcon },
						{ Vector3(7066.869141, 156.186646, 10975.546875), Resources::JungleTracker::DX11::JungleIcons::redTextureIcon },
						{ Vector3(7762.243652, 153.967743, 4011.186768), Resources::JungleTracker::DX11::JungleIcons::redTextureIcon },
						{ Vector3(12703.628906, 151.690781, 6443.983887), Resources::JungleTracker::DX11::JungleIcons::grompTextureIcon },
						{ Vector3(2288.018555, 151.777313, 8448.133789), Resources::JungleTracker::DX11::JungleIcons::grompTextureIcon },
						{ Vector3(11059.769531, 160.352585, 8419.830078), Resources::JungleTracker::DX11::JungleIcons::murkwolfTextureIcon },
						{ Vector3(3783.379883, 152.462723, 6495.560059), Resources::JungleTracker::DX11::JungleIcons::murkwolfTextureIcon },
						{ Vector3(7820.220215, 152.192017, 9644.450195), Resources::JungleTracker::DX11::JungleIcons::razorbeakTextureIcon },
						{ Vector3(7061.500000, 150.123642, 5325.509766), Resources::JungleTracker::DX11::JungleIcons::razorbeakTextureIcon },
						{ Vector3(6499.490234, 156.476807, 12287.379883), Resources::JungleTracker::DX11::JungleIcons::krugTextureIcon },
						{ Vector3(8394.769531, 150.731064, 2641.590088), Resources::JungleTracker::DX11::JungleIcons::krugTextureIcon },
						{ Vector3(9866.148438, 28.759399, 4414.014160), Resources::JungleTracker::DX11::JungleIcons::dragonTextureIcon },
						{ Vector3(5007.123535, 28.759399, 10471.446289), (functions::GetGameTime() >= 900) ? Resources::JungleTracker::DX11::JungleIcons::baronTextureIcon : Resources::JungleTracker::DX11::JungleIcons::riftheraldTextureIcon },
						};

						for (const auto& jungleIconInfo : jungleIconInfos)
						{
							if (render::IsVectorEqual(objDrawPos, jungleIconInfo.position))
							{
								render::RenderImage(jungleIconInfo.icon, ImVec2(pos.x, pos.y), ImVec2(pos.x + def_scale, pos.y + def_scale), COLOR_WHITE);
								break; // Esci dal loop dopo il rendering
							}
						}

						const auto jungleIconCenter = ImVec2(pos.x + (def_scale / 2), pos.y + (def_scale / 2));
						render::RenderCircleFilled(jungleIconCenter, def_scale / 2, COLOR_DARK_TRANSPARENT, 0);
					}

					void DrawTimers(Object* obj, ImVec2 pos)
					{
						const float minimap_size = functions::GetMinimapSize();
						const float scale_factor = 30 * ((minimap_size - 192) / 193);
						const float def_scale = max(20.46, scale_factor);
						const auto jungleIconMin = ImVec2(pos.x - (def_scale / 2), pos.y - (def_scale / 2));

						float currentGameTime = functions::GetGameTime();

						if (obj->GetBuffByName("camprespawncountdownhidden"))
						{
							float respawnAtGameTime = obj->GetBuffByName("camprespawncountdownhidden")->GetEndTime() + 60;
							float timeToShow = respawnAtGameTime - currentGameTime;

							if (timeToShow > 0.0f)
							{
								render::RenderText(functions::ConvertTime(timeToShow), jungleIconMin, def_scale/1.50, COLOR_WHITE, false);
							}

						}
						else if (obj->GetBuffByName("camprespawncountdownvisible"))
						{
							float respawnAtGameTime = obj->GetBuffByName("camprespawncountdownvisible")->GetEndTime();
							float timeToShow = respawnAtGameTime - currentGameTime;

							if (timeToShow > 0.0f)
							{
								render::RenderText(functions::ConvertTime(timeToShow), jungleIconMin, def_scale/ 1.50, COLOR_WHITE, false);
							}
						}
					}
				}
			}
		}
	}
}
