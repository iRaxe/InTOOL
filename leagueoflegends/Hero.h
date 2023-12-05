#pragma once
#include "Client.h"

class Hero : public Client
{
public:
#define DEFINE_PADDING sizeof(Client)
	union {
		std::vector<Pointer*> vec;
		//DEFINE_MEMBER_N(std::vector<Pointer*> vec, 0x4600);

	};
#define DEFINE_PADDING 0



	float GetAttackDelay();
	float GetAttackCastDelay();
	void SetVisibility(bool on);


	//ISSUE ORDER QUA CHE RICHIAMA ENGINE::HUDCLICK
	//SPELLBOOK STESSA COSA!
};


//enum class GameObjectCharacterState : int
//{
//	State_CanAttack = (1 << 0),
//	State_CanCast = (1 << 1),
//	State_CanMove = (1 << 2),
//	State_Immovable = (1 << 5),
//	State_RevealSpecificUnit = (1 << 6),
//	State_Taunted = (1 << 7),
//	State_Feared = (1 << 8),
//	State_Fleeing = (1 << 9),
//	State_Supressed = (1 << 10),
//	State_Asleep = (1 << 11),
//	State_NearSight = (1 << 12),
//	State_Ghosted = (1 << 13),
//	State_GhostProof = (1 << 14),
//	State_Charmed = (1 << 15)
//};
//
//constexpr QWORD oActionState = oGameObjectBaseState + 0x28; //action of obj, moving attacking
//constexpr QWORD oActionState2 = oGameObjectBaseState + 0x40; //info type of attack/spell
//
//bool GameObject::HasState(GameObjectCharacterState state) {
//	return *(int*)(this + Offsets::GameObject::Flags::oActionState) & (int)state;
//}