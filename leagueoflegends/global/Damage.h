#pragma once
#include "classes/classes.h"

namespace UPasta
{
	namespace SDK
	{
		enum DamageType
		{
			DamageType_Physical,
			DamageType_Magical,
			DamageType_True,
			DamageType_Adaptive
		};

		struct DamageModifierResult {
			float Flat = 0.0f;
			float Percent = 0.0f;
		};

		struct DamageOnHitResult {
			float PhysicalDamage = 0.0f;
			float MagicalDamage = 0.0f;
			float TrueDamage = 0.0f;
		};

		namespace Damage
		{
			float CalculateAutoAttackDamage(Object* source, Object* target);
			float CalculatePhysicalDamage(Object* source, Object* target, float amount);
			float CalculateMagicalDamage(Object* source, Object* target, float amount);

			DamageModifierResult ComputeDamageModifier(Object* source, Object* target, DamageType damageType);
			DamageOnHitResult ComputeDamageOnHit(Object* source, Object* target);
		};
	}
}
