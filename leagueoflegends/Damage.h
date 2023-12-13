#pragma once
#include "classes/classes.h"

namespace UPasta
{
	namespace SDK
	{
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
			float CalculateSkillDamage(SpellIndex spellIndex, Object* pEnemy, float* dmgSkillArray, DamageType dmgType, float additionalPercentageAP = 0.0f, float additionalPercentageAD = 0.0f);
			float CalculateAutoAttackDamage(Object* source, Object* target);
			float CalculatePhysicalDamage(Object* source, Object* target, float amount);
			float CalculateMagicalDamage(Object* source, Object* target, float amount);

			DamageModifierResult ComputeDamageModifier(Object* source, Object* target, DamageType damageType);
			DamageOnHitResult ComputeDamageOnHit(Object* source, Object* target);
		};
	}
}
