#pragma once
#include "../stdafx.h"


class LolString
{
	char content[0x10]; // 0x0
	int len = 0; // 0x10
	int max = 0; // 0x14

public:
	LolString()
	{
		len = *(int*)((uintptr_t)this + 0x10);
	}

	operator const char*(void)
	{
		return c_str();
	}

	operator std::string(void)
	{
		return std::string(c_str());
	}

	std::string str()
	{
		return std::string(c_str());
	}

private:
	char* c_str(void)
	{
		if (QWORD(this) <= 0x1000)
			return (char*)"";
		if (len > 15)
		{
			auto text = *reinterpret_cast<char**>((uintptr_t)this);
			return text;
		}
		return content;
	}
};

class InventorySlot
{
public:
	ItemsDatabase GetId();
	std::string GetTexturePath();
	std::string GetName();
};


class ItemListObject
{
public:
	InventorySlot* GetSlot();
};

class HeroInventory
{
public:
	InventorySlot* GetInventorySlot(int slotId);
	InventorySlot* FindItemID(ItemsDatabase itemID);
	std::vector<InventorySlot*> ItemsList();
};

class Perk
{
public:
	PerkID GetId();
	std::string GetName();
	std::string GetRawName();
	std::string GetRawDescription();
};

class Perks
{
public:
	Perk* GetPerkByIndex(int index);
};

class Cooldown
{
public:
	float GetBaseCooldown();
};

class CooldownArray
{
public:
	Cooldown* GetArrayIndex(int index);
};

class CharacterStackData
{
public:
	const char* model;
	char pad1[0x18];
	int skin;
	char pad2[0x60];
	char gear;
	char pad3[0x7];
};

class CharacterDataStack
{
public:
	std::vector<CharacterStackData> stack;
	CharacterStackData base_skin;

	void Update(bool change);
};

class CharacterStateIntermediate
{
public:
	enum StatType {
		AbilityHasteMod,
		PercentCooldownCapMod,
		PassiveCooldownEndTime,
		PassiveCooldownTotalTime,
		FlatPhysicalDamageMod,
		PercentPhysicalDamageMod,
		PercentBonusPhysicalDamageMod,
		PercentBasePhysicalDamageAsFlatBonusMod,
		FlatMagicDamageMod,
		PercentMagicDamageMod,
		FlatMagicReduction,
		PercentDamageToBarracksMinionMod,
		FlatDamageReductionFromBarracksMinionMod,
		PercentMagicReduction,
		FlatCastRangeMod,
		AttackSpeedMod,
		PercentAttackSpeedMod,
		PercentMultiplicativeAttackSpeedMod,
		BaseAttackDamage,
		BaseAttackDamageSansPercentScale,
		FlatBaseAttackDamageMod,
		PercentBaseAttackDamageMod,
		BaseAbilityDamage,
		CritDamageMultiplier,
		ScaleSkinCoef,
		Dodge,
		CritPercent,
		FlatBaseHPPoolMod,
		Armor,
		BonusArmor,
		SpellBlock,
		BonusSpellBlock,
		HPRegenRate,
		BaseHPRegenRate,
		MoveSpeed,
		AttackRange,
		FlatBubbleRadiusMod,
		PercentBubbleRadiusMod,
		FlatArmorPenetration,
		PhysicalLethality,
		PercentArmorPenetration,
		PercentBonusArmorPenetration,
		PercentCritBonusArmorPenetration,
		PercentCritTotalArmorPenetration,
		FlatMagicPenetration,
		MagicLethality,
		PercentMagicPenetration,
		PercentBonusMagicPenetration,
		PercentLifeStealMod,
		PercentSpellVampMod,
		PercentOmnivampMod,
		PercentPhysicalVamp,
		PathfindingRadiusMod,
		PercentCCReduction,
		PercentEXPBonus,
		FlatBaseArmorMod,
		FlatBaseSpellBlockMod,
		PrimaryARBaseRegenRateRep,
		SecondaryARRegenRateRep,
		SecondaryARBaseRegenRateRep
	};

	float ReadClientStat(StatType statToReturn);
};

class AiManager
{
public:
	std::vector<Vector3> GetFutureSegments();
	Vector3 GetSegment(int index);
	Vector3 GetPosition();
	Vector3 GetTargetPosition();
	Vector3 GetMouseClickPosition();
	Vector3 GetPathStart();
	Vector3 GetDashStart();
	Vector3 GetPathEnd();
	Vector3 GetDirection();

	bool IsMoving();
	bool IsDashing();
	bool IsNotDashing();
	int GetCurrentSegment();
	int GetSegmentsCount();
	float GetDashSpeed();
	float GetMovementSpeed();
};

class CharacterData
{
public:
	float GetSize();
	ChampionID GetHeroID();
	QWORD GetObjectTypeHash();
	QWORD GetObjectTypeHashDetailed();
};

class SpellData
{
public:
	std::string GetName();
	std::string GetTexturePath();
	Vector3 GetSpellEndPos();
	float GetManaCostByLevel(int level);
	CooldownArray* GetCooldownArray();
};


class Missiles
{
public:
	int GetMissileId();
	SpellData* GetSpellData();
	int GetMissileSrcId();
	Vector3 GetSpellStartPos();
	Vector3 GetSpellEndPos();
};

class MissileData
{
public:
	std::string GetMissileName();
	bool IsAutoAttack();
	std::string GetSpellName();

	int Resource;
};

class Missile
{
public:
	MissileData* GetMissileData();
	int GetMissileSrcId();
	Vector3 GetSpellStartPos();
	Vector3 GetSpellPos();
	Vector3 GetSpellEndPos();
};

