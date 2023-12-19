#pragma once
#include "stdafx.h"

class ObjectManager
{
	static QWORD GetFirst(const QWORD& objectManager);
	static QWORD GetNext(const QWORD& objectManager, const QWORD& obj);

	static inline std::vector<Object*> _hero_list;
	static inline std::vector<Object*> _minion_list;
	static inline std::vector<Object*> _turret_list;
	static inline std::vector<Object*> _inhibitor_list;
	static inline std::vector<Object*> _nexus_list;
	static inline std::vector<Object*> _particle_list;

	static inline std::unordered_map<DWORD, Object*> _client_map;

	static void HandleObject(Object* obj);
	static void Flush();

public:
	uintptr_t* vtable;
	std::map<uintptr_t, Missile*> missile_map;
	std::map<uintptr_t, Object*> units_map;
	Object* GetIndex(int index);
	CLASS_GETTER(int, GetListSize, UPasta::Offsets::Instance::Lists::ManagerListSize);

	static Object* GetLocalPlayer();
	static ObjectManager* GetHeroList();
	static ObjectManager* GetMinionList();
	static ObjectManager* GetTurretsList();
	static ObjectManager* GetInhibitorsList();
	static ObjectManager* GetAttackableList();
	static ObjectManager* GetMissileList();


	class iterator
	{
	private:
		ObjectManager* objectManager;
		int index;

	public:
		iterator(ObjectManager* _objectManager, int _index)
			: objectManager(_objectManager), index(_index)
		{
		}

		iterator& operator++()
		{
			index++;
			return *this;
		}

		bool operator!=(const iterator& other) const
		{
			return index != other.index;
		}

		Object* operator*()
		{
			return objectManager->GetIndex(index);
		}
	};

	iterator begin()
	{
		return iterator(this, 0);
	}

	iterator end()
	{
		return iterator(this, GetListSize());
	}

	static void Update();

	static std::vector<Object*> GetHeroes() { return _hero_list; }
	static std::vector<Object*> GetHeroesAs(Alliance team);
	static Object* GetHeroAs(Alliance team, Vector3 position, float range);

	static int CountHeroesInRange(Alliance team, Vector3 position, float range);

	static Object* GetObjectInRange(std::string name, float range);
	static Object* GetObjectInRange(float range, std::string name, std::vector<QWORD> includeFilterTypeHashes, std::vector<QWORD> excludeFilterTypeHashesDetailed, bool isSpecial);

	static std::vector<Object*> GetMinions() { return _minion_list; }
	static std::vector<Object*> GetMinionsAs(Alliance team);
	static int CountMinionsInRange(Alliance team, Vector3 position, float range);

	static std::vector<Object*> GetJungleMonsters();
	static int CountJungleMonstersInRange(Vector3 position, float range);

	static std::vector<Object*> GetJunglePlants();
	static std::vector<Object*> GetJungleRespawnCamps();

	static std::vector<Object*> GetWards(Alliance team);

	static std::vector<Object*> GetTurrets() { return _turret_list; }
	static std::vector<Object*> GetTurretsAs(Alliance team);
	static int CountTurretsInRange(Alliance team, Vector3 position, float range);

	static std::vector<Object*> GetInhibitors() { return _inhibitor_list; }
	static std::vector<Object*> GetInhibitorsAs(Alliance team);
	static int CountInhibitorsInRange(Alliance team, Vector3 position, float range);

	static std::vector<Object*> GetNexuses() { return _nexus_list; }
	static Object* GetNexusAs(Alliance team);

	static std::vector<Object*> GetParticles() { return _particle_list; }
	static std::map<QWORD, Object*> GetMissilesAs(Alliance type);

	static Object* GetClientByHandle(DWORD handle);
};
