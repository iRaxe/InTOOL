#pragma once
#include "CharacterData.h"
#include "Object.h"
#include "RenderPipeline.h"
#include "SpellBook.h"

class AttackableUnit : public Object2
{
protected:

#define DEFINE_PADDING sizeof(Object2)

	union {
		DEFINE_MEMBER_N(float _attack_range, UPasta::Offsets::Client::AttackRange);

		DEFINE_MEMBER_N(float _health, UPasta::Offsets::AttackableUnit::Health);
		DEFINE_MEMBER_N(float _bonus_health, UPasta::Offsets::AttackableUnit::BonusHealth);
		DEFINE_MEMBER_N(float _max_health, UPasta::Offsets::AttackableUnit::MaxHealth);
		DEFINE_MEMBER_N(float _life_regen, UPasta::Offsets::Client::LifeRegeneration);

		DEFINE_MEMBER_N(float _mana, UPasta::Offsets::AttackableUnit::Mana);
		DEFINE_MEMBER_N(float _max_mana, UPasta::Offsets::AttackableUnit::MaxMana);

		DEFINE_MEMBER_N(float _basic_attack_damage, UPasta::Offsets::Client::BaseAttackDamage);
		DEFINE_MEMBER_N(float _bonus_attack_damage, UPasta::Offsets::Client::BonusAttackDamage);

		DEFINE_MEMBER_N(float _ability_haste, UPasta::Offsets::Client::AbilityHaste);

		DEFINE_MEMBER_N(float _lethality, UPasta::Offsets::Client::Lethality);

		DEFINE_MEMBER_N(float _armor_penetration_flat, UPasta::Offsets::Client::ArmorPenetrationFlat);

		DEFINE_MEMBER_N(float _magic_penetration_flat, UPasta::Offsets::Client::MagicPenetrationFlat);
		DEFINE_MEMBER_N(float _magic_penetration_percentage, UPasta::Offsets::Client::MagicPenetrationMultiplier);

		DEFINE_MEMBER_N(float _ability_power, UPasta::Offsets::Client::AbilityPower);

		DEFINE_MEMBER_N(float _bonus_armor, UPasta::Offsets::Client::BonusArmor);
		DEFINE_MEMBER_N(float _total_armor, UPasta::Offsets::Client::BaseArmor);

		DEFINE_MEMBER_N(float _shield, UPasta::Offsets::Client::Shield);
		DEFINE_MEMBER_N(float _physical_shield, UPasta::Offsets::Client::PhysicalShield);
		DEFINE_MEMBER_N(float _magical_shield, UPasta::Offsets::Client::MagicalShield);

		DEFINE_MEMBER_N(float _crit_flat, UPasta::Offsets::Client::CritFlat);
		DEFINE_MEMBER_N(float _crit_percentage, UPasta::Offsets::Client::CritMultiplier);

		DEFINE_MEMBER_N(float _attack_speed_bonus, UPasta::Offsets::Client::AttackSpeedBonus);
		DEFINE_MEMBER_N(float _attack_speed_multi, UPasta::Offsets::Client::AttackSpeedMulti);

		DEFINE_MEMBER_N(float _bonus_magic_resist, UPasta::Offsets::Client::BonusMagicResist);
		DEFINE_MEMBER_N(float _total_magic_resist, UPasta::Offsets::Client::TotalMagicResist);

		DEFINE_MEMBER_N(float _movement_speed, UPasta::Offsets::Client::MovementSpeed);

		DEFINE_MEMBER_N(float _ammo, UPasta::Offsets::Client::Ammo);
		DEFINE_MEMBER_N(float _ammo_maximum, UPasta::Offsets::Client::MaxAmmo);

		DEFINE_MEMBER_N(float _size_multiplier, UPasta::Offsets::Client::ScaleMulti);

		DEFINE_MEMBER_N(float _experience, UPasta::Offsets::Client::Experience);
		DEFINE_MEMBER_N(int _level, UPasta::Offsets::Client::Level);

		DEFINE_MEMBER_N(bool _visibility, UPasta::Offsets::AttackableUnit::Visibility); // DA CAMBIARE TIPO PER LE FLAGS!
		DEFINE_MEMBER_N(bool _targetable, UPasta::Offsets::AttackableUnit::Targetable);
		DEFINE_MEMBER_N(bool _invulnerable, UPasta::Offsets::AttackableUnit::Invulnerability); // NON FUNZIONA
		DEFINE_MEMBER_N(SpellBook2 _spell_book, UPasta::Offsets::Client::SpellBookInstance);
		DEFINE_MEMBER_N(CharacterData2* _character_data, 0x3570);


	};

#define DEFINE_PADDING 0

public:
	float GetAttackRange();

	bool IsAlive();
	float GetHealthP();
	float GetManaP();
	bool IsVisible();
	bool IsInvulnerable();
	bool IsTargetable();

	void GoldHealthBar(bool on) { _invulnerable = on; }

	void Glow(RenderPipeline* pipe, void* screen_buffer, MouseOverData* data);
};

