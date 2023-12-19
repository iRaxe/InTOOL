#include "ObjManager.h"

Object* ObjectManager::GetIndex(int index)
{
	return *(Object**)(*(QWORD*)((QWORD)this + UPasta::Offsets::Instance::Lists::ManagerList) + (sizeof(QWORD) * index));
}

Object* ObjectManager::GetLocalPlayer()
{
	return *(Object**)(RVA(UPasta::Offsets::Instance::LocalPlayer));
}

ObjectManager* ObjectManager::GetHeroList() {
	return *(ObjectManager**)(RVA(UPasta::Offsets::Instance::Lists::HeroList));
}

ObjectManager* ObjectManager::GetMinionList()
{
	return *(ObjectManager**)(RVA(UPasta::Offsets::Instance::Lists::MinionList));
}

ObjectManager* ObjectManager::GetTurretsList()
{
	return *(ObjectManager**)(RVA(UPasta::Offsets::Instance::Lists::TurretList));
}

ObjectManager* ObjectManager::GetInhibitorsList()
{
	return *(ObjectManager**)(RVA(UPasta::Offsets::Instance::Lists::InhibList));
}

ObjectManager* ObjectManager::GetAttackableList()
{
	return *(ObjectManager**)(RVA(UPasta::Offsets::Instance::Lists::AttackableList));
}

ObjectManager* ObjectManager::GetMissileList()
{
	return *(ObjectManager**)(RVA(UPasta::Offsets::Instance::Lists::MissileList));
}


QWORD ObjectManager::GetFirst(const QWORD& objectManager)
{
	static QWORD v1 = *reinterpret_cast<QWORD*>(objectManager + 0x18);
	static QWORD v2 = *reinterpret_cast<QWORD*>(objectManager + 0x20);
	if (v1 == v2)
		return 0;
	while (*reinterpret_cast<BYTE*>(v1) & 1 || !*reinterpret_cast<QWORD*>(v1))
	{
		v1 += 8;
		if (v1 == v2)
			return 0;
	}
	return *reinterpret_cast<QWORD*>(v1);
}

QWORD ObjectManager::GetNext(const QWORD& objectManager, const QWORD& obj)
{
	unsigned int v3; // edx
	QWORD v5; // eax

	static QWORD v2 = *(QWORD*)(objectManager + 0x18);
	v3 = *(unsigned __int16*)(obj + 16);
	static unsigned __int64 v4 = *(QWORD*)(objectManager + 0x20) - v2 >> 3;
	while (++v3 < v4)
	{
		if ((*(BYTE*)(v2 + 8i64 * v3) & 1) == 0 && *(QWORD*)(v2 + 8i64 * v3))
			return *(QWORD*)(v2 + 8i64 * v3);
	}
	return 0i64;
}

void ObjectManager::HandleObject(Object* obj)
{
	switch (obj->GetType())
	{
	case Object::AIHeroClient:
		_hero_list.emplace_back(obj);
		_client_map.insert({ obj->GetHandle(), obj });
		break;

	case Object::AIMinionClient:
		_minion_list.emplace_back(obj);
		_client_map.insert({ obj->GetHandle(), obj });
		break;

	case Object::AITurretClient:
		_turret_list.emplace_back(obj);
		break;

	case Object::obj_GeneralParticleEmitter:
		_particle_list.emplace_back(obj);
		break;

	case Object::BarracksDampener:
		_inhibitor_list.emplace_back(obj);
		break;

	case Object::HQ:
		_nexus_list.emplace_back(obj);
		break;

	default: break;
	}
}

void ObjectManager::Flush()
{
	_hero_list.clear();
	_minion_list.clear();
	_turret_list.clear();
	_inhibitor_list.clear();
	_nexus_list.clear();
	_particle_list.clear();
	_client_map.clear();
}