class SpellInfo
{
public:
	SpellData* GetSpellData();
	Vector3 EndPosition;
	Vector3 StartPosition;
	uintptr_t SpellIndex;
	MissileData* BasicAttackSpellData;
	int SourceNetworkID;
};

class SpellInput
{
public:
	void SetCaster(int index);
	void SetTarget(int index);
	void SetStartPos(Vector3 pos);
	void SetEndPos(Vector3 pos);
	void SetClickedPos(Vector3 pos);
	void SetUnkPos(Vector3 pos);
};

class Spell
{
public:
	std::string_view GetTextureName();
	int GetLevel();
	float GetCooldownTimer();
	float GetCooldown();
	int GetStacks();
	float GetTotalCooldown();
	float GetRelativeCooldown();
	SpellInput* GetSpellInput();
	SpellInfo* GetSpellInfo();
	bool IsReady();
	std::string GetName();
	float GetManaCost();
};

class SpellCast
{
public:
	SpellInfo* GetSpellInfo();
	SpellInfo* GetProcessSpellInfo();
	int GetSpellId();
	bool IsAutoAttack();
	bool IsSpell();
	float GetCastTime();
	std::string GetCasterName();
	Vector3 GetStartPosition();
	Vector3 GetEndPosition();
	Vector3 GetMousePosition();
	DWORD GetCasterHandle();
};

class Buff
{
public:
	std::string GetName();
	BuffType GetType();
	float GetStartTime();
	float GetEndTime();
	int GetStacksAlt();
	int GetStacks();
	int GetMaxStacks();
	bool isActive();
};

class BuffEntry
{
public:
	Buff* GetBuff();
};

class BuffManager
{
public:
	BuffEntry* GetBuffEntryByIndex(int index);
};

template <class T>
struct ManagerTemplate
{
	uintptr_t VTable;
	T** list;
	uint32_t length;
	uint32_t capacity;
};

class Object
{
public:
	bool IsVisible();
	bool IsAlive();
	bool IsTargetable();
	bool IsInvulnerable();
	bool IsCursed();
	bool HasSummonerSpell(SummonerSpells summSpellToFind);
	bool HasConqueror();
	bool IsCastingSpell();
	bool CanAttack();
	bool CanCast();
	bool CanMove();
	bool IsEnemy();
	bool IsAlly();
	bool IsJungle();
	bool IsValidTarget();
	bool IsRespawnMarker();
	bool IsMelee();
	bool IsRanged();
	bool IsHero();
	bool IsSpecial();
	bool IsMinion();
	bool IsSiegeMinion();
	bool IsVoidMinion();
	bool IsRangedMinion();
	bool IsMeleeMinion();
	bool IsSuperMinion();
	bool IsMonster();
	bool IsDragon();
	bool IsWard();
	bool IsEpicMonster();
	bool IsTurret();
	bool IsBuilding();
	bool IsInRange(Vector3 pos, float radius);
	bool IsUnderEnemyTower();
	bool IsUnderAllyTower();
	bool IsInAARange();
	bool CanCastSpell(int slotId);
	bool HasBuff(const char* buffname);
	Vector3 GetPosition();
	Buff* GetBuffByName(std::string name);
	Buff* GetBuffByType(BuffType type);
	int GetBuffListSize();
	int GetTeam();
	int GetLevel();
	unsigned int GetNetId();
	float GetBoundingRadius();
	float GetAttackDelay();
	float GetAttackWindup();
	float GetEffectiveHealth(int damageType);
	float GetRealAttackRange();
	float GetDistanceTo(Object* obj);
	float GetPercentHealth();
	unsigned short GetActionState();
	float GetExperience();
	float GetMana();
	float GetMaxMana();
	float GetPercentMana();
	float GetAttackDamage();
	std::string GetName();
	std::string GetClassicName();
	BuffManager* GetBuffManager();
	DWORD GetHandle();
	QWORD* GetBuffManagerEntriesEnd();
	SpellCast* GetActiveSpellCast();
	Spell* GetSpellBySlotId(int slotId);
	Missile* GetMissileByIndex();
	CharacterData* GetCharacterData();
	AiManager* GetAiManager();
	CombatType GetCombatType();
	HeroInventory* GetHeroInventory();
	Perks* GetHeroPerks();
	CharacterDataStack* GetCharacterDataStack();
	CharacterStateIntermediate* GetCharacterStateIntermediate();

	enum StatType {
		AttackRange,
		Health,
		BonusHealth,
		MaxHealth,
		LifeRegeneration,
		Mana,
		MaxMana,
		BaseAttackDamage,
		BonusAttackDamage,
		TotalAttackDamage,
		AbilityHaste,
		Lethality,
		ArmorPenetrationFlat,
		MagicPenetrationFlat,
		MagicPenetrationMultiplier,
		AbilityPower,
		BaseArmor,
		BonusArmor,
		TotalArmor,
		Shield,
		PhysicalShield,
		MagicalShield,
		CritFlat,
		CritMultiplier,
		AttackSpeed,
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
	float ReadClientStat(StatType s);

};

class ObjectManager
{
public:
	uintptr_t* vtable;
	std::map<uintptr_t, Missile*> missile_map;
	std::map<uintptr_t, Object*> units_map;
	int GetListSize();
	Object* GetIndex(int index);

	class iterator
	{
	private:
		ObjectManager* objectManager;
		int index;

	public:
		iterator(ObjectManager* _objectManager, int _index)
			: objectManager(_objectManager), index(_index)
		{
		}

		iterator& operator++()
		{
			index++;
			return *this;
		}

		bool operator!=(const iterator& other) const
		{
			return index != other.index;
		}

		Object* operator*()
		{
			return objectManager->GetIndex(index);
		}
	};

	iterator begin()
	{
		return iterator(this, 0);
	}

	iterator end()
	{
		return iterator(this, GetListSize());
	}
};
