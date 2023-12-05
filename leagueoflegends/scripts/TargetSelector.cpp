#include "../stdafx.h"
#include "../TargetSelector.h"

namespace UPasta
{
	namespace SDK
	{
		namespace TargetSelector
		{
			namespace Configs
			{
				Menu* TSMenu;
				void Configs::Initialize()
				{
					TSMenu = Menu::CreateMenu("TargetSelector", "TargetSelector");

					__try { InitializeComboTargets(); }
					__except (1) { LOG("[TS MENU] Error in initializing combo targets"); }

					__try { InitializeClearTargets(); }
					__except (1) { LOG("[TS MENU] Error in initializing clear targets"); }

					__try { InitializeLastHitTargets(); }
					__except (1) { LOG("[TS MENU] Error in initializing lasthit targets"); }

					__try { InitializeUnderTowerTargets(); }
					__except (1) { LOG("[TS MENU] Error in initializing undertower targets"); }

					__try { InitializeDrawSettings(); }
					__except (1) { LOG("[TS MENU] Error in initializing drawings settings"); }
				}

				std::vector<Object*> Configs::Enemies;
				void Configs::PopulateComboTargets()
				{
					for (int i = 0; i < globals::heroManager->GetListSize(); i++)
					{
						auto obj = globals::heroManager->GetIndex(i);
						if (obj != nullptr && obj->IsEnemy())
						{
							Enemies.push_back(obj);
						}
					}
				}

				Menu* TargetPrioritiesMenu;
				TargetSelectorMode ActiveMode = TargetSelectorMode::Auto;
				std::unordered_map<std::string, unsigned int> PrioritiesMap = 
				{	{ "Aatrox", 3 }, { "Ahri", 4 }, { "Akali", 4 },	{ "Akshan", 4 }, { "Alistar", 1 }, { "Amumu", 2 },
					{ "Anivia", 4 }, { "Annie", 4 }, { "Aphelios", 5 },	{ "Ashe", 5 }, { "Aurelion", 5 }, { "Azir", 5 },
					{ "Bard", 2 }, { "Belveth", 3 }, { "Blitzcrank", 1 }, { "Brand", 3 },{ "Brerr", 3 }, { "Braum", 1 }, { "Caitlyn", 5 },
					{ "Camille", 3 }, { "Cassiopeia", 5 }, { "Chogath", 1 }, { "Corki", 5 }, { "Darius", 2 }, { "Diana", 4 },
					{ "DrMundo", 1 }, { "Draven", 5 }, { "Ekko", 3 }, { "Elise", 4 }, { "Evelynn", 4 }, { "Ezreal", 4 },
					{ "Fiddlesticks", 4 }, { "Fiora", 3 }, { "Fizz", 4 }, { "Galio", 2 }, { "Gankplank", 5 }, { "Garen", 2 },
					{ "Gnar", 3 }, { "Gragas", 3 }, { "Graves", 4 }, { "Gwen", 4 }, { "Hecarim", 3 }, { "Heimerdinger", 4 },
					{ "Illaoi", 2 }, { "Irelia", 3 }, { "Ivern", 2 }, { "Janna", 2 }, { "Jarvan", 1 }, { "Jax", 3 },
					{ "Jayce", 4 }, { "Jhin", 4 }, { "Jinx", 5 }, { "KSante", 2 }, { "Kaisa", 5 }, { "Kalista", 4 },
					{ "Karma", 2 }, { "Karthus", 5 }, { "Kassadin", 5 }, { "Katarina", 5 }, { "Kayle", 5 }, { "Kayn", 3 },
					{ "Kennen", 4 }, { "Khazix", 4 }, { "Leblanc", 4 }, { "LeeSin", 2 }, { "Leona", 1 }, { "Lillia", 3 },
					{ "Lissandra", 3 }, { "Lulcian", 4 }, { "Lulu", 4 }, { "Lux", 3 }, { "Malphite", 1 }, { "Malzahar", 3 },
					{ "Maokai", 1 }, { "MasterYi", 5 }, { "Milio", 2 }, { "MissFortune", 5 }, { "Mordekaiser", 2 }, { "Morgana", 3 },
					{ "Naafiri", 4 }, { "Nami", 2 }, { "Nasus", 1 }, { "Nautilus", 1 }, { "Neeko", 3 }, { "Nidalee", 3 },
					{ "Nilah", 4 }, { "Nocturne", 3 }, { "Nunu", 2 }, { "Olaf", 2 }, { "Orianna", 4 }, { "Ornn", 2 },
					{ "Pantheon", 4 }, { "Poppy", 1 }, { "Pyke", 3 }, { "Qiyana", 4 }, { "Quinn", 4 }, { "Rakan", 2 },
					{ "Rammus", 1 }, { "RekSai", 3 }, { "Rell", 1 }, { "Renata", 3 }, { "Renekton", 2 }, { "Rengar", 4 },
					{ "Riven", 3 }, { "Rumble", 4 }, { "Ryze", 4 }, { "Samira", 5 }, { "Sejuani", 1 }, { "Senna", 5 },
					{ "Seraphine", 3 }, { "Sett", 2 }, { "Shaco", 4 }, { "Shen", 1 }, { "Shyvana", 3 }, { "Singed", 1 },
					{ "Sion", 1 }, { "Sivir", 5 }, { "Skarner", 2 }, { "Sona", 3 }, { "Soraka", 4 }, { "Swain", 3 },
					{ "Sylas", 3 }, { "Syndra", 4 }, { "Tahm", 1 }, { "Taliyah", 4 }, { "Talon", 3 }, { "Taric", 1 },
					{ "Teemo", 3 }, { "Thresh", 1 }, { "Tristana", 5 },	{ "Trundle", 2 }, { "Tryndamere", 4 }, { "TwistedFate", 3 },
					{ "Twitch", 5 }, { "Udyr", 2 }, { "Urgot", 2 },	{ "Varus", 5 }, { "Vayne", 5 }, { "Veigar", 5 },
					{ "Velkoz", 3 }, { "Vex", 3 }, { "Vi", 2 },	{ "Viego", 2 }, { "Viktor", 5 }, { "Vladimir", 5 },
					{ "Volibear", 2 }, { "Warwick", 2 }, { "Wukong", 3 }, { "Xayah", 5 }, { "Xerath", 3 }, { "XinZhao", 2 },
					{ "Yasuo", 5 }, { "Yone", 5 }, { "Yorick", 1 }, { "Yuumi", 4 }, { "Zac", 1 }, { "Zed", 3 },
					{ "Zeri", 5 }, { "Ziggs", 4 }, { "Zilean", 3 },	{ "Zoe", 3 }, { "Zyra", 3 }
				};

