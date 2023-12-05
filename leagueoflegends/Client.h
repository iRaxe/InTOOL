#pragma once
#include "AttackableUnit.h"
#include "SpellBook.h"
class Client : public AttackableUnit
{
protected:
//#define DEFINE_PADDING sizeof(AttackableUnit)

	union {
		DEFINE_MEMBER_N(DWORD _combat_type, UPasta::Offsets::Client::CombatType);
		DEFINE_MEMBER_N(BuffManager _buff_manager, UPasta::Offsets::Client::BuffInstance);
		DEFINE_MEMBER_N(QWORD _encrypted_navigation, UPasta::Offsets::Client::NavigationInstance);
		DEFINE_MEMBER_N(LolString _name, UPasta::Offsets::Client::Name);
		//DEFINE_MEMBER_N(DWORD _level, UPasta::Offsets::Client::Level);

	};
//#define DEFINE_PADDING 0
public:
	enum STAT {
		AttackRange,
		Health,
		BonusHealth,
		MaxHealth,
		LifeRegeneration,
		Mana,
		MaxMana,
		BaseAttackDamage,
		BonusAttackDamage,
		AbilityHaste,
		Lethality,
		ArmorPenetrationFlat,
		MagicPenetrationFlat,
		MagicPenetrationMultiplier,
		AbilityPower,
		BonusArmor,
		TotalArmor,
		Shield,
		PhysicalShield,
		MagicalShield,
		CritFlat,
		CritMultiplier,
		AttackSpeedBonus,
		AtackSpeedMulti,
		BonusMagicResist,
		TotalMagicResist,
		MovementSpeed,
		Ammo,
		MaxAmmo,
		ScaleMulti,
		Experience,
		Level,
		Visibility,
		Targetable,
		Invulnerability
	};

	std::string GetName();
	float GetBoundingRadius();
	//float GetAttackRange();
	float ReadClientStat(STAT s);


	bool IsRanged();
	//float GetSizeMultiplier();
	//float GetHealthBarOffset();
	//bool GetHealthBarPosition(Vector3* out);
	bool CompareHeroID(ChampionID champID);
	BuffManager* GetBuffs();
	SpellBook2* GetSpellBook();
	//Navigation* GetNavigation();

};

