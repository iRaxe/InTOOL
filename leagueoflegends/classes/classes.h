#pragma once
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
	std::string GetName();
	std::string GetTexturePath();

	ItemsDatabase GetId();
};

class ItemListObject
{
public:
	CLASS_GETTER(InventorySlot*, GetSlot, UPasta::Offsets::ItemManager::InventorySlot);
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
	CLASS_GETTER_P(PerkID, GetId, UPasta::Offsets::Perks::PerkID);
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
	CLASS_GETTER(float, GetBaseCooldown, 0x0);
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
	CLASS_GETTER(bool, IsMoving, UPasta::Offsets::AIManager::Path::IsMoving);
	CLASS_GETTER(bool, IsDashing, UPasta::Offsets::AIManager::Path::IsDashing);
	CLASS_GETTER(bool, IsNotDashing, UPasta::Offsets::AIManager::Path::IsNotDashing);

	CLASS_GETTER(int, GetCurrentSegment, UPasta::Offsets::AIManager::Path::PassedWayPoints);
	CLASS_GETTER(int, GetSegmentsCount, UPasta::Offsets::AIManager::Path::SegmentsCount);

	CLASS_GETTER(float, GetDashSpeed, UPasta::Offsets::AIManager::Path::DashSpeed);
	CLASS_GETTER(float, GetMovementSpeed, UPasta::Offsets::AIManager::Path::MovementSpeed);
	CLASS_GETTER(float, GetVelocity, UPasta::Offsets::AIManager::Velocity);

	std::vector<Vector3> GetFutureSegments();
	Vector3 GetSegment(int index);
	Vector3 GetPosition();
	Vector3 GetTargetPosition();
	Vector3 GetMouseClickPosition();
	Vector3 GetPathStart();
	Vector3 GetDashStart();
	Vector3 GetPathEnd();
	Vector3 GetDirection();
};

class CharacterData
{
public:
	CLASS_GETTER(float, GetSize, UPasta::Offsets::CharData::Size);
	CLASS_GETTER(ChampionID, GetHeroID, UPasta::Offsets::CharData::HeroID);
	
	QWORD GetObjectTypeHash();
	QWORD GetObjectTypeHashDetailed();
};

class SpellData
{
public:

	CLASS_GETTER_P(CooldownArray, GetCooldownArray, 0x2F0);

	CLASS_GETTER(float, GetMaxCastRange, UPasta::Offsets::SpellData::CastRange);
	CLASS_GETTER(float, GetCastRadius, UPasta::Offsets::SpellData::CastRadius);
	CLASS_GETTER(float, GetLineWidth, UPasta::Offsets::SpellData::LineWidth);
	CLASS_GETTER(float, GetCastSpeed, UPasta::Offsets::SpellData::MissileSpeed);
	CLASS_GETTER(float, GetDelay, UPasta::Offsets::SpellData::DelayCastOffsetPerce);
	CLASS_GETTER(float, GetCastTime, UPasta::Offsets::SpellData::CastTime);
	std::string GetName();
	std::string GetTexturePath();
	Vector3 GetSpellEndPos();
	float GetManaCostByLevel(int level);
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
	CLASS_GETTER(DWORD, GetHandle, UPasta::Offsets::Client::Handle);
	CLASS_GETTER(MissileData*, GetMissileData, UPasta::Offsets::MissileManager::SpellInfo::SpellInfoInstance);


	CLASS_GETTER(float, GetSpellSpeed, UPasta::Offsets::MissileManager::Speed);
	CLASS_GETTER(int, GetMissileSrcId, UPasta::Offsets::MissileManager::SrcIdx);

	Vector3 GetSpellStartPos();
	Vector3 GetSpellPos();
	Vector3 GetSpellEndPos();
};

class SpellInfo
{
public:
	CLASS_GETTER(SpellData*, GetSpellData, UPasta::Offsets::SpellInfo::SpellDataInstance);

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
	CLASS_GETTER(SpellInfo*, GetSpellInfo, UPasta::Offsets::SpellBook::SpellSlot::SpellInfoInstance);

	CLASS_GETTER(int, GetLevel, UPasta::Offsets::SpellBook::SpellSlot::Level);
	CLASS_GETTER(int, GetStacks, UPasta::Offsets::SpellBook::SpellSlot::Stacks);

	CLASS_GETTER(float, GetCooldownTimer, UPasta::Offsets::SpellBook::SpellSlot::Cooldown);
	CLASS_GETTER(float, GetTotalCooldown, UPasta::Offsets::SpellBook::SpellSlot::TotalCooldown);


	bool IsReady() { return this->GetRelativeCooldown() == 0.0f;	};
	std::string GetName() { return this->GetSpellInfo()->GetSpellData()->GetName(); }
	std::string_view GetTextureName();
	float GetCooldown();
	float GetRelativeCooldown();
	SpellInput* GetSpellInput();
	float GetManaCost();
};

