#include "../Awareness.h"
#include "../stdafx.h"
#include "../TargetSelector.h"

int InventorySlot::GetId()
{
	const auto item_id = reinterpret_cast<uintptr_t>(this) + oInventorySlotItemId;
	const auto pointer = *reinterpret_cast<int*>(item_id);
	return pointer;
}

std::string InventorySlot::GetTexturePath()
{
	const auto item_texture_path = reinterpret_cast<uintptr_t>(this) + oInventorySlotItemTexturePath;
	const auto pointer = *reinterpret_cast<char**>(item_texture_path);
	if (IsValidPtr(pointer))
		return pointer;

	LOG("GetTexturePath Doesnt work");
	return "";
}

std::string InventorySlot::GetName()
{
	const auto item_name = reinterpret_cast<uintptr_t>(this) + oInventorySlotItemName;
	const auto pointer = *reinterpret_cast<char**>(item_name);
	if (IsValidPtr(pointer))
		return pointer;

	LOG("GetName Doesnt work");
	return "";
}

InventorySlot* ItemListObject::GetSlot()
{
	const auto item_slot = reinterpret_cast<uintptr_t>(this) + oInventorySlot;
	const auto pointer_item_slot = *reinterpret_cast<uintptr_t*>(item_slot);
	const auto pointer = reinterpret_cast<InventorySlot*>(pointer_item_slot);
	if (IsValidPtr(pointer)) 
		return pointer;

	LOG("GetSlot Doesnt work");
	return nullptr;
}

InventorySlot* Object::GetInventorySlotById(int slotId)
{
	const auto hero_inventory = reinterpret_cast<QWORD>(this) + 0x4040;
	const auto inventory_slot = *reinterpret_cast<QWORD*>(hero_inventory) + itemListObjectSize * slotId + oInventorySlotWrapper;
	const auto pointer_to_slot = *reinterpret_cast<QWORD*>(inventory_slot);

	if (auto list_obj = reinterpret_cast<ItemListObject*>(pointer_to_slot); IsValidPtr(list_obj))
	{
		InventorySlot* wrapper = list_obj->GetSlot();
		return wrapper;
	}

	LOG("GetInventorySlotById Doesnt work");
	return nullptr;
}

void CharacterDataStack::Update(bool change)
{
	typedef void(__thiscall* fnUpdate)(QWORD, bool);
	const auto _update = reinterpret_cast<fnUpdate>(globals::moduleBase + oCharacterDataStackUpdate);
	spoof_call(functions::spoof_trampoline, _update, reinterpret_cast<QWORD>(this), change);
}

Vector3 AiManager::GetTargetPosition()
{
	return functions::ReadVector3((QWORD)this + oObjAiManagerManagerTargetPosition);
}

bool AiManager::IsMoving()
{
	return *(bool*)((QWORD)this + oObjAiManagerManagerIsMoving);
}

int AiManager::GetCurrentSegment()
{
	return *(int*)((QWORD)this + oObjAiManagerManagerCurrentSegment);
}

Vector3 AiManager::GetPathStart()
{
	return functions::ReadVector3((QWORD)this + oObjAiManagerManagerPathStart);
}

Vector3 AiManager::GetPathEnd()
{
	return functions::ReadVector3((QWORD)this + oObjAiManagerManagerPathEnd);
}

int AiManager::GetSegmentsCount()
{
	return *(int*)((QWORD)this + oObjAiManagerManagerSegmentsCount);
}

float AiManager::GetDashSpeed()
{
	return *(float*)((QWORD)this + oObjAiManagerManagerDashSpeed);
}

bool AiManager::IsDashing()
{
	return *(bool*)((QWORD)this + oObjAiManagerManagerIsDashing);
}

Vector3 AiManager::GetPosition()
{
	return functions::ReadVector3((QWORD)this + oObjAiManagerManagerPosition);
}

Vector3 AiManager::GetSegment(int index)
{
	return functions::ReadVector3(*(QWORD*)((QWORD)this + oObjAiManagerManagerSegments) + (min(index, this->GetSegmentsCount() - 1) * sizeof(Vector3)));
}

