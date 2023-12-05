#pragma once
#include "Pointer.h"
#include "SpellInfo.h"

class SpellCast2 : public Pointer
{
	union {
		DEFINE_MEMBER_0(SpellInfo2* _spell_info);
		DEFINE_MEMBER_N(DWORD _slot_id, UPasta::Offsets::SpellCast::SlotID);
		DEFINE_MEMBER_N(LolString _caster_name, UPasta::Offsets::SpellCast::CasterName);
		DEFINE_MEMBER_N(DWORD _caster_handle, UPasta::Offsets::SpellCast::CasterHandle);
		DEFINE_MEMBER_N(Vector3 _start_position, UPasta::Offsets::SpellCast::StartPosition);
		DEFINE_MEMBER_N(Vector3 _end_position, UPasta::Offsets::SpellCast::EndPosition);
		DEFINE_MEMBER_N(Vector3 _mouse_position, UPasta::Offsets::SpellCast::MousePosition);
		DEFINE_MEMBER_N(bool _is_spell, UPasta::Offsets::SpellCast::IsSpell);
		DEFINE_MEMBER_N(bool _is_basic_attack, UPasta::Offsets::SpellCast::IsBasicAttack);
		DEFINE_MEMBER_N(DWORD* _destination_handles, UPasta::Offsets::SpellCast::DestinationHandlesArray);
		DEFINE_MEMBER_N(bool _has_destination_handles, UPasta::Offsets::SpellCast::HasDestinationHandle);
		DEFINE_MEMBER_N(DWORD _n_destination_handles, UPasta::Offsets::SpellCast::NumerOfDestinationHandles);
		DEFINE_MEMBER_N(float _casted_at, UPasta::Offsets::SpellCast::CastedAtTime);
	};

public:


	SpellInfo2* GetSpellInfo() { return _spell_info; }
	DWORD GetSlotID() { return _slot_id; }
	std::string GetCasterName() { return _caster_name.str(); }
	DWORD GetCasterHandle() { return _caster_handle; }
	Vector3 GetStartPosition() { return _start_position; }
	Vector3 GetEndPosition() { return _end_position; }
	Vector3 GetMousePosition() { return _mouse_position; }
	bool IsBasicAttack() { return _is_basic_attack; }
	bool IsSpell() { return !_is_basic_attack and _slot_id >= 0 and _slot_id <= 5; }
	std::vector<DWORD> GetTargedHandles();
	float GetCastTime() { return _casted_at; }

};