class SpellCast
{
public:
	CLASS_GETTER(DWORD, GetCasterHandle, UPasta::Offsets::SpellCast::CasterHandle);
	CLASS_GETTER(SpellInfo*, GetSpellInfo, UPasta::Offsets::ActiveCast::Info);
	CLASS_GETTER(SpellInfo*, GetProcessSpellInfo, 0x0);

	CLASS_GETTER(int, GetSpellId, UPasta::Offsets::SpellCast::SlotID);

	CLASS_GETTER(float, GetCastTime, UPasta::Offsets::SpellCast::CastedAtTime);

	CLASS_GETTER(bool, IsAutoAttack, UPasta::Offsets::SpellCast::IsBasicAttack);
	CLASS_GETTER(bool, IsSpell, UPasta::Offsets::SpellCast::IsSpell);

	std::string GetCasterName();

	Vector3 GetStartPosition();
	Vector3 GetEndPosition();
	Vector3 GetMousePosition();
};

class Buff
{
public:
	CLASS_GETTER(BuffType, GetType, UPasta::Offsets::BuffManager::BuffType);

	CLASS_GETTER(float, GetStartTime, UPasta::Offsets::BuffManager::BuffEntryBuffStartTime);
	CLASS_GETTER(float, GetEndTime, UPasta::Offsets::BuffManager::BuffEntryBuffEndTime);

	CLASS_GETTER(int, GetStacksAlt, UPasta::Offsets::BuffManager::BuffEntryBuffCountAlt);
	CLASS_GETTER(int, GetStacks, UPasta::Offsets::BuffManager::BuffEntryBuffCount);

	int GetMaxStacks() { return max(this->GetStacksAlt(), this->GetStacks()); }


	std::string GetName();
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
	CLASS_GETTER(DWORD, GetHandle, UPasta::Offsets::Client::Handle);
	CLASS_GETTER(DWORD, GetTurretTargetNetworkID, UPasta::Offsets::Turret::TargetNetworkID);

	CLASS_GETTER(unsigned short, GetActionState, UPasta::Offsets::AttackableUnit::State);

	CLASS_GETTER(int, GetNetId, UPasta::Offsets::Client::Index);
	CLASS_GETTER(int, GetLevel, UPasta::Offsets::Client::Level);
	CLASS_GETTER(float, GetExperience, UPasta::Offsets::Client::Experience);

	CLASS_GETTER(float, GetAttackRange, UPasta::Offsets::Client::AttackRange);

	CLASS_GETTER(float, GetHealth, UPasta::Offsets::AttackableUnit::Health);
	CLASS_GETTER(float, GetBonusHealth, UPasta::Offsets::AttackableUnit::BonusHealth);
	CLASS_GETTER(float, GetMaxHealth, UPasta::Offsets::AttackableUnit::MaxHealth);
	CLASS_GETTER(float, GetLifeRegeneration, UPasta::Offsets::Client::LifeRegeneration);

	CLASS_GETTER(float, GetMana, UPasta::Offsets::AttackableUnit::Mana);
	CLASS_GETTER(float, GetMaxMana, UPasta::Offsets::AttackableUnit::MaxMana);

	CLASS_GETTER(float, GetBaseAttackDamage, UPasta::Offsets::Client::BaseAttackDamage);
	CLASS_GETTER(float, GetBonusAttackDamage, UPasta::Offsets::Client::BonusAttackDamage);

	CLASS_GETTER(float, GetAbilityHaste, UPasta::Offsets::Client::AbilityHaste);

	CLASS_GETTER(float, GetLethality, UPasta::Offsets::Client::Lethality);

	CLASS_GETTER(float, GetArmorPenetrationFlat, UPasta::Offsets::Client::ArmorPenetrationFlat);

	CLASS_GETTER(float, GetMagicPenetrationFlat, UPasta::Offsets::Client::MagicPenetrationFlat);
	CLASS_GETTER(float, GetMagicPenetrationMultiplier, UPasta::Offsets::Client::MagicPenetrationMultiplier);

	CLASS_GETTER(float, GetAbilityPower, UPasta::Offsets::Client::AbilityPower);

	CLASS_GETTER(float, GetBonusArmor, UPasta::Offsets::Client::BonusArmor);
	CLASS_GETTER(float, GetBaseArmor, UPasta::Offsets::Client::BaseArmor);

	CLASS_GETTER(float, GetShield, UPasta::Offsets::Client::Shield);
	CLASS_GETTER(float, GetPhysicalShield, UPasta::Offsets::Client::PhysicalShield);
	CLASS_GETTER(float, GetMagicalShield, UPasta::Offsets::Client::MagicalShield);

