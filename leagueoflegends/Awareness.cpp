#include "Awareness.h"
#include "Orbwalker.h"

#include "LoadImages.h"
#include "TargetSelector.h"
#include "zoom.h"
#include "../stdafx.h"
#include "Geometry.h"
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

								__try { DX11::JungleIcons::InitializeJungleImages(); }
								__except (1) { LOG("[AWARENESS - JUNGLE TRACKER] Error in initializing jungle images"); }

								__try { DX11::WardIcons::InitializeWardImages(); }
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

					void DrawWardsTracker()
					{
						const auto wardsList = TargetSelector::Functions::GetWardsInRange(globals::localPlayer->GetPosition(), 1100.0f);

						for (const auto ward : wardsList)
						{
							if (ward == nullptr)
								continue;

							Vector3 objDrawPos = functions::GetBaseDrawPosition(ward);
							Vector2 objPos = functions::WorldToScreen(objDrawPos);
							const ImVec2 objMapVec = objPos.ToImVec();

							if (Configs::EnemyTracker::showWardRange->Value)
							{
								ImColor wardColor = COLOR_DARK_TRANSPARENT;

								if (ward->GetName() == BLUEWARD)
									wardColor = COLOR_BLUE;
								else if (ward->GetName() == YELLOWWARD || ward->GetName() == SIGHTWWARD)
									wardColor = COLOR_YELLOW;
								else if (ward->GetName() == JAMMERDEVICE)
									wardColor = COLOR_PURPLE;

								render::RenderWardRange(objDrawPos, wardColor);
							}

							if (Configs::EnemyTracker::showWardPosition->Value)
							{
								const auto wardIconMin = ImVec2(objMapVec.x - 25.0f, objMapVec.y + 10.0f);
								const auto wardIconMax = ImVec2(objMapVec.x + 25.0f, objMapVec.y + 60.0f);

								if (ward->GetName() == BLUEWARD)
									render::RenderImage(Resources::JungleTracker::DX11::WardIcons::blueWardTextureIcon, wardIconMin, wardIconMax, COLOR_WHITE);
								else if (ward->GetName() == YELLOWWARD || ward->GetName() == SIGHTWWARD)
									render::RenderImage(Resources::JungleTracker::DX11::WardIcons::yellowWardTextureIcon, wardIconMin, wardIconMax, COLOR_WHITE);
								else if (ward->GetName() == JAMMERDEVICE)
									render::RenderImage(Resources::JungleTracker::DX11::WardIcons::controlWardTextureIcon, wardIconMin, wardIconMax, COLOR_WHITE);

								render::RenderCircleFilled(ImVec2(objMapVec.x, objMapVec.y + 35.0f), 25, COLOR_DARK_TRANSPARENT, 0);
							}

							if (Configs::EnemyTracker::showWardTimer->Value && ward->GetName() != JAMMERDEVICE && ward->GetName() != BLUEWARD)
							{
								render::RenderText(functions::ConvertTime(ward->GetMana()), ImVec2(objMapVec.x - 5.0f, objMapVec.y), 24, COLOR_RED, true);
							}
						}
					}

					void DrawCooldownBar(Object* obj)
					{
						const float barWidth = 128.0f;
						const float cdWidth = 26.5f;
						const float cdHeight = 6.0f;
						const float yOffset = 3.0f;
						const float xOffset = -barWidth / 2.0f + 18.0f;

						Vector2 screenPos = functions::GetHpBarPosition(obj);
						Vector2 basePos = Vector2(screenPos.x + xOffset, screenPos.y - yOffset);

						if (Configs::EnemyTracker::showExperience->Value == true)
						{
							const float expMaxWidth = 128.0f;
							const float expHeight = 30.0f;
							const float expYOffset = 1.0f;
							const float expXOffset = -expMaxWidth / 2.0f + 18.0f;

							// Calculate experience progress
							float expAttuale = obj->GetExperience();
							int livelloCorrente = obj->GetLevel();
							float progresso = calcolaProgresso(expAttuale, livelloCorrente);

							// Calculate the width of the filled bar
							float width = 108.0f * (progresso / 100);
							float hpBarWidthLimit = screenPos.x + expXOffset + width;

							Vector2 outerBorderAngle1 = Vector2(screenPos.x + expXOffset, screenPos.y - expYOffset - expHeight);
							Vector2 outerBorderAngle2 = Vector2(hpBarWidthLimit, screenPos.y - expYOffset - expHeight + 2);

							render::RenderRectFilled(outerBorderAngle1.ToImVec(), outerBorderAngle2.ToImVec(), COLOR_WHITE, 0.0f, 0);
						}

						// Function to draw a spell cooldown bar
						auto drawSpellCooldown = [&](int slotId, float xOffset, float xLimit, float yOffset, float yLimit)
						{
							Vector2 spellAngle1 = Vector2(basePos.x + xOffset, basePos.y + yOffset);
							Vector2 spellAngle2 = Vector2(basePos.x + xLimit, basePos.y + yLimit);

							Spell* spell = obj->GetSpellBySlotId(slotId);
							float relativeCooldown = spell->GetRelativeCooldown();
							spellAngle2.x -= relativeCooldown * cdWidth;

							uint32_t color = (relativeCooldown == 0.0f) ? COLOR_LIGHT_GREEN : COLOR_ORANGE;
							render::RenderRectFilled(spellAngle1.ToImVec(), spellAngle2.ToImVec(), color, 0.0f, 0);
							};

						// Draw spell cooldown bars for slots 0 to 3
						for (int i = 0; i < 4; i++) {
							drawSpellCooldown(i,
								i * (cdWidth + 1.0f) + 1.0f, 
								(i + 1) * (cdWidth + 1.0f),
								1.0f, 
								cdHeight);
						}

						// Draw D spell cooldown bar
						drawSpellCooldown(4, 
							4 * (cdWidth + 1.0f) + 1.0f,
							(4 + 1) * (cdWidth + 1.0f),
							-30.0f,
							-13.0f);

						// Draw F spell cooldown bar
						drawSpellCooldown(5,
							4 * (cdWidth + 1.0f) + 1.0f,
							(4 + 1) * (cdWidth + 1.0f),
							-11.0f,
							cdHeight);
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

					void drawLastPosCircle(Object* obj, Vector3 pos1, Vector3 pos2, float distanza, float gametime)
					{
						if (!Configs::EnemyTracker::showPosGuesser->Value)
							return;

						const float castTime = functions::GetGameTime() - gametime;
						const float velocita = obj->GetMovementSpeed();
						const float spostamento = velocita * castTime;
						const Vector3 objPosAfter = pos1.Extend(pos2, spostamento);
						const float rangeSpostamento = min(pos1.Distance(objPosAfter), distanza);
						render::RenderCircleWorld(pos1, Configs::Radius::qualityDraw->Value, rangeSpostamento, COLOR_DARK_TRANSPARENT, 1.0f);
					}

					constexpr float hiding_threshold_distance = 1.0f;
					float time_hidden(Object* hero,
						std::unordered_map<uint64_t, bool>& previous_visibility,
						std::unordered_map<uint64_t, Vector3>& previous_position,
						std::unordered_map<uint64_t, Vector3>& previous_endpath,
						std::unordered_map<uint64_t, float>& hidden_time)
					{
						// If hero is not visible
						if (!hero->IsVisible())
						{
							const auto netId = hero->GetNetId();

							// Check if hero was previously visible or if their distance from their previous position is bigger than 1 distance
							const bool was_previously_visible = previous_visibility[netId];
							const auto it = previous_position.find(netId);
							const auto was_previous_position = it != previous_position.end() ? it->second : hero->GetPosition();
							const auto was_previous_endpath = previous_endpath.find(netId);
							const auto was_previous_endpath_position = was_previous_endpath != previous_endpath.end() ? was_previous_endpath->second : it->second;

							if (was_previously_visible || was_previous_position.Distance(hero->GetPosition()) > hiding_threshold_distance)
							{
								// If hero was previously visible or if they moved further away than 1 from their previous position, update the time they have been hidden
								hidden_time[netId] = functions::GetGameTime();

								// Draw a circle to show the last hidden position
								const auto distanzaSpostamento = was_previous_position.Distance(was_previous_endpath_position);
								drawLastPosCircle(hero, was_previous_position, was_previous_endpath_position, distanzaSpostamento, hidden_time[netId]);
							}

							// Return the time they have been hidden
							return hidden_time[netId];
						}
						else {
							// If hero is visible, set time hidden to 0
							return 0;
						}
					}

					std::unordered_map<uint64_t, bool> previous_visibility;
					std::unordered_map<uint64_t, Vector3> previous_position;
					std::unordered_map<uint64_t, Vector3> previous_endpath;
					std::unordered_map<uint64_t, float> hidden_time;
					void ShowLastEnemyPosition(Object* obj, int index)
					{
						const auto objNetId = obj->GetNetId();
						const auto itPos = previous_position.find(objNetId);
						const auto wasPreviousPosition = (itPos != previous_position.end()) ? itPos->second : obj->GetPosition();

						float timeHidden = time_hidden(obj, previous_visibility, previous_position, previous_endpath, hidden_time);
						auto time = functions::ConvertTime(functions::GetGameTime() - timeHidden);

						const auto heroIcons = Resources::EnemySidebar::DX11::HeroIcons::textureArray;
						const float minimapSize = functions::GetMinimapSize();
						const float scaleFactor = 35 * ((minimapSize - 192) / 193);
						const float defScale = max(25.46f, scaleFactor);

						const auto objMinimapPos = functions::WorldToMinimap(obj);
						const auto pMiniMap = objMinimapPos.ToImVec();
						const auto heroIconCenter = ImVec2(pMiniMap.x + (defScale / 2), pMiniMap.y + (defScale / 2));

						if (Configs::EnemyTracker::showLastPosition->Value)
						{
							Vector2 objPos = functions::WorldToScreen(wasPreviousPosition);
							ImVec2 pWorldPos = objPos.ToImVec();
							ImVec2 heroWorldIconCenter = ImVec2(pWorldPos.x, pWorldPos.y);

							render::RenderImage(heroIcons[index], ImVec2(pWorldPos.x - 25, pWorldPos.y - 25), ImVec2(pWorldPos.x + 25, pWorldPos.y + 25), COLOR_WHITE);
							render::RenderCircleFilled(heroWorldIconCenter, 25, COLOR_DARK_TRANSPARENT, 0);
							render::RenderText(time, ImVec2(heroWorldIconCenter.x - 6, heroWorldIconCenter.y), 25, COLOR_RED, true);
						}

						if (Configs::EnemyTracker::showLastPositionMiniMap->Value)
						{
							const auto heroIconMin = ImVec2(pMiniMap.x, pMiniMap.y);
							const auto heroIconMax = ImVec2(pMiniMap.x + defScale, pMiniMap.y + defScale);

							render::RenderImage(heroIcons[index], heroIconMin, heroIconMax, COLOR_WHITE);
							render::RenderCircleFilled(heroIconCenter, defScale / 2, COLOR_DARK_TRANSPARENT, 0);
							render::RenderCircle(heroIconCenter, defScale / 2, COLOR_RED, 1.0f, 0);
						}
					}
									

					void DrawLastHitDamage()
					{
						if (!Configs::EnemyTracker::showDamagePrediction->Value)
							return;

						const auto localPlayer = globals::localPlayer;

						for (int i = 0; i < globals::minionManager->GetListSize(); i++)
						{
							auto obj = globals::minionManager->GetIndex(i);
							if (!obj || !obj->IsValidTarget() || obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::Minion_Lane)
								continue;

							constexpr float barWidth = 100.0f;
							constexpr float yOffset = 5.5f;
							constexpr float xOffset = -barWidth / 3.3f;

							const Vector2 screenPos = functions::GetHpBarPosition(obj);
							const float objHealthPercent = obj->GetPercentHealth();
							const float hpBarWidthLimit = screenPos.x + xOffset + ((60 * objHealthPercent) / 100);

							const Vector2 outerBorderAngle3 = Vector2(screenPos.x + xOffset, screenPos.y - yOffset);
							const Vector2 outerBorderAngle4 = Vector2(hpBarWidthLimit, screenPos.y - yOffset + 3.5);

							const float damage = Damage::CalculateAutoAttackDamage(localPlayer, obj);
							const bool canKill = damage > obj->GetHealth();

							render::RenderRectFilled(outerBorderAngle3.ToImVec(), outerBorderAngle4.ToImVec(), canKill ? COLOR_GREEN : COLOR_RED, 0.0f, 0);

							const float aaNeeded = ceil(obj->GetHealth() / damage);
							const float partWidth = (outerBorderAngle4.x - outerBorderAngle3.x) / aaNeeded;

							for (int i2 = 1; i2 < aaNeeded; i2++)
							{
								const float xPosition = outerBorderAngle3.x + i2 * partWidth;
								Vector2 startPoint = Vector2(xPosition, outerBorderAngle3.y);
								Vector2 endPoint = Vector2(xPosition, outerBorderAngle4.y);
								render::RenderLine(startPoint.ToImVec(), endPoint.ToImVec(), COLOR_BLACK, 1.0f);
							}

							if (Configs::EnemyTracker::showKillableStatus->Value)
								Radius::DrawRadius(obj->GetPosition(), 60.0f, canKill ? COLOR_GREEN : COLOR_RED, 1.0f);
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
								ImVec2 heroIconMin = ImVec2(p.x + 40 * size, p.y + 30 * size);
								ImVec2 heroIconMax = ImVec2(p.x + 170 * size, p.y + 150 * size);

								render::RenderImage(heroIcons[i], heroIconMin, heroIconMax, COLOR_WHITE);

								if (!obj->IsAlive())
								{
									ImVec2 circleCenter = ImVec2(p.x + 105 * size, p.y + 85 * size);
									float circleRadius = 58 * size;
									render::RenderCircleFilled(circleCenter, circleRadius, COLOR_DARK_TRANSPARENT, 0);
									float respawnTime = functions::GetRespawnTimer(obj) + 1;
									ImVec2 respawnTextPos = ImVec2(p.x + 100 * size, p.y + 50 * size);
									render::RenderText(functions::ConvertTime(respawnTime), respawnTextPos, 24 * size, COLOR_RED, true);
								}

								float altezzaIntera = 140.0f * size;
								float progresso = EnemyTracker::calcolaProgresso(obj->GetExperience(), obj->GetLevel());
								float altezzaDaTogliere = 100 - progresso;
								float altezzaTotale = altezzaIntera * (altezzaDaTogliere / 100);

								ImVec2 hudExpMin = ImVec2(p.x + 130 * size, p.y + altezzaTotale + 14 * size);
								ImVec2 hudExpMax = ImVec2(p.x + 195 * size, p.y + altezzaIntera);
								const auto& hudHpBar = Resources::EnemySidebar::DX11::HudDesign::hudHpBar;
								ImGui::GetWindowDrawList()->AddImage(hudHpBar, hudExpMin, hudExpMax, { 0.0f, altezzaDaTogliere / 100 }, { 1.0f, 1.0f }, COLOR_WHITE);

								float altezzaDaTogliere2 = 100 - obj->GetPercentMana();
								float altezzaTotale2 = altezzaIntera * (altezzaDaTogliere2 / 100);
								ImVec2 hudManaMin = ImVec2(p.x + 20 * size, p.y + altezzaTotale2 + 14 * size);
								ImVec2 hudManaMax = ImVec2(p.x + 85 * size, p.y + altezzaIntera);
								ImGui::GetWindowDrawList()->AddImage(hudHpBar, hudManaMin, hudManaMax, { 1.0f,  altezzaDaTogliere2 / 100 }, { 0.0f, 1.0f }, COLOR_RED);

								float altezzaDaTogliere3 = 100 - obj->GetPercentHealth();
								float altezzaTotale3 = altezzaIntera * (altezzaDaTogliere3 / 100);
								ImVec2 hudHpMin = ImVec2(p.x + 5 * size, p.y + altezzaTotale3 + 5 * size);
								ImVec2 hudHpMax = ImVec2(p.x + 60 * size, p.y + altezzaIntera);
								ImGui::GetWindowDrawList()->AddImage(hudHpBar, hudHpMin, hudHpMax, { 1.0f,  altezzaDaTogliere3 / 100 }, { 0.0f, 1.0f }, COLOR_GREEN);

								ImVec2 SummSpell1Min = ImVec2(p.x + 26 * size, p.y + 111 * size);
								ImVec2 SummSpell1Max = ImVec2(p.x + 65 * size, p.y + 148 * size);
								render::RenderImageRounded(GetSummonerSpellTexture(obj, 4), SummSpell1Min, SummSpell1Max, COLOR_WHITE, 200.0f, ImDrawFlags_RoundCornersAll);

								ImVec2 SummSpell2Min = ImVec2(p.x + 68 * size, p.y + 131 * size);
								ImVec2 SummSpell2Max = ImVec2(p.x + 107 * size, p.y + 168 * size);
								render::RenderImageRounded(GetSummonerSpellTexture(obj, 5), SummSpell2Min, SummSpell2Max, COLOR_WHITE, 200.0f, ImDrawFlags_RoundCornersAll);

								ImVec2 hudChampPortraitPosition = ImVec2(p.x + 200 * size, p.y + 172 * size);
								const auto& hudChampPortrait = Resources::EnemySidebar::DX11::HudDesign::hudChampPortrait;
								render::RenderImage(hudChampPortrait, p, hudChampPortraitPosition, COLOR_WHITE);

								ImVec2 SummLevelPos = ImVec2(p.x + 146 * size, p.y + 114 * size);
								render::RenderText(std::to_string(obj->GetLevel()), SummLevelPos, 20 * size, COLOR_WHITE, true);

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
								auto startPos = missile->GetSpellStartPos().ToGround();
								auto endPos = missile->GetSpellEndPos().ToGround();
								auto missilePos = missile->GetSpellPos().ToGround();
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

					void DrawJungleTracker()
					{

						for (auto obj : TargetSelector::Functions::GetJungleRespawnInRange(10000))
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
						}

					}

					void DrawIcons(Vector3 objDrawPos, ImVec2 pos)
					{
						const float minimap_size = functions::GetMinimapSize();
						const float scale_factor = 30 * ((minimap_size - 192) / 193);
						const float def_scale = max(20.46, scale_factor);

						const auto jungleIconMin = ImVec2(pos.x, pos.y);
						const auto jungleIconMax = ImVec2(pos.x + def_scale, pos.y + def_scale);

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
