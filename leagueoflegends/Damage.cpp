#include "stdafx.h"
#include "Damage.h"

namespace UPasta
{
	namespace SDK
	{
		float Damage::CalculateAutoAttackDamage(Object* source, Object* target)
		{
			auto autoAttackDamageType = DamageType_Physical;
			auto rawPhysicalDamage = 0.0f;
			auto rawMagicalDamage = 0.0f;
			auto rawTrueDamage = 0.0f;
			auto calculatedPhysicalDamage = 0.0f;
			auto calculatedMagicalDamage = 0.0f;
			auto rawTotalDamage = source->GetAttackDamage();

			if (target->IsMinion()) 
			{
				if (target->GetMaxHealth() <= 6.0f) 
				{
					return 1.0f;
				}
			}

			if (source->IsHero()) 
			{
				auto damageOnHit = ComputeDamageOnHit(source, target);
				rawPhysicalDamage += damageOnHit.PhysicalDamage;
				rawMagicalDamage += damageOnHit.MagicalDamage;
				rawTrueDamage += damageOnHit.TrueDamage;
				
			}

			switch (autoAttackDamageType)
			{
			case DamageType_Physical:
				rawPhysicalDamage += rawTotalDamage;
				break;
			case DamageType_Magical:
				rawMagicalDamage += rawTotalDamage;
				break;
			}
			calculatedPhysicalDamage += CalculatePhysicalDamage(source, target, rawPhysicalDamage);
			calculatedMagicalDamage += CalculateMagicalDamage(source, target, rawMagicalDamage);
			return calculatedPhysicalDamage + calculatedMagicalDamage + rawTrueDamage;
		}

		float Damage::CalculatePhysicalDamage(Object* source, Object* target, float amount)
		{
			if (amount <= 0.0f) 
				return 0.0f;

			auto flatArmorPenetration = source->GetCharacterStateIntermediate()->GetPhysicalLethality() * (0.6f + (0.4f * source->GetLevel() / 18.0f));
			auto percentArmorPenetration = source->GetCharacterStateIntermediate()->GetPercentArmorPenetration();
			auto percentBonusArmorPenetration = source->GetCharacterStateIntermediate()->GetPercentBonusArmorPenetration();
			auto armor = target->GetArmor();
			auto bonusArmor = target->GetBonusArmor();

			if (source->IsMinion()) {
				percentArmorPenetration = 1.0f;
				percentBonusArmorPenetration = 1.0f;
			}
			if (source->IsTurret()) {
				percentArmorPenetration = 0.7f;
				percentBonusArmorPenetration = 1.0f;
			}
			
			auto bonusTrueDamage = 0.0f;

			if (source->IsHero() && target->IsHero()) 
			{
				if (source->HasConqueror()) 
				{
					bonusTrueDamage += amount * 0.2f;
					amount *= 0.8f;
				}
			}

			auto damageModifier = ComputeDamageModifier(source, target, DamageType_Physical);
			amount += damageModifier.Flat;
			amount *= damageModifier.Percent;

			if (armor < 0.0f)
			{
				amount *= 2.0f - 100.0f / (100.0f - armor);
			}
			else if (armor * percentArmorPenetration - bonusArmor * (1.0f - percentBonusArmorPenetration) - flatArmorPenetration >= 0.0f) {
				amount *= 100.0f / (100.0f + (armor * percentArmorPenetration) - (bonusArmor * (1.0f - percentBonusArmorPenetration)) - flatArmorPenetration);
			}

			return max(amount + bonusTrueDamage, 0.0f);
		}

