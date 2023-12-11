#include "Awareness.h"

#include <unordered_set>

#include "Cooldowns.h"
#include "Awarenesss.h"
#include "Experience.h"
#include "Orbwalker.h"

#include "TargetSelector.h"
#include "zoom.h"
#include "../stdafx.h"
#include "Geometry.h"
#include "imgui_notify.h"
#include "ListManager.h"
#include "Icons.h"
#include "LastHitDamage.h"
#include "LastPosition.h"
#include "Path.h"
#include "Sidebar.h"

namespace UPasta
{
	namespace SDK
	{
		namespace Awareness
		{


			namespace ChampionSpells
			{
				void test()
				{
					for (const auto& pair : Icons::Map) // Usa una referenza const per evitare copie inutili
					{
						const auto& file = pair.second; // Accedi direttamente al valore, senza usare find()

						try {
							auto texture = render::CreateTexture(file.icon, file.size, false);
							if (!texture)	continue;


							_spell_map.insert({ pair.first, texture }); // Non è necessario usare c_str() qui
						}
						catch (const std::exception& e) {
							LOG("Eccezione catturata durante il caricamento della texture per: %s. Dettaglio: %s", pair.first.c_str(), e.what());
						}
					}
				}
				void InitializeSpellsImages()
				{
					test();
				}
			}

			namespace Configs
			{
				Menu* AwarenessMenu;
				Menu* HealthbarOverlayMenu;

				void Initialize()
				{
					AwarenessMenu = Menu::CreateMenu("Awareness", "Awareness");
					HealthbarOverlayMenu = AwarenessMenu->AddMenu("HPBarOverlay", "HPBar Overlay");
					initializedAwarenessMenu = true;
				}

				namespace Radius
				{
					void InitializeRadiusMenu()
					{
						const auto RadiusMenu = AwarenessMenu->AddMenu("utilityRadius", "Drawings Settings");
						status = RadiusMenu->AddCheckBox("status", "Enable radius drawings", true);
						drawMode = RadiusMenu->AddList("drawMode", "Draw Mode", std::vector<std::string>{"Outlined Circle", "Filled Circle", "Arc"}, 0);
						rainbowMode = RadiusMenu->AddCheckBox("rainbowMode", "Enable rainbow drawings", false);

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
						Awarenesss::Initialize();
						ChampionSpells::InitializeSpellsImages();
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
					Awarenesss::Sidebar::Draw(&Configs::EnemySidebar::status->Value);
					Radius::Update();
					Zoom::Update();
					JungleTracker::Update();
				}

				namespace EnemyTracker
				{
					void Update()
					{
						Awarenesss::LastHitDamage::Draw();
						DrawWardsTracker();

						DrawTracker();
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
									Awarenesss::HPBar::Draw(obj);
									Awarenesss::PathTracker::Draw(obj);
								}
								else if (!obj->IsAlly())
								{
									Awarenesss::LastPosition::Draw(obj);
								}

							}
						}
					}

					void DrawIcon(std::string iconName, const ImVec2& topLeft, const ImVec2& bottomRight) {
						const auto spellName = str_tolower(iconName);
						const auto it = ChampionSpells::_spell_map.find(spellName);
						if (it != ChampionSpells::_spell_map.end())
							render::Texture2D(it->second, topLeft, bottomRight, true);
					}

					void DrawWardsTracker() {
						for (auto ward : ObjectManager::GetWards(Alliance::Enemy))
						{
							if (!ward) continue;
							if (ward->GetDistanceTo(globals::localPlayer) > 1100.0f) continue;

							auto objDrawPos = Engine::GetBaseDrawPosition(ward);
							auto objPos = Engine::WorldToScreen(objDrawPos);
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
								DrawIcon(ward->GetName(), wardIconMin, wardIconMax);
								render::RenderCircle(ImVec2(objMapVec.x, objMapVec.y + 35.0f), 25, COLOR_DARK_TRANSPARENT, 0, 20, true);
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

					void Initialize()
					{
						__try { ChampionSpells::InitializeSpellsImages(); }
						__except (1) { LOG("[AWARENESS - ENEMY SIDEBAR] Error in initializing spells images"); }
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
								render::RenderPolygonWorld(poly, COLOR_WHITE, 1.0f);

								if (Configs::Radius::showMissilesAnimation->Value == true)
								{
									Geometry::Polygon poly2 = Geometry::Rectangle(startPos, missilePos, 70.f).ToPolygon();
									render::RenderPolygonWorld(poly2, 0x40FFFFFF, 1.0f, true, 0x40FFFFFF);
								}

							}
						}
					}