	CLASS_GETTER(float, GetCritFlat, UPasta::Offsets::Client::CritFlat);
	CLASS_GETTER(float, GetCritMultiplier, UPasta::Offsets::Client::CritMultiplier);

	CLASS_GETTER(float, GetAttackSpeed, UPasta::Offsets::Client::AttackSpeedMulti);

	CLASS_GETTER(float, GetBonusMagicResist, UPasta::Offsets::Client::BonusMagicResist);
	CLASS_GETTER(float, GetTotalMagicResist, UPasta::Offsets::Client::TotalMagicResist);

	CLASS_GETTER(float, GetMovementSpeed, UPasta::Offsets::Client::MovementSpeed);

	CLASS_GETTER(float, GetAmmo, UPasta::Offsets::Client::Ammo);
	CLASS_GETTER(float, GetMaxAmmo, UPasta::Offsets::Client::MaxAmmo);

	CLASS_GETTER(float, GetScaleMulti, UPasta::Offsets::Client::ScaleMulti);

	CLASS_GETTER(bool, IsVisible, UPasta::Offsets::AttackableUnit::Visibility);
	CLASS_GETTER(bool, IsTargetable, UPasta::Offsets::AttackableUnit::Targetable);
	CLASS_GETTER(bool, IsInvulnerable, UPasta::Offsets::AttackableUnit::Invulnerability);

	float GetBoundingRadius();
	float GetAttackDelay();
	float GetAttackWindup();

	float GetEffectiveHealth(DamageType damageType);
	float GetAttackDamage() { return this->GetBaseAttackDamage() + this->GetBonusAttackDamage(); }
	float GetTotalArmor() { return this->GetBaseArmor() + this->GetBonusArmor(); }

	float GetRealAttackRange() { return this->GetAttackRange() + this->GetBoundingRadius(); }
	float GetPercentHealth() { return 100 * this->GetHealth() / this->GetMaxHealth() > 100 ? 100 : 100 * this->GetHealth() / this->GetMaxHealth(); }
	float GetPercentMana() { return 100 * this->GetMana() / this->GetMaxMana() > 100 ? 100 : 100 * this->GetMana() / this->GetMaxMana(); }

	bool IsAlive();
	bool IsLocalPlayer();

	CLASS_GETTER(int, GetTeam, UPasta::Offsets::BaseObject::Team);
	bool IsJungle() { return this->GetTeam() == 300; }
	bool IsAlly();
	bool IsEnemy() { return !this->IsAlly(); }
	bool IsValidTarget() { return this->IsVisible() && this->IsAlive() && this->IsEnemy() && this->IsTargetable(); }

	bool HasSummonerSpell(SummonerSpells summSpellToFind);
	bool IsCasting();

	//TODO: DEPRECATE ACTIONSTATE, USELESS.

	bool CanAttack() { return this->GetActionState() & ActionState::CanAttack && !this->GetBuffByName("KaisaE"); }
	bool CanCast() { return this->GetActionState() & ActionState::CanCast; }
	bool CanMove() { return this->GetActionState() & ActionState::CanMove; }

	bool CanCastSpell(SpellIndex slotId);

	CLASS_GETTER(CombatType, GetCombatType, UPasta::Offsets::Client::CombatType);
	bool IsMelee() { return this->GetCombatType() == CombatType::Melee; }
	bool IsRanged() { return this->GetCombatType() == CombatType::Ranged; }


	bool IsHero() { return this->GetType() == AIHeroClient; }
	bool IsMinion() { return this->GetType() == AIMinionClient; }
	bool IsTurret() { return this->GetType() == AITurretClient; }
	bool IsInhibitor() { return this->GetType() == BarracksDampener; }
	bool IsNexus() { return this->GetType() == HQ; }
	bool IsMissile() { return this->GetType() == MissileClient; }
	bool IsParticle() { return this->GetType() == obj_GeneralParticleEmitter; }

	CharacterData* GetCharacterData();
	bool IsRespawnMarker() { return this->GetCharacterData()->GetObjectTypeHash() == RespawnMarker; }
	bool IsSpecial() { return this->GetCharacterData()->GetObjectTypeHash() == Special; }
	bool IsWard() { return this->GetCharacterData()->GetObjectTypeHash() == Ward; }
	bool IsSiegeMinion() { return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Siege; }
	bool IsVoidMinion() { return this->GetCharacterData()->GetObjectTypeHash() == Special_Void; }
	bool IsRangedMinion() {	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Ranged; }
	bool IsMeleeMinion() { return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Melee; }
	bool IsSuperMinion() { return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Super; }
	bool IsMonster() { return this->GetCharacterData()->GetObjectTypeHash() == Monster; }
	bool IsEpicMonster() { return this->GetCharacterData()->GetObjectTypeHash() == Monster_Epic; }
	bool IsDragon() { return this->GetCharacterData()->GetObjectTypeHash() == Monster_Dragon; }

