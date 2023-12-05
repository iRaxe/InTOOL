#include "Client.h"

float Client::GetBoundingRadius() {

	typedef float(*OriginalFn)(PVOID);
	return CallVirtual<OriginalFn>(this, 36)(this);
}

/*float Client::GetAttackRange() {
	return _attack_range + GetBoundingRadius();
}*/

float Client::ReadClientStat(Client::STAT s) {
	switch (s) {
	case AttackRange: return _attack_range;

	case Health: return _health;
	case BonusHealth: return _bonus_health;
	case MaxHealth: return _max_health;
	case LifeRegeneration: return _life_regen;

	case Mana: return _mana;
	case MaxMana: return _max_mana;

	case BaseAttackDamage: return _basic_attack_damage;
	case BonusAttackDamage: return _bonus_attack_damage;

	case AbilityHaste: return _ability_haste;

	case Lethality: return _lethality;

	case ArmorPenetrationFlat: return _armor_penetration_flat;

	case MagicPenetrationFlat: return _magic_penetration_flat;
	case MagicPenetrationMultiplier: return _magic_penetration_percentage;

	case AbilityPower: return _ability_power;

	case BonusArmor: return _bonus_armor;
	case TotalArmor: return _total_armor;

	case Shield: return _shield;
	case PhysicalShield: return _physical_shield;
	case MagicalShield: return _magical_shield;

	case CritFlat: return _crit_flat;
	case CritMultiplier: return _crit_percentage;

	case AttackSpeedBonus: return _attack_speed_bonus;
	case AtackSpeedMulti: return _attack_speed_multi;

	case BonusMagicResist: return _bonus_magic_resist;
	case TotalMagicResist: return _total_magic_resist;

	case MovementSpeed: return _movement_speed;

	case Ammo: return _ammo;
	case MaxAmmo: return _ammo_maximum;

	case ScaleMulti: return _size_multiplier;

	case Experience: return _experience;
	case Level: return _level;

	case Visibility: return _visibility;
	case Targetable: return _targetable;
	case Invulnerability: return _invulnerable;
	default: return 0;
	}
}

bool Client::IsRanged() {
	return _combat_type == 2;
}

bool Client::CompareHeroID(ChampionID champID) {
	if (auto a1 = _character_data->GetBaseCharacterData()->GetHeroID(); a1 = (DWORD)champID)
		return true;

	return false;
}

SpellBook2* Client::GetSpellBook() {
	return &_spell_book;
}

BuffManager* Client::GetBuffs() {
	return &_buff_manager;
}

std::string Client::GetName() {
	return _name.str();
}
