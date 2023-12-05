#pragma once
#include "Object.h"
class ObjectTypeHolder {
	union
	{
		DEFINE_MEMBER_N(Object2::TYPE _type, UPasta::Offsets::Instance::ObjectTypeHolder::Type);
	};

public:

	inline Object2::TYPE GetType() { return _type; };
};
