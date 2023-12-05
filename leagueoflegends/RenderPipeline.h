#pragma once
#include "MouseOverData.h"

class RenderPipeline
{
	union {
		DEFINE_MEMBER_N(MouseOverData* _data, 0x2e8);
	};

public:

	MouseOverData* GetMouseOverData() { return _data; }
};

