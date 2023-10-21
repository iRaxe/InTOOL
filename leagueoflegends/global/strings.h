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


