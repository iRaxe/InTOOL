#include "../Awareness.h"
#include "../ListManager.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../ObjectTypeHolder.h"
#include "../ObjManager.h"

class AttackableUnit;


std::string_view Spell::GetTextureName()
{
	std::string_view path = this->GetSpellInfo()->GetSpellData()->GetTexturePath();
	constexpr std::string_view keyword = "Icons2D/";

	std::string_view targetPath = path;

	const size_t pos = path.find(keyword);
	if (pos != std::string_view::npos)
	{
		targetPath = path.substr(pos + keyword.length());
	}

	const size_t lastDot = targetPath.rfind('.');
	if (lastDot != std::string_view::npos)
	{
		return targetPath.substr(0, lastDot);
	}

	return targetPath;
}

std::string InventorySlot::GetTexturePath()
{
	const auto item_texture_path = reinterpret_cast<uintptr_t>(this) + UPasta::Offsets::ItemManager::ItemTexturePath;
	const auto pointer = *reinterpret_cast<char**>(item_texture_path);
	if (IsValidPtr(pointer))
		return pointer;

	LOG("GetTexturePath Doesnt work");
	return "";
}

std::string InventorySlot::GetName()
{
	const auto item_name = *(char**)(*(QWORD*)this + UPasta::Offsets::ItemManager::ItemName);
	if (IsValidPtr(item_name))
	{
		return item_name;
	}

	LOG("GetName Doesnt work");
	return "";
}

ItemsDatabase InventorySlot::GetId()
{
	const auto item_id = *(ItemsDatabase*)(*(QWORD*)this + UPasta::Offsets::ItemManager::ItemId);
	return item_id;
}

std::string SpellData::GetName()
{
	return *(char**)((QWORD)this + UPasta::Offsets::SpellBook::SpellSlot::Name);
}

std::string SpellData::GetTexturePath()
{
	const auto TextureInfoPtr = this + UPasta::Offsets::SpellInfo::TexturePath;
	const auto TexturePtr = *(char**)TextureInfoPtr;
	const auto TextureInfo = *(char**)TexturePtr;

	return TextureInfo;
}

InventorySlot* HeroInventory::GetInventorySlot(int slotID)
{
	const auto inventory_slot = (ItemListObject*)(*(QWORD*)this + (UPasta::Offsets::ItemManager::Size * slotID) +
		UPasta::Offsets::ItemManager::Wrapper);
	if (IsNotZeroPtr(*(QWORD*)(inventory_slot)))
	{
		InventorySlot* wrapper = inventory_slot->GetSlot();

		if (IsNotZeroPtr(wrapper) && IsValidPtr(wrapper) && wrapper != nullptr)
			return wrapper;
	}
	return nullptr;
}

InventorySlot* HeroInventory::FindItemID(ItemsDatabase itemID)
{
	const std::vector<InventorySlot*> items = ItemsList();
	for (auto* item : items)
	{
		if (item != nullptr && item->GetId() == itemID)
		{
			return item;
		}
	}
	return nullptr;
}

std::vector<InventorySlot*> HeroInventory::ItemsList()
{
	std::unordered_map<ItemsDatabase, size_t> itemToSlotMap;
	std::vector<InventorySlot*> rs;

	for (size_t i = 0; i < 7; i++)
	{
		auto itemPtr = GetInventorySlot(i);
		if (itemPtr != nullptr)
		{
			auto found = itemToSlotMap.find(itemPtr->GetId());
			if (found != itemToSlotMap.end())
			{
				rs[found->second] = itemPtr;
			}
			else
			{
				itemToSlotMap[itemPtr->GetId()] = rs.size();
				rs.push_back(itemPtr);
			}
		}
	}
	return rs;
}

Perk* Perks::GetPerkByIndex(int index)
{
	auto perkSlot = (Perk*)(*(QWORD*)(*(QWORD*)this + (UPasta::Offsets::Perks::Size * index)));

	if (IsNotZeroPtr(perkSlot))
		return perkSlot;

	return nullptr;
}


void CharacterDataStack::Update(bool change)
{
	using fnUpdate = void(__thiscall*)(QWORD, bool);
	const auto _update = reinterpret_cast<fnUpdate>(RVA(UPasta::Offsets::Functions::Skin::CharacterDataStackUpdate));
	spoof_call(Engine::spoof_trampoline, _update, reinterpret_cast<QWORD>(this), change);
}

std::vector<Vector3> AiManager::GetFutureSegments()
{
	std::vector<Vector3> segments = {};
	const int segmentsCount = this->GetSegmentsCount();
	for (int i = this->GetCurrentSegment(); i < segmentsCount; i++)
	{
		segments.push_back(this->GetSegment(i));
	}
	return segments;
}

