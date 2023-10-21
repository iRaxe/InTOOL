#pragma once

#include "../itemsdatabase.h"
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

    operator const char* (void)
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
        else
        {
            return content;
        }
    }
};

class InventorySlot
{
public:
    int GetId();
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
    InventorySlot* FindItemID(int itemID);
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
    float GetAbilityHasteMod();
    float GetPercentCooldownCapMod();
    float GetPassiveCooldownEndTime();
    float GetPassiveCooldownTotalTime();
    float GetFlatPhysicalDamageMod();
    float GetPercentPhysicalDamageMod();
    float GetPercentBonusPhysicalDamageMod();
    float GetPercentBasePhysicalDamageAsFlatBonusMod();
    float GetFlatMagicDamageMod();
    float GetPercentMagicDamageMod();
    float GetFlatMagicReduction();
    float GetPercentDamageToBarracksMinionMod();
    float GetFlatDamageReductionFromBarracksMinionMod();
    float GetPercentMagicReduction();
    float GetFlatCastRangeMod();
    float GetAttackSpeedMod();
    float GetPercentAttackSpeedMod();
    float GetPercentMultiplicativeAttackSpeedMod();
    float GetBaseAttackDamage();
    float GetBaseAttackDamageSansPercentScale();
    float GetFlatBaseAttackDamageMod();
    float GetPercentBaseAttackDamageMod();
    float GetBaseAbilityDamage();
    float GetCritDamageMultiplier();
    float GetScaleSkinCoef();
    float GetDodge();
    float GetCritPercent();
    float GetFlatBaseHPPoolMod();
    float GetArmor();
    float GetBonusArmor();
    float GetSpellBlock();
    float GetBonusSpellBlock();
    float GetHPRegenRate();
    float GetBaseHPRegenRate();
    float GetMoveSpeed();
    float GetAttackRange();
    float GetFlatBubbleRadiusMod();
    float GetPercentBubbleRadiusMod();
    float GetFlatArmorPenetration();
    float GetPhysicalLethality();
    float GetPercentArmorPenetration();
    float GetPercentBonusArmorPenetration();
    float GetPercentCritBonusArmorPenetration();
    float GetPercentCritTotalArmorPenetration();
    float GetFlatMagicPenetration();
    float GetMagicLethality();
    float GetPercentMagicPenetration();
    float GetPercentBonusMagicPenetration();
    float GetPercentLifeStealMod();
    float GetPercentSpellVampMod();
    float GetPercentOmnivampMod();
    float GetPercentPhysicalVamp();
    float GetPathfindingRadiusMod();
    float GetPercentCCReduction();
    float GetPercentEXPBonus();
    float GetFlatBaseArmorMod();
    float GetFlatBaseSpellBlockMod();
    float GetPrimaryARBaseRegenRateRep();
    float GetSecondaryARRegenRateRep();
    float GetSecondaryARBaseRegenRateRep();
};

class AiManager
{
public:
    Vector3 GetTargetPosition();
    bool IsMoving();
    int GetCurrentSegment();
    Vector3 GetPathStart();
    Vector3 GetPathEnd();
    int GetSegmentsCount();
    float GetDashSpeed();
    bool IsDashing();
    Vector3 GetPosition();
public:
    Vector3 GetSegment(int index);
    std::vector<Vector3> GetFutureSegments();
};

class CharacterData
{
public:
    float GetSize();
    QWORD GetObjectTypeHash();
    QWORD GetObjectTypeHashDetailed();
};

class SpellData
{
public:
	std::string GetName();
    Vector3 GetSpellEndPos();
    float GetManaCostByLevel(int level);
};

class SpellInfo
{
public:
	SpellData* GetSpellData();
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

class SpellInput
{
public:
	void SetCaster(int index);
	void SetTarget(int index);
	void SetStartPos(Vector3 pos);
	void SetEndPos(Vector3 pos);
	void SetClickedPos(Vector3 pos);
	void SetUnkPos(Vector3 pos);
	void SetReleasePos(Vector3 pos);
};

class Spell
{
public:
	int GetLevel();
	float GetCooldownTimer();
	float GetCooldown();
    int GetStacks();
	float GetTotalCooldown();
	float GetRelativeCooldown();
	SpellInput* GetSpellInput();
	SpellInfo* GetSpellInfo();
public:
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
public:
    bool IsAutoAttack();
};

class Buff
{
public:
    std::string GetName();
    float GetStartTime();
    float GetEndTime();
    int GetStacksAlt();
    int GetStacks();
public:
    int GetMaxStacks();
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

struct KappaManager
{
public:
    uintptr_t* vtable;
    std::map<uintptr_t, Missile*> missile_map;
};

class Object
{
public:
	unsigned int GetNetId();
	int GetTeam();
    int GetLevel();
	float GetExperience();
	Vector3 GetPosition();
    bool IsVisible();
	bool IsAlive();
    float GetMana();
    float GetMaxMana();
    float GetPercentMana();
    bool IsTargetable();
	bool IsInvulnerable();
	bool IsCursed();
    bool HasBarrier(int slotId);
    bool HasFlash(int slotId);
    bool HasCleanse(int slotId);
    bool HasExhaust(int slotId);
    bool HasGhost(int slotId);
    bool HasHeal(int slotId);
    bool HasIgnite(int slotId);
    bool HasSmite(int slotId);
    bool HasTeleport(int slotId);
    bool HasConqueror();
    float GetHealth();
    float GetMaxHealth();
	float GetPercentHealth();

    unsigned short GetActionState();
    float GetBonusAttackDamage();
    float GetAbilityPower();
    float GetBaseAttackDamage();
	float GetScale();
    float GetMovementSpeed();
	float GetAttackSpeed();
	float GetLethality();
	float GetArmor();
    float GetBonusArmor();
    float GetTotalArmor();
    float GetArmorPenetration();
	float GetMagicResist();
    float GetMagicPenetration();
    float GetMagicPenetrationMulti();
    float GetTotalMagicPenetration();
    float GetAttackRange();
    std::string GetName();
    MissileData* MissileMgr();
    BuffManager* GetBuffManager();
    QWORD* GetBuffManagerEntriesEnd();
    SpellCast* GetActiveSpellCast();
	Spell* GetSpellBySlotId(int slotId);
    Missile* GetMissileByIndex();
    CharacterData* GetCharacterData();
    AiManager* GetAiManager();
	HeroInventory* GetHeroInventory();

public:
    float GetBoundingRadius();
    float GetAttackDelay();
    float GetAttackWindup();
	bool IsCastingSpell();

public:
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
	float CharGetAttackDamage();
    float GetAttackDamage();
    float GetEffectiveHealth(int damageType);
    float GetRealAttackRange();
    float GetDistanceTo(Object* obj);
    bool IsInRange(Vector3 pos, float radius);
	bool IsUnderEnemyTower();
	bool IsUnderAllyTower();
	bool IsInAARange();
    bool CanCastSpell(int slotId);
    Vector3 GetServerPosition();
    int GetBuffListSize();
    Buff* GetBuffByName(std::string name);
public:
    CharacterDataStack* GetCharacterDataStack();
    CharacterStateIntermediate* GetCharacterStateIntermediate();
};

class ObjectManager
{
public:
    uintptr_t* vtable;
    std::map<uintptr_t, Missile*> missile_map;
    int GetListSize();
    Object* GetIndex(int index);
public:
    class iterator
    {
    private:
        ObjectManager* objectManager;
        int index;

    public:
        iterator(ObjectManager* _objectManager, int _index)
            : objectManager(_objectManager), index(_index) {}

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