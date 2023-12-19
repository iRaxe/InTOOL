#include "../stdafx.h"
#include "../ListManager.h"

#include <unordered_set>

namespace UPasta
{
	namespace SDK
	{
		namespace ListManager
		{
			namespace Functions
			{
				void PopulateSpellsMap()
				{
					for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); i++)
					{
						const auto obj = ObjectManager::GetHeroList()->GetIndex(i);
						if (obj != nullptr && obj->GetName() != "PracticeTool_TargetDummy")
						{
							if (obj->IsAlly()) continue;

							for (int slot = 0; slot < 6; ++slot)
							{
								std::vector<int> cooldownsArray;
								constexpr int numCooldowns = 5;

								for (int cooldownIndex = 1; cooldownIndex <= numCooldowns; ++cooldownIndex)
								{
									int cooldownValue = ceil(obj->GetSpellBySlotId(slot)->GetSpellInfo()->GetSpellData()->GetCooldownArray()->GetArrayIndex(cooldownIndex)->GetBaseCooldown());
									cooldownsArray.push_back(cooldownValue);
								}

								playerSpells[obj->GetName()][slot][obj->GetSpellBySlotId(slot)->GetName().c_str()] = cooldownsArray;
							}
						}
					}
				}

				void InsertCooldown(Object* champion, int spellSlot, float readyAt) {
					ChampionCooldownsMap.insert(std::make_pair(std::make_pair(champion, spellSlot), readyAt));
				}

				float GetCooldown(Object* champion, int spellSlot) {
					auto it = ChampionCooldownsMap.find(std::make_pair(champion, spellSlot));
					if (it != ChampionCooldownsMap.end()) {
						if (it->second < Engine::GetGameTime()) {
							ChampionCooldownsMap.erase(it);
							return 0.0f;
						}
						return it->second - Engine::GetGameTime();
					}
					return 0.0f;
				}

				void PopulatePlayersMap()
				{
					for (int i = 0; i < ObjectManager::GetHeroList()->GetListSize(); ++i)
					{
						auto obj = ObjectManager::GetHeroList()->GetIndex(i);

						if (IsValidPtr(obj) && obj && !playersMap.contains(obj))
							playersMap[obj] = i;
					}

					if (playersMap.size() > ObjectManager::GetHeroList()->GetListSize())
						playersMap.clear();
				}

				void PopulatePlayersMaps()
				{
					for (auto it = playersMap.begin(); it != playersMap.end(); ++it)
					{
						Object* correspondingObject = it->first;
						if (!correspondingObject)
							continue;

						const int correspondingIndex = it->second;

						if (IsValidPtr(correspondingObject) && correspondingObject)
						{
							if (correspondingObject->IsEnemy() && !enemiesMap.contains(correspondingObject))
								enemiesMap[correspondingObject] = correspondingIndex;

							if (correspondingObject->IsAlly() && !alliesMap.contains(correspondingObject))
								alliesMap[correspondingObject] = correspondingIndex;
							
							playersMap.erase(correspondingObject);
						}
					}
					
				}

				void PopulateMinionsMap()
				{
					for (int i = 0; i < ObjectManager::GetMinionList()->GetListSize(); ++i)
					{
						auto obj = ObjectManager::GetMinionList()->GetIndex(i);

						if (IsValidPtr(obj) && obj && !playersMap.contains(obj))
							minionsMap[obj] = i;
					}

					if (minionsMap.size() > ObjectManager::GetMinionList()->GetListSize())
						minionsMap.clear();
				}

				void PopulateMinionsMaps()
				{
					for (auto it = minionsMap.begin(); it != minionsMap.end(); ++it)
					{
						Object* correspondingObject = it->first;
						if (!correspondingObject)
							continue;

						const int correspondingIndex = it->second;

						if (IsValidPtr(correspondingObject) && correspondingObject)
						{
							if (correspondingObject->IsEnemy() && !enemiesMinionsMap.contains(correspondingObject))
								enemiesMinionsMap[correspondingObject] = correspondingIndex;

							if (correspondingObject->IsAlly() && !alliesMinionsMap.contains(correspondingObject))
								alliesMinionsMap[correspondingObject] = correspondingIndex;

							if (correspondingObject->IsJungle() && !jungleMonstersMap.contains(correspondingObject))
								jungleMonstersMap[correspondingObject] = correspondingIndex;

							if (correspondingObject->IsRespawnMarker() && !respawnMarkersMap.contains(correspondingObject))
								respawnMarkersMap[correspondingObject] = correspondingIndex;

							minionsMap.erase(correspondingObject);
						}
					}
				}

				void PopulateTurretsMap()
				{
					for (int i = 0; i < ObjectManager::GetTurretsList()->GetListSize(); ++i)
					{
						auto obj = ObjectManager::GetTurretsList()->GetIndex(i);

						if (IsValidPtr(obj) && obj && !turretsMap.contains(obj))
							turretsMap[obj] = i;
					}

					if (turretsMap.size() > ObjectManager::GetTurretsList()->GetListSize())
						turretsMap.clear();
				}

