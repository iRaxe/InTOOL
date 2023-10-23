#pragma once

#define JUNGLE_MONSTER_GROMP std::string("SRU_Gromp") // OK
#define JUNGLE_MONSTER_WOLF std::string("SRU_Murkwolf") // OK
#define JUNGLE_MONSTER_WOLF_MINI std::string("SRU_MurkwolfMini")
#define JUNGLE_MONSTER_RAPTOR std::string("SRU_Razorbeak") // OK
#define JUNGLE_MONSTER_RAPTOR_MINI std::string("SRU_RazorbeakMini")
#define JUNGLE_MONSTER_KRUG std::string("SRU_Krug") // OK
#define JUNGLE_MONSTER_KRUG_MINI std::string("SRU_KrugMini")
#define JUNGLE_MONSTER_KRUG_MINI_MINI std::string("SRU_KrugMiniMini")
#define JUNGLE_MONSTER_CRAB std::string("Sru_Crab") // OK
#define JUNGLE_MONSTER_BLUE std::string("SRU_Blue") // OK
#define JUNGLE_MONSTER_RED std::string("SRU_Red") // OK
#define JUNGLE_MONSTER_BARON std::string("SRU_Baron") // OK
#define JUNGLE_MONSTER_HERALD std::string("SRU_RiftHerald") //OK
#define JUNGLE_MONSTER_DRAGON_INFERNAL std::string("SRU_Dragon_Fire")
#define JUNGLE_MONSTER_DRAGON_OCEAN std::string("SRU_Dragon_Water")
#define JUNGLE_MONSTER_DRAGON_MOUNTAIN std::string("SRU_Dragon_Earth")
#define JUNGLE_MONSTER_DRAGON_CLOUD std::string("SRU_Dragon_Air")
#define JUNGLE_MONSTER_DRAGON_HEXTECH std::string("SRU_Dragon_Hextech")
#define JUNGLE_MONSTER_DRAGON_CHEMTECH std::string("SRU_Dragon_Chemtech")
#define JUNGLE_MONSTER_DRAGON_ELDER std::string("SRU_Dragon_Elder")

#define MINION_MELEE_BLUE std::string("SRU_OrderMinionMelee")
#define MINION_MELEE_RED std::string("SRU_ChaosMinionMelee")
#define MINION_RANGED_BLUE std::string("SRU_OrderMinionRanged")
#define MINION_RANGED_RED std::string("SRU_ChaosMinionRanged")
#define MINION_CANNON_BLUE std::string("SRU_OrderMinionSiege")
#define MINION_CANNON_RED std::string("SRU_ChaosMinionSiege")
#define MINION_SUPER_BLUE std::string("SRU_OrderMinionSuper")
#define MINION_SUPER_RED std::string("SRU_ChaosMinionSuper")
#define MINION_SENNA_SOUL std::string("SennaSoul")

// Example: spell_hash("ZeriR");
#define buff_hash(str) (std::integral_constant<std::uint32_t, hash_fnv1a_ignorecase(str)>::value)
#define buff_hash_real hash_fnv1a_ignorecase
#define spell_hash(str) (std::integral_constant<std::uint32_t, hash_elf_ignorecase(str)>::value)
#define spell_hash_real hash_elf_ignorecase

#define character_hash(str) (std::integral_constant<std::uint32_t, hash_sdbm_ignorecase(str)>::value)

#define translation_hash(str) (std::integral_constant<std::uint64_t, translation_hash_64_runtime(str)>::value)

constexpr std::uint8_t char_to_lower(std::uint8_t input)
{
	if (static_cast<std::uint8_t>(input - 0x41) > 0x19u)
		return input;

	return input + 0x20;
}

constexpr std::uint32_t const hash_elf_ignorecase(const char* str)
{
	std::uint32_t hash = 0;

	for (auto i = 0u; str[i]; ++i)
	{
		hash = char_to_lower(str[i]) + 0x10 * hash;

		if (hash & 0xF0000000)
			hash ^= (hash & 0xF0000000) ^ ((hash & 0xF0000000) >> 24);
	}

	return hash;
}

constexpr std::uint32_t const hash_sdbm_ignorecase(const char* str)
{
	std::uint32_t hash = 0;

	for (auto i = 0u; str[i]; ++i)
	{
		hash = hash * 65599 + char_to_lower(str[i]);
	}

	return hash;
}

constexpr std::uint32_t const hash_fnv1a_ignorecase(const char* str)
{
	std::uint32_t hash = 0x811C9DC5;

	for (auto i = 0u; str[i]; ++i)
	{
		hash = 16777619 * (hash ^ char_to_lower(str[i]));
	}

	return hash;
}

constexpr std::uint64_t const translation_hash_64_runtime(const char* str)
{
	std::uint64_t hash = 0xCBF29CE484222325;
	std::uint32_t len = 0;

	while (str[len] != '\0')
		len++;

	for (auto i = 0u; i < len; ++i)
	{
		auto input = str[i];
		if (!(static_cast<std::uint8_t>(input - 0x41) > 0x19u))
			input = input + 0x20;

		hash = 0x100000001B3 * (hash ^ input);
	}

	return hash;
}

inline std::string str_tolower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

inline std::string RemoveSubstring(const std::string substringToAnalyze, const std::string substringToRemove)
{
	std::string result = substringToAnalyze;

	std::size_t startPos = result.find(substringToRemove);

	if (startPos != std::string::npos)
	{
		result.erase(startPos, substringToRemove.length());
	}

	return result;
}


