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
					OrbwalkerMenu = Menu::CreateMenu("vez.Orbwalker", "vez.Orbwalker");
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
						laneClearKey = KeyBindingsMenu->AddKeyBind("laneClearKey", "LaneClear Key", 'V', false, false);
						fastClearKey = KeyBindingsMenu->AddKeyBind("fastClearKey", "FastClear Key", 'X', false, false);
						lastHitKey = KeyBindingsMenu->AddKeyBind("lastHitKey", "LastHit Key", 'Z', false, false);
						harassKey = KeyBindingsMenu->AddKeyBind("harassKey", "Harass Key", 'C', false, false);

						initializedKeyBindingsMenu = true;
					}
				}

			}

			namespace Functions
			{
				float nextRngBuffer = 0.0f;
				float gameTime = 0.0f;
				float lastAttackTime = 0.0f;
				QWORD lastSpellCastAddress = 0;
				float lastActionTime = 0.0f;
				bool shouldWait = false;
				bool isReloading = false;
				bool canDoAction = true;

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
				}


				void Update()
				{
					srand(static_cast <unsigned> (time(0)));
					gameTime = functions::GetGameTime();

					KeyChecks();
					CheckActiveAttack();
					StopOrbwalkCheck();
					IsReloadingCheck();


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

					switch (globals::scripts::orbwalker::orbwalkState)
					{
					case Attack:
						States::Attack();
						break;
					case Clear:
						States::Laneclear();
						break;
					case Harass:
						States::Harass();
						break;
					case Lasthit:
						States::Lasthit();
						break;
					
					}

				}

				void CheckActiveAttack()
				{
					auto spellCast = globals::localPlayer->GetActiveSpellCast();
					if (spellCast)
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

				bool CanDoAction()
				{
					if (!lastActionTime) lastActionTime = gameTime;
					if (gameTime < lastActionTime + (Configs::Humanizer::clickDelay->Value / 1000.0f) + nextRngBuffer)
						return false;

					lastActionTime = gameTime;
					return true;
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
					if (!Configs::Humanizer::randomizeDelay)
					{
						nextRngBuffer = 0.0f;
						return;
					}
					nextRngBuffer = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.01f);
				}

				namespace Actions
				{
					void Idle()
					{
						if (!(CanDoAction() && globals::localPlayer->CanMove()))
							return;

						if (Configs::Status::statusFollowMouse->Value)
							functions::MoveToMousePos();

						RefreshBuffer();
					}

					void AttackObject(Object* obj)
					{
						if (!CanDoAction())
							return;

						functions::AttackObject(obj);
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
						const float objectHeight = *(float*)(target->GetCharacterData() + oObjCharDataDataSize) * target->GetScale();
						headPos.y += objectHeight;
						CastSpell(spellId, headPos);
					}

					void CastSpell(int spellId, Vector3 pos)
					{
						if (!CanDoAction()) return;
						functions::CastSpell(spellId, pos);
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
							auto turret = TargetSelector::Functions::GetEnemyTurretInRange(globals::localPlayer->GetRealAttackRange());
							if (turret)
							{
								Actions::AttackObject(turret);
								return;
							}

							auto inhibitor = TargetSelector::Functions::GetEnemyInhibitorInRange(globals::localPlayer->GetRealAttackRange());
							if (inhibitor)
							{
								Actions::AttackInhib(inhibitor);
								return;
							}

							auto nexus = TargetSelector::Functions::GetEnemyNexusInRange(globals::localPlayer->GetRealAttackRange());
							if (nexus)
							{
								if (nexus->GetMaxHealth() == 5500)
								{
									Actions::AttackInhib(nexus);
									return;
								}
							}

							auto minion = TargetSelector::Functions::GetEnemyMinionInRange(globals::localPlayer->GetRealAttackRange());
							if (minion)
							{
								Actions::AttackObject(minion);
								return;
							}
						}

						Actions::Idle();
					}

					void Harass()
					{
						if (globals::localPlayer->CanAttack())
						{
							auto obj = UPasta::SDK::TargetSelector::Functions::GetEnemyChampionInRange(globals::localPlayer->GetRealAttackRange());
							if (obj != nullptr)
							{
								Actions::AttackObject(obj);
								return;
							}
						}

						Actions::Idle();
					}

					void Lasthit()
					{
						if (globals::localPlayer->CanAttack())
						{
							auto minion = UPasta::SDK::TargetSelector::Functions::GetObjectInRange(globals::localPlayer->GetRealAttackRange());
							if (minion)
							{
								Actions::AttackObject(minion);
								return;
							}
						}

						Actions::Idle();
					}
				}
			}
		}
	}
}