std::vector<Vector3> AiManager::GetFutureSegments()
{
	std::vector<Vector3> segments = {};
	int segmentsCount = this->GetSegmentsCount();
	for (int i = this->GetCurrentSegment(); i < segmentsCount; i++)
	{
		segments.push_back(this->GetSegment(i));
	}
	return segments;
}

float CharacterData::GetSize()
{
	return *(float*)((QWORD)this + oObjCharDataDataSize);
}

QWORD CharacterData::GetObjectTypeHash()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + oObjCharDataDataObjType));
}

QWORD CharacterData::GetObjectTypeHashDetailed()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + oObjCharDataDataObjType) + oObjCharDataDataObjTypeDetailed);
}

std::string SpellData::GetName()
{
	return *(char**)((QWORD)this + oSpellDataSpellName);
}

float SpellData::GetManaCostByLevel(int level)
{
	return *(float*)((QWORD)this + oSpellDataManaCost + ((level - 1) * sizeof(float)));
}

SpellData* SpellInfo::GetSpellData()
{
	return *(SpellData**)((QWORD)this + oSpellInfoSpellData);
}

void SpellInput::SetCaster(int index)
{
	*reinterpret_cast<uintptr_t*>((QWORD)this + 0x10) = index;
}

void SpellInput::SetTarget(int index)
{
	*reinterpret_cast<uintptr_t*>((QWORD)this + 0x14) = index;
}

void SpellInput::SetStartPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + oSpellInputStartPos) = pos;
}

void SpellInput::SetEndPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + oSpellInputEndPos) = pos;
}

void SpellInput::SetClickedPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + oSpellInputEndPos + sizeof(Vector3)) = pos;
}

void SpellInput::SetUnkPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + oSpellInputEndPos + sizeof(Vector3) * 2) = pos;
}

int Spell::GetLevel()
{
	return *(int*)((QWORD)this + oSpellSlotLevel);
}

float Spell::GetCooldownTimer()
{
	return *(float*)((QWORD)this + oSpellSlotCooldown);
}

float Spell::GetCooldown()
{
	float cooldownTimer = this->GetCooldownTimer();
	if (cooldownTimer == 0.0f) return cooldownTimer;
	return max(cooldownTimer - functions::GetGameTime(), 0.0f);
}

int Spell::GetStacks()
{
	return *(int*)((QWORD)this + oSpellSlotStacks);
}

float Spell::GetTotalCooldown()
{
	return *(float*)((QWORD)this + oSpellSlotTotalCooldown);
}

float Spell::GetRelativeCooldown()
{
	if (!this->GetLevel()) return 1.0f;
	if (this->GetLevel() && this->GetCooldownTimer() < 1.0f) return 0.0f;
	return this->GetTotalCooldown() ? min(this->GetCooldown() / this->GetTotalCooldown(), 1.0f) : 0.0f;
}

SpellInput* Spell::GetSpellInput()
{
	return *(SpellInput**)((QWORD)this + oSpellSlotSpellInput);
}

SpellInfo* Spell::GetSpellInfo()
{
	return *(SpellInfo**)((QWORD)this + oSpellSlotSpellInfo);
}

bool Spell::IsReady()
{
	return this->GetRelativeCooldown() == 0.0f;
};

std::string Spell::GetName()
{
	return this->GetSpellInfo()->GetSpellData()->GetName();
}

float Spell::GetManaCost()
{
	return this->GetSpellInfo()->GetSpellData()->GetManaCostByLevel(this->GetLevel());
}

SpellInfo* SpellCast::GetSpellInfo()
{
	return *(SpellInfo**)((QWORD)this + oActiveSpellCastSpellInfo);
}

int SpellCast::GetSpellId()
{
	return *(int*)((QWORD)this + oActiveSpellCastSpellId);
}

bool SpellCast::IsAutoAttack()
{
	return *(int*)((QWORD)this + oActiveSpellCastSpellType) == -1;
}