Vector3 AiManager::GetSegment(int index)
{
	return Engine::ReadVector3(
		ReadQWORD(this, UPasta::Offsets::AIManager::Path::NavigationPoints) + (min(index, this->GetSegmentsCount() - 1)
			* sizeof(Vector3)));
}

Vector3 AiManager::GetPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::ServerPosition);
}

Vector3 AiManager::GetTargetPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::TargetPos);
}

Vector3 AiManager::GetMouseClickPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::MouseClickPosition);
}

Vector3 AiManager::GetPathStart()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::CurrentPathStartPosition);
}

Vector3 AiManager::GetDashStart()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::DashStartPosition);
}

Vector3 AiManager::GetPathEnd()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::CurrentPathEndPosition);
}

Vector3 AiManager::GetDirection()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::Direction);
}

QWORD CharacterData::GetObjectTypeHash()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::CharData::Type));
}

QWORD CharacterData::GetObjectTypeHashDetailed()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::CharData::Type) + UPasta::Offsets::CharData::TypeDetailed);
}

float SpellData::GetManaCostByLevel(int level)
{
	return ReadFLOAT(this, UPasta::Offsets::SpellInfo::DataManaCost + ((level - 1) * sizeof(float)));
}

void SpellInput::SetCaster(int index)
{
	ReadQWORD(this, 0x10) = index;
}

void SpellInput::SetTarget(int index)
{
	ReadQWORD(this, 0x14) = index;
}

void SpellInput::SetStartPos(Vector3 pos)
{
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::StartPosition) = pos;
}

void SpellInput::SetEndPos(Vector3 pos)
{
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::EndPosition) = pos;
}

void SpellInput::SetClickedPos(Vector3 pos)
{
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::MousePosition) = pos;
}

void SpellInput::SetUnkPos(Vector3 pos)
{
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::MousePosition2) = pos;
}

float Spell::GetCooldown()
{
	const float cooldownTimer = this->GetCooldownTimer();
	if (cooldownTimer == 0.0f) return cooldownTimer;
	return max(cooldownTimer - Engine::GetGameTime(), 0.0f);
}

Cooldown* CooldownArray::GetArrayIndex(int index)
{
	auto baseSlot = (Cooldown*)(this + (0x4 * index));

	if (IsNotZeroPtr(baseSlot))
		return baseSlot;

	return nullptr;
}

float Spell::GetRelativeCooldown()
{
	if (!this->GetLevel()) return 1.0f;
	if (this->GetLevel() && this->GetCooldownTimer() < 1.0f) return 0.0f;
	return this->GetTotalCooldown() ? min(this->GetCooldown() / this->GetTotalCooldown(), 1.0f) : 0.0f;
}

SpellInput* Spell::GetSpellInput()
{
	return *(SpellInput**)((QWORD)this + UPasta::Offsets::SpellBook::SpellSlot::SpellInputInstance);
}

float Spell::GetManaCost()
{
	return this->GetSpellInfo()->GetSpellData()->GetManaCostByLevel(this->GetLevel());
}

std::string SpellCast::GetCasterName()
{
	return ReadQWORD2(LolString, this, UPasta::Offsets::SpellCast::CasterName);
}

Vector3 SpellCast::GetStartPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::StartPosition);
}

Vector3 SpellCast::GetEndPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::EndPosition);
}

Vector3 SpellCast::GetMousePosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::MousePosition);
}

std::string Buff::GetName()
{
	auto namePtr = (QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::BuffManager::BuffNamePtr));
	if (!IsValidPtr(namePtr)) return "";
	return *(char**)((QWORD)namePtr + UPasta::Offsets::BuffManager::BuffName);
}

bool Buff::isActive()
{
	return ReadQWORD2(BYTE, this, UPasta::Offsets::BuffManager::BuffEntryBuffCountAlt)
		&& ReadQWORD2(int64_t, this, UPasta::Offsets::BuffManager::BuffNamePtr)
		|| ReadQWORD2(BYTE, this, UPasta::Offsets::BuffManager::IsPermanent);
}

Buff* BuffEntry::GetBuff()
{
	auto buff = (Buff*)(*(QWORD*)((QWORD)this + UPasta::Offsets::BuffManager::BuffEntry));
	if (!IsValidPtr(buff)) return nullptr;
	return buff;
}

BuffEntry* BuffManager::GetBuffEntryByIndex(int index)
{
	auto address = (BuffEntry*)(*(QWORD*)((QWORD)this + (sizeof(QWORD) * index)));
	if (!IsValidPtr(address)) return nullptr;
	return address;
}

