#pragma once
#include "stdafx.h"

class MouseOverData
{
public:
	union {

		DEFINE_MEMBER_0(DWORD AllyColor);
		DEFINE_MEMBER_N(DWORD EnemyColor, 0x4);
		DEFINE_MEMBER_N(DWORD NeutralColor, 0x8);
		DEFINE_MEMBER_N(DWORD SelfColor, 0xC);
		DEFINE_MEMBER_N(float_t InteractionTimes[4], 0x10);
		DEFINE_MEMBER_N(int32_t InteractionSizes[4], 0x20);
		DEFINE_MEMBER_N(int32_t MouseOverSize, 0x34);
		DEFINE_MEMBER_N(float_t MouseOverColorFactor, 0x38);
		DEFINE_MEMBER_N(uint32_t MouseOverBlurPassCount, 0x3C);
		DEFINE_MEMBER_N(int32_t SelectedSize, 0x44);
		DEFINE_MEMBER_N(float_t SelectedColorFactor, 0x48);
		DEFINE_MEMBER_N(uint32_t SelectedBlurPassCount, 0x4C);
		DEFINE_MEMBER_N(int32_t AvatarSize, 0x50);
		DEFINE_MEMBER_N(float_t AvatarColorFactor, 0x54);
		DEFINE_MEMBER_N(DWORD AvatarColor, 0x58);
		DEFINE_MEMBER_N(uint32_t  mvatarBlurPassCount, 0x5C);
		DEFINE_MEMBER_N(int32_t  KillerSize, 0x60);
		DEFINE_MEMBER_N(float_t  KillerColorFactor, 0x64);
		DEFINE_MEMBER_N(uint32_t  KillerBlurPassCount, 0x68);

	};
};