std::string Buff::GetName()
{
	QWORD* namePtr = (QWORD*)(*(QWORD*)((QWORD)this + oBuffNamePtr));
	if (!IsValidPtr(namePtr)) return "";
	return (char*)((QWORD)namePtr + oBuffNamePtrName);
}

float Buff::GetStartTime()
{
	return *(float*)((QWORD)this + oBuffStartTime);
}

float Buff::GetEndTime()
{
	return *(float*)((QWORD)this + oBuffEndTime);
}

int Buff::GetStacksAlt()
{
	return *(int*)((QWORD)this + oBuffStacksAlt);
}

int Buff::GetStacks()
{
	return *(int*)((QWORD)this + oBuffStacks);
}

int Buff::GetMaxStacks()
{
	return max(this->GetStacksAlt(), this->GetStacks());
}

Buff* BuffEntry::GetBuff()
{
	Buff* buff = (Buff*)(*(QWORD*)((QWORD)this + oBuffEntryBuff));
	if (!IsValidPtr(buff)) return nullptr;
	return buff;
}

BuffEntry* BuffManager::GetBuffEntryByIndex(int index)
{
	BuffEntry* address = (BuffEntry*)(*(QWORD*)((QWORD)this + (sizeof(QWORD) * index)));
	if (!IsValidPtr(address)) return nullptr;
	return address;
}

unsigned int Object::GetNetId()
{
	return *(int*)((QWORD)this + oObjNetId);
}

int Object::GetTeam()
{
	return *(int*)((QWORD)this + oObjTeam);
}

int Object::GetLevel()
{
	return *(int*)((QWORD)this + oObjLevel);
}

float Object::GetExperience()
{
	return *(float*)((QWORD)this + oObjExperience);
}

Vector3 Object::GetPosition()
{
	return functions::ReadVector3((QWORD)this + oObjPosition);
}

bool Object::IsVisible()
{
	//return functions::IsCanSee(this);
	return *(bool*)((QWORD)this + oObjVisible);
}

bool Object::IsAlive()
{
	//return !functions::IsDead(this);
	return !(*(int*)((QWORD)this + oObjAlive) % 2);
}

float Object::GetMana()
{
	return *(float*)((QWORD)this + oObjMana);
}

float Object::GetMaxMana()
{
	return *(float*)((QWORD)this + oObjMaxMana);
}

float Object::GetPercentMana()
{
	return 100 * this->GetMana() / this->GetMaxMana() > 100 ? 100 : 100 * this->GetMana() / this->GetMaxMana();
}

bool Object::IsTargetable()
{
	//return functions::IsTargetable(this);
	return *(bool*)((QWORD)this + oObjTargetable);
}

bool Object::IsCursed()
{
	return this->GetBuffByName("cursedtouch");
}

bool Object::HasBarrier(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Barrier")
		return true;

	return false;
}

bool Object::HasFlash(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Flash")
		return true;

	return false;
}

bool Object::HasCleanse(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Boost")
		return true;

	return false;
}

bool Object::HasExhaust(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Exhaust")
		return true;

	return false;
}

bool Object::HasGhost(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Haste")
		return true;

	return false;
}

bool Object::HasHeal(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Heal")
		return true;

	return false;
}

bool Object::HasIgnite(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (RemoveSubstring(spell->GetName(), "Summoner") == "Dot")
		return true;

	return false;
}

bool Object::HasSmite(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (spell->GetName().contains("Smite"))
		return true;

	return false;
}

bool Object::HasTeleport(int slotId)
{
	Spell* spell = this->GetSpellBySlotId(slotId);

	if (spell->GetName().contains("Teleport"))
		return true;

	return false;
}

bool Object::HasConqueror()
{
	return this->GetBuffByName("ASSETS/Perks/Styles/Precision/Conqueror/ConquerorEnrage.lua");
}

float Object::GetHealth()
{
	return *(float*)((QWORD)this + oObjHealth);
}

float Object::GetMaxHealth()
{
	return *(float*)((QWORD)this + oObjMaxHealth);
}

float Object::GetPercentHealth()
{
	return 100 * this->GetHealth() / this->GetMaxHealth() > 100 ? 100 : 100 * this->GetHealth() / this->GetMaxHealth();
}