				std::unordered_map<std::string, bool> AntiGapcloserMap =
				{   { "Aatrox", true }, { "Ahri", true }, { "Akali", true },	{ "Akshan", true },
					{ "Alistar", true }, { "Amumu", true },{ "Aurelion", true }, { "Azir", true },
					{ "Belveth", true }, { "Blitzcrank", true }, { "Brand", 3 }, { "Caitlyn", true },
					{ "Camille", true }, { "Corki", true },{ "Diana", true },{ "Ekko", true },
					{ "Ezreal", true },{ "Fiddlesticks", true },{ "Galio", true },{ "Gnar", true },
					{ "Gragas", true }, { "Gwen", true }, { "Hecarim", true }, { "Irelia", true },
					{ "KSante", true }, { "Kalista", true },{ "Kassadin", true }, { "Katarina", true },
					{ "Kayn", true }, { "Khazix", true }, { "Leblanc", true }, { "LeeSin", true },
					{ "Leona", true }, { "Lissandra", true }, { "Malphite", true },{ "Maokai", true },
					{ "MasterYi", true },{ "Naafiri", true },{ "Nautilus", true },{ "Nilah", true },
					{ "Nocturne", true },{ "Pantheon", true }, { "Poppy", true }, { "Pyke", true }, 
					{ "RekSai", true }, { "Rell", true }, { "Renekton", true }, { "Rengar", true },
					{ "Riven", true },  { "Samira", true }, { "Sejuani", true },	{ "Shaco", true },
					{ "Shen", true },{ "Tahm", true }, { "Talon", true },{ "Tristana", true },
					{ "Tryndamere", true },{ "Vi", true },	{ "Viego", true }, { "Wukong", true },
					{ "Xayah", true },{ "Yasuo", true }, { "Yone", true }, { "Zac", true },
					{ "Zed", true }
				};

				void Configs::InitializeComboTargets()
				{
					PopulateComboTargets();
					if (!Enemies.empty())
					{
						TargetPrioritiesMenu = TSMenu->AddMenu("targetPriorities", "Target priorities");
						TargetPrioritiesMenu->AddTooltip("Higher value means higher priority\nFor ignore put 0 as priority");

						ActiveMode = (TargetSelectorMode)TargetPrioritiesMenu->AddList("Mode", "Mode",
							{ "Auto",  "Least Health", "Most AD", "Most AP", "Closest", "Highest Priority", "Less Attack", "Less Cast", "Near Mouse" },
							0,
							[](List* self, unsigned int newValue)
							{
								ActiveMode = (TargetSelectorMode)newValue;
							})->Value;

						for (auto enemy : Enemies)
						{
							auto iterator = PrioritiesMap.find(enemy->GetName().c_str());
							TargetPrioritiesMenu->AddSlider(enemy->GetName().c_str(), enemy->GetName().c_str(), iterator != PrioritiesMap.end() ? iterator->second : 1, 1, 5, 1);
						}

						TargetPrioritiesMenu->AddCheckBox("Reset", "Reset priorities", false, [](CheckBox* self, bool newValue)
							{
								if (newValue)
								{
									for (auto enemy : Enemies)
									{
										auto iterator = PrioritiesMap.find(enemy->GetName());
										(*TargetPrioritiesMenu)[enemy->GetName().c_str()]->Cast<Slider*>()->Value = iterator != PrioritiesMap.end() ? iterator->second : 1;
										self->Value = false;
									}
								}
							});
						TSModes::Combo::Advanced::avoidAttackInvulnerable = TargetPrioritiesMenu->AddCheckBox("AvoidAttackInvulnerable", "Dont attack invulnerable targets", true);

						TSModes::Combo::Advanced::attackSelectedTarget = TargetPrioritiesMenu->AddCheckBox("ForceSelectedTarget", "Force Selected Target", true);
						TSModes::Combo::Advanced::attackOnlySelectedTarget = TargetPrioritiesMenu->AddCheckBox("AttackOnlySelectedTarget", "Attack only selected target");
						TSModes::Combo::Advanced::attackClones = TargetPrioritiesMenu->AddCheckBox("AttackClones", "Attack clones");
					}
				}

