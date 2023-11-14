#include "Orbwalker.h"

#include "TargetSelector.h"

namespace UPasta
{
	namespace SDK
	{
		namespace Orbwalker
		{
			namespace Configs
			{
				Menu* OrbwalkerMenu;
				void Initialize()
				{
					OrbwalkerMenu = Menu::CreateMenu("vez.Orbwalker", "Orbwalker");
					status = OrbwalkerMenu->AddCheckBox("status", "Enable Orbwalker", true);
					initializedOrbwalkerMenu = true;

					if (initializedOrbwalkerMenu)
					{
						if (!Humanizer::initializedHumanizerMenu)
							Humanizer::InitializeHumanizerMenu();

						if (Humanizer::initializedHumanizerMenu)
							Status::InitializeStatusMenu();

						if (Status::initializedStatusMenu)
							KeyBindings::InitializeKeyBindingsMenu();
					}
				}

				namespace Humanizer
				{
					Menu* HumanizerMenu;
					void InitializeHumanizerMenu()
					{
						HumanizerMenu = OrbwalkerMenu->AddMenu("humanizerSection", "Humanizer settings");
						status = HumanizerMenu->AddCheckBox("status", "Enable the humanizer", true);

						randomizeDelay = HumanizerMenu->AddCheckBox("randomizeDelay", "Randomize clicks delay", false);
						clickDelay = HumanizerMenu->AddSlider("clickDelay", "Delay between clicks", 50, 10, 100, 1);

						windupDelay = HumanizerMenu->AddSlider("windupDelay", "AA Travel time delay", 30, 10, 100, 1);

						beforeAttackDelay = HumanizerMenu->AddSlider("beforeAttackDelay", "AA before attack order delay", 10, 10, 100, 1);
						farmAttackDelay = HumanizerMenu->AddSlider("farmAttackDelay", "Farm delay", 10, 10, 100, 1);


						initializedHumanizerMenu = true;
					}
				}

				namespace Status
				{
					Menu* StatusMenu;

					void InitializeStatusMenu()
					{
						StatusMenu = OrbwalkerMenu->AddMenu("statusSection", "Status settings");
						statusFollowMouse = StatusMenu->AddCheckBox("statusFollowMouse", "Enable Mouse follow", true);
						statusComboMode = StatusMenu->AddCheckBox("statusComboMode", "Enable Combo mode", true);
						statusLaneClearMode = StatusMenu->AddCheckBox("statusLaneClearMode", "Enable LaneClear mode", true);
						statusFastClearMode = StatusMenu->AddCheckBox("statusFastClearMode", "Enable FastClear mode", true);
						statusLastHitMode = StatusMenu->AddCheckBox("statusLastHitMode", "Enable LastHit mode", true);
						statusHarassMode = StatusMenu->AddCheckBox("statusHarassMode", "Enable Harass mode", true);
						statusFleeMode = StatusMenu->AddCheckBox("statusFleeMode", "Enable Flee mode", true);

						initializedStatusMenu = true;
					}
				}

				namespace KeyBindings
				{
					Menu* KeyBindingsMenu;
					void InitializeKeyBindingsMenu()
					{
						KeyBindingsMenu = OrbwalkerMenu->AddMenu("keybindingsSection", "KeyBindings settings");
						comboKey = KeyBindingsMenu->AddKeyBind("comboKey", "Combo Key", ' ', false, false);
						harassKey = KeyBindingsMenu->AddKeyBind("harassKey", "Harass Key", 'C', false, false);
						lastHitKey = KeyBindingsMenu->AddKeyBind("lastHitKey", "LastHit Key", 'X', false, false);
						laneClearKey = KeyBindingsMenu->AddKeyBind("laneClearKey", "LaneClear Key", 'V', false, false);
						fastClearKey = KeyBindingsMenu->AddKeyBind("fastClearKey", "FastClear Key", 'CTRL', false, false);
						fleeKey = KeyBindingsMenu->AddKeyBind("fleeKey", "Flee Key", 'T', false, false);

						initializedKeyBindingsMenu = true;
					}
				}

			}

			namespace Functions
			{
				float nextRngBuffer = 0.0f;
				float gameTime = 0;
				float lastAttackTime = 0.0f;
				QWORD lastSpellCastAddress = 0;
				bool shouldWait = false;
				bool isReloading = false;
				void Initialize()
				{
					if (!Configs::initializedOrbwalkerMenu)
					{
						Configs::Initialize();
					}
				}

