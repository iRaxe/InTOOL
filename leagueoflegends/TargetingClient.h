#pragma once
#include "stdafx.h"
class TargetingClient2
{
	union {
		DEFINE_MEMBER_N(DWORD _source_handle, UPasta::Offsets::SpellBook::SpellSlot::TargetingClient::SourceHandle);
		DEFINE_MEMBER_N(DWORD _target_handle, UPasta::Offsets::SpellBook::SpellSlot::TargetingClient::TargetHandle);
		DEFINE_MEMBER_N(Vector3 _start_position, UPasta::Offsets::SpellBook::SpellSlot::TargetingClient::StartPosition);
		DEFINE_MEMBER_N(Vector3 _end_position, UPasta::Offsets::SpellBook::SpellSlot::TargetingClient::EndPosition);
		DEFINE_MEMBER_N(Vector3 _mouse_position, UPasta::Offsets::SpellBook::SpellSlot::TargetingClient::MousePosition);
		DEFINE_MEMBER_N(Vector3 _mouse_position_second, UPasta::Offsets::SpellBook::SpellSlot::TargetingClient::MousePosition2);
	};
public:

	void SetSource(DWORD val) { _source_handle = val; }
	void SetTarget(DWORD val) { _target_handle = val; }
	void SetStartPosition(Vector3 val) { _start_position = val; }
	void SetEndPosition(Vector3 val) { _end_position = val; }
	void SetMousePosition(Vector3 val) { _mouse_position = val; }
	void SetMousePosition2(Vector3 val) { _mouse_position_second = val; }
};

