#pragma once
#include "Client.h"
#include "Hero.h"
#include "stdafx.h"
#include "Turret.h"

class ObjectManager2
{

	static QWORD GetFirst(const QWORD& objectManager);
	static QWORD GetNext(const QWORD& objectManager, const QWORD& obj);

	static inline std::vector<Hero*> _hero_list;
	static inline std::vector<Client*> _minion_list;
	static inline std::vector<Turret*> _turret_list;
	static inline std::vector<AttackableUnit*> _inhibitor_list;
	static inline std::vector<AttackableUnit*> _nexus_list;
	static inline std::vector<Object2*> _particle_list;


	static void HandleObject(Object2* obj);
	static void Flush();

public:

	static void Update();
	static inline std::unordered_map<DWORD, Client*> _client_map;

	static std::vector<Hero*> GetHeroes() { return _hero_list; }
	static std::vector<Client*> GetMinions() { return _minion_list; }
	static std::vector<Turret*> GetTurrets() { return _turret_list; }
	static std::vector<AttackableUnit*> GetInhibitors() { return _inhibitor_list; }
	static std::vector<AttackableUnit*> GetNexuses() { return _nexus_list; }
	static std::vector<Object2*> GetParticles() { return _particle_list; }
	static Object* GetClientByHandle(DWORD handle);
};