		float Damage::CalculateMagicalDamage(Object* source, Object* target, float amount)
		{
			if (amount <= 0.0f)
				return 0.0f;

			auto flatMagicPenetration = source->GetCharacterStateIntermediate()->GetFlatMagicPenetration();
			auto percentMagicPenetration = source->GetCharacterStateIntermediate()->GetPercentMagicPenetration();
			auto magicResist = target->GetMagicResist();

			auto bonusTrueDamage = 0.0f;

			if (target->IsCursed()) 
				bonusTrueDamage += amount * 0.1f;

			if (source->IsHero()) 
			{
				if (source->HasConqueror()) 
				{
					bonusTrueDamage += amount * 0.2f;
					amount *= 0.8f;
				}
			}

			auto damageModifier = ComputeDamageModifier(source, target, DamageType_Magical);
			amount += damageModifier.Flat;
			amount *= damageModifier.Percent;

			if (magicResist < 0.0f) 
			{
				amount *= 2.0f - 100.0f / (100.0f - magicResist);
			}
			else if (magicResist * percentMagicPenetration - flatMagicPenetration >= 0.0f) 
			{
				amount *= 100.0f / (100.0f + magicResist * percentMagicPenetration - flatMagicPenetration);
			}

			return max(amount + bonusTrueDamage, 0.0f);
		}

		DamageModifierResult Damage::ComputeDamageModifier(Object* source, Object* target, DamageType damageType)
		{
			DamageModifierResult result;
			result.Flat = - source->GetCharacterStateIntermediate()->GetFlatMagicReduction();
			result.Percent = 1.0f - source->GetCharacterStateIntermediate()->GetFlatMagicReduction();

			// Pushing advatange
			if (source->IsMinion() && target->IsMinion())
			{
				result.Flat -= target->GetCharacterStateIntermediate()->GetFlatDamageReductionFromBarracksMinionMod();
				result.Percent *= 1.0f + source->GetCharacterStateIntermediate()->GetPercentDamageToBarracksMinionMod();
			}

			// Target buffs
			if (target->IsHero())
			{
				if (target->GetName() == "Annie")
				{
					if (target->GetBuffByName("MoltenShield"))
					{
						auto spell = target->GetSpellBySlotId(2);
						float annieShield[6] = { 0, 60, 95, 130, 165, 200 };
						result.Percent *= 1.0f - (annieShield[spell->GetLevel()] + 0.40f * target->GetAbilityPower()) / 100.0f;
					}
				}
				else if (target->GetName() == "Braum")
				{
					if (target->GetBuffByName("braumeshieldbuff"))
					{
						auto spell = target->GetSpellBySlotId(2);
						float braumShield[6] = { 0, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f };
						result.Percent *= 1.0f - braumShield[spell->GetLevel()];
					}
				}
				else if (target->GetName() == "Galio")
				{
					if (target->GetBuffByName("GalioW"))
					{
						auto spell = target->GetSpellBySlotId(1);
						float galioShield[6] = { 0, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f };
						result.Percent *= 1.0f - (galioShield[spell->GetLevel()] + (0.12f * target->GetMagicPenetrationMulti()));
					}
				}
				else if (target->GetName() == "Kassadin")
				{
					if (damageType == DamageType_Magical)
						result.Percent *= 0.85f;
				}
				else if (target->GetName() == "Malzahar")
				{
					if (target->GetBuffByName("malzaharpassiveshield"))
						result.Percent *= 0.1f;
				}
				else if (target->GetName() == "MasterYi")
				{
					if (target->GetBuffByName("Meditate"))
					{
						auto spell = target->GetSpellBySlotId(1);
						float yiShield[6] = { 0, 0.45f, 0.475f, 0.50f, 0.525f, 0.550f };
						result.Percent *= 1.0f - yiShield[spell->GetLevel()];
					}
				}

			}

			if (source->IsHero())
			{
				if (source->GetBuffByName("SummonerExhaust")
				|| source->GetBuffByName("sonapassivedebuff")
				|| source->GetBuffByName("itemsmitechallenge")
				|| source->GetBuffByName("itemphantomdancerdebuff")
				|| source->GetBuffByName("barontarget"))
					result.Percent *= 0.6f;
			}

			return result;
		}