Core* Core::GetBase()
{
	return (Core*)globals::moduleBase;
}

Vector3 Object::GetPosition()
{
	return Engine::ReadVector3(reinterpret_cast<uintptr_t>(this) + UPasta::Offsets::BaseObject::Position);
}

bool Object::HasSummonerSpell(SummonerSpells summSpellToFind)
{
	Spell* spellD = this->GetSpellBySlotId(D);
	Spell* spellF = this->GetSpellBySlotId(F);
	switch (summSpellToFind)
	{
	case SummonerSpells::Barrier: 
		return spellD->GetName() == "SummonerBarrier" or spellF->GetName() == "SummonerBarrier";
	case SummonerSpells::Flash:
		return spellD->GetName() == "SummonerFlash" or spellF->GetName() == "SummonerFlash";
	case SummonerSpells::Cleanse:
		return spellD->GetName() == "SummonerBoost" or spellF->GetName() == "SummonerBoost";
	case SummonerSpells::Exhaust:
		return spellD->GetName() == "SummonerExhaust" or spellF->GetName() == "SummonerExhaust";
	case SummonerSpells::Ghost:
		return spellD->GetName() == "SummonerHaste" or spellF->GetName() == "SummonerHaste";
	case SummonerSpells::Heal: 
		return spellD->GetName() == "SummonerHeal" or spellF->GetName() == "SummonerHeal";
	case SummonerSpells::Ignite:
		return spellD->GetName() == "SummonerDot" or spellF->GetName() == "SummonerDot";
	case SummonerSpells::Smite:
		return spellD->GetName().contains("Smite") or spellF->GetName().contains("Smite");
	case SummonerSpells::Teleport:
		return spellD->GetName().contains("Teleport") or spellF->GetName().contains("Teleport");
	}
	return false;
}

bool Object::IsCasting() { return Engine::call_virtual<248, bool>(this); }

CharacterData* Object::GetCharacterData()
{
	return (CharacterData*)(*(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::CharData::CharDataInstance) +
		UPasta::Offsets::CharData::Array));
}

AiManager* Object::GetAiManager()
{
	LeagueObfuscation<QWORD> aiManagerObf = *(LeagueObfuscation<QWORD>*)((QWORD)this + UPasta::Offsets::AIManager::AiManagerInstance);
	return (AiManager*)(*(QWORD*)(Decrypt(aiManagerObf) + 0x10));
}

std::string Object::GetName()
{
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + UPasta::Offsets::Client::AiName);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

std::string Object::GetClassicName()
{
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + UPasta::Offsets::Client::Name);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

Spell* Object::GetSpellBySlotId(int slotId)
{
	return *(Spell**)((QWORD)this + UPasta::Offsets::Client::SpellBookInstance +
		UPasta::Offsets::SpellBook::SpellSlotInstance + (sizeof(QWORD) * slotId));
}

std::string MissileData::GetMissileName()
{
	const auto missileName = (char*)((QWORD)this + UPasta::Offsets::MissileManager::MissileName);
	if (IsValidPtr(missileName))
		return missileName;

	LOG("GetMissileName Doesnt work");
	return "";
}

bool MissileData::IsAutoAttack()
{
	return this->GetMissileName().contains("Attack") || this->GetSpellName().contains("Attack");
}

std::string MissileData::GetSpellName()
{
	const auto missileDataPtr = *reinterpret_cast<MissileData**>(reinterpret_cast<QWORD>(this) +
		UPasta::Offsets::MissileManager::SpellName);
	if (IsValidPtr(missileDataPtr))
	{
		const char* textToReturnPtr = reinterpret_cast<char*>(missileDataPtr);
		return textToReturnPtr;
	}

	return "";
}

Vector3 Missile::GetSpellStartPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::StartPosition);
}

Vector3 Missile::GetSpellPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::Position);
}

Vector3 Missile::GetSpellEndPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::EndPosition);
}

float Object::GetEffectiveHealth(DamageType damageType)
{
	if (damageType == True) return this->GetHealth();

	return this->GetHealth() * (1 + (damageType == Physical ? this->GetBaseArmor() : this->GetAbilityPower()) / 100);
}

float Object::GetBoundingRadius() { return Engine::call_virtual<37, float>(this); }

float Object::GetAttackDelay()
{
	return Engine::call_function<float>(RVA(UPasta::Offsets::Functions::Stats::GetAttackDelay), this);
}

float Object::GetAttackWindup()
{
	return Engine::call_function<float>(RVA(UPasta::Offsets::Functions::Stats::GetAttackCastDelay), this, 0x40);
}

