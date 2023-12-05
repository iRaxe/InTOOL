#include "AttackableUnit.h"
#include "Hero.h"
#include "RenderPipeline.h"

float AttackableUnit::GetHealthP() {
	//print("health: %f, max_health: %f", _health, _max_health);
	return _health / _max_health * 100;
}

float AttackableUnit::GetManaP() {
	//print("health: %f, max_health: %f", _health, _max_health);
	return _mana / _max_mana * 100;
}

bool AttackableUnit::IsVisible() {
	return _visibility == 1;
}

bool AttackableUnit::IsTargetable() {
	return _targetable == 1;
}

bool AttackableUnit::IsInvulnerable() {
	return _invulnerable & 1;
}

float AttackableUnit::GetAttackRange() {
	return _attack_range;
}

bool AttackableUnit::IsAlive() {
	typedef bool(*_fnIsAslive)(AttackableUnit* pObj);
	static _fnIsAslive funCanll = (_fnIsAslive)(RVA(UPasta::Offsets::AttackableUnit::fnCanHighlight));

	auto res = funCanll(this);
	//print("return value: %d", res);
	return res;
}

Hero* GetLocalPlayer() {
	return *(Hero**)(RVA(UPasta::Offsets::Instance::LocalPlayer));
}

void AttackableUnit::Glow(RenderPipeline* pipe, void* screen_buffer, MouseOverData* data) {

	reinterpret_cast<QWORD(*)(void*, DWORD, Hero*, AttackableUnit*, AttackableUnit*, AttackableUnit*, AttackableUnit*, float, MouseOverData*)>(RVA(UPasta::Offsets::Functions::HUD::RenderUsualSuspects))(screen_buffer, 0, GetLocalPlayer(), this, this, 0, 0, 5.0f, data);
}

