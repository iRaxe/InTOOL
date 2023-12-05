#pragma once
#include "Pointer.h"

class BaseCharacterData : public Pointer {
private:
	DEFINE_MEMBER_N(float _healthbar_offset, UPasta::Offsets::Client::CharacterData::HealthBarOffset);
	DEFINE_MEMBER_N(uintptr_t _type, UPasta::Offsets::Client::CharacterData::Type);
	DEFINE_MEMBER_N(uintptr_t _type_detailed, UPasta::Offsets::Client::CharacterData::TypeDetailed);
	DEFINE_MEMBER_N(DWORD _hero_id, 0x858);
public:

	float GetHealthBarOffset() { return _healthbar_offset; }
	uintptr_t GetObjectTypeHash() { return _type; }
	uintptr_t GetObjectTypeHashDetailed() { return _type_detailed; }
	DWORD GetHeroID() { return _hero_id; }
};

class CharacterData2 : public Pointer {

private:
	union {
		DEFINE_MEMBER_N(BaseCharacterData* _base_character_data, UPasta::Offsets::Client::CharacterData::Base);
	};

public:

	BaseCharacterData* GetBaseCharacterData() { return _base_character_data; }


};