				void KeyChecks()
				{
					globals::scripts::orbwalker::orbwalkState = OrbwalkState::Off;

					if (Configs::KeyBindings::comboKey->Value && Configs::Status::statusComboMode->Value)
						globals::scripts::orbwalker::orbwalkState = OrbwalkState::Attack;
					if (Configs::KeyBindings::laneClearKey->Value && Configs::Status::statusLaneClearMode->Value)
						globals::scripts::orbwalker::orbwalkState = OrbwalkState::Clear;
					if (Configs::KeyBindings::harassKey->Value && Configs::Status::statusHarassMode->Value)
						globals::scripts::orbwalker::orbwalkState = OrbwalkState::Harass;
					if (Configs::KeyBindings::lastHitKey->Value && Configs::Status::statusLastHitMode->Value)
						globals::scripts::orbwalker::orbwalkState = OrbwalkState::Lasthit;
					if (Configs::KeyBindings::fleeKey->Value && Configs::Status::statusFleeMode->Value)
						globals::scripts::orbwalker::orbwalkState = OrbwalkState::Flee;
				}


				void Update()
				{
					gameTime = functions::GetGameTime();
					__try { KeyChecks(); }
					__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> KEYCHECKS UPDATE"); }
					__try { CheckActiveAttack(); }
					__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> CHECKATTIVEATTACK UPDATE"); }
					__try { StopOrbwalkCheck(); }
					__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> STOPORBWALK UPDATE"); }
					__try { IsReloadingCheck(); }
					__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> ISRELOADING UPDATE"); }
					

					/*if (Configs::KeyBindings::comboKey->Value)
					{
						Vector2 screenPos = functions::WorldToScreen(globals::localPlayer->GetPosition());

						render::RenderText("test", (screenPos - Vector2(0.0f, 0.0f)).ToImVec(), 18.0f, COLOR_WHITE, true);
					}*/

					if (shouldWait)
						return;

					if (globals::scripts::orbwalker::orbwalkState && isReloading)
					{
						Actions::Idle();
						return;
					}

					/*if (globals::localPlayer->IsCastingSpell())
						return;*/

					switch (globals::scripts::orbwalker::orbwalkState)
					{
					case Attack:
						__try { States::Attack(); }
						__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> ATTACK UPDATE"); }
						break;
					case Clear:
						__try { States::Laneclear(); }
						__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> LANECLEAR UPDATE"); }
						break;
					case Harass:
						__try { States::Harass(); }
						__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> HARASS UPDATE"); }
						break;
					case Lasthit:
						__try { States::Lasthit(); }
						__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> LASTHIT UPDATE"); }
					case Flee:
						__try { Actions::Idle(); }
						__except (1) { LOG("ERROR IN SCRIPTS -> ORBWALKER -> IDLE UPDATE"); }
						break;
					
					}

				}

				void CheckActiveAttack()
				{
					auto spellCast = globals::localPlayer->GetActiveSpellCast();
					if (spellCast != nullptr)
					{
						if ((spellCast->IsAutoAttack() ||
							functions::IsAttackWindupSpell(spellCast->GetSpellId())) &&
							(QWORD)spellCast != lastSpellCastAddress)
						{
							lastAttackTime = gameTime;
						}
					}

					lastSpellCastAddress = (QWORD)spellCast;
				}

				void StopOrbwalkCheck()
				{
					if (!functions::CanSendInput()
						|| gameTime < lastAttackTime + globals::localPlayer->GetAttackWindup() + (Configs::Humanizer::windupDelay->Value / 1000.0f))
						shouldWait = true;
					else
						shouldWait = false;
				}

				float lastActionTime = 0.0f;
				bool CanDoAction()
				{
					if (lastActionTime == 0.0f) 
						lastActionTime = gameTime;

					const float humanizerTimer = (Configs::Humanizer::clickDelay->Value / 1000.0f) + nextRngBuffer;
					if (gameTime < lastActionTime + humanizerTimer) 
						return false;

					lastActionTime = gameTime;
					return true;
				}

