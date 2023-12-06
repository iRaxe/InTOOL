#include "Hero.h"

void Hero::SetVisibility(bool on) {
	this->_visibility = on;

	//this->_healthbar_visibility = on;

}

float Hero::GetAttackDelay() {

	typedef float(*fGetAttackDelay)(Hero* obj);
	fGetAttackDelay fAttackDelay = (fGetAttackDelay)(RVA(UPasta::Offsets::Functions::Stats::GetAttackDelay));
	return fAttackDelay(this);

}

float Hero::GetAttackCastDelay() {

	typedef float(*fGetAttackCastDelay)(Hero* obj);
	fGetAttackCastDelay fAttackDelay = (fGetAttackCastDelay)(RVA(UPasta::Offsets::Functions::Stats::GetAttackCastDelay));
	return fAttackDelay(this);

}	