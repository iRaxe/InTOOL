#pragma once
#include "SpellCast.h"
#include "SpellSlot.h"
#include "stdafx.h"
class SpellBook2
{
	union {
		DEFINE_MEMBER_N(DWORD _owner_handle, UPasta::Offsets::SpellBook::OwnerHandle);
	};

public:

	enum SLOT
	{
		Q = 0,
		W = 1,
		E = 2,
		R = 3,
		D = 4,
		F = 5,
		Item1 = 6,
		Item2 = 7,
		Item3 = 8,
		Item4 = 9,
		Item5 = 10,
		Item6 = 11,
		Trinket = 12,
		Recall = 13
	};

	enum STATE
	{
		READY = 0,
		NOTAVAILABLE = 4,
		SUPRESSED = 8,
		NOTLEARNED = 12,
		COOLDOWN = 32,
		NOMANA = 64,
		UNKNOWN
	};

	DWORD GetOwnerHandle() { return _owner_handle; }
	SpellCast2* GetActiveSpellCast();
	SpellSlot2* GetSpellSlot(SLOT s);
	STATE GetSpellSlotState(SLOT s);
	bool CanCast(SLOT s) { return GetSpellSlotState(s) == READY; }

};