unsigned short Object::GetActionState()
{
	return *(unsigned short*)((QWORD)this + oObjActionState);
}

float Object::GetBaseAttackDamage()
{
	return *(float*)((QWORD)this + oObjBaseAttackDamage);
}

float Object::GetAbilityPower()
{
	return *(float*)((QWORD)this + oObjAbilityPower);
}

float Object::GetBonusAttackDamage()
{
	return *(float*)((QWORD)this + oObjBonusAttackDamage);
}

float Object::GetScale()
{
	return *(float*)((QWORD)this + oObjScale);
}

float Object::GetMovementSpeed()
{
	return *(float*)((QWORD)this + oObjMovementSpeed);
}

float Object::GetLethality()
{
	return *(float*)((QWORD)this + oObjLethality);
}

float Object::GetArmor()
{
	return *(float*)((QWORD)this + oObjArmor);
}

float Object::GetBonusArmor()
{
	return *(float*)((QWORD)this + oObjBonusArmor);
}

float Object::GetTotalArmor()
{
	return this->GetArmor() + this->GetBonusArmor();
}

float Object::GetArmorPenetration()
{
	return *(float*)((QWORD)this + oObjArmorPen);
}

float Object::GetMagicResist()
{
	return *(float*)((QWORD)this + oObjMagicResist);
}

float Object::GetMagicPenetration()
{
	return *(float*)((QWORD)this + oObjMagicPen);
}

float Object::GetMagicPenetrationMulti()
{
	return *(float*)((QWORD)this + oObjMagicPenMulti);
}


float Object::GetTotalMagicPenetration()
{
	return this->GetMagicPenetration() + this->GetMagicPenetrationMulti();
}

float Object::GetAttackRange()
{
	return *(float*)((QWORD)this + oObjAttackRange) ;
}

std::string Object::GetName()
{
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + oObjName);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

BuffManager* Object::GetBuffManager()
{
	return (BuffManager*)(*(QWORD*)((QWORD)this + oObjBuffManager));
}

QWORD* Object::GetBuffManagerEntriesEnd()
{
	return *(QWORD**)((QWORD)this + oObjBuffManager + oObjBuffManagerEntriesEnd);
}

SpellCast* Object::GetActiveSpellCast()
{
	QWORD* activeSpellCastOffset = *(QWORD**)((QWORD)this + oObjActiveSpellCast);
	if (!IsValidPtr(activeSpellCastOffset)) return nullptr;
	return (SpellCast*)activeSpellCastOffset;
}

CharacterData* Object::GetCharacterData()
{
	return (CharacterData*)(*(QWORD*)(*(QWORD*)((QWORD)this + oObjCharData) + oObjCharDataData));
}

AiManager* Object::GetAiManager()
{
	LeagueObfuscation<QWORD> aiManagerObf = *(LeagueObfuscation<QWORD>*)((QWORD)this + oObjAiManager);
	return (AiManager*)(*(QWORD*)(Decrypt(aiManagerObf) + 0x10));
}

Spell* Object::GetSpellBySlotId(int slotId)
{
	return *(Spell**)((QWORD)this + oObjSpellBook + oObjSpellBookSpellSlot + (sizeof(QWORD) * slotId));
}

bool isASCII(const std::string& s)
{
	return !std::any_of(s.begin(), s.end(), [](char c) {
		return static_cast<unsigned char>(c) > 0x80;
		});
}

std::string MissileData::GetMissileName()
{
	const auto missileDataPtr = *reinterpret_cast<MissileData**>(reinterpret_cast<QWORD>(this) + oMissileName);
	if (IsValidPtr(missileDataPtr))
	{
		const char* textToReturnPtr = reinterpret_cast<char*>(missileDataPtr);
		return textToReturnPtr;
	}

	return "";
}

bool MissileData::IsAutoAttack()
{
	return this->GetMissileName().contains("Attack") || this->GetSpellName().contains("Attack");
}