				bool ShouldWaitUnderTurret(Object* noneKillableMinion)
				{
					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto minion = globals::minionManager->GetIndex(i);
						if (minion->IsAlive() && minion->IsVisible())
						{
							if (minion->GetName() == "") continue;
							if (!minion->IsValidTarget()) continue;
							if (minion->GetCharacterData()->GetObjectTypeHash() != ObjectType::Minion_Lane) continue;
							if (!minion->IsInRange(globals::localPlayer->GetPosition(), globals::localPlayer->GetRealAttackRange())) continue;
							if (minion)
							{
								return
									((noneKillableMinion != nullptr ? noneKillableMinion->GetNetId() != minion->GetNetId() : true)
										&& minion->IsInAARange() && Damage::CalculateAutoAttackDamage(globals::localPlayer, minion) > minion->GetHealth());
							}
						}
					}
				}

				void IsReloadingCheck()
				{
					if (gameTime < lastAttackTime + globals::localPlayer->GetAttackDelay() - (Configs::Humanizer::beforeAttackDelay->Value / 1000.0f))
						isReloading = true;
					else
						isReloading = false;
				}

				void RefreshBuffer()
				{
					if (Configs::Humanizer::randomizeDelay->Value == false)
					{
						nextRngBuffer = 0.0f;
						return;
					}
					nextRngBuffer = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.01f);
				}

				float GetAttackMissileSpeed()
				{
					float def = globals::localPlayer->GetRealAttackRange() < 300.0f ? FLT_MAX : globals::localPlayer->GetAttackWindup();

					if (globals::localPlayer->GetName() == "Thresh" 
					|| globals::localPlayer->GetName() == "Azir" 
					|| globals::localPlayer->GetName() == "Velkoz" 
					|| globals::localPlayer->GetName() == "Rakan")
						return FLT_MAX;

					else if (globals::localPlayer->GetName() == "Thresh")
					{
						if (globals::localPlayer->GetBuffByName("ViktorPowerTransferReturn"))
							return FLT_MAX;
					}

					else if (globals::localPlayer->GetName() == "Jinx")
					{
						if (globals::localPlayer->GetBuffByName("JinxQ"))
							return 2000.0f;
					}

					else if (globals::localPlayer->GetName() == "Poppy")
					{
						if (globals::localPlayer->GetBuffByName("poppypassivebuff"))
							return 1600.0f;
					}

					else if (globals::localPlayer->GetName() == "Ivern")
					{
						if (globals::localPlayer->GetBuffByName("ivernwpassive"))
							return 1600.0f;
					}

					else if (globals::localPlayer->GetName() == "Caitlyn")
					{
						if (globals::localPlayer->GetBuffByName("caitlynheadshot"))
							return 3000.0f;
					}

					else if (globals::localPlayer->GetName() == "Twitch")
					{
						if (globals::localPlayer->GetBuffByName("TwitchFullAutomatic"))
							return 4000.0f;
					}

					else if (globals::localPlayer->GetName() == "Jayce")
					{
						if (globals::localPlayer->GetBuffByName("jaycestancegun"))
							return 2000.0f;
					}

					return def;
				}

				namespace Actions
				{
					void Idle()
					{
						if (!(CanDoAction() && globals::localPlayer->CanMove()))
							return;

						if (Configs::Status::statusFollowMouse->Value)
						{
							SpellCast* spellCast = globals::localPlayer->GetActiveSpellCast();
							if (spellCast && spellCast->GetSpellInfo()->GetSpellData()->GetName() == "XerathLocusOfPower2")
								return;

							functions::MoveToMousePos();
						}

						RefreshBuffer();
					}

					void AttackObject(Object* obj)
					{
						if (!CanDoAction())
							return;

						functions::AttackObject(obj->GetPosition());
						RefreshBuffer();
					}

					void AttackInhib(Object* obj)
					{
						if (!CanDoAction())
							return;

						functions::AttackObject(obj->GetPosition());
						RefreshBuffer();
					}

					void CastSpell(int spellId, Object* target)
					{
						Vector3 headPos = target->GetPosition();
						const float objectHeight = *(float*)(target->GetCharacterData() + Offsets::GameObject::CharData::Size) * target->GetScale();
						headPos.y += objectHeight;
						CastSpell(spellId, headPos);
					}

					void CastSpell(int spellId, Vector3 pos)
					{
						/*if (!CanDoAction())
							return;*/
						functions::CastSpell(spellId, pos);
						RefreshBuffer();
					}
				}

				namespace States
				{
					void Attack()
					{
						if (globals::localPlayer->CanAttack())
						{
							auto obj = TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetRealAttackRange());
							if (obj != nullptr)
							{
								Actions::AttackObject(obj);
								return;
							}
						}