				Menu* ClearPrioritiesMenu;
				void Configs::InitializeClearTargets()
				{
					ClearPrioritiesMenu = TSMenu->AddMenu("laneclearPriorities", "Laneclear priorities");
					ClearPrioritiesMenu->AddTooltip("Higher value means higher priority\nFor ignore put 0 as priority");

					TSModes::Laneclear::wardPriority = ClearPrioritiesMenu->AddSlider("wardsPriorityClear", "Wards priority", 3, 1, 5, 1);
					TSModes::Laneclear::superMinionPriority = ClearPrioritiesMenu->AddSlider("superMinionPriorityClear", "Super Minion priority", 5, 1, 5, 1);
					TSModes::Laneclear::meleeMinionPriority = ClearPrioritiesMenu->AddSlider("meleeMinionPriorityClear", "Melee Minion priority", 3, 1, 5, 1);
					TSModes::Laneclear::rangedMinionPriority = ClearPrioritiesMenu->AddSlider("rangedMinionPriorityClear", "Ranged Minion priority", 2, 1, 5, 1);
					TSModes::Laneclear::siegeMinionPriority = ClearPrioritiesMenu->AddSlider("siegeMinionPriorityClear", "Siege Minion priority", 4, 1, 5, 1);
					TSModes::Laneclear::voidMinionPriority = ClearPrioritiesMenu->AddSlider("voidMinionPriorityClear", "Void Minion priority", 1, 1, 5, 1);
					TSModes::Laneclear::specialObjectsPriority = ClearPrioritiesMenu->AddSlider("specialObjectsPriorityClear", "Special Objects Minion priority", 5, 1, 5, 1);

					TSModes::Laneclear::attackSpecialObjects = ClearPrioritiesMenu->AddCheckBox("attackSpecialObjects", "Attack Special Objects", true);
					TSModes::Laneclear::focusHarassOverClear = ClearPrioritiesMenu->AddCheckBox("focusHarassOverClear", "Focus Harass over Clear", false);

				}

				Menu* LasthitPrioritiesMenu;
				void Configs::InitializeLastHitTargets()
				{
					LasthitPrioritiesMenu = TSMenu->AddMenu("lasthitPriorities", "Lasthit priorities");
					LasthitPrioritiesMenu->AddTooltip("Higher value means higher priority\nFor ignore put 0 as priority");

					TSModes::Lasthit::superMinionPriority = LasthitPrioritiesMenu->AddSlider("superMinionPriorityLastHit", "Super Minion priority", 5, 1, 5, 1);
					TSModes::Lasthit::meleeMinionPriority = LasthitPrioritiesMenu->AddSlider("meleeMinionPriorityLastHit", "Melee Minion priority", 2, 1, 5, 1);
					TSModes::Lasthit::rangedMinionPriority = LasthitPrioritiesMenu->AddSlider("rangedMinionPriorityLastHit", "Ranged Minion priority", 1, 1, 5, 1);
					TSModes::Lasthit::siegeMinionPriority = LasthitPrioritiesMenu->AddSlider("siegeMinionPriorityLastHit", "Siege Minion priority", 4, 1, 5, 1);
					TSModes::Lasthit::voidMinionPriority = LasthitPrioritiesMenu->AddSlider("voidMinionPriorityLastHit", "Void Minion priority", 1, 1, 5, 1);
					TSModes::Lasthit::specialObjectsPriority = LasthitPrioritiesMenu->AddSlider("specialObjectsPriorityLastHit", "Special Objects Minion priority", 3	, 1, 5, 1);

					TSModes::Lasthit::attackSpecialObjects = LasthitPrioritiesMenu->AddCheckBox("attackSpecialObjects", "Attack Special Objects", true);
				}

				Menu* UnderTowerPrioritiesMenu;
				void Configs::InitializeUnderTowerTargets()
				{
					UnderTowerPrioritiesMenu = TSMenu->AddMenu("underTowerPriorities", "Under Tower priorities");
					UnderTowerPrioritiesMenu->AddTooltip("Higher value means higher priority\nFor ignore put 0 as priority");

					TSModes::UnderTower::superMinionPriority = UnderTowerPrioritiesMenu->AddSlider("superMinionPriorityUnderTower", "Super Minion priority", 5, 1, 5, 1);
					TSModes::UnderTower::meleeMinionPriority = UnderTowerPrioritiesMenu->AddSlider("meleeMinionPriorityUnderTower", "Melee Minion priority", 2, 1, 5, 1);
					TSModes::UnderTower::rangedMinionPriority = UnderTowerPrioritiesMenu->AddSlider("rangedMinionPriorityUnderTower", "Ranged Minion priority", 1, 1, 5, 1);
					TSModes::UnderTower::siegeMinionPriority = UnderTowerPrioritiesMenu->AddSlider("siegeMinionPriorityUnderTower", "Siege Minion priority", 4, 1, 5, 1);
					TSModes::UnderTower::voidMinionPriority = UnderTowerPrioritiesMenu->AddSlider("voidMinionPriorityUnderTower", "Void Minion priority", 1, 1, 5, 1);
					TSModes::UnderTower::specialObjectsPriority = UnderTowerPrioritiesMenu->AddSlider("specialObjectsPriorityUnderTower", "Special Objects Minion priority", 2, 1, 5, 1);

					TSModes::UnderTower::attackSpecialObjects = UnderTowerPrioritiesMenu->AddCheckBox("attackSpecialObjects", "Attack Special Objects", true);
				}