bool Object::IsAlive() { return !Engine::IsDead(this); }
bool Object::IsLocalPlayer() { return this->GetHandle() == ObjectManager::GetLocalPlayer()->GetHandle(); }
bool Object::IsAlly() { return this->GetTeam() == ObjectManager::GetLocalPlayer()->GetTeam(); }

bool Object::IsInAARange()
{
	return ObjectManager::GetLocalPlayer()->GetRealAttackRange() >= ObjectManager::GetLocalPlayer()->GetDistanceTo(this);
}

bool Object::HasBuff(const char* buffname)
{
	return Engine::call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::HasBuff)), this, 0,
		RealNameInHash(buffname);
}

bool Object::IsUnderTower(Alliance team)
{
	const auto turret = TargetSelector::FindTurret(this->GetPosition(), 992.0f, team);
	if (turret == nullptr) return false;
	if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) return false;

	return this->GetDistanceTo(turret) < 992.0f;
}

bool Object::CanCastSpell(SpellIndex slotId)
{
	if (!this->CanCast()) return false;

	Spell* spellSlot = this->GetSpellBySlotId(slotId);
	if (spellSlot == nullptr) return false;

	return spellSlot->IsReady();
}

bool Object::IsFacing(Object* secondObj, float angle)
{
	if (this == nullptr) return false;
	if (secondObj == nullptr) return false;

	const int currentSegment = this->GetAiManager()->GetCurrentSegment();
	const Vector3 segmentToCheck = this->GetAiManager()->GetSegment(currentSegment);
	if (!segmentToCheck.IsValid()) return false;

	const Vector3 directionPos = this->GetAiManager()->GetPosition().Extend(segmentToCheck, 400.0f);
	if (!directionPos.IsValid()) return false;

	return render::AngleBetween3point(this->GetAiManager()->GetPosition(), directionPos,
	                                  secondObj->GetAiManager()->GetPosition()) <= angle / 2;
}

int Object::GetBuffListSize()
{
	return static_cast<int>((QWORD)this->GetBuffManagerEntriesEnd() - (QWORD)this->GetBuffManager()) / static_cast<int>(sizeof(QWORD));
}

Buff* Object::GetBuffByName(std::string name)
{
	for (int i = 0; i < this->GetBuffListSize(); i++)
	{
		const auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		const auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= Engine::GetGameTime() && buff->GetName() == name) return buff;
	}
	return nullptr;
}

Buff* Object::GetBuffByType(BuffType type)
{
	for (int i = 0; i < this->GetBuffListSize(); i++)
	{
		const auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		const auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= Engine::GetGameTime() && buff->GetType() == type) return buff;
	}
	return nullptr;
}