					void DrawRadius(Vector3 worldPos, float radius, uintptr_t color, float thickness, bool takeHeightInConsideration, bool glow)
					{
						switch (Configs::Radius::drawMode->Value)
						{
						case 0: //Outlined Circle
							render::RenderCircleWorld(worldPos, Configs::Radius::qualityDraw->Value, radius, color, thickness, takeHeightInConsideration, glow);
							break;
						case 1: //Filled Circle
							render::RenderCircleWorld(worldPos, Configs::Radius::qualityDraw->Value, radius, color, thickness, takeHeightInConsideration, glow, true, color, 0.10f);
							break;
						case 2: //Arc
							render::RenderArcWorld(worldPos, Configs::Radius::qualityDraw->Value, radius, color, thickness, PI / 3, Engine::GetMouseWorldPos(), true);
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
									DrawRadius(obj->GetPosition(), obj->GetBoundingRadius(), COLOR_WHITE, 1.0f, false, Configs::Radius::rainbowMode->Value);
								}
							}
							else
							{
								if (obj->IsEnemy() && Configs::Radius::showAARadiusEnemies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetBoundingRadius(), COLOR_RED, 1.0f, false, Configs::Radius::rainbowMode->Value);
								if (obj->IsAlly() && Configs::Radius::showAARadiusAllies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetBoundingRadius(), COLOR_BLUE, 1.0f, false, Configs::Radius::rainbowMode->Value);
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
									DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_WHITE, 1.0f, false, Configs::Radius::rainbowMode->Value);
							}
							else
							{
								if (obj->IsEnemy() && Configs::Radius::showAARadiusEnemies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_RED, 1.0f, false, Configs::Radius::rainbowMode->Value);
								if (obj->IsAlly() && Configs::Radius::showAARadiusAllies->Value == true)
									DrawRadius(obj->GetPosition(), obj->GetRealAttackRange(), COLOR_BLUE, 1.0f, false, Configs::Radius::rainbowMode->Value);
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
											DrawRadius(obj->GetPosition(), 850.0f, obj->IsAlly() ? COLOR_BLUE : COLOR_RED, 1.0f, false, Configs::Radius::rainbowMode->Value);
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
						zoomAddress = Engine::GetZoomAddress();

						shouldInject = Configs::Zoom::status->Value;

						shouldWrite = Configs::Zoom::status3D->Value;

						useKeyboard = Configs::Zoom::statusKeyboard->Value;

						useMouseWheel = Configs::Zoom::statusMouseWheel->Value;

						isLeagueFocused = Engine::IsGameFocused();

						zoomSlider = Configs::Zoom::zoomValue->Value / 10.0f;
					}

