#include "stdafx.h"
#include "Damage.h"
using namespace UPasta::SDK;

float Damage::CalculateAutoAttackDamage(Object* source, Object* target) {
	auto autoAttackDamageType = Physical;
	auto rawPhysicalDamage = 0.0f;
	auto rawMagicalDamage = 0.0f;
	auto rawTrueDamage = 0.0f;
	auto calculatedPhysicalDamage = 0.0f;
	auto calculatedMagicalDamage = 0.0f;
	auto rawTotalDamage = source->GetAttackDamage();
	//auto targetFlags = target->Flags();
	//auto sourceFlags = source->Flags();

	//if (targetFlags && GameObjectFlags_AIMinionClient) {
	if (target->IsMinion()) {
		if (target->GetMaxHealth() <= 6.0f) {
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
	if (source == nullptr || target == nullptr) return 0.0f;
	if (amount <= 0.0f) return 0.0f;

	const auto characterState = source->GetCharacterStateIntermediate();
	if (characterState == nullptr) return 0.0f;

	float physicalLethality = characterState->ReadClientStat(CharacterStateIntermediate::PhysicalLethality);

	const auto levelFactor = (0.6f + 0.4f * source->GetLevel() / 18.0f);

	const auto flatArmorPenetration = physicalLethality * levelFactor;

	auto percentArmorPenetration = source->GetArmorPenetrationFlat();//characterState->ReadClientStat(CharacterStateIntermediate::PercentArmorPenetration);

	auto percentBonusArmorPenetration = characterState->ReadClientStat(CharacterStateIntermediate::PercentBonusArmorPenetration);;


	if (source->IsMinion()) {
		percentArmorPenetration = 1.0f;
		percentBonusArmorPenetration = 1.0f;
	}
	else if (source->IsTurret()) {
		percentArmorPenetration = 0.7f;
	}

	const auto armor = target->GetBaseArmor();
	const auto bonusArmor = target->GetBonusArmor();

	float adjustedArmor = max(0.0f, armor * percentArmorPenetration - bonusArmor * (1.0f - percentBonusArmorPenetration) - flatArmorPenetration);

	float damageMultiplier = 1.0f;
	if (adjustedArmor != 0.0f) {
		if (adjustedArmor > 0.0f) {
			damageMultiplier = 100.0f / (100.0f + adjustedArmor);
		}
		else {
			damageMultiplier = 2.0f - 100.0f / (100.0f - adjustedArmor);
		}
	}

	amount *= damageMultiplier;

	const auto bonusTrueDamage = source->IsHero() && target->IsHero() && source->HasConqueror() ? amount * 0.2f : 0.0f;
	amount = source->IsHero() && target->IsHero() && source->HasConqueror() ? amount * 0.8f : amount;

	const auto damageModifier = ComputeDamageModifier(source, target, Physical);
	amount = (amount + damageModifier.Flat) * damageModifier.Percent;

	return max(amount + bonusTrueDamage, 0.0f);
}



float Damage::CalculateMagicalDamage(Object* source, Object* target, float amount) {
	if (source == nullptr || target == nullptr) return 0.0f;
	if (amount <= 0.0f) return 0.0f;

	const auto characterState = source->GetCharacterStateIntermediate();
	if (characterState == nullptr) return 0.0f;

	const auto flatMagicPenetration = source->GetMagicPenetrationFlat();
	const auto percentMagicPenetration = characterState->ReadClientStat(CharacterStateIntermediate::PercentMagicPenetration);
	const auto magicResist = target->GetTotalMagicResist();

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
		const float adjustedMagicResist = max(0.0f, magicResist * percentMagicPenetration - flatMagicPenetration);
		if (adjustedMagicResist >= 0.0f) {
			amount *= 100.0f / (100.0f + adjustedMagicResist);
		}
	}

	return max(amount + bonusTrueDamage, 0.0f);
}


// Helper lambda for shield calculations
void applyShieldCalculation(Object* target,const float shieldArray[], size_t arraySize, float additionalMultiplier, DamageModifierResult& result) {
	if (target == nullptr) return;
	auto spell = target->GetSpellBySlotId(arraySize > 0 ? 2 : 1);  // Assuming spell slot 2, adjust as necessary
	if (spell) {
		auto spellLevel = spell->GetLevel();
		if (spellLevel >= 1 && static_cast<size_t>(spellLevel) <= arraySize) {
			result.Percent *= 1.0f - (shieldArray[spellLevel - 1] + additionalMultiplier) / 100.0f;
		}
		else {
			LOG("Invalid spell level: %d for array size: %zu", spellLevel, arraySize);
		}
	}
};

DamageModifierResult Damage::ComputeDamageModifier(Object* source, Object* target, DamageType damageType) {
	DamageModifierResult result{ 0.0f, 1.0f };  // Initialize to default values

	if (!source || !target) {
		return result;  // Early return if source or target is null
	}

	auto sourceState = source->GetCharacterStateIntermediate();
	auto targetState = target->GetCharacterStateIntermediate();
	if (!sourceState || !targetState) {
		return result;  // Early return if null
	}

	float initialFlatMagicReduction = sourceState->ReadClientStat(CharacterStateIntermediate::FlatMagicReduction);
	result.Flat = -initialFlatMagicReduction;
	result.Percent = 1.0f - initialFlatMagicReduction;

	// Additional logic for minions
	if (source->IsMinion() && target->IsMinion()) {
		float barracksMinionMod = targetState->ReadClientStat(CharacterStateIntermediate::FlatDamageReductionFromBarracksMinionMod);
		float percentDamageToBarracksMinionMod = sourceState->ReadClientStat(CharacterStateIntermediate::PercentDamageToBarracksMinionMod);

		result.Flat -= barracksMinionMod;
		result.Percent *= 1.0f + percentDamageToBarracksMinionMod;
	}






	// Target buffs
	// Define the map with the correct function signature
	std::unordered_map<std::string, std::function<void(Object*, DamageModifierResult&)>> heroBuffHandlers = {
		{"Annie", [](Object* target, DamageModifierResult& result) {
			auto buff = target->GetBuffByName("MoltenShield");
			if (buff != nullptr) {
				if (!buff->isActive()) return;
				static const float annieShield[] = { 0, 60, 95, 130, 165, 200 };
				applyShieldCalculation(target, annieShield, std::size(annieShield), 0.40f * target->GetAbilityPower(), result);
			}
		}},
		{"Braum", [](Object* target, DamageModifierResult& result) {
			auto buff = target->GetBuffByName("braumeshieldbuff");
			if (buff != nullptr) {
				if (!buff->isActive()) return;
				static const float braumShield[] = { 0, 0.35f, 0.40f, 0.45f, 0.50f, 0.55f };
				applyShieldCalculation(target, braumShield, std::size(braumShield), 0.0f, result);
			}
		}},
		{"Galio", [](Object* target, DamageModifierResult& result) {
			auto buff = target->GetBuffByName("GalioW");
			if (buff != nullptr) {
				if (!buff->isActive()) return;
				static const float galioShield[] = { 0, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f };
				applyShieldCalculation(target,galioShield, std::size(galioShield), 0.0f, result);
			}
		}},
		// ... other champions ...
	};

	// Usage in the ComputeDamageModifier function
	auto heroName = target->GetName();
	if (heroBuffHandlers.find(heroName) != heroBuffHandlers.end()) {
		heroBuffHandlers[heroName](target, result);
	}


	if (target->GetName() == "Kassadin" && damageType == Magical) {
		result.Percent *= 0.85f;
	}
	if (target->GetName() == "Malzahar" && target->GetBuffByName("malzaharpassiveshield")) {
		result.Percent *= 0.1f;
	}
	

	auto meditateBuff = target->GetBuffByName("Meditate");
	if (meditateBuff != nullptr) {
		if (meditateBuff->isActive()) {
			static const float yiShield[] = { 0, 0.45f, 0.475f, 0.50f, 0.525f, 0.550f };
			applyShieldCalculation(target, yiShield, std::size(yiShield), 0.0f, result);
		}
	}

	// Source debuffs
	if (source->IsHero()) {
		// Summoner Exhaust
		auto exhaustBuff = source->GetBuffByName("SummonerExhaust");
		if (exhaustBuff != nullptr) {
			if (exhaustBuff->isActive()) {
				result.Percent *= 0.6f;
			}
		}

		// Sona Passive Debuff
		auto sonaDebuff = source->GetBuffByName("sonapassivedebuff");
		if (sonaDebuff != nullptr) {
			if (sonaDebuff->isActive()) {
				result.Percent *= 0.6f;
			}
		}

		// Smite Challenge
		auto smiteChallengeBuff = source->GetBuffByName("itemsmitechallenge");
		if (smiteChallengeBuff != nullptr) {
			if (smiteChallengeBuff->isActive()) {
				result.Percent *= 0.6f;
			}
		}

		// Phantom Dancer Debuff
		auto phantomDancerDebuff = source->GetBuffByName("itemphantomdancerdebuff");
		if (phantomDancerDebuff != nullptr) {
			if (phantomDancerDebuff->isActive()) {
				result.Percent *= 0.6f;
			}
		}

		// Baron Target
		auto baronTargetBuff = source->GetBuffByName("barontarget");
		if (baronTargetBuff != nullptr) {
			if (baronTargetBuff->isActive()) {
				result.Percent *= 0.6f;
			}
		}

		// Add checks for any other buffs/debuffs as required
	}

	return result;
}


DamageOnHitResult Damage::ComputeDamageOnHit(Object* source, Object* target)
{
	DamageOnHitResult result;
	HeroInventory* inventory = ObjectManager::GetLocalPlayer()->GetHeroInventory();

	// Struttura per associare item ID ai loro calcoli di danno
	std::unordered_map<ItemsDatabase, std::function<void()>> itemDamageHandlers = {
		{ItemsDatabase::Noonquiver, [&]() {
			if (target->IsMinion())
				result.PhysicalDamage += 20;
		}},
		{ItemsDatabase::Blade_of_the_Ruined_King, [&]() {
			auto itemDamage = target->GetHealth() * 0.08f;
			if (target->IsMinion()) {
				itemDamage = min(itemDamage, 60.0f);
			}
			result.PhysicalDamage += max(itemDamage, 15.0f);
		}},
		{ItemsDatabase::Nashors_Tooth, [&]() {
			result.MagicalDamage += 15.0f + 0.15f * source->GetAbilityPower();
		}},
		{ItemsDatabase::Recurve_Bow, [&]() {
			result.PhysicalDamage += 15.0f;
		}},
		{ItemsDatabase::Titanic_Hydra, [&]() {
			result.PhysicalDamage += source->GetBuffByName("itemtitanichydracleavebuff") ? 40.0f + 0.1f * source->GetMaxHealth() : 5.0f + 0.01f * source->GetMaxHealth();
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
			result.MagicalDamage += values[min(source->GetLevel() - 1, 17U)] + 0.6f * source->GetBonusAttackDamage() + 0.55f * source->GetAbilityPower();
		}},
		{"sonaqprocattacker", [&]() {
			result.MagicalDamage += 5.0f + 5.0f * source->GetSpellBySlotId(0)->GetLevel() + 0.3f * target->GetAbilityPower();
		}},
		{"NamiE", [&]() {
			result.MagicalDamage += 5.0f + 15.0f * source->GetSpellBySlotId(2)->GetLevel() + 0.2f * target->GetAbilityPower();
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
			result.PhysicalDamage += source->GetAttackDamage() * qBonus[spell->GetLevel()];
		}},
	};
	if (source->IsHero()) {
		for (const auto& buffHandler : sourceBuffHandlers) {
			auto buff = source->GetBuffByName(buffHandler.first);
			if (buff != nullptr) {
				if (buff->isActive()) {
					buffHandler.second();
				}
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

	if (target->IsHero()) {
		for (const auto& buffHandler : targetBuffHandlers) {
			auto buff = target->GetBuffByName(buffHandler.first);
			if (buff != nullptr) {
				if (buff->isActive()) {
					buffHandler.second();
				}
			}
		}
	}

#pragma endregion

	return result;

}
	
