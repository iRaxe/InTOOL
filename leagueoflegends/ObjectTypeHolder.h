#pragma once
#include "Object.h"
class ObjectTypeHolder {
	union
	{
		DEFINE_MEMBER_N(Object::TYPE _type, UPasta::Offsets::Instance::ObjectTypeHolder::Type);
	};

public:

	inline Object::TYPE GetType() { return _type; };
};