				Menu* PrioritiesDrawMenu;
				void Configs::InitializeDrawSettings()
				{
					PrioritiesDrawMenu = TSMenu->AddMenu("drawingSettings", "Drawing settings");
					TSDrawings::drawCurrentTarget = PrioritiesDrawMenu->AddCheckBox("drawCurrentTarget", "Draw Current Target", true);
					TSDrawings::drawSelectedChampion = PrioritiesDrawMenu->AddCheckBox("drawSelectedChampion", "Draw Selected Target", true);
					TSDrawings::drawKillableMinion = PrioritiesDrawMenu->AddCheckBox("drawKillableMinion", "Draw Killable Minion", true);

					TSDrawings::drawWard = PrioritiesDrawMenu->AddCheckBox("drawWard", "Draw Wards priority",true);
					TSDrawings::drawSuperMinion = PrioritiesDrawMenu->AddCheckBox("drawSuperMinion", "Draw Super Minion priority", true);
					TSDrawings::drawMeleeMinion = PrioritiesDrawMenu->AddCheckBox("drawMeleeMinion", "Draw Melee Minion priority", true);
					TSDrawings::drawRangedMinion = PrioritiesDrawMenu->AddCheckBox("drawRangedMinion", "Draw Ranged Minion priority", true);
					TSDrawings::drawSiegeMinion = PrioritiesDrawMenu->AddCheckBox("drawSiegeMinion", "Draw Siege Minion priority", true);
					TSDrawings::drawVoidMinion = PrioritiesDrawMenu->AddCheckBox("drawVoidMinion", "Draw Void Minion priority", true);
					TSDrawings::drawSpecialObjects = PrioritiesDrawMenu->AddCheckBox("drawSpecialObjects", "Draw Special Objects priority", true);
				}

			}

			namespace Functions
			{
				std::vector<Object*> Functions::GetWardsInRange(Vector3 pos, float range)
				{
					std::vector<Object*> validWards; // Lista di oggetti validi

					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto obj = globals::minionManager->GetIndex(i);
						if (obj->IsAlive())
						{
							if (obj->GetName() == "") continue;
							if (!obj->IsWard()) continue;
							if (!obj->IsEnemy()) continue;

							if (!obj->IsInRange(pos, range)) continue;
							if (obj)
							{
								validWards.push_back(obj);
							}
						}
					}
					if (validWards.size() >= 1)
						return validWards; 
					else
						validWards.clear();

					return validWards;
				}

				Object* Functions::GetWard(std::vector<Object*> validWards, Vector3 pos)
				{
					switch (validWards.size())
					{
					case 0:
						return nullptr;
					case 1:
						return validWards[0];
					}

					Object* hero = nullptr;
					float highestDistance = 0.0f;
					for (auto ward : validWards)
					{
						auto distance = pos.Distance(ward->GetPosition());
						if (distance > highestDistance)
						{
							hero = ward;
							highestDistance = distance;
						}
					}
					return hero;
				}

				std::vector<Object*> Functions::GetMinionsInRange(Vector3 pos, float range)
				{
					std::vector<Object*> validMinions; // Lista di oggetti validi

					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto obj = globals::minionManager->GetIndex(i);
						if (obj->IsAlive() && obj->IsVisible())
						{
							if (obj->GetName() == "") continue;
							if (!obj->IsValidTarget()) continue;
							if (obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::Minion_Lane) continue;
							if (!obj->IsInRange(pos, range)) continue;
							if (obj)
							{
								validMinions.push_back(obj);
							}
						}
					}
					if (validMinions.size() >= 1)
						return validMinions;
					else
						validMinions.clear();

					return validMinions;
				}

