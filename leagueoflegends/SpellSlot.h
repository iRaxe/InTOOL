#pragma once
#include "SpellInfo.h"
#include "stdafx.h"
#include "TargetingClient.h"

class SpellSlot2
{
	union {
		DEFINE_MEMBER_N(DWORD _level, UPasta::Offsets::SpellBook::SpellSlot::Level);
		DEFINE_MEMBER_N(float _cooldown_expire, UPasta::Offsets::SpellBook::SpellSlot::Cooldown);
		DEFINE_MEMBER_N(DWORD _charges, UPasta::Offsets::SpellBook::SpellSlot::Charges);
		DEFINE_MEMBER_N(float _cooldown, UPasta::Offsets::SpellBook::SpellSlot::TotalCooldown);
		DEFINE_MEMBER_N(SpellInfo2* _spell_info, UPasta::Offsets::SpellBook::SpellSlot::SpellInfoInstance);
		DEFINE_MEMBER_N(TargetingClient2* _targeting_client, UPasta::Offsets::SpellBook::SpellSlot::SpellInputInstance);
	};

public:

	int GetLevel() { return _level; }
	float GetCooldownExpire() { return _cooldown_expire; }
	float GetCooldown() { return _cooldown; }
	int GetCharges() { return _charges; }

	SpellInfo2* GetSpellInfo() { return _spell_info; }
	TargetingClient2* GetTargetingClient() { return _targeting_client; }


	bool IsReady();
	float ReadyIn();
	float ReadyP();

};