	Vector3 GetPosition();

	float GetDistanceTo(Object* obj) { return this->GetPosition().Distance(obj->GetPosition()); }
	bool IsUnderTower(Alliance team);
	bool IsFacing(Object* secondObj, float angle);

	bool IsInAARange();


	CLASS_GETTER(BuffManager*, GetBuffManager, UPasta::Offsets::BuffManager::BuffManagerInstance);
	CLASS_GETTER(QWORD*, GetBuffManagerEntriesEnd, UPasta::Offsets::BuffManager::BuffManagerInstance + UPasta::Offsets::BuffManager::BuffEntriesEnd);
	Buff* GetBuffByName(std::string name);
	Buff* GetBuffByType(BuffType type);
	int GetBuffListSize();

	bool HasBuff(const char* buffname);
	bool IsCursed() { return this->GetBuffByName("cursedtouch"); }
	bool HasConqueror() { return this->GetBuffByName("ASSETS/Perks/Styles/Precision/Conqueror/ConquerorEnrage.lua"); }

	std::string GetName();
	std::string GetClassicName();


	Spell* GetSpellBySlotId(int slotId);
	AiManager* GetAiManager();

	CLASS_GETTER(SpellCast*, GetActiveSpellCast, UPasta::Offsets::ActiveCast::ActiveCastInstance);
	CLASS_GETTER(Missile*, GetMissileByIndex, UPasta::Offsets::MissileManager::Index);
	CLASS_GETTER_P(HeroInventory, GetHeroInventory, UPasta::Offsets::ItemManager::ItemManagerInstance);
	CLASS_GETTER_P(Perks, GetHeroPerks, UPasta::Offsets::Perks::PerksManagerInstance);
	CLASS_GETTER_P(CharacterDataStack, GetCharacterDataStack, UPasta::Offsets::CharData::CharDataInstance);
	CLASS_GETTER_P(CharacterStateIntermediate, GetCharacterStateIntermediate, UPasta::Offsets::CharacterStateIntermediate::oCharacterStateIntermediate);

	enum TYPE : DWORD {
		NeutralMinionCamp = 0xFE7449A3,
		AIHeroClient = 0xE260302C, //HEROES
		AIMarker = 0x11F7583D,
		AIMinionClient = 0xCAA58CB2, //ALL MINIONS
		ObjectAttacher = 0x9E317160,
		LevelPropAIClient = 0x12E24BCD,
		AITurretClient = 0xBEBA9102, //LANETURRETS
		AITurretCommon = 0x70678BD0,
		obj_GeneralParticleEmitter = 0xDD4DE76F, // includes troys
		GameObject = 0x1FAC8B64,
		MissileClient = 0x9C8ADE94, //MISSILES
		DrawFX = 0x42D144F5,
		UnrevealedTarget = 0xB98F49AF,
		BarracksDampener = 0x60BB49C0, //INHIB
		Barracks = 0xD1ED70FE,
		AnimatedBuilding = 0x8F83FB9C,
		BuildingClient = 0x3CCABB2E,
		obj_Levelsizer = 0x6F2E6CAC,
		obj_NavPoint = 0x96B0A5E6,
		obj_SpawnPoint = 0xE3E9B36C,
		GrassObject = 0xAA2B7AB2, // IVERN BUSH?
		HQ = 0x503AD0D2, //NEXUS
		obj_InfoPoint = 0xF4753AD3,
		LevelPropGameObject = 0x5A730CB9,
		LevelPropSpawnerPoint = 0x4D8B713A,
		Shop = 0xA847E0A9,
		obj_Turret = 0x3D775D09 // AZIR TURRET?

	};

	TYPE GetType();
};

class Camera
{
public:
	CLASS_GETTER(float, GetLimit, UPasta::Offsets::Instance::HUD::Camera::Limit);
};

class CameraStats
{
public:
	CLASS_GETTER(uintptr_t, GetCamera, UPasta::Offsets::Instance::HUD::Camera::Camera);
	CLASS_GETTER(float, GetValue, UPasta::Offsets::Instance::HUD::Camera::Value);
	void SetZoomMultiplier(float new_value) { *(float*)UPasta::Offsets::Instance::HUD::Camera::Value = new_value; }
};

class HUD
{
public:
	CLASS_GETTER(CameraStats*, GetCamera, UPasta::Offsets::Instance::HUD::Camera::Camera);
};

class Core
{
public:
	static Core* GetBase();

	CLASS_GETTER(HUD*, GetHUD, UPasta::Offsets::Instance::HUD::HudInstance);
	CLASS_GETTER(Camera*, GetCameraInstance, UPasta::Offsets::Instance::HUD::Camera::CameraInstance);

};

