#include "stdafx.h"
#include "Damage.h"

namespace UPasta
{
	namespace SDK
	{
		float Damage::CalculateSkillDamage(SpellIndex spellIndex, Object* pEnemy, const std::vector<float>& dmgSkillArray, DamageType dmgType, float additionalPercentageAP, float additionalPercentageAD)
		{
			if (globals::localPlayer != nullptr && pEnemy != nullptr && Engine::GetSpellState(spellIndex) == 0)
				return -9999;

			const int levelSpell = globals::localPlayer->GetSpellBySlotId(spellIndex)->GetLevel();

			const float skillDamage = dmgSkillArray[levelSpell];
			const float abilityPowerDamage = globals::localPlayer->ReadClientStat(Object::AbilityPower);
			const float attackDamage = globals::localPlayer->ReadClientStat(Object::TotalAttackDamage);
			const float totalDamage = skillDamage + (additionalPercentageAP * abilityPowerDamage) + (additionalPercentageAD * attackDamage);

			switch (dmgType) {
			case Magical:
				return CalculateMagicalDamage(globals::localPlayer, pEnemy, totalDamage);
			case Physical:
				return CalculatePhysicalDamage(globals::localPlayer, pEnemy, totalDamage);
			case True:
				return totalDamage;
			}
			return 0;
		}

		float Damage::CalculateAutoAttackDamage(Object* source, Object* target) {
			auto autoAttackDamageType = Physical;
			auto rawPhysicalDamage = 0.0f;
			auto rawMagicalDamage = 0.0f;
			auto rawTrueDamage = 0.0f;
			auto calculatedPhysicalDamage = 0.0f;
			auto calculatedMagicalDamage = 0.0f;
			auto rawTotalDamage = source->ReadClientStat(Object::TotalAttackDamage);

			//auto targetFlags = target->Flags();
			//auto sourceFlags = source->Flags();

			//if (targetFlags && GameObjectFlags_AIMinionClient) {
			if (target->IsMinion()) {
				if (target->ReadClientStat(Object::MaxHealth) <= 6.0f) {
					return 1.0f;
				}
			}

			//if (sourceFlags & GameObjectFlags_AIHeroClient) {
			if (source->IsHero()) {
				auto damageOnHit = ComputeDamageOnHit(source, target);
				rawPhysicalDamage += damageOnHit.PhysicalDamage;
				rawMagicalDamage += damageOnHit.MagicalDamage;
				rawTrueDamage += damageOnHit.TrueDamage;
			}

			switch (autoAttackDamageType) {
			case Physical:
				rawPhysicalDamage += rawTotalDamage;
				break;
			case Magical:
				rawMagicalDamage += rawTotalDamage;
				break;
			}

			calculatedPhysicalDamage += CalculatePhysicalDamage(source, target, rawPhysicalDamage);
			calculatedMagicalDamage += CalculateMagicalDamage(source, target, rawMagicalDamage);

			return calculatedPhysicalDamage + calculatedMagicalDamage + rawTrueDamage;
		}

		
		float Damage::CalculatePhysicalDamage(Object* source, Object* target, float amount) {
			if (source == nullptr || target == nullptr) return 0;
			if (amount <= 0.0f)
				return 0.0f;

			const auto characterState = source->GetCharacterStateIntermediate();
			const auto flatArmorPenetration = characterState->ReadClientStat(CharacterStateIntermediate::PhysicalLethality) *(0.6f + 0.4f * source->GetLevel() / 18.0f);

			auto percentArmorPenetration = characterState->ReadClientStat(CharacterStateIntermediate::PercentArmorPenetration);
			auto percentBonusArmorPenetration = characterState->ReadClientStat(CharacterStateIntermediate::PercentBonusArmorPenetration);

			if (source->IsMinion()) {
				percentArmorPenetration = 1.0f;
				percentBonusArmorPenetration = 1.0f;
			}
			else if (source->IsTurret()) {
				percentArmorPenetration = 0.7f;
			}

			const auto armor = target->ReadClientStat(Object::BaseArmor);
			const auto bonusArmor = target->ReadClientStat(Object::BonusArmor);
			const auto adjustedArmor = armor * percentArmorPenetration - bonusArmor * (1.0f - percentBonusArmorPenetration) - flatArmorPenetration;

			const auto bonusTrueDamage = source->IsHero() && target->IsHero() && source->HasConqueror() ? amount * 0.2f : 0.0f;
			amount = source->IsHero() && target->IsHero() && source->HasConqueror() ? amount * 0.8f : amount;

			const auto damageModifier = ComputeDamageModifier(source, target, Physical);
			amount = (amount + damageModifier.Flat) * damageModifier.Percent;

			if (armor < 0.0f) {
				amount *= 2.0f - 100.0f / (100.0f - armor);
			}
			else if (adjustedArmor >= 0.0f) {
				amount *= 100.0f / (100.0f + adjustedArmor);
			}

			return max(amount + bonusTrueDamage, 0.0f);
		}

