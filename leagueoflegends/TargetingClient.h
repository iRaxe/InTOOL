#pragma once
#include "stdafx.h"
class TargetingClient2
{
	union {
		DEFINE_MEMBER_N(DWORD _source_handle, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::SourceHandle);
		DEFINE_MEMBER_N(DWORD _target_handle, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::TargetHandle);
		DEFINE_MEMBER_N(Vector3 _start_position, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::StartPosition);
		DEFINE_MEMBER_N(Vector3 _end_position, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::EndPosition);
		DEFINE_MEMBER_N(Vector3 _mouse_position, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::MousePosition);
		DEFINE_MEMBER_N(Vector3 _mouse_position_second, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::MousePosition2);
	};
public:

	void SetSource(DWORD val) { _source_handle = val; }
	void SetTarget(DWORD val) { _target_handle = val; }
	void SetStartPosition(Vector3 val) { _start_position = val; }
	void SetEndPosition(Vector3 val) { _end_position = val; }
	void SetMousePosition(Vector3 val) { _mouse_position = val; }
	void SetMousePosition2(Vector3 val) { _mouse_position_second = val; }
};