float CharacterStateIntermediate::ReadClientStat(StatType statToReturn)
{
	using namespace UPasta::Offsets::CharacterStateIntermediate;
	switch (statToReturn)
	{
	case AbilityHasteMod: return ReadFLOAT(this, oAbilityHasteMod);
	case PercentCooldownCapMod: return ReadFLOAT(this, PercentCooldownCapMod);
	case PassiveCooldownEndTime: return ReadFLOAT(this, PassiveCooldownEndTime);
	case PassiveCooldownTotalTime: return ReadFLOAT(this, PassiveCooldownTotalTime);
	case FlatPhysicalDamageMod: return ReadFLOAT(this, FlatPhysicalDamageMod);
	case PercentPhysicalDamageMod: return ReadFLOAT(this, PercentPhysicalDamageMod);
	case PercentBonusPhysicalDamageMod: return ReadFLOAT(this, PercentBonusPhysicalDamageMod);
	case PercentBasePhysicalDamageAsFlatBonusMod: return ReadFLOAT(this, PercentBasePhysicalDamageAsFlatBonusMod);
	case FlatMagicDamageMod: return ReadFLOAT(this, FlatMagicDamageMod);
	case PercentMagicDamageMod: return ReadFLOAT(this, PercentMagicDamageMod);
	case FlatMagicReduction: return ReadFLOAT(this, FlatMagicReduction);
	case PercentDamageToBarracksMinionMod: return ReadFLOAT(this, PercentDamageToBarracksMinionMod);
	case FlatDamageReductionFromBarracksMinionMod: return ReadFLOAT(this, FlatDamageReductionFromBarracksMinionMod);
	case PercentMagicReduction: return ReadFLOAT(this, PercentMagicReduction);
	case FlatCastRangeMod: return ReadFLOAT(this, FlatCastRangeMod);
	case AttackSpeedMod: return ReadFLOAT(this, AttackSpeedMod);
	case PercentAttackSpeedMod: return ReadFLOAT(this, PercentAttackSpeedMod);
	case PercentMultiplicativeAttackSpeedMod: return ReadFLOAT(this, PercentMultiplicativeAttackSpeedMod);
	case BaseAttackDamage: return ReadFLOAT(this, BaseAttackDamage);
	case BaseAttackDamageSansPercentScale: return ReadFLOAT(this, BaseAttackDamageSansPercentScale);
	case FlatBaseAttackDamageMod: return ReadFLOAT(this, FlatBaseAttackDamageMod);
	case PercentBaseAttackDamageMod: return ReadFLOAT(this, PercentBaseAttackDamageMod);
	case BaseAbilityDamage: return ReadFLOAT(this, BaseAbilityDamage);
	case CritDamageMultiplier: return ReadFLOAT(this, CritDamageMultiplier);
	case ScaleSkinCoef: return ReadFLOAT(this, ScaleSkinCoef);
	case Dodge: return ReadFLOAT(this, Dodge);
	case CritPercent: return ReadFLOAT(this, CritPercent);
	case FlatBaseHPPoolMod: return ReadFLOAT(this, FlatBaseHPPoolMod);
	case Armor: return ReadFLOAT(this, Armor);
	case BonusArmor: return ReadFLOAT(this, BonusArmor);
	case SpellBlock: return ReadFLOAT(this, SpellBlock);
	case BonusSpellBlock: return ReadFLOAT(this, BonusSpellBlock);
	case HPRegenRate: return ReadFLOAT(this, HPRegenRate);
	case BaseHPRegenRate: return ReadFLOAT(this, BaseHPRegenRate);
	case MoveSpeed: return ReadFLOAT(this, MoveSpeed);
	case AttackRange: return ReadFLOAT(this, AttackRange);
	case FlatBubbleRadiusMod: return ReadFLOAT(this, FlatBubbleRadiusMod);
	case PercentBubbleRadiusMod: return ReadFLOAT(this, PercentBubbleRadiusMod);
	case FlatArmorPenetration: return ReadFLOAT(this, FlatArmorPenetration);
	case PhysicalLethality: return ReadFLOAT(this, PhysicalLethality);
	case PercentArmorPenetration: return ReadFLOAT(this, PercentArmorPenetration);
	case PercentBonusArmorPenetration: return ReadFLOAT(this, PercentBonusArmorPenetration);
	case PercentCritBonusArmorPenetration: return ReadFLOAT(this, PercentCritBonusArmorPenetration);
	case PercentCritTotalArmorPenetration: return ReadFLOAT(this, PercentCritTotalArmorPenetration);
	case FlatMagicPenetration: return ReadFLOAT(this, FlatMagicPenetration);
	case MagicLethality: return ReadFLOAT(this, MagicLethality);
	case PercentMagicPenetration: return ReadFLOAT(this, PercentMagicPenetration);
	case PercentBonusMagicPenetration: return ReadFLOAT(this, PercentBonusMagicPenetration);
	case PercentLifeStealMod: return ReadFLOAT(this, PercentLifeStealMod);
	case PercentSpellVampMod: return ReadFLOAT(this, PercentSpellVampMod);
	case PercentOmnivampMod: return ReadFLOAT(this, PercentOmnivampMod);
	case PercentPhysicalVamp: return ReadFLOAT(this, PercentPhysicalVamp);
	case PathfindingRadiusMod: return ReadFLOAT(this, PathfindingRadiusMod);
	case PercentCCReduction: return ReadFLOAT(this, PercentCCReduction);
	case PercentEXPBonus: return ReadFLOAT(this, PercentEXPBonus);
	case FlatBaseArmorMod: return ReadFLOAT(this, FlatBaseArmorMod);
	case FlatBaseSpellBlockMod: return ReadFLOAT(this, FlatBaseSpellBlockMod);
	case PrimaryARBaseRegenRateRep: return ReadFLOAT(this, PrimaryARBaseRegenRateRep);
	case SecondaryARRegenRateRep: return ReadFLOAT(this, SecondaryARRegenRateRep);
	case SecondaryARBaseRegenRateRep: return ReadFLOAT(this, SecondaryARBaseRegenRateRep);

	default: return 0;
	}
}

Vector3 SpellData::GetSpellEndPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::EndPosition);
}

Object::TYPE Object::GetType()
{
	using OriginalFn = ObjectTypeHolder* (*)(PVOID);
	auto holder = Engine::CallVirtual<OriginalFn>(this, 1)(this);
	return holder->GetType();
}