						Actions::Idle();
					}

					void Laneclear()
					{
						if (globals::localPlayer->CanAttack())
						{
							const auto killableMinion = TargetSelector::Functions::GetKillableEnemyMinionInRange(globals::localPlayer->GetRealAttackRange());
							if (killableMinion != nullptr) //&& !ShouldWaitUnderTurret(killableMinion))
							{
								LOG("teST");
								if (killableMinion->GetEffectiveHealth(Physical) + 50 < Damage::CalculateAutoAttackDamage(globals::localPlayer, killableMinion))
									Actions::AttackObject(killableMinion);
							}

							const auto jungleMonster = TargetSelector::Functions::GetJungleInRange(globals::localPlayer->GetRealAttackRange());
							if (jungleMonster != nullptr)
								Actions::AttackObject(jungleMonster);

							const auto turret = TargetSelector::Functions::GetEnemyTurretInRange(globals::localPlayer->GetRealAttackRange());
							if (turret != nullptr)
								Actions::AttackObject(turret);

							const auto minion = TargetSelector::Functions::GetEnemyMinionInRange(globals::localPlayer->GetRealAttackRange());
							if (minion != nullptr && !ShouldWaitUnderTurret(minion))
							{
								if (minion->GetHealth() > Damage::CalculateAutoAttackDamage(globals::localPlayer, minion) * 2 
									|| Damage::CalculateAutoAttackDamage(globals::localPlayer, minion) - 10.0f > minion->GetHealth())
									Actions::AttackObject(minion);
							}

							/*else 
							{
								//const int missileSpeed = GetAttackMissileSpeed();
								for (int i = 0; i < globals::minionManager->GetListSize(); i++)
								{
									auto minion = globals::minionManager->GetIndex(i);
									if (minion->IsAlive() && minion->IsVisible())
									{
										if (!minion->IsValidTarget()) continue;
										if (!minion->IsMinion()) continue;
										if (minion->GetDistanceTo(globals::localPlayer) > globals::localPlayer->GetRealAttackRange()) continue;

										if (minion && minion->GetDistanceTo(globals::localPlayer) < globals::localPlayer->GetRealAttackRange())
										{
											const float minionHealth = minion->GetHealth() - Damage::CalculateAutoAttackDamage(globals::localPlayer, minion);
											const auto attackDamage = Damage::CalculateAutoAttackDamage(globals::localPlayer, minion);

											const auto turret = TargetSelector::Functions::GetAllyTurretInRange(globals::localPlayer->GetRealAttackRange());
											if (turret && turret->GetDistanceTo(minion) <= 900)
											{
												if (minion->GetHealth() > attackDamage)
												{
													const auto turretDamage = Damage::CalculateAutoAttackDamage(turret, minion);
													if (minion->GetHealth() - turretDamage > 0.0f)
													{
														for (auto minionHealth = minion->GetHealth(); minionHealth > 0.0f && turretDamage > 0.0f; minionHealth -= turretDamage)
														{
															if (minionHealth <= attackDamage)
																break;
														}

														if (!ShouldWaitUnderTurret(globals::localPlayer))
														{
															Actions::AttackObject(minion);
															break;
														}
													}
												}
												break;
											}

											if (minionHealth >= 2.0f * attackDamage || minionHealth < attackDamage)
											{
												Actions::AttackObject(minion);
												return;
											}

										}
									}
								}
							}*/
						}

						Actions::Idle();
					}

					void Harass()
					{
						if (globals::localPlayer->CanAttack())
						{
							for (auto killableMinion : TargetSelector::Functions::GetKillableMinionsInRange(globals::localPlayer->GetPosition(), globals::localPlayer->GetRealAttackRange(), Physical))
								Actions::AttackObject(killableMinion);

							const auto obj = UPasta::SDK::TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetRealAttackRange());
							if (obj != nullptr)
								Actions::AttackObject(obj);
						}

						Actions::Idle();
					}

					void Lasthit()
					{
						if (globals::localPlayer->CanAttack())
						{
							const auto minion = TargetSelector::Functions::GetKillableEnemyMinionInRange(globals::localPlayer->GetRealAttackRange());
							if (minion != nullptr && !ShouldWaitUnderTurret(minion))
								Actions::AttackObject(minion);
						}

						Actions::Idle();
					}
				}
			}
		}
	}
}