				void PopulateTurretsMaps()
				{
					for (auto it = turretsMap.begin(); it != turretsMap.end(); ++it)
					{
						Object* correspondingObject = it->first;
						if (!correspondingObject)
							continue;

						const int correspondingIndex = it->second;

						if (IsValidPtr(correspondingObject) && correspondingObject)
						{
							if (correspondingObject->IsEnemy() && !enemiesMap.contains(correspondingObject))
								enemiesTurretsMap[correspondingObject] = correspondingIndex;

							if (correspondingObject->IsAlly() && !alliesMap.contains(correspondingObject))
								alliesTurretsMap[correspondingObject] = correspondingIndex;

							turretsMap.erase(correspondingObject);
						}
					}
				}

				void PopulateInhibitorsMap()
				{
					for (int i = 0; i < ObjectManager::GetInhibitorsList()->GetListSize(); ++i)
					{
						auto obj = ObjectManager::GetInhibitorsList()->GetIndex(i);

						if (IsValidPtr(obj) && obj && !inhibitorsMap.contains(obj))
							inhibitorsMap[obj] = i;
					}

					if (inhibitorsMap.size() > ObjectManager::GetInhibitorsList()->GetListSize())
						inhibitorsMap.clear();
				}

				void PopulateInhibitorsMaps()
				{
					for (auto it = inhibitorsMap.begin(); it != inhibitorsMap.end(); ++it)
					{
						Object* correspondingObject = it->first;
						if (!correspondingObject)
							continue;

						const int correspondingIndex = it->second;

						if (IsValidPtr(correspondingObject) && correspondingObject)
						{
							if (correspondingObject->IsEnemy() && !enemiesMap.contains(correspondingObject))
								enemiesInhibitorsMap[correspondingObject] = correspondingIndex;

							if (correspondingObject->IsAlly() && !alliesMap.contains(correspondingObject))
								alliesInhibitorsMap[correspondingObject] = correspondingIndex;

							inhibitorsMap.erase(correspondingObject);
						}
					}
				}
				void Test()
				{
					LOG("PLAYERS MAP SIZE: %d", playersMap.size());
					LOG("MINIONS MAP SIZE: %d", minionsMap.size());
					LOG("TURRETS MAP SIZE: %d", turretsMap.size());
					LOG("INHIBITORS MAP SIZE: %d", inhibitorsMap.size());
					//LOG("MISSILES MAP SIZE: %d", missileMap.size());
				}

				void Initialize()
				{
					__try { PopulatePlayersMap(); }
					__except (1) { LOG("[MAP] Error in initializing players map"); }
					__try { PopulatePlayersMaps(); }
					__except (1) { LOG("[MAPS] Error in initializing players maps"); }

					__try { PopulateMinionsMap(); }
					__except (1) { LOG("[MAP] Error in initializing minions map"); }
					__try { PopulateMinionsMaps(); }
					__except (1) { LOG("[MAPS] Error in initializing minions maps"); }

					__try { PopulateTurretsMap(); }
					__except (1) { LOG("[MAP] Error in initializing turrets map"); }
					__try { PopulateTurretsMaps(); }
					__except (1) { LOG("[MAPS] Error in initializing turrets maps"); }

					__try { PopulateInhibitorsMap(); }
					__except (1) { LOG("[MAP] Error in initializing inhibitors map"); }
					__try { PopulateInhibitorsMaps(); }
					__except (1) { LOG("[MAPS] Error in initializing inhibitors maps"); }

					__try { PopulateSpellsMap(); }
					__except (1) { LOG("[MAP] Error in initializing spells map"); }
					//Test();
				}

			

				void Refresh()
				{
					__try { if (playersMap.size() < ObjectManager::GetHeroList()->GetListSize())	PopulatePlayersMaps(); }
					__except (1) { LOG("[MAPS] Error in refreshing players maps"); }

					__try { if (minionsMap.size() < ObjectManager::GetHeroList()->GetListSize())	PopulateMinionsMaps(); }
					__except (1) { LOG("[MAPS] Error in refreshing minions maps"); }

					__try { if (turretsMap.size() < ObjectManager::GetHeroList()->GetListSize())	PopulateTurretsMaps(); }
					__except (1) { LOG("[MAPS] Error in refreshing turrets maps"); }

					__try { if (inhibitorsMap.size() < ObjectManager::GetHeroList()->GetListSize())	PopulateInhibitorsMaps(); }
					__except (1) { LOG("[MAPS] Error in refreshing inhibitors maps"); }

					/*__try { if (missileMap.size() < ObjectManager::GetHeroList()->GetListSize())	PopulateMissilesMap(); }
					__except (1) { LOG("[MAPS] Error in refreshing missiles map"); }*/
				}

				
			}
		}
	}
}
