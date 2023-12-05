#include "Object.h"

#include "ObjectTypeHolder.h"

int Object2::GetHandle() {
	return _handle;
}

int Object2::GetNetworkID() {
	return _network_id;
}

int Object2::GetTeam() {
	return _team;
}

bool Object2::IsEnemyTo(Object* target) {
	return this->GetTeam() != target->GetTeam();
}

std::string Object2::GetName() {
	//print("%s", _name->str().c_str());

	return _name.str();
}

Vector3 Object2::GetWorldPosition() {
	//	print("x: %f, y: %f, z: %f", _position.x, _position.y, _position.z);
	return _position;
}

float Object2::GetDistanceTo(Object* target) {
	return this->GetWorldPosition().Distance(target->GetPosition());
}

float Object2::GetDistanceTo(Vector3 pos) {
	return this->GetWorldPosition().Distance(pos);
}


Object2::TYPE Object2::GetType() {
	typedef ObjectTypeHolder* (*OriginalFn)(PVOID);
	auto holder = Engine::CallVirtual<OriginalFn>(this, 1)(this);
	return holder->GetType();

	auto pointg = Engine::CallVirtual<ObjectTypeHolder*>(this, 1);
	return pointg->GetType();
}