void ObjectManager::Update()
{
	Flush();

	static const QWORD obj_manager = *(QWORD*)(RVA(UPasta::Offsets::Instance::Lists::ObjManager));
	QWORD obj = GetFirst(obj_manager);
	while (obj)
	{
		const std::array<uint8_t, 8> vfunc_bytes = *reinterpret_cast<std::array<std::uint8_t, 8>*>((
			ReadVTable(obj, 1)));
		if (vfunc_bytes.at(0) != 0x48 && vfunc_bytes.at(7) != 0xC3)
		{
		}
		else
		{
			HandleObject((Object*)obj);
		}
		obj = GetNext(obj_manager, obj);
	}
}

Object* ObjectManager::GetClientByHandle(DWORD handle)
{
	auto it = _client_map.find(handle);
	if (it != _client_map.end())
		return it->second;
	return nullptr;
}


std::vector<Object*> ObjectManager::GetHeroesAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto hero : GetHeroes())
	{
		if (!hero) continue;
		if (team == Alliance::Ally && !hero->IsAlly() || team == Alliance::Enemy && !hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable())
			possible_targets.push_back(hero);
	}

	return possible_targets;
}

Object* ObjectManager::GetHeroAs(Alliance team, Vector3 position, float range)
{
	Object* heroToReturn = nullptr;
	for (auto hero : GetHeroesAs(team))
	{
		if (!hero) continue;
		if (team == Alliance::Ally && !hero->IsAlly() || team == Alliance::Enemy && !hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->
			GetPosition().distanceTo(position) < range)
		{
			if (!heroToReturn)
			{
				heroToReturn = hero;
				break;
			}
		}
	}

	return heroToReturn;
}

int ObjectManager::CountHeroesInRange(Alliance team, Vector3 position, float range)
{
	int heroesInRange = 0;
	for (auto hero : GetHeroesAs(team))
	{
		if (!hero) continue;
		if (team == Alliance::Ally && !hero->IsAlly() || team == Alliance::Enemy && !hero->IsEnemy()) continue;
		if (hero->GetPosition().distanceTo(position) > range) continue;
		heroesInRange++;
	}
	return heroesInRange;
}

Object* ObjectManager::GetObjectInRange(std::string name, float range)
{
	Object* best = nullptr;

	for (auto objToFind : GetMinions())
	{
		if (!objToFind) continue;
		if (!objToFind->GetDistanceTo(ObjectManager::GetLocalPlayer()) > range) continue;

		if (objToFind->GetName() != name) continue;

		if (!best)
		{
			best = objToFind;
		}
	}

	return best;
}

Object* ObjectManager::GetObjectInRange(float range, std::string name, std::vector<QWORD> includeFilterTypeHashes,
	std::vector<QWORD> excludeFilterTypeHashesDetailed, bool isSpecial)
{
	Object* best = nullptr;
	for (auto obj : GetMinions())
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
		if (!obj->GetDistanceTo(GetLocalPlayer()) > range) continue;

		if (!best)
		{
			best = obj;
			continue;
		}

		if (best->GetDistanceTo(GetLocalPlayer()) > best->GetDistanceTo(GetLocalPlayer()))
		{
			best = obj;
		}
	}

	return best;
}

std::vector<Object*> ObjectManager::GetMinionsAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto minion : GetMinions())
	{
		if (!minion) continue;
		if (team == Alliance::Ally && !minion->IsAlly() || team == Alliance::Enemy && !minion->IsEnemy()) continue;

		if (minion->IsAlive() and minion->IsVisible() and minion->IsTargetable() and !minion->IsInvulnerable())
			possible_targets.push_back(minion);
	}

	return possible_targets;
}

int ObjectManager::CountMinionsInRange(Alliance team, Vector3 position, float range)
{
	int minionsInRange = 0;
	for (auto minion : GetMinionsAs(team))
	{
		if (!minion) continue;
		if (minion->IsJungle()) continue;
		if (minion->GetPosition().distanceTo(position) > range) continue;
		minionsInRange++;
	}
	return minionsInRange;
}

std::vector<Object*> ObjectManager::GetJungleMonsters()
{
	std::vector<Object*> possible_targets;
	for (auto monster : GetMinions())
	{
		if (!monster) continue;
		if (!monster->IsJungle()) continue;
		if (monster->GetCharacterData()->GetObjectTypeHash() == Plants) continue;
		if (monster->IsAlive() and monster->IsVisible() and monster->IsTargetable() and !monster->IsInvulnerable())
			possible_targets.push_back(monster);
	}
	return possible_targets;
}