				std::vector<Object*> Functions::GetKillableMinionsInRange(Vector3 pos, float range, float damage)
				{
					std::vector<Object*> validTargets;

					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto obj = globals::minionManager->GetIndex(i);
						if (!obj->IsValidTarget()) continue;
						if (obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::Minion_Lane) continue;
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range))continue;
						if (obj->ReadClientStat(Object::Health)  > damage) continue;
						if (obj)
							validTargets.push_back(obj);
					}

					if (validTargets.size() >= 1)
						return validTargets;
					else
						validTargets.clear();

					return validTargets;
				}

				int Functions::GetMinionPriority(Object* minion)
				{
					if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Clear)
					{
						if (minion->GetCharacterData()->GetObjectTypeHash() == ObjectType::Ward)
							return Configs::TSModes::Laneclear::wardPriority->Value;
						else if (minion->GetName().find("Melee") != std::string::npos)
							return Configs::TSModes::Laneclear::meleeMinionPriority->Value;
						else if (minion->GetName().find("Ranged") != std::string::npos)
							return Configs::TSModes::Laneclear::rangedMinionPriority->Value;
						else if (minion->GetName().find("Siege") != std::string::npos)
							return Configs::TSModes::Laneclear::siegeMinionPriority->Value;
						else if (minion->GetName().find("Super") != std::string::npos)
							return Configs::TSModes::Laneclear::superMinionPriority->Value;
						else if (minion->GetName().find("Void") != std::string::npos)
							return Configs::TSModes::Laneclear::voidMinionPriority->Value;
						return 1;
					}
					else if (globals::scripts::orbwalker::orbwalkState == OrbwalkState::Lasthit)
					{
						if (minion->GetName().find("Melee") != std::string::npos)
							return Configs::TSModes::Lasthit::meleeMinionPriority->Value;
						else if (minion->GetName().find("Ranged") != std::string::npos)
							return Configs::TSModes::Lasthit::rangedMinionPriority->Value;
						else if (minion->GetName().find("Siege") != std::string::npos)
							return Configs::TSModes::Lasthit::siegeMinionPriority->Value;
						else if (minion->GetName().find("Super") != std::string::npos)
							return Configs::TSModes::Lasthit::superMinionPriority->Value;
						else if (minion->GetName().find("Void") != std::string::npos)
							return Configs::TSModes::Lasthit::voidMinionPriority->Value;
					}

					return 1;
				}

				float Functions::GetMinionReducedPriority(Object* minion)
				{
					switch (GetMinionPriority(minion))
					{
					case 5:
						return 2.5f;
					case 4:
						return 2.0f;
					case 3:
						return 1.75f;
					case 2:
						return 1.5f;
					default:
						return 1.0f;
					}
				}

				Object* Functions::GetMinion(std::vector<Object*> minions, DamageType damageType)
				{
					switch (minions.size())
					{
					case 0:
						return nullptr;
					case 1:
						return minions[0];
					}

					Object* hero = nullptr;
					float highestPriority = 0.0f;
					for (auto minion : minions)
					{
						auto priority = GetMinionReducedPriority(minion) * (damageType == Magical ? Damage::CalculateMagicalDamage(globals::localPlayer, minion, 100.0f) : Damage::CalculatePhysicalDamage(globals::localPlayer, minion, 100.0f)) / minion->ReadClientStat(Object::Health);
						if (priority > highestPriority)
						{
							hero = minion;
							highestPriority = priority;
						}
					}
					return hero;
														
				}

				Object* Functions::GetEnemyMinionInRange(float radius)
				{
					return GetMinion(GetMinionsInRange(globals::localPlayer->GetPosition(), radius),
						(globals::localPlayer->ReadClientStat(Object::TotalAttackDamage) > globals::localPlayer->ReadClientStat(Object::AbilityPower)) ? Physical : Magical);
				}

					
				Object* Functions::GetKillableEnemyMinionInRange(float radius)
				{
					for (auto killableMinion : GetKillableMinionsInRange(globals::localPlayer->GetPosition(), radius, Physical))
						return killableMinion;

					return nullptr;
				}

				std::vector<Object*> Functions::GetJungleMonstersInRange(float range)
				{
					std::vector<Object*> validMonsters; // Lista di oggetti validi

					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto obj = globals::minionManager->GetIndex(i);

						if (!obj->IsAlive()) continue;
						if (obj->GetName() == "") continue;
						if (obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::Monster) continue;
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;
						if (obj)
						{
							validMonsters.push_back(obj);
						}
					}

					if (validMonsters.size() >= 1)
						return validMonsters;
					else
						validMonsters.clear();

					return validMonsters;
				}

				std::vector<Object*> Functions::GetJungleRespawnInRange(float range)
				{
					std::vector<Object*> validMonsters; // Lista di oggetti validi

					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto obj = globals::minionManager->GetIndex(i);
						if (obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::RespawnMarker) continue;
						if (obj->GetName() != "SRU_CampRespawnMarker") continue;
						if (!obj->IsAlive()) continue;
						
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;
						if (obj)
						{
							validMonsters.push_back(obj);
						}
					}

					if (validMonsters.size() >= 1)
						return validMonsters;
					else
						validMonsters.clear();

					return validMonsters;
				}

				std::vector<Object*> Functions::GetTargetsInRange(Vector3 pos, float range)
				{
					std::vector<Object*> validTargets;

					for (int i = 0; i < globals::heroManager->GetListSize(); i++)
					{
						auto obj = globals::heroManager->GetIndex(i);
						if (!obj->IsValidTarget()) continue;

						if (!obj->IsInRange(pos, range))continue;
						if (obj)
							validTargets.push_back(obj);
					}

					if (validTargets.size() >= 1)
						return validTargets;
					else
						validTargets.clear();

					return validTargets;
				}

				std::vector<Object*> Functions::GetKillableTargetsInRange(Vector3 pos, float range, float damage)
				{
					std::vector<Object*> validTargets;

					for (int i = 0; i < globals::heroManager->GetListSize(); i++)
					{
						auto obj = globals::heroManager->GetIndex(i);
						if (!obj->IsValidTarget()) continue;

						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range))continue;
						if (obj->ReadClientStat(Object::Health) > damage) continue;
						if (obj)
							validTargets.push_back(obj);
					}

					if (validTargets.size() >= 1)
						return validTargets;
					else
						validTargets.clear();

					return validTargets;
				}

				int Functions::GetPriority(Object* hero)
				{
					if (hero->IsAlly())
					{
						auto iterator = Configs::PrioritiesMap.find(hero->GetName());
						if (iterator != Configs::PrioritiesMap.end())
						{
							return iterator->second;
						}
					}

					for (auto enemy : Configs::Enemies)
					{
						if (enemy == hero)
						{
							return (*Configs::TargetPrioritiesMenu)[enemy->GetName()]->Cast<Slider*>()->Value;
						}
					}

					return 1;
				}

				float Functions::GetReducedPriority(Object* hero)
				{
					switch (GetPriority(hero))
					{
					case 5:
						return 2.5f;
					case 4:
						return 2.0f;
					case 3:
						return 1.75f;
					case 2:
						return 1.5f;
					default:
						return 1.0f;
					}
				}

				bool Functions::ChooseSelectedObject(Object* selectedObject, Object* checkObj)
				{
					return selectedObject == checkObj && Configs::TSModes::Combo::Advanced::attackSelectedTarget->Value == true;
				}
				
				Object* SelectedTarget = nullptr;

				// ReSharper disable once CppNotAllPathsReturnValue
				Object* Functions::GetTarget(std::vector<Object*> targets, DamageType damageType)
				{
					if (Configs::TSModes::Combo::Advanced::attackOnlySelectedTarget->Value
						&& Configs::TSModes::Combo::Advanced::attackSelectedTarget->Value)
					{
						Object* selectedObject = Engine::GetSelectedObject();
						if (selectedObject && selectedObject->IsValidTarget())
							return selectedObject;
					}

					// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
					switch (targets.size())
					{
					case 0:
						return nullptr;
					case 1:
						return targets[0];
					}

					switch (Configs::ActiveMode)
					{
					case Configs::TargetSelectorMode::Auto:
					{
						Object* hero = nullptr;
						float highestPriority = 0.0f;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto priority = GetReducedPriority(target) * (damageType == Magical ? Damage::CalculateMagicalDamage(globals::localPlayer, target, 100.0f) : Damage::CalculatePhysicalDamage(globals::localPlayer, target, 100.0f)) / target->ReadClientStat(Object::Health);
							if (priority > highestPriority)
							{
								hero = target;
								highestPriority = priority;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::LeastHealth:
					{
						Object* hero = nullptr;
						float leastHealth = FLT_MAX;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto health = target->ReadClientStat(Object::MaxHealth);
							if (health < leastHealth)
							{
								hero = target;
								leastHealth = health;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::MostAttackDamage:
					{
						Object* hero = nullptr;
						float mostAttackDamage = 0.0f;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto attackDamage = target->ReadClientStat(Object::TotalAttackDamage);
							if (attackDamage > mostAttackDamage)
							{
								hero = target;
								mostAttackDamage = attackDamage;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::MostAbilityPower:
					{
						Object* hero = nullptr;
						float mostAbilityPower = 0.0f;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto abilityPower = target->ReadClientStat(Object::AbilityPower);
							if (abilityPower > mostAbilityPower)
							{
								hero = target;
								mostAbilityPower = abilityPower;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::Closest:
					{
						Object* hero = nullptr;
						float closest = FLT_MAX;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto distance = globals::localPlayer->GetDistanceTo(target);
							if (distance < closest)
							{
								hero = target;
								closest = distance;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::HighestPriority:
					{
						Object* hero = nullptr;
						int highestPriority = 0;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto priority = GetPriority(target);
							if (priority > highestPriority) {
								hero = target;
								highestPriority = priority;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::LessAttack:
					{
						Object* hero = nullptr;
						float highestPriority = 0.0f;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto priority = GetReducedPriority(target) * Damage::CalculatePhysicalDamage(globals::localPlayer, target, 100.0f) / target->ReadClientStat(Object::MaxHealth);
							if (priority > highestPriority)
							{
								hero = target;
								highestPriority = priority;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::LessCast:
					{
						Object* hero = nullptr;
						float highestPriority = 0.0f;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto priority = GetReducedPriority(target) * Damage::CalculateMagicalDamage(globals::localPlayer, target, 100.0f) / target->ReadClientStat(Object::MaxHealth);
							if (priority > highestPriority) {
								hero = target;
								highestPriority = priority;
							}
						}
						return hero;
					}

					case Configs::TargetSelectorMode::NearMouse:
					{
						Object* hero = nullptr;
						float closest = FLT_MAX;
						for (auto target : targets)
						{
							if (Configs::TSModes::Combo::Advanced::avoidAttackInvulnerable->Value == true && target->IsInvulnerable())
								continue;

							auto distance = Engine::GetMouseWorldPos().Distance(target->GetPosition());
							if (distance < closest)
							{
								hero = target;
								closest = distance;
							}
						}
						return hero;
					}

					}
				}
				
				Object* Functions::GetEnemyChampionInRange(float range, int damageType, Skillshot skillshot)
				{
					Object* selectedObject = Engine::GetSelectedObject();
					Object* best = nullptr;
					for (Object* obj : *globals::heroManager)
					{
						if (!obj->IsValidTarget()) continue;

						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						Modules::prediction::PredictionOutput prediction;
						if (skillshot.GetType() != SkillshotNone && !GetPrediction(globals::localPlayer, obj, skillshot, prediction)) continue;

						if (ChooseSelectedObject(selectedObject, obj)) return obj;

						if (!best) {
							best = obj;
							continue;
						}

						if (best->GetEffectiveHealth(damageType) > obj->GetEffectiveHealth(damageType)) {
							best = obj;
							continue;
						}
					}

					return best;
				}

				Object* Functions::GetEnemyChampionInRange(float radius)
				{
					return GetTarget(GetTargetsInRange(globals::localPlayer->GetPosition(), radius),
						(globals::localPlayer->ReadClientStat(Object::TotalAttackDamage) > globals::localPlayer->ReadClientStat(Object::AbilityPower)) ? Physical : Magical);
				}

				Object* Functions::GetEnemyChampionInRange(Vector3 pos, float radius)
				{
					return GetTarget(GetTargetsInRange(pos, radius),
						(globals::localPlayer->ReadClientStat(Object::TotalAttackDamage) > globals::localPlayer->ReadClientStat(Object::AbilityPower)) ? Physical : Magical);
				}

				Object* Functions::GetEnemyChampionInRange(float radius, Skillshot skillshot)
				{
					return  GetEnemyChampionInRange(radius,
						(globals::localPlayer->ReadClientStat(Object::TotalAttackDamage) > globals::localPlayer->ReadClientStat(Object::AbilityPower)) ? Physical : Magical, skillshot);
				}

				

				Object* Functions::GetEnemyNexusInRange(float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::attackableManager)
					{
						if (!obj->IsValidTarget()) continue;
						if (!Engine::GetCollisionFlags(obj->GetPosition()) & CollisionFlags::Building) continue;
						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;
						if (obj->ReadClientStat(Object::MaxHealth) != 5500) continue;
						
						if (obj)
						{
								best = obj;
								break;
						}
					}

					return best;
				}

				Object* Functions::GetEnemyInhibitorInRange(float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::inhibitorsManager)
					{
						if (!obj->IsValidTarget()) continue;
						if (!Engine::GetCollisionFlags(obj->GetPosition()) & CollisionFlags::Building) continue;
						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;

						if (obj)
						{
							best = obj;
							break;
						}
					}

					return best;
				}

				Object* Functions::GetTurretInRange(TeamType team, float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::turretManager)
					{
						if (!obj->IsVisible()) continue;
						if (!obj->IsAlive()) continue;
						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;

						if (team == TeamType::Ally && obj->IsAlly()) {}
						if (team == TeamType::Enemy && obj->IsEnemy()){}

						if (!obj->IsValidTarget()) continue;

						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;

						if (obj)
						{
							best = obj;
							break;
						}
					}

					return best;
				}

				Object* Functions::GetEnemyTurretInRange(float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::turretManager)
					{
						if (!obj->IsValidTarget()) continue;

						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;

						if (obj)
						{
							best = obj;
							break;
						}
					}

					return best;
				}

				Object* Functions::GetAllyTurretInRange(float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::turretManager)
					{
						if (!obj->IsVisible()) continue;
						if (!obj->IsAlive()) continue;
						if (!obj->IsAlly()) continue;
						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;


						if (obj)
						{
							best = obj;
							break;
						}
					}

					return best;
				}

				Object* Functions::GetMinionInRange(float range)
				{
					Object* selectedObject = Engine::GetSelectedObject();
					Object* best = nullptr;
					for (Object* obj : *globals::minionManager)
					{
						if (!obj->IsValidTarget()) continue;

						if (obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::Minion_Lane) continue;

						if (obj->GetDistanceTo(globals::localPlayer) > range) continue;

						if (ChooseSelectedObject(selectedObject, obj)) return obj;

						if (!best)
						{
							best = obj;
							continue;
						}

						if (best->GetDistanceTo(globals::localPlayer) > best->GetDistanceTo(globals::localPlayer))
						{
							best = obj;
							continue;
						}
					}

					return best;
				}

				/*Object* Functions::GetMinionDaFinire(std::vector<Object*> targets, DamageType damageType)
				{
					if (Configs::TSModes::Combo::Advanced::attackOnlySelectedTarget->Value
						&& Configs::TSModes::Combo::Advanced::attackSelectedTarget->Value)
					{
						Object* selectedObject = Engine::GetSelectedObject();
						if (selectedObject && selectedObject->IsValidTarget())
							return selectedObject;
					}

					// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
					switch (targets.size())
					{
					case 0:
						return nullptr;
					case 1:
						return targets[0];
					}

					switch (Configs::ActiveMode)
					{
					case Configs::TargetSelectorMode::Auto:
					{
						Object* hero = nullptr;
						float highestPriority = 0.0f;
						for (auto target : targets)
						{
							auto priority = GetReducedPriority(target) * (damageType == Magical ? Damage::CalculateMagicalDamage(globals::localPlayer, target, 100.0f) : Damage::CalculatePhysicalDamage(globals::localPlayer, target, 100.0f)) / target->GetMaxHealth();
							if (priority > highestPriority)
							{
								hero = target;
								highestPriority = priority;
							}
						}
						return hero;
					}
					}
				}*/

				Object* Functions::GetJungleInRange(float range)
				{
					Object* selectedObject = Engine::GetSelectedObject();
					Object* best = nullptr;
					for (Object* obj : *globals::minionManager)
					{
						if (obj->GetName() == "") continue;
						if (!obj->IsValidTarget()) continue;
						if (!obj->IsEnemy()) continue;
						if (!obj->IsJungle()) continue;
						if (obj->GetCharacterData()->GetObjectTypeHash() != ObjectType::Monster) continue;
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						if (ChooseSelectedObject(selectedObject, obj)) return obj;

						if (!best)
						{
							best = obj;
							continue;
						}

						if (best->GetDistanceTo(globals::localPlayer) > best->GetDistanceTo(globals::localPlayer))
						{
							best = obj;
							continue;
						}
					}

					return best;
				}


				Object* Functions::GetWardInRange(float range)
				{
					Object* selectedObject = Engine::GetSelectedObject();
					Object* best = nullptr;
					for (Object* obj : *globals::minionManager)
					{
						if (obj->GetName() == "") continue;
						if (!obj->IsValidTarget()) continue;
						if (!obj->IsWard()) continue;
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						if (ChooseSelectedObject(selectedObject, obj)) return obj;

						if (!best)
						{
							best = obj;
							continue;
						}

						if (best->GetDistanceTo(globals::localPlayer) > best->GetDistanceTo(globals::localPlayer))
						{
							best = obj;
							continue;
						}
					}

					return best;
				}

				Object* Functions::GetEnemyObjectInRange(float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::objManager)
					{
						if (!obj->IsValidTarget()) continue;

						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						if (!best)
						{
							best = obj;
							continue;
						}
					}

					return best;
				}

				std::vector<Object*> Functions::GetObjectsInRange(Vector3 pos, std::string name, float range)
				{
					std::vector<Object*> validMinions;

					for (int i = 0; i < globals::minionManager->GetListSize(); i++)
					{
						auto obj = globals::minionManager->GetIndex(i);
						if (obj->IsAlive() && obj->IsVisible())
						{
							if (obj->GetName() == "") continue;
							if (!obj->IsInRange(pos, range)) continue;
							if (obj->GetName() != name) continue;

							if (obj)
							{
								validMinions.push_back(obj);
							}
						}
					}
					if (validMinions.size() >= 1)
						return validMinions;
					else
						validMinions.clear();

					return validMinions;
				}

				Object* Functions::GetObjectInRange(std::string name, float range)
				{
					Object* best = nullptr;

					for (Object* obj : *globals::minionManager)
					{
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						if (obj->GetName() != name) continue;

						if (!best)
						{
							best = obj;
							continue;
						}
					}

					return best;
				}

				Object* Functions::GetObjectInRange(float range, std::string name, std::vector<QWORD> includeFilterTypeHashes, std::vector<QWORD> excludeFilterTypeHashesDetailed, bool isSpecial)
				{
					Object* selectedObject = Engine::GetSelectedObject();
					Object* best = nullptr;
					for (Object* obj : *globals::minionManager)
					{
						if (!(isSpecial || obj->IsValidTarget())) continue;

						bool excludeHashMatched = false;
						for (auto hash : excludeFilterTypeHashesDetailed)
						{
							if (obj->GetCharacterData()->GetObjectTypeHashDetailed() == hash)
							{
								excludeHashMatched = true;
								break;
							}
						}
						if (excludeHashMatched) continue;

						bool includeHashMatched = false;
						for (auto hash : includeFilterTypeHashes)
						{
							if (obj->GetCharacterData()->GetObjectTypeHash() == hash)
							{
								includeHashMatched = true;
								break;
							}
						}
						if (includeFilterTypeHashes.size() && !includeHashMatched) continue;

						if (name != "" && obj->GetName() != name) continue;
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						if (ChooseSelectedObject(selectedObject, obj)) return obj;

						if (!best) {
							best = obj;
							continue;
						}

						if (best->GetDistanceTo(globals::localPlayer) > best->GetDistanceTo(globals::localPlayer)) {
							best = obj;
							continue;
						}
					}

					return best;
				}

				Object* Functions::GetAllyChampionInRange(float range)
				{
					Object* best = nullptr;
					for (Object* obj : *globals::heroManager)
					{
						if (!obj->IsAlly()) continue;

						if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

						if (!best) {
							best = obj;
							continue;
						}
					}

					return best;
				}
			}

			namespace Drawings
			{
				void DrawSelectedTarget()
				{
					if (Configs::TSDrawings::drawCurrentTarget->Value == true)
					{
						Object* selectedObject = Engine::GetSelectedObject();
						if (selectedObject && selectedObject->IsValidTarget())
							render::RenderArcWorld(selectedObject->GetPosition(), 10, selectedObject->ReadClientStat(Object::AttackRange), COLOR_BLUE, 1.0f, PI, globals::localPlayer->GetPosition(), true);
					}
					
				}

				void DrawMinionsPriority()
				{
					for (Object* obj : *globals::minionManager)
					{
						if (!obj->IsValidTarget()) continue;
						if (!obj->IsMinion()) continue;
						if (!obj->IsInRange(globals::localPlayer->GetPosition(), 2000)) continue;
						if (obj)
						{
							Vector2 screenPos = Engine::WorldToScreen(obj->GetPosition());
							if (!render::IsOnScreen(screenPos)) return;

							render::RenderText(std::to_string(Functions::GetMinionPriority(obj)), (screenPos - Vector2(0.0f, 22.0f)).ToImVec(), 18.0f, COLOR_WHITE, true);
						}
					}
						
				}
			}
		}
	}
}