		DamageOnHitResult Damage::ComputeDamageOnHit(Object* source, Object* target)
		{
			DamageOnHitResult result;
#pragma region Source items
			for (auto i = 0; i < 6; i++)
			{
				InventorySlot* item = globals::localPlayer->GetInventorySlotById(i);
				if (item == nullptr) continue;

				if (item)
				{
					auto itemID = item->GetId();

					switch (itemID)
					{
					case Noonquiver: {
						if (target->IsMinion())
							result.PhysicalDamage += 20;
						break;
					}
					case Blade_of_the_Ruined_King: {
						auto itemDamage = target->GetHealth() * 0.08f;
						//if (targetFlags & GameObjectFlags_AIMinionClient) {
						if (target->IsMinion()) {
							itemDamage = min(itemDamage, 60.0f);
						}
						result.PhysicalDamage += max(itemDamage, 15.0f);
						break;
					}
					case Nashors_Tooth:
						result.MagicalDamage += 15.0f + 0.15f * source->GetAbilityPower();
						break;
					case Recurve_Bow:
						result.PhysicalDamage += 15.0f;
						break;
					case Titanic_Hydra:
						result.PhysicalDamage += source->GetBuffByName("itemtitanichydracleavebuff") ? 40.0f + 0.1f * source->GetMaxHealth() : 5.0f + 0.01f * source->GetMaxHealth();
						break;
					case Dead_Mans_Plate:
					{
						auto buff = source->GetBuffByName("dreadnoughtmomentumbuff");
						if (buff)
						{
							result.MagicalDamage += buff->GetStacks();
						}
						break;
					}
					case Wits_End:
						result.MagicalDamage += 42.0f;
						break;
					case Muramana:
						if (target->IsHero()) {
							auto mana = source->GetMana();
							if (mana / source->GetMaxMana() > 0.2f) {
								result.PhysicalDamage += 0.06f * mana;
							}
						}
						break;
					}
				}
			}
#pragma endregion

#pragma region Source buffs

			if (source->IsHero())
			{
				if (source->GetBuffByName("akalipweapon"))
				{
					static float values[18] = { 35, 38, 41, 44, 47, 50, 53, 62, 71, 80, 89, 98, 107, 122, 137, 152, 167, 182 };
					result.MagicalDamage += values[min(source->GetLevel() - 1, 17U)] + 0.6f * source->GetBonusAttackDamage() + 0.55f * source->GetAbilityPower();
				}
				else if (source->GetBuffByName("sonaqprocattacker"))
					result.MagicalDamage += 5.0f + 5.0f * source->GetSpellBySlotId(0)->GetLevel() + 0.3f * target->GetAbilityPower();
				else if (source->GetBuffByName("NamiE"))
					result.MagicalDamage += 5.0f + 15.0f * source->GetSpellBySlotId(2)->GetLevel() + 0.2f * target->GetAbilityPower();
				else if (source->GetBuffByName("itemangelhandbuff"))
				{
					static float values[18] = { 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f };
					result.MagicalDamage += values[min(target->GetLevel() - 1, 17U)];
				}
				else if (source->GetBuffByName("BlessingoftheLizardElder"))
					result.TrueDamage += 2.0f + 2.0f * min(source->GetLevel(), 18U);
				else if (source->GetBuffByName("aatroxpassivereadyr"))
				{
					auto damage = 0.07529f + 0.00471f * min(source->GetLevel(), 18U);

					if (target->IsJungle())
						damage = min(damage, 400.0f);

					result.PhysicalDamage += damage;
				}
				else if (source->GetBuffByName("alistareattack"))
					result.MagicalDamage += 40 + 15 * min(source->GetLevel(), 18U);
				if (source->GetBuffByName("vaynetumblebonus"))
				{
					auto spell = source->GetSpellBySlotId(0);
					float qBonus[6] = { 0, 0.75f, 0.85f, 0.95f, 1.05f, 1.15f };
					result.PhysicalDamage += source->GetAttackDamage() * qBonus[spell->GetLevel()];
				}

			}
		
#pragma endregion

#pragma region Target buffs

			if (target->IsHero())
			{
				if (target->GetBuffByName("LeonaSunlight"))
				{
					result.MagicalDamage += 18.0f + 7.0f * max(target->GetLevel(), 18U);
				}
				else if (target->GetBuffByName("braummarkcounter"))
				{
					result.MagicalDamage += 16.0f + 10.0f * max(target->GetLevel(), 18U);
				}
			}
#pragma endregion

			return result;

		}
	}
}