#pragma once
#include "stdafx.h"

class Pointer
{
public:
	QWORD Hex();
	std::string HexS();

	inline operator const std::string() { return std::string(HexS().c_str()); }
	inline operator const char* () { return HexS().c_str(); }

};