std::string MissileData::GetSpellName()
{
	const auto missileDataPtr = *reinterpret_cast<MissileData**>(reinterpret_cast<QWORD>(this) + oMissileSpellName);
	if (IsValidPtr(missileDataPtr))
	{
		const char* textToReturnPtr = reinterpret_cast<char*>(missileDataPtr);
		return textToReturnPtr;
	}

	return "";
}

MissileData* Missile::GetMissileData()
{
	return *(MissileData**)((QWORD)this + oMissileSpellInfo);
}

Missile* Object::GetMissileByIndex()
{
	return *(Missile**)((QWORD)this + oMissileMapVal);
}

int Missile::GetMissileSrcId()
{
	return *(int*)((QWORD)this + oMissileSrcIdx);
}

Vector3 Missile::GetSpellStartPos()
{
	return functions::ReadVector3((QWORD)this + oMissileStartPos);
}

Vector3 Missile::GetSpellPos()
{
	return functions::ReadVector3((QWORD)this + oMissilePos);
}

Vector3 Missile::GetSpellEndPos()
{
	return functions::ReadVector3((QWORD)this + oMissileEndPos);
}

float Object::GetBoundingRadius()
{
	typedef float(__fastcall* fnGetBoundingRadius)(Object* obj);
	fnGetBoundingRadius _fnGetBoundingRadius = (fnGetBoundingRadius)(globals::moduleBase + oGetBoundingRadius);
	return _fnGetBoundingRadius(this);
}

float Object::GetAttackDelay()
{
	typedef float(__cdecl* fnGetAttackDelay)(Object* obj);
	fnGetAttackDelay _fnGetAttackDelay = (fnGetAttackDelay)(globals::moduleBase + oGetAttackDelay);
	return _fnGetAttackDelay(this);
}

float Object::GetAttackWindup()
{
	typedef float(__cdecl* fnGetAttackWindup)(Object* obj, int flags);
	fnGetAttackWindup _fnGetAttackWindup = (fnGetAttackWindup)(globals::moduleBase + oGetAttackWindup);
	return _fnGetAttackWindup(this, 0x40);
}

bool Object::CanAttack()
{
	return this->GetActionState() & CharacterState::CanAttack;
}

bool Object::CanCast()
{
	return this->GetActionState() & CharacterState::CanCast;
}

bool Object::CanMove()
{
	return this->GetActionState() & CharacterState::CanMove;
}

bool Object::IsEnemy()
{
	return this->GetTeam() != globals::localPlayer->GetTeam();
}

bool Object::IsAlly()
{
	return this->GetTeam() == globals::localPlayer->GetTeam();
}

bool Object::IsJungle()
{
	return this->GetTeam() == 300;
}

bool Object::IsValidTarget()
{
	return this->IsVisible() && this->IsAlive() && this->IsEnemy() && this->IsTargetable();
}

bool Object::IsRespawnMarker()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::RespawnMarker;
}

bool Object::IsHero()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Champion;
}

bool Object::IsSpecial()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Special;
}

bool Object::IsWard()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Ward;
}

bool Object::IsMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Minion_Lane;
}

bool Object::IsSiegeMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Minion_Lane_Siege;
}

bool Object::IsVoidMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Special_Void;
}

bool Object::IsRangedMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Minion_Lane_Ranged;
}

bool Object::IsMeleeMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Minion_Lane_Melee;
}

bool Object::IsSuperMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Minion_Lane_Super;
}

bool Object::IsMonster()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Monster;
}

bool Object::IsDragon()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Monster_Dragon;
}

bool Object::IsEpicMonster()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Monster_Epic;
}

bool Object::IsTurret()
{
	return this->GetCharacterData()->GetObjectTypeHash() == ObjectType::Structure_Turret;
}

float Object::CharGetAttackDamage()
{
	return *(float*)((QWORD)this + oBaseAttackDamage);
}

float Object::GetAttackDamage()
{
	return this->GetBaseAttackDamage() + this->GetBonusAttackDamage();
}

float Object::GetEffectiveHealth(int damageType)
{
	if (damageType == DamageType::True) return this->GetHealth();
	return this->GetHealth() * (1 + (damageType == DamageType::Physical ? this->GetArmor() : this->GetAbilityPower()) / 100);
}

