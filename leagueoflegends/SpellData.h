#pragma once
#include "Pointer.h"

class SpellData2 : public Pointer {

	union {
		DEFINE_MEMBER_N(float _cast_time, UPasta::Offsets::SpellData::CastTime);
		DEFINE_MEMBER_N(float _cooldowns[5], UPasta::Offsets::SpellData::CooldownTime);
		DEFINE_MEMBER_N(float _delay_cast, UPasta::Offsets::SpellData::DelayCastOffsetPerce);
		DEFINE_MEMBER_N(float _cast_range, UPasta::Offsets::SpellData::CastRange); //MAX CASTING DISTANCE
		DEFINE_MEMBER_N(float _cast_range_display_override, UPasta::Offsets::SpellData::CastRangeDisplayOverride);
		DEFINE_MEMBER_N(float _cast_radius, UPasta::Offsets::SpellData::CastRadius); // RADIUS OF THE SPELL
		DEFINE_MEMBER_N(float _missile_speed, UPasta::Offsets::SpellData::MissileSpeed);
		DEFINE_MEMBER_N(float _line_width, UPasta::Offsets::SpellData::LineWidth); // 0 FOR NON LINE MISSILES
	};

public:

	float* GetCooldowns() { return _cooldowns; }
	float GetCastTime() { return _cast_time; }
	float GetDelayCast() { return _delay_cast; }
	float GetCastRange() { return _cast_range; }
	float GetCastRangeDisplayOverride() { return _cast_range_display_override; }
	float GetCastRadius() { return _cast_radius; }
	float GetMissileSpeed() { return _missile_speed; }
	float GetLineWidth() { return _line_width; }
};
