#pragma once
#include "AttackableUnit.h"

class Turret : public AttackableUnit
{
#define DEFINE_PADDING sizeof(AttackableUnit)
	union {
		DEFINE_MEMBER_N(DWORD _target_network_id, UPasta::Offsets::Turret::TargetNetworkID);
	};
#define DEFINE_PADDING 0

public:

	DWORD GetTargetNetworkID() { return _target_network_id; }
	float GetAttackRange() { return 950; }
};