float Object::GetRealAttackRange()
{
	return this->GetAttackRange() + this->GetBoundingRadius();
}

float Object::GetDistanceTo(Object* obj)
{
	return this->GetPosition().Distance(obj->GetPosition());
}

bool Object::IsInRange(Vector3 pos, float radius)
{
	return radius + this->GetBoundingRadius() >= render::Distance(pos, this->GetPosition());
}

bool Object::IsUnderEnemyTower()
{
	const auto turret = UPasta::SDK::TargetSelector::Functions::GetEnemyTurretInRange(2000.0f);
	if (turret && turret->IsEnemy())
		return this->IsInRange(turret->GetPosition(), 992.0f);

	return false;
}

bool Object::IsUnderAllyTower()
{
	const auto turret = UPasta::SDK::TargetSelector::Functions::GetAllyTurretInRange(2000.0f);
	if (turret && turret->IsAlly())
		return this->IsInRange(turret->GetPosition(), 992.0f);

	return false;
}

bool Object::IsInAARange()
{
	return globals::localPlayer->GetRealAttackRange() + globals::localPlayer->GetBoundingRadius() >= render::Distance(globals::localPlayer->GetPosition(), this->GetPosition());
}

bool Object::CanCastSpell(int slotId)
{
	auto spell = this->GetSpellBySlotId(slotId);
	return this->CanCast() && functions::GetSpellState(slotId) == SpellState::IsReady;
}

Vector3 Object::GetServerPosition()
{
	if (this->IsHero())
	{
		auto pos = this->GetAiManager()->GetPosition();
		if (pos.IsValid()) return pos;
	}

	return this->GetPosition();
}

int Object::GetBuffListSize()
{
	return (int)((QWORD)this->GetBuffManagerEntriesEnd() - (QWORD)this->GetBuffManager()) / (int)sizeof(QWORD);
}

Buff* Object::GetBuffByName(std::string name)
{
	for (int i = 0; i < this->GetBuffListSize(); i++)
	{
		auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= functions::GetGameTime() && buff->GetName() == name) return buff;
	}
	return nullptr;
}

CharacterDataStack* Object::GetCharacterDataStack()
{
	return (CharacterDataStack*)((QWORD)this + oObjCharacterDataStack);
}

#pragma region CharacterStateIntermediate
CharacterStateIntermediate* Object::GetCharacterStateIntermediate()
{
	return (CharacterStateIntermediate*)((QWORD)this + oCharacterStateIntermediate);
}

float CharacterStateIntermediate::GetAbilityHasteMod()
{
	return *(float*)((QWORD)this + oAbilityHasteMod);
}

float CharacterStateIntermediate::GetPercentCooldownCapMod()
{
	return *(float*)((QWORD)this + oPercentCooldownCapMod);
}

float CharacterStateIntermediate::GetPassiveCooldownEndTime()
{
	return *(float*)((QWORD)this + oPassiveCooldownEndTime);
}

float CharacterStateIntermediate::GetPassiveCooldownTotalTime()
{
	return *(float*)((QWORD)this + oPassiveCooldownTotalTime);
}

float CharacterStateIntermediate::GetFlatPhysicalDamageMod()
{
	return *(float*)((QWORD)this + oFlatPhysicalDamageMod);
}

float CharacterStateIntermediate::GetPercentPhysicalDamageMod()
{
	return *(float*)((QWORD)this + oPercentPhysicalDamageMod);
}

float CharacterStateIntermediate::GetPercentBonusPhysicalDamageMod()
{
	return *(float*)((QWORD)this + oPercentBonusPhysicalDamageMod);
}

float CharacterStateIntermediate::GetPercentBasePhysicalDamageAsFlatBonusMod()
{
	return *(float*)((QWORD)this + oPercentBasePhysicalDamageAsFlatBonusMod);
}

float CharacterStateIntermediate::GetFlatMagicDamageMod()
{
	return *(float*)((QWORD)this + oFlatMagicDamageMod);
}

