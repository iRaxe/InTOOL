#include "../TargetSelector.h"

#include "../Damage.h"

void TargetSelector::Initialize() {
	Event::Subscribe(Event::OnWndProc, &TargetSelector::OnWndProc);
}

void TargetSelector::OnWndProc(UINT msg, WPARAM param) {

	if (msg != WM_LBUTTONDOWN) return;

	auto mouse_entity = Engine::GetSelectedObject();
	if (!mouse_entity) return;
	if (mouse_entity->GetType() != Object::AIHeroClient) return;
	if (!mouse_entity->IsEnemy()) return;

	_override_target = (Object*)mouse_entity;
}

bool TargetSelector::IsValid(Object* target, Vector3 from, float range) {

	if (!target) return false;

	if (!target->IsAlive() or !target->IsVisible() or !target->IsTargetable() or target->IsInvulnerable() or target->GetPosition().Distance(from) > range + target->GetBoundingRadius() / 2) return false;

	return true;

}

Object* TargetSelector::FindBestTarget(Vector3 from, float range) {

	if (IsValid(_override_target, from, range)) return _override_target;

	std::vector<Object*> possible_targets;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {

		if (!hero) continue;

		if (!hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->GetPosition().Distance(from) <= range + hero->GetBoundingRadius() / 2)
			possible_targets.push_back(hero);

	}

	if (possible_targets.empty()) {
		_last_target = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = ObjectManager::GetLocalPlayer();
				auto my_aa = me->GetAttackDamage();

				auto damage_first = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pFirst, my_aa);
				auto damage_second = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pSecond, my_aa);;

				auto health_first = pFirst->GetHealth();
				auto health_second = pSecond->GetHealth();

				auto autos_first = health_first / damage_first;
				auto autos_second = health_second / damage_second;

				return autos_first < autos_second;

			});

	_last_target = possible_targets.front();
	return _last_target;
}

Object* TargetSelector::FindFarestBestTarget(Vector3 from, float range) {

	if (IsValid(_override_target, from, range)) return _override_target;

	std::vector<Object*> possible_targets;
	for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {

		if (!hero) continue;

		if (!hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->GetPosition().Distance(from) <= range + hero->GetBoundingRadius() / 2)
			possible_targets.push_back(hero);

	}

	if (possible_targets.empty()) {
		_last_target = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = ObjectManager::GetLocalPlayer();

				auto distance_first = me->GetDistanceTo(pFirst);
				auto distance_second = me->GetDistanceTo(pSecond);

				return distance_first > distance_second;

			});

	_last_target = possible_targets.front();
	return _last_target;
}

Object* TargetSelector::FindBestMinion(Vector3 from, float range, Alliance team) {

	std::vector<Object*> possible_targets;
	for (auto minion : ObjectManager::GetMinionsAs(team)) {

		if (!minion) continue;
		if (team == Alliance::Ally && !minion->IsAlly() || team == Alliance::Enemy && !minion->IsEnemy()) continue;
		if (minion->GetPosition().Distance(from) > range) continue;
		if (minion->IsAlive() and minion->IsVisible() and minion->IsTargetable())
			possible_targets.push_back(minion);
	}

	if (possible_targets.empty()) {
		_last_minion = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = ObjectManager::GetLocalPlayer();
				auto my_aa = me->GetAttackDamage();

				auto damage_first = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pFirst, my_aa);
				auto damage_second = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pSecond, my_aa);;

				auto health_first = pFirst->GetHealth();
				auto health_second = pSecond->GetHealth();

				auto autos_first = health_first / damage_first;
				auto autos_second = health_second / damage_second;

				return autos_first < autos_second;

			});

	_last_minion = possible_targets.front();
	return _last_minion;
}

Object* TargetSelector::FindBestJungle(Vector3 from, float range) {

	std::vector<Object*> possible_targets;
	for (auto jungle : ObjectManager::GetJungleMonsters()) {
		if (jungle == nullptr) continue;
		if (!IsValid(jungle, from, range)) continue;
		possible_targets.push_back(jungle);
	}

	if (possible_targets.empty()) {
		_last_jungle = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = ObjectManager::GetLocalPlayer();
				auto my_aa = me->GetAttackDamage();

				auto damage_first = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pFirst, my_aa);
				auto damage_second = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pSecond, my_aa);;

				auto health_first = pFirst->GetHealth();
				auto health_second = pSecond->GetHealth();

				auto autos_first = health_first / damage_first;
				auto autos_second = health_second / damage_second;

				return autos_first < autos_second;

			});

	_last_jungle = possible_targets.front();
	return _last_jungle;
}

Object* TargetSelector::FindTurret(Vector3 from, float range, Alliance team) {
	std::vector<Object*> possible_targets;
	for (auto turret : ObjectManager::GetTurretsAs(team)) {

		if (!turret) continue;
		if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) continue;
		if (!IsValid(turret, from, range)) continue;
		possible_targets.push_back(turret);
	}

	if (possible_targets.empty()) {
		_last_target = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = ObjectManager::GetLocalPlayer();
				auto my_aa = me->GetAttackDamage();

				auto damage_first = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pFirst, my_aa);
				auto damage_second = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pSecond, my_aa);;

				auto health_first = pFirst->GetHealth();
				auto health_second = pSecond->GetHealth();

				auto autos_first = health_first / damage_first;
				auto autos_second = health_second / damage_second;

				return autos_first < autos_second;

			});

	_last_turret = possible_targets.front();
	return _last_turret;
}

Object* TargetSelector::FindInhibitor(Vector3 from, float range, Alliance team) {
	std::vector<Object*> possible_targets;
	for (auto inhibitor : ObjectManager::GetInhibitorsAs(team)) {

		if (!inhibitor) continue;
		if (team == Alliance::Ally && !inhibitor->IsAlly() || team == Alliance::Enemy && !inhibitor->IsEnemy()) continue;
		if (!IsValid(inhibitor, from, range)) continue;
		possible_targets.push_back(inhibitor);
	}

	if (possible_targets.empty()) {
		_last_target = nullptr;
		return nullptr;
	}

	if (possible_targets.size() > 1)
		std::sort(possible_targets.begin(), possible_targets.end(),
			[](Object* pFirst, Object* pSecond) -> bool
			{
				auto me = ObjectManager::GetLocalPlayer();
				auto my_aa = me->GetAttackDamage();

				auto damage_first = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pFirst, my_aa);
				auto damage_second = UPasta::SDK::Damage::CalculatePhysicalDamage(me, pSecond, my_aa);;

				auto health_first = pFirst->GetHealth();
				auto health_second = pSecond->GetHealth();

				auto autos_first = health_first / damage_first;
				auto autos_second = health_second / damage_second;

				return autos_first < autos_second;

			});

	_last_turret = possible_targets.front();
	return _last_turret;
}

Object* TargetSelector::FindBestLaneClear(Vector3 from, float range) {
	Object* minionTarget = FindBestMinion(from, range, Alliance::Enemy);
	if (minionTarget)
		return minionTarget;

	Object* jungleTarget = FindBestJungle(from, range);
	if (jungleTarget)
		return jungleTarget;

	Object* turretTarget = FindTurret(from, range, Alliance::Enemy);
	if (turretTarget)
		return turretTarget;

	Object* inhibitorTarget = FindInhibitor(from, range, Alliance::Enemy);
	if (inhibitorTarget)
		return inhibitorTarget;

	return nullptr;
}