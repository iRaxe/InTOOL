#include "SpellSlot.h"

bool SpellSlot2::IsReady() {
	return GetLevel() and (Engine::GetGameTime() > _cooldown_expire);
}

float SpellSlot2::ReadyIn() {
	auto cd = _cooldown_expire - Engine::GetGameTime();
	return floor(cd * 10) / 10;
}

float SpellSlot2::ReadyP() {
	float remaining_cd = _cooldown_expire - Engine::GetGameTime();
	float cooldown = _cooldown;
	return 100.f / cooldown * (cooldown - remaining_cd);
}