float CharacterStateIntermediate::GetPercentMagicDamageMod()
{
	return *(float*)((QWORD)this + oPercentMagicDamageMod);
}

float CharacterStateIntermediate::GetFlatMagicReduction()
{
	return *(float*)((QWORD)this + oFlatMagicReduction);
}

float CharacterStateIntermediate::GetPercentDamageToBarracksMinionMod()
{
	return *(float*)((QWORD)this + oGetPercentDamageToBarracksMinionMod);
}

float CharacterStateIntermediate::GetFlatDamageReductionFromBarracksMinionMod()
{
	return *(float*)((QWORD)this + oGetFlatDamageReductionFromBarracksMinionMod);
}

float CharacterStateIntermediate::GetPercentMagicReduction()
{
	return *(float*)((QWORD)this + oPercentMagicReduction);
}

float CharacterStateIntermediate::GetFlatCastRangeMod()
{
	return *(float*)((QWORD)this + oFlatCastRangeMod);
}

float CharacterStateIntermediate::GetAttackSpeedMod()
{
	return *(float*)((QWORD)this + oAttackSpeedMod);
}

float CharacterStateIntermediate::GetPercentAttackSpeedMod()
{
	return *(float*)((QWORD)this + oPercentAttackSpeedMod);
}

float CharacterStateIntermediate::GetPercentMultiplicativeAttackSpeedMod()
{
	return *(float*)((QWORD)this + oPercentMultiplicativeAttackSpeedMod);
}

float CharacterStateIntermediate::GetBaseAttackDamage()
{
	return *(float*)((QWORD)this + oBaseAttackDamage);
}

float CharacterStateIntermediate::GetBaseAttackDamageSansPercentScale()
{
	return *(float*)((QWORD)this + oBaseAttackDamageSansPercentScale);
}

float CharacterStateIntermediate::GetFlatBaseAttackDamageMod()
{
	return *(float*)((QWORD)this + oFlatBaseAttackDamageMod);
}

float CharacterStateIntermediate::GetPercentBaseAttackDamageMod()
{
	return *(float*)((QWORD)this + oPercentBaseAttackDamageMod);
}

float CharacterStateIntermediate::GetBaseAbilityDamage()
{
	return *(float*)((QWORD)this + oBaseAbilityDamage);
}

float CharacterStateIntermediate::GetCritDamageMultiplier()
{
	return *(float*)((QWORD)this + oCritDamageMultiplier);
}

float CharacterStateIntermediate::GetScaleSkinCoef()
{
	return *(float*)((QWORD)this + oScaleSkinCoef);
}

float CharacterStateIntermediate::GetDodge()
{
	return *(float*)((QWORD)this + oDodge);
}

float CharacterStateIntermediate::GetCritPercent()
{
	return *(float*)((QWORD)this + oCritPercent);
}

float CharacterStateIntermediate::GetFlatBaseHPPoolMod()
{
	return *(float*)((QWORD)this + oFlatBaseHPPoolMod);
}

float CharacterStateIntermediate::GetArmor()
{
	return *(float*)((QWORD)this + oArmor);
}

float CharacterStateIntermediate::GetBonusArmor()
{
	return *(float*)((QWORD)this + oBonusArmor);
}

float CharacterStateIntermediate::GetSpellBlock()
{
	return *(float*)((QWORD)this + oSpellBlock);
}

float CharacterStateIntermediate::GetBonusSpellBlock()
{
	return *(float*)((QWORD)this + oBonusSpellBlock);
}

float CharacterStateIntermediate::GetHPRegenRate()
{
	return *(float*)((QWORD)this + oHPRegenRate);
}

float CharacterStateIntermediate::GetBaseHPRegenRate()
{
	return *(float*)((QWORD)this + oBaseHPRegenRate);
}

float CharacterStateIntermediate::GetMoveSpeed()
{
	return *(float*)((QWORD)this + oMoveSpeed);
}

float CharacterStateIntermediate::GetAttackRange()
{
	return *(float*)((QWORD)this + oAttackRange);
}

float CharacterStateIntermediate::GetFlatBubbleRadiusMod()
{
	return *(float*)((QWORD)this + oFlatBubbleRadiusMod);
}

