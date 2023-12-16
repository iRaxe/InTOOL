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
			float CalculateAutoAttackDamage(Object* source, Object* target);
			float CalculatePhysicalDamage(Object* source, Object* target, float amount);
			float CalculateMagicalDamage(Object* source, Object* target, float amount);

			template <typename DmgSkill, typename ADMulti>
			float CalculateSlotPhysicalDamage(SpellIndex spellIndex, Object* pEnemy, DmgSkill dmgSkill, ADMulti additionalPercentageAD) {
				const int levelSpell = globals::localPlayer->GetSpellBySlotId(spellIndex)->GetLevel();

				float spellslotDamage;
				if constexpr (std::is_pointer_v<DmgSkill>) spellslotDamage = dmgSkill[levelSpell];
				else spellslotDamage = dmgSkill;

				float attackDamageMultiplier;
				if constexpr (std::is_pointer_v<ADMulti>) attackDamageMultiplier = additionalPercentageAD[levelSpell];
				else attackDamageMultiplier = additionalPercentageAD;

				const float attackDamage = globals::localPlayer->GetBonusAttackDamage();
				const float attackDamageModifier = attackDamageMultiplier * attackDamage;
				const float physicalDamage = spellslotDamage + attackDamageModifier;

				return CalculatePhysicalDamage(globals::localPlayer, pEnemy, physicalDamage);
			}

			template <typename DmgSkill, typename APMulti>
			float CalculateSlotMagicalDamage(SpellIndex spellIndex, Object* pEnemy, DmgSkill dmgSkill, APMulti additionalPercentageAP) {
				const int levelSpell = globals::localPlayer->GetSpellBySlotId(spellIndex)->GetLevel();

				float spellslotDamage;
				if constexpr (std::is_pointer_v<DmgSkill>) spellslotDamage = dmgSkill[levelSpell];
				else spellslotDamage = dmgSkill;

				float abilityPowerMultiplier;
				if constexpr (std::is_pointer_v<APMulti>) abilityPowerMultiplier = additionalPercentageAP[levelSpell];
				else abilityPowerMultiplier = additionalPercentageAP;

				const float abilityPower = globals::localPlayer->GetAbilityPower();
				const float abilityPowerModifier = abilityPowerMultiplier * abilityPower;
				const float magicalDamage = spellslotDamage + abilityPowerModifier;

				return CalculateMagicalDamage(globals::localPlayer, pEnemy, magicalDamage);
			}

			template <typename DmgSkill, typename APMulti, typename ADMulti>
			float CalculateSlotMixedDamage(SpellIndex spellIndex, Object* pEnemy, DmgSkill dmgSkill, APMulti additionalPercentageAP, ADMulti additionalPercentageAD) {
				const float magicalDamage = CalculateSlotMagicalDamage(spellIndex, pEnemy, dmgSkill, additionalPercentageAP);
				const float physicalDamage = CalculateSlotPhysicalDamage(spellIndex, pEnemy, dmgSkill, additionalPercentageAD);
				return magicalDamage + physicalDamage;
			}

			template <typename DmgSkill, typename APMulti, typename ADMulti>
			float CalculateSlotTrueDamage(SpellIndex spellIndex, DmgSkill dmgSkill, APMulti additionalPercentageAP, ADMulti additionalPercentageAD) {
				const int levelSpell = globals::localPlayer->GetSpellBySlotId(spellIndex)->GetLevel();
				float spellslotDamage;
				if constexpr (std::is_pointer_v<DmgSkill>) spellslotDamage = dmgSkill[levelSpell];
				else spellslotDamage = dmgSkill;

				float abilityPowerMultiplier;
				if constexpr (std::is_pointer_v<APMulti>) abilityPowerMultiplier = additionalPercentageAP[levelSpell];
				else abilityPowerMultiplier = additionalPercentageAP;

				float attackDamageMultiplier;
				if constexpr (std::is_pointer_v<ADMulti>) attackDamageMultiplier = additionalPercentageAD[levelSpell];
				else attackDamageMultiplier = additionalPercentageAD;

				const float abilityPower = globals::localPlayer->GetAbilityPower();
				const float abilityPowerModifier = abilityPowerMultiplier * abilityPower;

				const float attackDamage = globals::localPlayer->GetBonusAttackDamage();
				const float attackDamageModifier = attackDamageMultiplier * attackDamage;

				return spellslotDamage + abilityPowerModifier + attackDamageModifier;
			}

			DamageModifierResult ComputeDamageModifier(Object* source, Object* target, DamageType damageType);
			DamageOnHitResult ComputeDamageOnHit(Object* source, Object* target);
		};
	}
}