		float Damage::CalculateMagicalDamage(Object* source, Object* target, float amount) {
			if (source == nullptr || target == nullptr) return 0;
			if (amount <= 0.0f)
				return 0.0f;

			const auto characterState = source->GetCharacterStateIntermediate();
			const auto flatMagicPenetration = characterState->ReadClientStat(CharacterStateIntermediate::FlatMagicPenetration);
			const auto percentMagicPenetration = characterState->ReadClientStat(CharacterStateIntermediate::PercentMagicPenetration);
			const auto magicResist = target->ReadClientStat(Object::TotalMagicResist);

			float bonusTrueDamage = 0.0f;
			if (target->IsCursed()) {
				bonusTrueDamage += amount * 0.1f;
			}

			if (source->IsHero() && source->HasConqueror()) {
				bonusTrueDamage += amount * 0.2f;
				amount *= 0.8f;
			}

			const auto damageModifier = ComputeDamageModifier(source, target, Magical);
			amount = (amount + damageModifier.Flat) * damageModifier.Percent;

			if (magicResist < 0.0f) {
				amount *= 2.0f - 100.0f / (100.0f - magicResist);
			}
			else {
				const float adjustedMagicResist = magicResist * percentMagicPenetration - flatMagicPenetration;
				if (adjustedMagicResist >= 0.0f) {
					amount *= 100.0f / (100.0f + adjustedMagicResist);
				}
			}

			return max(amount + bonusTrueDamage, 0.0f);
		}

		DamageModifierResult Damage::ComputeDamageModifier(Object* source, Object* target, DamageType damageType) {
			DamageModifierResult result;
			auto sourceState = source->GetCharacterStateIntermediate();
			auto targetState = target->GetCharacterStateIntermediate();
			result.Flat = -sourceState->ReadClientStat(CharacterStateIntermediate::FlatMagicReduction);
			result.Percent = 1.0f - sourceState->ReadClientStat(CharacterStateIntermediate::FlatMagicReduction);

			// Pushing advantage for minions
			if (source->IsMinion() && target->IsMinion()) {
				result.Flat -= targetState->ReadClientStat(CharacterStateIntermediate::FlatDamageReductionFromBarracksMinionMod);
				result.Percent *= 1.0f + sourceState->ReadClientStat(CharacterStateIntermediate::PercentDamageToBarracksMinionMod);
			}

			// Target buffs
			if (target->IsHero()) {
				static const std::unordered_map<std::string, std::function<void()>> heroBuffHandlers = {
					{"Annie", [&]() {
						if (target->GetBuffByName("MoltenShield")) {
							auto spell = target->GetSpellBySlotId(2);
							static const float annieShield[] = { 0, 60, 95, 130, 165, 200 };
							result.Percent *= 1.0f - (annieShield[spell->GetLevel()] + 0.40f * target->ReadClientStat(Object::AbilityPower)) / 100.0f;
						}
					}},
					{"Braum", [&]() {
						if (target->GetBuffByName("braumeshieldbuff")) {
							auto spell = target->GetSpellBySlotId(2);
							static const float braumShield[] = { 0, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f };
							result.Percent *= 1.0f - braumShield[spell->GetLevel()];
						}
					}},
					{"Galio", [&]() {
						if (target->GetBuffByName("GalioW")) {
							auto spell = target->GetSpellBySlotId(1);
							static const float galioShield[] = { 0, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f };
							result.Percent *= 1.0f - (galioShield[spell->GetLevel()] + (0.12f * target->ReadClientStat(Object::MagicPenetrationMultiplier)));
						}
					}},
					// Add other heroes similarly...
				};

				if (heroBuffHandlers.find(target->GetName()) != heroBuffHandlers.end()) {
					heroBuffHandlers.at(target->GetName())();
				}

				if (target->GetName() == "Kassadin" && damageType == Magical) {
					result.Percent *= 0.85f;
				}
				if (target->GetName() == "Malzahar" && target->GetBuffByName("malzaharpassiveshield")) {
					result.Percent *= 0.1f;
				}
				if (target->GetName() == "MasterYi" && target->GetBuffByName("Meditate")) {
					auto spell = target->GetSpellBySlotId(1);
					static const float yiShield[] = { 0, 0.45f, 0.475f, 0.50f, 0.525f, 0.550f };
					result.Percent *= 1.0f - yiShield[spell->GetLevel()];
				}
			}

			// Source debuffs
			if (source->IsHero() && (source->GetBuffByName("SummonerExhaust") || source->GetBuffByName("sonapassivedebuff") ||
				source->GetBuffByName("itemsmitechallenge") || source->GetBuffByName("itemphantomdancerdebuff") || source->GetBuffByName("barontarget"))) {
				result.Percent *= 0.6f;
			}

			return result;
		}