int ObjectManager::CountJungleMonstersInRange(Vector3 position, float range)
{
	int jungleMonstersInRange = 0;
	for (auto jungle : GetJungleMonsters())
	{
		if (!jungle) continue;
		if (jungle->GetPosition().distanceTo(position) > range) continue;
		jungleMonstersInRange++;
	}
	return jungleMonstersInRange;
}

std::vector<Object*> ObjectManager::GetJunglePlants()
{
	std::vector<Object*> possible_targets;
	for (auto plant : GetMinions())
	{
		if (!plant) continue;
		if (plant->GetCharacterData()->GetObjectTypeHash() != Plants) continue;
		if (plant->IsAlive() and plant->IsVisible() and plant->IsTargetable() and !plant->IsInvulnerable())
			possible_targets.push_back(plant);
	}
	return possible_targets;
}

std::vector<Object*> ObjectManager::GetJungleRespawnCamps()
{
	std::vector<Object*> possible_targets;
	for (auto campRespawn : GetMinions())
	{
		if (!campRespawn) continue;
		if (campRespawn->GetCharacterData()->GetObjectTypeHash() != RespawnMarker) continue;
		possible_targets.push_back(campRespawn);
	}
	return possible_targets;
}

std::vector<Object*> ObjectManager::GetWards(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto ward : GetMinions())
	{
		if (!ward) continue;
		if (team == Alliance::Ally && !ward->IsAlly() || team == Alliance::Enemy && !ward->IsEnemy()) continue;

		if (!ward->IsWard()) continue;
		if (ward->IsAlive())
			possible_targets.push_back(ward);
	}
	return possible_targets;
}

std::vector<Object*> ObjectManager::GetTurretsAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto turret : GetTurrets())
	{
		if (!turret) continue;
		if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) continue;
		if (turret->IsAlive() and turret->IsVisible() and turret->IsTargetable() and !turret->IsInvulnerable())
			possible_targets.push_back(turret);
	}

	return possible_targets;
}

int ObjectManager::CountTurretsInRange(Alliance team, Vector3 position, float range)
{
	int turretsInRange = 0;
	for (auto turret : GetMinionsAs(team))
	{
		if (!turret) continue;
		if (turret->GetPosition().distanceTo(position) > range) continue;
		turretsInRange++;
	}
	return turretsInRange;
}

std::vector<Object*> ObjectManager::GetInhibitorsAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto inhibitor : GetInhibitors())
	{
		if (!inhibitor) continue;
		if (team == Alliance::Ally && !inhibitor->IsAlly() || team == Alliance::Enemy && !inhibitor->IsEnemy()) continue
			;
		if (inhibitor->IsAlive() and inhibitor->IsVisible() and inhibitor->IsTargetable() and !inhibitor->
			IsInvulnerable())
			possible_targets.push_back(inhibitor);
	}

	return possible_targets;
}

int ObjectManager::CountInhibitorsInRange(Alliance team, Vector3 position, float range)
{
	int inhibitorsInRange = 0;
	for (auto inhibitor : GetInhibitorsAs(team))
	{
		if (!inhibitor) continue;
		if (inhibitor->GetPosition().distanceTo(position) > range) continue;
		inhibitorsInRange++;
	}
	return inhibitorsInRange;
}

Object* ObjectManager::GetNexusAs(Alliance team)
{
	Object* nexusToReturn = nullptr;
	for (auto nexus : GetNexuses())
	{
		if (!nexus) continue;
		if (team == Alliance::Ally && !nexus->IsAlly() || team == Alliance::Enemy && !nexus->IsEnemy()) continue;
		if (nexus->IsAlive() and nexus->IsVisible() and nexus->IsTargetable() and !nexus->IsInvulnerable())

			if (!nexusToReturn)
			{
				nexusToReturn = nexus;
				break;
			}
	}

	return nexusToReturn;
}
