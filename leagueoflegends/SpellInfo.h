#pragma once
#include "SpellData.h"
#include "stdafx.h"
class SpellInfo2
{
	union {
		DEFINE_MEMBER_N(LolString _name, UPasta::Offsets::MissileManager::SpellInfo::SpellName);
		DEFINE_MEMBER_N(SpellData2* _spell_data, UPasta::Offsets::MissileManager::SpellInfo::SpellInfoInstance);
	};

public:

	//DA FARE SPELLDATA
	std::string GetName() { return _name.str(); }
	SpellData2* GetSpellData() { return _spell_data; }
};