		DamageOnHitResult Damage::ComputeDamageOnHit(Object* source, Object* target)
		{
			DamageOnHitResult result;
			HeroInventory* inventory = globals::localPlayer->GetHeroInventory();

			// Struttura per associare item ID ai loro calcoli di danno
			std::unordered_map<ItemsDatabase, std::function<void()>> itemDamageHandlers = {
				{ItemsDatabase::Noonquiver, [&]() {
					if (target->IsMinion())
						result.PhysicalDamage += 20;
				}},
				{ItemsDatabase::Blade_of_the_Ruined_King, [&]() {
					auto itemDamage = target->ReadClientStat(Object::Health) * 0.08f;
					if (target->IsMinion()) {
						itemDamage = min(itemDamage, 60.0f);
					}
					result.PhysicalDamage += max(itemDamage, 15.0f);
				}},
				{ItemsDatabase::Nashors_Tooth, [&]() {
					result.MagicalDamage += 15.0f + 0.15f * source->ReadClientStat(Object::AbilityPower);
				}},
				{ItemsDatabase::Recurve_Bow, [&]() {
					result.PhysicalDamage += 15.0f;
				}},
				{ItemsDatabase::Titanic_Hydra, [&]() {
					result.PhysicalDamage += source->GetBuffByName("itemtitanichydracleavebuff") ? 40.0f + 0.1f * source->ReadClientStat(Object::MaxHealth) : 5.0f + 0.01f * source->ReadClientStat(Object::MaxHealth);
				}},
				{ItemsDatabase::Dead_Mans_Plate, [&]() {
					auto buff = source->GetBuffByName("dreadnoughtmomentumbuff");
					if (buff)
						result.MagicalDamage += buff->GetStacks();
				}},
				{ItemsDatabase::Wits_End, [&]() {
					result.MagicalDamage += 42.0f;
				}},
				{ItemsDatabase::Muramana, [&]() {
					if (target->IsHero()) {
					auto mana = source->GetMana();
					if (mana / source->GetMaxMana() > 0.2f) 
						result.PhysicalDamage += 0.06f * mana;					
				}}},
			};

			// Calcolo del danno per ogni oggetto nell'inventario
			for (auto& itemHandler : itemDamageHandlers) {
				if (inventory->FindItemID(itemHandler.first)) {
					itemHandler.second();
				}
			}

#pragma region Source buffs

			std::unordered_map<std::string, std::function<void()>> sourceBuffHandlers = {
				{"akalipweapon", [&]() {
					static float values[18] = { 35, 38, 41, 44, 47, 50, 53, 62, 71, 80, 89, 98, 107, 122, 137, 152, 167, 182 };
					result.MagicalDamage += values[min(source->GetLevel() - 1, 17U)] + 0.6f * source->ReadClientStat(Object::BonusAttackDamage) + 0.55f * source->ReadClientStat(Object::AbilityPower);
				}},
				{"sonaqprocattacker", [&]() {
					result.MagicalDamage += 5.0f + 5.0f * source->GetSpellBySlotId(0)->GetLevel() + 0.3f * target->ReadClientStat(Object::AbilityPower);
				}},
				{"NamiE", [&]() {
					result.MagicalDamage += 5.0f + 15.0f * source->GetSpellBySlotId(2)->GetLevel() + 0.2f * target->ReadClientStat(Object::AbilityPower);
				}},
				{"itemangelhandbuff", [&]() {
					static float values[18] = { 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f };
					result.MagicalDamage += values[min(target->GetLevel() - 1, 17U)];
				}},
				{"BlessingoftheLizardElder", [&]() {
					result.TrueDamage += 2.0f + 2.0f * min(source->GetLevel(), 18U);
				}},
				{"aatroxpassivereadyr", [&]() {
					auto damage = 0.07529f + 0.00471f * min(source->GetLevel(), 18U);

					if (target->IsJungle())
						damage = min(damage, 400.0f);

					result.PhysicalDamage += damage;
				}},
				{"alistareattack", [&]() {
					result.MagicalDamage += 40 + 15 * min(source->GetLevel(), 18U);
				}},
				{"vaynetumblebonus", [&]() {
					auto spell = source->GetSpellBySlotId(0);
					float qBonus[6] = { 0, 0.75f, 0.85f, 0.95f, 1.05f, 1.15f };
					result.PhysicalDamage += source->ReadClientStat(Object::TotalAttackDamage) * qBonus[spell->GetLevel()];
				}},
			};
			if (source->IsHero())
			{
				for (const auto& buffHandler : sourceBuffHandlers) {
					if (source->GetBuffByName(buffHandler.first)) {
						buffHandler.second();
					}
				}
			}
		
#pragma endregion

#pragma region Target buffs
			std::unordered_map<std::string, std::function<void()>> targetBuffHandlers = {
				{"LeonaSunlight", [&]() {
					result.MagicalDamage += 18.0f + 7.0f * max(target->GetLevel(), 18U);
				}},
				{"braummarkcounter", [&]() {
					result.MagicalDamage += 16.0f + 10.0f * max(target->GetLevel(), 18U);
				}},
			};

			if (target->IsHero())
			{
				for (const auto& buffHandler : targetBuffHandlers) {
					if (target->GetBuffByName(buffHandler.first)) {
						buffHandler.second();
					}
				}
			}
#pragma endregion

			return result;

		}
	}
}