float CharacterStateIntermediate::GetPercentBubbleRadiusMod()
{
	return *(float*)((QWORD)this + oPercentBubbleRadiusMod);
}

float CharacterStateIntermediate::GetFlatArmorPenetration()
{
	return *(float*)((QWORD)this + oFlatArmorPenetration);
}

float CharacterStateIntermediate::GetPhysicalLethality()
{
	return *(float*)((QWORD)this + oPhysicalLethality);
}

float CharacterStateIntermediate::GetPercentArmorPenetration()
{
	return *(float*)((QWORD)this + oPercentArmorPenetration);
}

float CharacterStateIntermediate::GetPercentBonusArmorPenetration()
{
	return *(float*)((QWORD)this + oPercentBonusArmorPenetration);
}

float CharacterStateIntermediate::GetPercentCritBonusArmorPenetration()
{
	return *(float*)((QWORD)this + oPercentCritBonusArmorPenetration);
}

float CharacterStateIntermediate::GetPercentCritTotalArmorPenetration()
{
	return *(float*)((QWORD)this + oPercentCritTotalArmorPenetration);
}

float CharacterStateIntermediate::GetFlatMagicPenetration()
{
	return *(float*)((QWORD)this + oFlatMagicPenetration);
}

float CharacterStateIntermediate::GetMagicLethality()
{
	return *(float*)((QWORD)this + oMagicLethality);
}

float CharacterStateIntermediate::GetPercentMagicPenetration()
{
	return *(float*)((QWORD)this + oPercentMagicPenetration);
}

float CharacterStateIntermediate::GetPercentBonusMagicPenetration()
{
	return *(float*)((QWORD)this + oPercentBonusMagicPenetration);
}

float CharacterStateIntermediate::GetPercentLifeStealMod()
{
	return *(float*)((QWORD)this + oPercentLifeStealMod);
}

float CharacterStateIntermediate::GetPercentSpellVampMod()
{
	return *(float*)((QWORD)this + oPercentSpellVampMod);
}

float CharacterStateIntermediate::GetPercentOmnivampMod()
{
	return *(float*)((QWORD)this + oPercentOmnivampMod);
}

float CharacterStateIntermediate::GetPercentPhysicalVamp()
{
	return *(float*)((QWORD)this + oPercentPhysicalVamp);
}

float CharacterStateIntermediate::GetPathfindingRadiusMod()
{
	return *(float*)((QWORD)this + oPathfindingRadiusMod);
}

float CharacterStateIntermediate::GetPercentCCReduction()
{
	return *(float*)((QWORD)this + oPercentCCReduction);
}

float CharacterStateIntermediate::GetPercentEXPBonus()
{
	return *(float*)((QWORD)this + oPercentEXPBonus);
}

float CharacterStateIntermediate::GetFlatBaseArmorMod()
{
	return *(float*)((QWORD)this + oFlatBaseArmorMod);
}

float CharacterStateIntermediate::GetFlatBaseSpellBlockMod()
{
	return *(float*)((QWORD)this + oFlatBaseSpellBlockMod);
}

float CharacterStateIntermediate::GetPrimaryARBaseRegenRateRep()
{
	return *(float*)((QWORD)this + oPrimaryARBaseRegenRateRep);
}

float CharacterStateIntermediate::GetSecondaryARRegenRateRep()
{
	return *(float*)((QWORD)this + oSecondaryARRegenRateRep);
}

float CharacterStateIntermediate::GetSecondaryARBaseRegenRateRep()
{
	return *(float*)((QWORD)this + oSecondaryARBaseRegenRateRep);
}

#pragma endregion

int ObjectManager::GetListSize()
{
	return *(int*)((QWORD)this + oManagerListSize);
}

Object* ObjectManager::GetIndex(int index)
{
	return *(Object**)(*(QWORD*)((QWORD)this + oManagerList) + (sizeof(QWORD) * index));
}

Vector3 SpellData::GetSpellEndPos()
{
	return functions::ReadVector3((QWORD)this + oMissileDestIdx);
}

