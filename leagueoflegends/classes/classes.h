#pragma once

#include "../stdafx.h"

class InventorySlot
{
public:
    int GetId();
    std::string GetTexturePath();
    std::string GetName();
};

class InventorySlotWrapper
{
public:
    InventorySlot* GetSlot();
};

class ItemListObject
{
public:
    InventorySlotWrapper* GetWrapper();
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

struct MissileManager
{
public:
    QWORD* vtable;
    std::map<QWORD, void*> missile_map;
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

class Object
{
public:
	unsigned int GetNetId();
	int GetTeam();
    int GetLevel();
    Vector3 GetPosition();
    bool IsVisible();
	bool IsAlive();
    float GetMana();
    bool IsTargetable();
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
    float GetPercentHealth(Object* obj);

    unsigned short GetActionState();
    float GetBonusAttackDamage();
    float GetAbilityPower();
    float GetBaseAttackDamage();
	float GetScale();
    float GetMovementSpeed();
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
    BuffManager* GetBuffManager();
    QWORD* GetBuffManagerEntriesEnd();
    SpellCast* GetActiveSpellCast();
	Spell* GetSpellBySlotId(int slotId);
    CharacterData* GetCharacterData();
    AiManager* GetAiManager();
    InventorySlot* GetInventorySlotById(int slotId);
public:
    float GetBoundingRadius();
    float GetAttackDelay();
    float GetAttackWindup();
public:
    bool CanAttack();
    bool CanCast();
    bool CanMove();
    bool IsEnemy();
    bool IsAlly();
    bool IsJungle();
    bool IsValidTarget();
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
    float CharGetAttackDamage();
    float GetAttackDamage();
    float GetEffectiveHealth(int damageType);
    float GetRealAttackRange();
    float GetDistanceTo(Object* obj);
    bool IsInRange(Vector3 pos, float radius);
    bool IsInAARange();
    bool CanCastSpell(int slotId);
    Vector3 GetServerPosition();
    int GetBuffListSize();
    Buff* GetBuffByName(std::string name);
public:
    CharacterDataStack* GetCharacterDataStack();
};

class ObjectManager
{
public:
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