					void Update()
					{
						AssignValues();

						if (Configs::Zoom::status3D->Value && !globals::menuOpen)
						{
							gameTime = Engine::GetGameTime();

							const Vector3 pathEnd = globals::localPlayer->GetAiManager()->GetPathEnd();

							if (!globals::localPlayer->GetAiManager()->IsMoving()) //&& Orbwalker::Functions::lastActionTime + 5.0f < gameTime)
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
							diffVec.Multiply(globals::localPlayer->ReadClientStat(Object::MovementSpeed));

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
						Configs::JungleTracker::InitializeJungleTrackerMenu();
					}

					void Update()
					{
						if (Configs::JungleTracker::status->Value == true)
						{
							DrawJungleTracker();
						}
					}

					bool notificationShown = false;
					std::string notificationIcon;
					std::string notificationMessage;
					int notificationStartTime = 0;

					void ShowNotification(std::string iconToShow, std::string messageToShow)
					{
						notificationMessage = messageToShow;
						notificationStartTime = Engine::GetGameTime();
						notificationIcon = iconToShow;  // Ottieni l'icona dalla mappa

						notificationShown = true;
					}

					void RenderNotification()
					{
						if (notificationShown)
						{
							int currentTime = Engine::GetGameTime();
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

								render::RenderRect(minPos, maxPos, COLOR_DARK_TRANSPARENT, 4.f, ImDrawFlags_RoundCornersAll, 1.0f, true);
								ImVec2 iconMinPos = ImVec2(minPos.x + 100.0f, minPos.y + 10.0f);
								ImVec2 iconMaxPos = ImVec2(minPos.x + 190.0f, maxPos.y - 10.0f);
								EnemyTracker::DrawIcon(notificationIcon, iconMinPos, iconMaxPos);
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
							const char* displayName;

						}

						jungleInfos[] = {
						{ Vector3(11131.728516, 151.723694, 6990.844238), "sru_blue", "Blue" },
							{ Vector3(3821.488525, 151.128738, 8101.054199), "sru_blue", "Blue" },
						{ Vector3(7066.869141, 156.186646, 10975.546875), "sru_red" , "Red"},
							{ Vector3(7762.243652, 153.967743, 4011.186768), "sru_red" , "Red"},
						{ Vector3(12703.628906, 151.690781, 6443.983887), "sru_gromp" , "Gromp"},
							{ Vector3(2288.018555, 151.777313, 8448.133789), "sru_gromp" , "Gromp"},
						{ Vector3(11059.769531, 160.352585, 8419.830078), "sru_murkwolf" , "Wolves"},
							{ Vector3(3783.379883, 152.462723, 6495.560059), "sru_murkwolf" , "Wolves"},
						{ Vector3(7820.220215, 152.192017, 9644.450195), "sru_razorbeak" , "RazorBeaks"},
							{ Vector3(7061.500000, 150.123642, 5325.509766), "sru_razorbeak", "RazorBeaks" },
						{ Vector3(6499.490234, 156.476807, 12287.379883), "sru_blue" , "Golems"},
							{ Vector3(8394.769531, 150.731064, 2641.590088), "sru_blue" , "Golems"},
						{ Vector3(9866.148438, 28.759399, 4414.014160), "sru_dragon" , "Dragon"},
							{ Vector3(5007.123535, 28.759399, 10471.446289), (Engine::GetGameTime() >= 1000) ? "sru_baron" : "sru_riftherald" ,(Engine::GetGameTime() >= 1000) ? "Baron" : "RiftHerald"},
						};


						for (const auto& jungleInfo : jungleInfos)
						{
							if (render::IsVectorEqual(objDrawPos, jungleInfo.position))
							{
								if (jungleInfo.name == "sru_dragon" && Engine::GetGameTime() < 500 || jungleInfo.name == "sru_riftherald" && Engine::GetGameTime() < 950)
									return;

								ShowNotification(std::string(jungleInfo.name), std::string(jungleInfo.displayName) + " Killed");
								break;
							}
						}
					}

					std::unordered_set<Object*> insertedObjects;
					void DrawJungleTracker()
					{
						if (Engine::GetGameTime() < 100.0f) return;

						for (auto obj : ObjectManager::GetJungleRespawnCamps())
						{
							Vector3 objDrawPos = Engine::GetBaseDrawPosition(obj);
							Vector2 objMinimapPos = Engine::WorldToMinimap(obj);
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
						const float minimap_size = Engine::GetMinimapSize();
						const float scale_factor = 30 * ((minimap_size - 192) / 193);
						const float def_scale = max(20.46, scale_factor);

						const struct JungleIconInfo
						{
							Vector3 position;
							std::string iconName;
						}

						jungleIconInfos[] = { { Vector3(11131.728516, 151.723694, 6990.844238), "sru_blue" },
						{ Vector3(3821.488525, 151.128738, 8101.054199), "sru_blue" },
						{ Vector3(7066.869141, 156.186646, 10975.546875), "sru_red" },
						{ Vector3(7762.243652, 153.967743, 4011.186768), "sru_red" },
						{ Vector3(12703.628906, 151.690781, 6443.983887), "sru_gromp" },
						{ Vector3(2288.018555, 151.777313, 8448.133789), "sru_gromp"},
						{ Vector3(11059.769531, 160.352585, 8419.830078), "sru_murkwolf" },
						{ Vector3(3783.379883, 152.462723, 6495.560059), "sru_murkwolf" },
						{ Vector3(7820.220215, 152.192017, 9644.450195), "sru_razorbeak" },
						{ Vector3(7061.500000, 150.123642, 5325.509766), "sru_razorbeak" },
						{ Vector3(6499.490234, 156.476807, 12287.379883), "sru_krug" },
						{ Vector3(8394.769531, 150.731064, 2641.590088), "sru_krug"},
						{ Vector3(9866.148438, 28.759399, 4414.014160), "sru_dragon" },
						{ Vector3(5007.123535, 28.759399, 10471.446289), (Engine::GetGameTime() >= 900) ? "sru_baron" : "sru_riftherald" },
						};

						for (const auto& jungleIconInfo : jungleIconInfos)
						{
							if (render::IsVectorEqual(objDrawPos, jungleIconInfo.position))
							{
								EnemyTracker::DrawIcon(jungleIconInfo.iconName, ImVec2(pos.x, pos.y), ImVec2(pos.x + def_scale, pos.y + def_scale));
								break; // Esci dal loop dopo il rendering
							}
						}

						const auto jungleIconCenter = ImVec2(pos.x + (def_scale / 2), pos.y + (def_scale / 2));
						render::RenderCircle(jungleIconCenter, def_scale / 2, COLOR_DARK_TRANSPARENT, 0, 30, true);
					}

					void DrawTimers(Object* obj, ImVec2 pos)
					{
						const float minimap_size = Engine::GetMinimapSize();
						const float scale_factor = 30 * ((minimap_size - 192) / 193);
						const float def_scale = max(20.46, scale_factor);
						const auto jungleIconMin = ImVec2(pos.x - (def_scale / 2), pos.y - (def_scale / 2));

						float currentGameTime = Engine::GetGameTime();

						if (obj->GetBuffByName("camprespawncountdownhidden"))
						{
							float respawnAtGameTime = obj->GetBuffByName("camprespawncountdownhidden")->GetEndTime() + 60;
							float timeToShow = respawnAtGameTime - currentGameTime;

							if (timeToShow > 0.0f)
							{
								render::RenderText(Engine::ConvertTime(timeToShow), jungleIconMin, def_scale / 1.50, COLOR_WHITE, false);
							}

						}
						else if (obj->GetBuffByName("camprespawncountdownvisible"))
						{
							float respawnAtGameTime = obj->GetBuffByName("camprespawncountdownvisible")->GetEndTime();
							float timeToShow = respawnAtGameTime - currentGameTime;

							if (timeToShow > 0.0f)
							{
								render::RenderText(Engine::ConvertTime(timeToShow), jungleIconMin, def_scale / 1.50, COLOR_WHITE, false);
							}
						}
					}
				}
			}
		}
	}
}