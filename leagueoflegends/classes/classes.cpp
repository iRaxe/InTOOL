#include "../Awareness.h"
#include "../ListManager.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../ObjectTypeHolder.h"

class AttackableUnit;

CombatType Object::GetCombatType() {
	return ReadQWORD2(CombatType, this, UPasta::Offsets::Client::CombatType);
}

ItemsDatabase InventorySlot::GetId()
{
	const auto item_id = *(ItemsDatabase*)(*(QWORD*)this + UPasta::Offsets::ItemManager::ItemId);
	return item_id;
}

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

std::string SpellData::GetTexturePath()
{
	const auto TextureInfoPtr = this + UPasta::Offsets::SpellInfo::TexturePath;
	const auto TexturePtr = *(char**)TextureInfoPtr;
	const auto TextureInfo = *(char**)TexturePtr;

	return TextureInfo;
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

InventorySlot* ItemListObject::GetSlot()
{
	const auto pointer = (InventorySlot*)(*(QWORD*)this + UPasta::Offsets::ItemManager::InventorySlot);

	if (IsNotZeroPtr(*(QWORD*)pointer) && IsValidPtr(pointer))
		return pointer;

	LOG("GetSlot Doesnt work");
	return nullptr;
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

	//LOG("GetInventorySlotById Doesnt work");
	return nullptr;
}

InventorySlot* HeroInventory::FindItemID(ItemsDatabase itemID)
{
	const std::vector<InventorySlot*> items = ItemsList();
	for (auto* item : items) {
		if (item != nullptr && item->GetId() == itemID) {
			return item;
		}
	}
	return nullptr;
}

std::vector<InventorySlot*> HeroInventory::ItemsList()
{
	std::unordered_map<ItemsDatabase, size_t> itemToSlotMap;
	std::vector<InventorySlot*> rs;

	for (size_t i = 0; i < 7; i++) {
		auto itemPtr = GetInventorySlot(i);
		if (itemPtr != nullptr)
		{
			auto found = itemToSlotMap.find(itemPtr->GetId());
			if (found != itemToSlotMap.end()) {
				rs[found->second] = itemPtr;
			}
			else {
				itemToSlotMap[itemPtr->GetId()] = rs.size();
				rs.push_back(itemPtr);
			}
		}
	}
	return rs;
}

HeroInventory* Object::GetHeroInventory()
{
	const auto heroInventory = reinterpret_cast<HeroInventory*>(reinterpret_cast<QWORD>(this) +
		UPasta::Offsets::ItemManager::ItemManagerInstance);

	if (IsNotZeroPtr(heroInventory) && IsValidPtr(heroInventory)) {
		return heroInventory;
	}

	LOG("GetHeroInventory Doesnt work");
	return nullptr;
}

PerkID Perk::GetId() {
	return ReadQWORD2(PerkID, this, UPasta::Offsets::Perks::PerkID);
}

std::string Perk::GetName()
{
	const auto perkname = *(char**)((QWORD)this + UPasta::Offsets::Perks::PerkName);
	if (IsValidPtr(perkname))
		return perkname;

	LOG("GetName Doesnt work");
	return "";
}

std::string Perk::GetRawName()
{
	const auto perkRawName = *(char**)((QWORD)this + UPasta::Offsets::Perks::PerkRawName);
	if (IsValidPtr(perkRawName))
		return perkRawName;

	LOG("GetRawName Doesnt work");
	return "";
}

std::string Perk::GetRawDescription()
{
	const auto perkRawDescription = *(char**)(*(QWORD*)this + UPasta::Offsets::Perks::PerkRawDescription);
	if (IsValidPtr(perkRawDescription))
		return perkRawDescription;

	LOG("GetRawDescription Doesnt work");
	return "";
}

Perk* Perks::GetPerkByIndex(int index)
{
	auto perkSlot = (Perk*)(*(QWORD*)(*(QWORD*)this + (UPasta::Offsets::Perks::Size * index)));

	if (IsNotZeroPtr(perkSlot))
		return perkSlot;

	return nullptr;
}

Perks* Object::GetHeroPerks()
{
	const auto heroPerks = (Perks*)(this + UPasta::Offsets::Perks::PerksManagerInstance);
	if (IsNotZeroPtr(heroPerks) && IsValidPtr(heroPerks))
		return heroPerks;

	LOG("GetHeroPerks Doesnt work");
	return nullptr;
}

void CharacterDataStack::Update(bool change)
{
	using fnUpdate = void(__thiscall*)(QWORD, bool);
	const auto _update = reinterpret_cast<fnUpdate>(globals::moduleBase +
		UPasta::Offsets::Functions::Skin::CharacterDataStackUpdate);
	spoof_call(Engine::spoof_trampoline, _update, reinterpret_cast<QWORD>(this), change);
}

bool AiManager::IsMoving() {
	return ReadBOOL(this, UPasta::Offsets::AIManager::Path::IsMoving);
}

int AiManager::GetCurrentSegment() {
	return ReadINT(this, UPasta::Offsets::AIManager::Path::PassedWayPoints);
}

std::vector<Vector3> AiManager::GetFutureSegments() {
	std::vector<Vector3> segments = {};
	const int segmentsCount = this->GetSegmentsCount();
	for (int i = this->GetCurrentSegment(); i < segmentsCount; i++) {
		segments.push_back(this->GetSegment(i));
	}
	return segments;
}

Vector3 AiManager::GetSegment(int index) {
	return Engine::ReadVector3(ReadQWORD(this, UPasta::Offsets::AIManager::Path::NavigationPoints) + (min(index, this->GetSegmentsCount() - 1) * sizeof(Vector3)));
}

Vector3 AiManager::GetPosition() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::ServerPosition);
}

float AiManager::GetVelocity() {
	return ReadFLOAT(this, UPasta::Offsets::AIManager::Velocity);
}

Vector3 AiManager::GetTargetPosition() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::TargetPos);
}

Vector3 AiManager::GetMouseClickPosition() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::MouseClickPosition);
}

Vector3 AiManager::GetPathStart() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::CurrentPathStartPosition);
}

Vector3 AiManager::GetDashStart() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::DashStartPosition);
}

Vector3 AiManager::GetPathEnd() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::CurrentPathEndPosition);
}

Vector3 AiManager::GetDirection() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::AIManager::Path::Direction);
}

int AiManager::GetSegmentsCount() {
	return ReadINT(this, UPasta::Offsets::AIManager::Path::SegmentsCount);
}

float AiManager::GetDashSpeed() {
	return ReadFLOAT(this, UPasta::Offsets::AIManager::DashSpeed);
}

float AiManager::GetMovementSpeed() {
	return ReadFLOAT(this, UPasta::Offsets::AIManager::Path::MovementSpeed);
}

bool AiManager::IsDashing() {
	return ReadBOOL(this, UPasta::Offsets::AIManager::Path::IsDashing);
}

bool AiManager::IsNotDashing() {
	return ReadBOOL(this, UPasta::Offsets::AIManager::Path::IsNotDashing);
}

float CharacterData::GetSize() {
	return ReadFLOAT(this, UPasta::Offsets::CharData::Size);
}

ChampionID CharacterData::GetHeroID() {
	return ReadQWORD2(ChampionID,this, UPasta::Offsets::CharData::HeroID);
}

QWORD CharacterData::GetObjectTypeHash()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::CharData::Type));
}

QWORD CharacterData::GetObjectTypeHashDetailed()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::CharData::Type) +
		UPasta::Offsets::CharData::TypeDetailed);
}

std::string SpellData::GetName()
{
	return *(char**)((QWORD)this + UPasta::Offsets::SpellBook::SpellSlot::Name);
}

float SpellData::GetManaCostByLevel(int level) {
	return ReadFLOAT(this, UPasta::Offsets::SpellInfo::DataManaCost + ((level - 1) * sizeof(float)));
}

SpellData* SpellInfo::GetSpellData() {
	return *(SpellData**)((QWORD)this + UPasta::Offsets::SpellInfo::SpellDataInstance);
}

void SpellInput::SetCaster(int index) {
	ReadQWORD(this, 0x10) = index;
}

void SpellInput::SetTarget(int index) {
	ReadQWORD(this, 0x14) = index;
}

void SpellInput::SetStartPos(Vector3 pos) {
	ReadQWORD2(Vector3,this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::StartPosition) = pos;
}

void SpellInput::SetEndPos(Vector3 pos) {
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::EndPosition)= pos;
}

void SpellInput::SetClickedPos(Vector3 pos) {
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::MousePosition) = pos;
}

void SpellInput::SetUnkPos(Vector3 pos) {
	ReadQWORD2(Vector3, this, UPasta::Offsets::SpellBook::SpellSlot::SpellInput::MousePosition2) = pos;
}

int Spell::GetLevel() {
	return ReadINT(this, UPasta::Offsets::SpellBook::SpellSlot::Level);
}

float Spell::GetCooldownTimer() {
	return ReadFLOAT(this, UPasta::Offsets::SpellBook::SpellSlot::Cooldown);
}

float Spell::GetCooldown()
{
	const float cooldownTimer = this->GetCooldownTimer();
	if (cooldownTimer == 0.0f) return cooldownTimer;
	return max(cooldownTimer - Engine::GetGameTime(), 0.0f);
}

int Spell::GetStacks() {
	return ReadINT(this, UPasta::Offsets::SpellBook::SpellSlot::Stacks);
}

float Spell::GetTotalCooldown() {
	return ReadFLOAT(this, UPasta::Offsets::SpellBook::SpellSlot::TotalCooldown);
}

float Cooldown::GetBaseCooldown() {
	return ReadFLOAT(this, 0x0);
}

Cooldown* CooldownArray::GetArrayIndex(int index)
{
	auto baseSlot = (Cooldown*)(this + (0x4 * index));

	if (IsNotZeroPtr(baseSlot))
		return baseSlot;

	return nullptr;
}

CooldownArray* SpellData::GetCooldownArray()
{
	const auto cooldownArray = (CooldownArray*)(this + 0x2F0);
	if (IsNotZeroPtr(cooldownArray) && IsValidPtr(cooldownArray))
		return cooldownArray;

	LOG("GetCooldownArray Doesnt work");
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

SpellInfo* Spell::GetSpellInfo()
{
	return *(SpellInfo**)((QWORD)this + UPasta::Offsets::SpellBook::SpellSlot::SpellInfoInstance);
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
	return *(SpellInfo**)((QWORD)this + UPasta::Offsets::ActiveCast::Info);
}

SpellInfo* SpellCast::GetProcessSpellInfo()
{
	return *(SpellInfo**)((QWORD)this);
}

int SpellCast::GetSpellId() {
	return ReadINT(this, UPasta::Offsets::SpellCast::SlotID);
}

bool SpellCast::IsAutoAttack() {
	return ReadBOOL(this, UPasta::Offsets::SpellCast::IsBasicAttack);
}

bool SpellCast::IsSpell() {
	return ReadBOOL(this, UPasta::Offsets::SpellCast::IsSpell);
}

float SpellCast::GetCastTime() {
	return ReadFLOAT(this, UPasta::Offsets::SpellCast::CastedAtTime);
}

std::string SpellCast::GetCasterName() {
	return ReadQWORD2(LolString, this, UPasta::Offsets::SpellCast::CasterName);
}

Vector3 SpellCast::GetStartPosition() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::StartPosition);
}

Vector3 SpellCast::GetEndPosition() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::EndPosition);
}

Vector3 SpellCast::GetMousePosition() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::MousePosition);
}

std::string Buff::GetName() {
	auto namePtr = (QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::BuffManager::BuffNamePtr));
	if (!IsValidPtr(namePtr)) return "";
	return *(char**)((QWORD)namePtr + UPasta::Offsets::BuffManager::BuffName);
}

BuffType Buff::GetType() {
	return ReadQWORD2(BuffType, this, UPasta::Offsets::BuffManager::BuffType);
}

float Buff::GetStartTime() {
	return ReadFLOAT(this, UPasta::Offsets::BuffManager::BuffEntryBuffStartTime);
}

float Buff::GetEndTime() {
	return ReadFLOAT(this, UPasta::Offsets::BuffManager::BuffEntryBuffEndTime);
}

int Buff::GetStacksAlt() {
	return ReadINT(this, UPasta::Offsets::BuffManager::BuffEntryBuffCountAlt);
}

int Buff::GetStacks() {
	return ReadINT(this, UPasta::Offsets::BuffManager::BuffEntryBuffCount);
}

int Buff::GetMaxStacks()
{
	return max(this->GetStacksAlt(), this->GetStacks());
}

bool Buff::isActive() {
	return ReadQWORD2(BYTE, this, UPasta::Offsets::BuffManager::BuffEntryBuffCountAlt)
		&& ReadQWORD2(int64_t, this, UPasta::Offsets::BuffManager::BuffNamePtr)
		|| ReadQWORD2(BYTE, this, UPasta::Offsets::BuffManager::IsPermanent);
}

Buff* BuffEntry::GetBuff() {
	auto buff = (Buff*)(*(QWORD*)((QWORD)this + UPasta::Offsets::BuffManager::BuffEntry));
	if (!IsValidPtr(buff)) return nullptr;
	return buff;
}

BuffEntry* BuffManager::GetBuffEntryByIndex(int index) {
	auto address = (BuffEntry*)(*(QWORD*)((QWORD)this + (sizeof(QWORD) * index)));
	if (!IsValidPtr(address)) return nullptr;
	return address;
}

unsigned int Object::GetNetId() {
	return ReadINT(this, UPasta::Offsets::Client::Index);
}

DWORD Object::GetHandle() {
	return ReadDWORD(this, UPasta::Offsets::Client::Handle);
}

DWORD Missile::GetHandle() {
	return ReadDWORD(this, UPasta::Offsets::Client::Handle);
}


DWORD Object::GetTurretTargetNetworkID() {
	return ReadDWORD(this, UPasta::Offsets::Turret::TargetNetworkID);
}


DWORD SpellCast::GetCasterHandle() {
	return ReadDWORD(this, UPasta::Offsets::SpellCast::CasterHandle);
}

int Object::GetTeam() {
	return ReadINT(this, UPasta::Offsets::BaseObject::Team);
}

int Object::GetLevel() {
	return ReadINT(this, UPasta::Offsets::Client::Level);
}

float Object::GetExperience() {
	return ReadFLOAT(this, UPasta::Offsets::Client::Experience);
}

Vector3 Object::GetPosition(){
	return Engine::ReadVector3(reinterpret_cast<uintptr_t>(this) + UPasta::Offsets::BaseObject::Position);
}

bool Object::IsVisible() {
	return ReadBOOL(this, UPasta::Offsets::AttackableUnit::Visibility);
}

bool Object::IsAlive() {
	return !Engine::IsDead(this);
}

float Object::GetMana() {
	return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::Mana);
}

float Object::GetMaxMana() {
	return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::MaxMana);
}

float Object::GetPercentMana() {
	return 100 * this->GetMana() / this->GetMaxMana() > 100 ? 100 : 100 * this->GetMana() / this->GetMaxMana();
}

float Object::GetAttackDamage() {
	return this->ReadClientStat(BaseAttackDamage) + this->ReadClientStat(BonusAttackDamage);
}

float Object::ReadClientStat(StatType statToReturn) {
	using namespace UPasta::Offsets;
	switch (statToReturn) {
	case AttackRange: return ReadFLOAT(this, Client::AttackRange);

	case Health: return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::Health);
	case BonusHealth: return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::BonusHealth);
	case MaxHealth: return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::MaxHealth);
	case LifeRegeneration: return ReadFLOAT(this, Client::LifeRegeneration);

	case Mana: return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::Mana);
	case MaxMana: return ReadFLOAT(this, UPasta::Offsets::AttackableUnit::MaxMana);

	case BaseAttackDamage: return ReadFLOAT(this, Client::BaseAttackDamage);
	case BonusAttackDamage: return ReadFLOAT(this, Client::BonusAttackDamage);
	case TotalAttackDamage: return GetAttackDamage();

	case AbilityHaste: return ReadFLOAT(this, Client::AbilityHaste);

	case Lethality: return ReadFLOAT(this, Client::Lethality);

	case ArmorPenetrationFlat: return ReadFLOAT(this, Client::ArmorPenetrationFlat);

	case MagicPenetrationFlat: return ReadFLOAT(this, Client::MagicPenetrationFlat);
	case MagicPenetrationMultiplier: return ReadFLOAT(this, Client::MagicPenetrationMultiplier);

	case AbilityPower: return ReadFLOAT(this, Client::AbilityPower);

	case BonusArmor: return ReadFLOAT(this, Client::BonusArmor);
	case BaseArmor: return ReadFLOAT(this, Client::BaseArmor);
	case TotalArmor: return ReadFLOAT(this, Client::BaseArmor) + ReadFLOAT(this, Client::BaseArmor);

	case Shield: return ReadFLOAT(this, Client::Shield);
	case PhysicalShield: return ReadFLOAT(this, Client::PhysicalShield);
	case MagicalShield: return ReadFLOAT(this, Client::MagicalShield);

	case CritFlat: return ReadFLOAT(this, Client::CritFlat);
	case CritMultiplier: return ReadFLOAT(this, Client::CritMultiplier);

	case AttackSpeed: return ReadFLOAT(this, Client::AttackSpeedMulti);

	case BonusMagicResist: return ReadFLOAT(this, Client::BonusMagicResist);
	case TotalMagicResist: return ReadFLOAT(this, Client::TotalMagicResist);

	case MovementSpeed: return ReadFLOAT(this, Client::MovementSpeed);

	case Ammo: return ReadFLOAT(this, Client::Ammo);
	case MaxAmmo: return ReadFLOAT(this, Client::MaxAmmo);

	case ScaleMulti: return ReadFLOAT(this, Client::ScaleMulti);

	case Experience: return ReadFLOAT(this, Client::Experience);
	case Level: return ReadINT(this, Client::Level);

	case Visibility: return ReadBOOL(this, UPasta::Offsets::AttackableUnit::Visibility);
	case Targetable: return ReadBOOL(this, UPasta::Offsets::AttackableUnit::Targetable);
	case Invulnerability: return ReadBOOL(this, UPasta::Offsets::AttackableUnit::Invulnerability);
	default: return 0;
	}
}

bool Object::IsTargetable() {
	return ReadBOOL(this, UPasta::Offsets::AttackableUnit::Targetable);
}

bool Object::IsInvulnerable() {
	return ReadBOOL(this, UPasta::Offsets::AttackableUnit::Invulnerability);
}

bool Object::IsCursed() {
	return this->GetBuffByName("cursedtouch");
}

bool Object::HasSummonerSpell(SummonerSpells summSpellToFind)
{
	Spell* spellD = this->GetSpellBySlotId(SpellIndex::D);
	Spell* spellF = this->GetSpellBySlotId(SpellIndex::F);
	switch (summSpellToFind)
	{
		case SummonerSpells::Barrier:	return spellD->GetName() == "SummonerBarrier"		or spellF->GetName() == "SummonerBarrier";
		case SummonerSpells::Flash:		return spellD->GetName() == "SummonerFlash"			or spellF->GetName() == "SummonerFlash";
		case SummonerSpells::Cleanse:	return spellD->GetName() == "SummonerBoost"			or spellF->GetName() == "SummonerBoost";
		case SummonerSpells::Exhaust:	return spellD->GetName() == "SummonerExhaust"		or spellF->GetName() == "SummonerExhaust";
		case SummonerSpells::Ghost:		return spellD->GetName() == "SummonerHaste"			or spellF->GetName() == "SummonerHaste";
		case SummonerSpells::Heal:		return spellD->GetName() == "SummonerHeal"			or spellF->GetName() == "SummonerHeal";
		case SummonerSpells::Ignite:	return spellD->GetName() == "SummonerDot"			or spellF->GetName() == "SummonerDot";
		case SummonerSpells::Smite:		return spellD->GetName().contains("Smite")	or spellF->GetName().contains("Smite");
		case SummonerSpells::Teleport:	return spellD->GetName().contains("Teleport")	or spellF->GetName().contains("Teleport");
	}
	return false;
}

bool Object::HasConqueror() {
	return this->GetBuffByName("ASSETS/Perks/Styles/Precision/Conqueror/ConquerorEnrage.lua");
}

float Object::GetPercentHealth() {
	return 100 * this->ReadClientStat(Health) / this->ReadClientStat(MaxHealth) > 100 ? 100 : 100 * this->ReadClientStat(Health) / this->ReadClientStat(MaxHealth);
}

unsigned short Object::GetActionState() {
	return ReadQWORD2(unsigned short, this, UPasta::Offsets::AttackableUnit::State);
}

std::string Object::GetName() {
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + UPasta::Offsets::Client::AiName);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

std::string Object::GetClassicName() {
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + UPasta::Offsets::Client::Name);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

BuffManager* Object::GetBuffManager() {
	return (BuffManager*)(*(QWORD*)((QWORD)this + UPasta::Offsets::BuffManager::BuffManagerInstance));
}

QWORD* Object::GetBuffManagerEntriesEnd() {
	return *(QWORD**)((QWORD)this + UPasta::Offsets::BuffManager::BuffManagerInstance + UPasta::Offsets::BuffManager::BuffEntriesEnd);
}

SpellCast* Object::GetActiveSpellCast() {
	QWORD* activeSpellCastOffset = *(QWORD**)((QWORD)this + UPasta::Offsets::ActiveCast::ActiveCastInstance);
	if (!IsValidPtr(activeSpellCastOffset)) return nullptr;
	return (SpellCast*)activeSpellCastOffset;
}

CharacterData* Object::GetCharacterData() {
	return (CharacterData*)(*(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::CharData::CharDataInstance) +
		UPasta::Offsets::CharData::Array));
}

AiManager* Object::GetAiManager() {
	LeagueObfuscation<QWORD> aiManagerObf = *(LeagueObfuscation<QWORD>*)((QWORD)this +
		UPasta::Offsets::AIManager::AiManagerInstance);
	return (AiManager*)(*(QWORD*)(Decrypt(aiManagerObf) + 0x10));
}

Spell* Object::GetSpellBySlotId(int slotId) {
	return *(Spell**)((QWORD)this + UPasta::Offsets::Client::SpellBookInstance +
		UPasta::Offsets::SpellBook::SpellSlotInstance + (sizeof(QWORD) * slotId));
}

std::string MissileData::GetMissileName() {
	const auto missileName = (char*)((QWORD)this + UPasta::Offsets::MissileManager::MissileName);
	if (IsValidPtr(missileName))
		return missileName;

	LOG("GetMissileName Doesnt work");
	return "";
}

bool MissileData::IsAutoAttack() {
	return this->GetMissileName().contains("Attack") || this->GetSpellName().contains("Attack");
}

std::string MissileData::GetSpellName() {
	const auto missileDataPtr = *reinterpret_cast<MissileData**>(reinterpret_cast<QWORD>(this) +
		UPasta::Offsets::MissileManager::SpellName);
	if (IsValidPtr(missileDataPtr))
	{
		const char* textToReturnPtr = reinterpret_cast<char*>(missileDataPtr);
		return textToReturnPtr;
	}

	return "";
}

MissileData* Missile::GetMissileData() {
	return *(MissileData**)((QWORD)this + UPasta::Offsets::MissileManager::SpellInfo::SpellInfoInstance);
}

Missile* Object::GetMissileByIndex() {
	return *(Missile**)((QWORD)this + UPasta::Offsets::MissileManager::Index);
}

int Missile::GetMissileSrcId() {
	return ReadINT(this, UPasta::Offsets::MissileManager::SrcIdx);
}

Vector3 Missile::GetSpellStartPos() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::StartPosition);
}

Vector3 Missile::GetSpellPos() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::Position);
}

Vector3 Missile::GetSpellEndPos() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::EndPosition);
}

float Missile::GetSpellSpeed() {
	return ReadFLOAT(this, UPasta::Offsets::MissileManager::Speed);
}

float SpellData::GetMaxCastRange() {
	return ReadFLOAT(this, UPasta::Offsets::SpellData::CastRange);
}

float SpellData::GetCastRadius() {
	return ReadFLOAT(this, UPasta::Offsets::SpellData::CastRadius);
}

float SpellData::GetLineWidth() {
	return ReadFLOAT(this, UPasta::Offsets::SpellData::LineWidth);
}

float SpellData::GetCastSpeed() {
	return ReadFLOAT(this, UPasta::Offsets::SpellData::MissileSpeed);
}

float SpellData::GetDelay() {
	return ReadFLOAT(this, UPasta::Offsets::SpellData::DelayCastOffsetPerce);
}

float SpellData::GetCastTime() {
	return ReadFLOAT(this, UPasta::Offsets::SpellData::CastTime);
}

float Object::GetBoundingRadius() {
	using fnGetBoundingRadius = float(__fastcall*)(Object* obj);
	auto _fnGetBoundingRadius = (fnGetBoundingRadius)(globals::moduleBase +
		UPasta::Offsets::Functions::Stats::GetBoundingRadius);
	return _fnGetBoundingRadius(this);
}

float Object::GetAttackDelay() {
	using fnGetAttackDelay = float(__cdecl*)(Object* obj);
	auto _fnGetAttackDelay = (fnGetAttackDelay)(globals::moduleBase +
		UPasta::Offsets::Functions::Stats::GetAttackDelay);
	return _fnGetAttackDelay(this);
}

float Object::GetAttackWindup() { 
	using fnGetAttackWindup = float(__cdecl*)(Object* obj, int flags);
	auto _fnGetAttackWindup = (fnGetAttackWindup)(globals::moduleBase +
		UPasta::Offsets::Functions::Stats::GetAttackCastDelay);
	return _fnGetAttackWindup(this, 0x40);
}

bool Object::IsCastingSpell() {
	return this->GetActiveSpellCast() != nullptr && !this->GetActiveSpellCast()->IsAutoAttack();
}

bool Object::CanAttack() {
	return this->GetActionState() & ActionState::CanAttack && !globals::localPlayer->GetBuffByName("KaisaE");
}

bool Object::CanCast() {
	return this->GetActionState() & ActionState::CanCast;
}

bool Object::CanMove() {
	return this->GetActionState() & ActionState::CanMove;
}

bool Object::IsEnemy() {
	return this->GetTeam() != globals::localPlayer->GetTeam();
}

bool Object::IsLocalPlayer() {
	return this->GetNetId() == globals::localPlayer->GetNetId();
}

bool Object::IsAlly() {
	return this->GetTeam() == globals::localPlayer->GetTeam();
}

bool Object::IsJungle() {
	return this->GetTeam() == 300;
}

bool Object::IsValidTarget() {
	return this->IsVisible() && this->IsAlive() && this->IsEnemy() && this->IsTargetable();
}

bool Object::IsRespawnMarker() {
	return this->GetCharacterData()->GetObjectTypeHash() == RespawnMarker;
}

bool Object::IsMelee() {
	return this->GetCombatType() == CombatType::Melee;
}

bool Object::IsRanged() {
	return this->GetCombatType() == CombatType::Ranged;
}

bool Object::IsHero() {
	return this->GetCharacterData()->GetObjectTypeHash() == Champion;
}

bool Object::IsSpecial() {
	return this->GetCharacterData()->GetObjectTypeHash() == Special;
}

bool Object::IsWard() {
	return this->GetCharacterData()->GetObjectTypeHash() == Ward;
}

bool Object::IsMinion() {
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane;
}

bool Object::IsSiegeMinion() {
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Siege;
}

bool Object::IsVoidMinion() {
	return this->GetCharacterData()->GetObjectTypeHash() == Special_Void;
}

bool Object::IsRangedMinion() {
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Ranged;
}

bool Object::IsMeleeMinion() {
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Melee;
}

bool Object::IsSuperMinion() {
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Super;
}

bool Object::IsMonster() {
	return this->GetCharacterData()->GetObjectTypeHash() == Monster;
}

bool Object::IsDragon() {
	return this->GetCharacterData()->GetObjectTypeHash() == Monster_Dragon;
}

bool Object::IsEpicMonster() {
	return this->GetCharacterData()->GetObjectTypeHash() == Monster_Epic;
}

bool Object::IsTurret() {
	return this->GetType() == TYPE::AITurretClient;
}

bool Object::IsBuilding() {
	return Engine::GetCollisionFlags(this->GetPosition()) == Tower;
}

float Object::GetEffectiveHealth(int damageType) {
	if (damageType == True) return this->ReadClientStat(Health);
	return this->ReadClientStat(Health) * (1 + (damageType == Physical ? this->ReadClientStat(TotalArmor) : this->ReadClientStat(AbilityPower)) / 100);
}

float Object::GetRealAttackRange() {
	return this->ReadClientStat(AttackRange) + this->GetBoundingRadius();
}

float Object::GetDistanceTo(Object* obj) {
	return this->GetPosition().Distance(obj->GetPosition());
}

bool Object::IsInRange(Vector3 pos, float radius) {
	return radius + this->GetBoundingRadius() >= render::Distance(pos, this->GetPosition());
}

bool Vector3::IsUnderEnemyTower() {
	const auto turret = TargetSelector::FindTurret(*this, 992.0f, Alliance::Enemy);
	if (turret && turret->IsEnemy())
		return this->distanceTo(turret->GetPosition()) <= 992.0f;

	return false;
}

bool Object::IsUnderEnemyTower() {
	return this->GetPosition().IsUnderEnemyTower();
}

bool Object::IsUnderAllyTower() {
	const auto turret = TargetSelector::FindTurret(this->GetPosition(), 992.0f, Alliance::Ally);
	if (turret && turret->IsAlly())
		return this->IsInRange(turret->GetPosition(), 992.0f);

	return false;
}

bool Object::IsInAARange() {
	return globals::localPlayer->GetRealAttackRange() + globals::localPlayer->GetBoundingRadius() >= render::Distance(
		globals::localPlayer->GetPosition(), this->GetPosition());
}

bool Object::CanCastSpell(int slotId) {
	auto spell = this->GetSpellBySlotId(slotId);
	return this->CanCast() && spell->IsReady() && spell->GetManaCost() <= this->GetMana();
}

bool Object::HasBuff(const char* buffname) {
	return Engine::Read<bool>(RVA(UPasta::Offsets::Functions::Stats::HasBuff)), this, 0, RealNameInHash(buffname);
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

	return render::AngleBetween3point(this->GetAiManager()->GetPosition(), directionPos, secondObj->GetAiManager()->GetPosition()) <= angle / 2;
}

int Object::GetBuffListSize() {
	return static_cast<int>((QWORD)this->GetBuffManagerEntriesEnd() - (QWORD)this->GetBuffManager()) / static_cast<int>(
		sizeof(QWORD));
}

Buff* Object::GetBuffByName(std::string name) {
	for (int i = 0; i < this->GetBuffListSize(); i++) 	{
		const auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		const auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= Engine::GetGameTime() && buff->GetName() == name) return buff;
	}
	return nullptr;
}

Buff* Object::GetBuffByType(BuffType type) {
	for (int i = 0; i < this->GetBuffListSize(); i++) 	{
		const auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		const auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= Engine::GetGameTime() && buff->GetType() == type) return buff;
	}
	return nullptr;
}

CharacterDataStack* Object::GetCharacterDataStack() {
	return (CharacterDataStack*)((QWORD)this + UPasta::Offsets::CharData::CharDataInstance);
}


#pragma region CharacterStateIntermediate
CharacterStateIntermediate* Object::GetCharacterStateIntermediate() {
	return (CharacterStateIntermediate*)((QWORD)this +
		UPasta::Offsets::CharacterStateIntermediate::oCharacterStateIntermediate);
}

float CharacterStateIntermediate::ReadClientStat(StatType statToReturn) {
	using namespace UPasta::Offsets::CharacterStateIntermediate;
	switch (statToReturn) {
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
#pragma endregion

int ObjectManager::GetListSize() {
	return ReadINT(this, UPasta::Offsets::Instance::Lists::ManagerListSize);
}

Object* ObjectManager::GetIndex(int index) {
	return *(Object**)(*(QWORD*)((QWORD)this + UPasta::Offsets::Instance::Lists::ManagerList) + (sizeof(QWORD) *
		index));
}


QWORD ObjectManager::GetFirst(const QWORD& objectManager) {

	static QWORD v1 = *reinterpret_cast<QWORD*>(objectManager + 0x18);
	static QWORD v2 = *reinterpret_cast<QWORD*>(objectManager + 0x20);
	if (v1 == v2)
		return 0;
	while (*reinterpret_cast<BYTE*>(v1) & 1 || !*reinterpret_cast<QWORD*>(v1))
	{
		v1 += 8;
		if (v1 == v2)
			return 0;
	}
	return *reinterpret_cast<QWORD*>(v1);
}
QWORD ObjectManager::GetNext(const QWORD& objectManager, const QWORD& obj) {

	unsigned int v3; // edx
	QWORD v5; // eax

	static QWORD v2 = *(QWORD*)(objectManager + 0x18);
	v3 = *(unsigned __int16*)(obj + 16);
	static unsigned __int64 v4 = (unsigned __int64)((*(QWORD*)(objectManager + 0x20) - v2) >> 3);
	while (++v3 < v4) {
		if ((*(BYTE*)(v2 + 8i64 * v3) & 1) == 0 && *(QWORD*)(v2 + 8i64 * v3))
			return *(QWORD*)(v2 + 8i64 * v3);
	}
	return 0i64;
}

void ObjectManager::HandleObject(Object* obj) {

	switch (obj->GetType()) {
	case Object::AIHeroClient: ObjectManager::_hero_list.emplace_back((Object*)obj); _client_map.insert({ obj->GetHandle(), (Object*)obj }); break;
	case Object::AIMinionClient: ObjectManager::_minion_list.emplace_back((Object*)obj); _client_map.insert({ obj->GetHandle(), (Object*)obj }); break;
	case Object::AITurretClient: ObjectManager::_turret_list.emplace_back((Object*)obj); break;
	case Object::obj_GeneralParticleEmitter: _particle_list.emplace_back(obj); break;
	case Object::BarracksDampener: ObjectManager::_inhibitor_list.emplace_back((Object*)obj); break;
	case Object::HQ: ObjectManager::_nexus_list.emplace_back((Object*)obj); break;
	default: break;
	}
}

void ObjectManager::Flush() {

	_hero_list.clear();
	_minion_list.clear();
	_turret_list.clear();
	_inhibitor_list.clear();
	_nexus_list.clear();
	_particle_list.clear();
	_client_map.clear();
}

void ObjectManager::Update() {

	Flush();

	static const QWORD obj_manager = *(QWORD*)(RVA(UPasta::Offsets::Instance::Lists::ObjManager));
	QWORD obj = GetFirst(obj_manager);
	while (obj)
	{
		const std::array<uint8_t, 8> vfunc_bytes = *reinterpret_cast<std::array<std::uint8_t, 8> *>((ReadVTable(obj, 1)));
		if (vfunc_bytes.at(0) != 0x48 && vfunc_bytes.at(7) != 0xC3) {
			;
		}
		else {
			HandleObject((Object*)obj);
		}
		obj = GetNext(obj_manager, obj);
	}
}

Object* ObjectManager::GetClientByHandle(DWORD handle) {
	auto it = _client_map.find(handle);
	if (it != _client_map.end())
		return it->second;
	return nullptr;
}

Vector3 SpellData::GetSpellEndPos() {
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::MissileManager::EndPosition);
}

Object::TYPE Object::GetType() {
	typedef ObjectTypeHolder* (*OriginalFn)(PVOID);
	auto holder = Engine::CallVirtual<OriginalFn>(this, 1)(this);
	return holder->GetType();

	auto pointg = Engine::CallVirtual<ObjectTypeHolder*>(this, 1);
	return pointg->GetType();
}

std::vector<Object*> ObjectManager::GetHeroesAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto hero : ObjectManager::GetHeroes()) {
		if (!hero) continue;
		if (team == Alliance::Ally && !hero->IsAlly()  || team == Alliance::Enemy && !hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable())
			possible_targets.push_back(hero);
	}

	return possible_targets;
}

Object* ObjectManager::GetHeroAs(Alliance team, Vector3 position, float range)
{
	Object* heroToReturn = nullptr;
	for (auto hero : ObjectManager::GetHeroesAs(team)) {
		if (!hero) continue;
		if (team == Alliance::Ally && !hero->IsAlly() || team == Alliance::Enemy && !hero->IsEnemy()) continue;
		if (hero->IsAlive() and hero->IsVisible() and hero->IsTargetable() and !hero->IsInvulnerable() and hero->GetPosition().distanceTo(position) < range)
		{
			if (!heroToReturn) {
				heroToReturn = hero;
				break;
			}
		}
	}

	return heroToReturn;
}

int ObjectManager::CountHeroesInRange(Alliance team, Vector3 position, float range)
{
	int heroesInRange = 0;
	for (auto hero : ObjectManager::GetHeroesAs(team)) {
		if (!hero) continue;
		if (team == Alliance::Ally && !hero->IsAlly() || team == Alliance::Enemy && !hero->IsEnemy()) continue;
		if (hero->GetPosition().distanceTo(position) > range) continue;
		heroesInRange++;
	}
	return heroesInRange;
}

Object* ObjectManager::GetObjectInRange(std::string name, float range)
{
	Object* best = nullptr;

	for (auto objToFind : ObjectManager::GetMinions()) {
		if (!objToFind) continue;
		if (!objToFind->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

		if (objToFind->GetName() != name) continue;

		if (!best)
		{
			best = objToFind;
			continue;
		}
	}

	return best;
}

Object* ObjectManager::GetObjectInRange(float range, std::string name, std::vector<QWORD> includeFilterTypeHashes, std::vector<QWORD> excludeFilterTypeHashesDetailed, bool isSpecial)
{
	Object* best = nullptr;
	for (Object* obj : *globals::minionManager)
	{
		if (!(isSpecial || obj->IsValidTarget())) continue;

		bool excludeHashMatched = false;
		for (auto hash : excludeFilterTypeHashesDetailed)
		{
			if (obj->GetCharacterData()->GetObjectTypeHashDetailed() == hash)
			{
				excludeHashMatched = true;
				break;
			}
		}
		if (excludeHashMatched) continue;

		bool includeHashMatched = false;
		for (auto hash : includeFilterTypeHashes)
		{
			if (obj->GetCharacterData()->GetObjectTypeHash() == hash)
			{
				includeHashMatched = true;
				break;
			}
		}
		if (includeFilterTypeHashes.size() && !includeHashMatched) continue;

		if (name != "" && obj->GetName() != name) continue;
		if (!obj->IsInRange(globals::localPlayer->GetPosition(), range)) continue;

		if (!best) {
			best = obj;
			continue;
		}

		if (best->GetDistanceTo(globals::localPlayer) > best->GetDistanceTo(globals::localPlayer)) {
			best = obj;
			continue;
		}
	}

	return best;
}

std::vector<Object*> ObjectManager::GetMinionsAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto minion : ObjectManager::GetMinions()) {
		if (!minion) continue;
		if (team == Alliance::Ally && !minion->IsAlly() || team == Alliance::Enemy && !minion->IsEnemy()) continue;

		if (minion->IsAlive() and minion->IsVisible() and minion->IsTargetable() and !minion->IsInvulnerable())
			possible_targets.push_back(minion);
	}

	return possible_targets;
}

int ObjectManager::CountMinionsInRange(Alliance team, Vector3 position, float range)
{
	int minionsInRange = 0;
	for (auto minion : ObjectManager::GetMinionsAs(team)) {
		if (!minion) continue;
		if (minion->IsJungle()) continue;
		if (minion->GetPosition().distanceTo(position) > range) continue;
		minionsInRange++;
	}
	return minionsInRange;
}

std::vector<Object*> ObjectManager::GetJungleMonsters()
{
	std::vector<Object*> possible_targets;
	for (auto monster : ObjectManager::GetMinions()) {
		if (!monster) continue;
		if (!monster->IsJungle()) continue;
		if (monster->GetCharacterData()->GetObjectTypeHash() == Plants) continue;
		if (monster->IsAlive() and monster->IsVisible() and monster->IsTargetable() and !monster->IsInvulnerable())
			possible_targets.push_back(monster);
	}
	return possible_targets;
}

int ObjectManager::CountJungleMonstersInRange(Vector3 position, float range)
{
	int jungleMonstersInRange = 0;
	for (auto jungle : ObjectManager::GetJungleMonsters()) {
		if (!jungle) continue;
		if (jungle->GetPosition().distanceTo(position) > range) continue;
		jungleMonstersInRange++;
	}
	return jungleMonstersInRange;
}

std::vector<Object*> ObjectManager::GetJunglePlants()
{
	std::vector<Object*> possible_targets;
	for (auto plant : ObjectManager::GetMinions()) {
		if (!plant) continue;
		if (plant->GetCharacterData()->GetObjectTypeHash() != Plants) continue;
		if (plant->IsAlive() and plant->IsVisible() and plant->IsTargetable() and !plant->IsInvulnerable())
			possible_targets.push_back(plant);
	}
	return possible_targets;
}

std::vector<Object*> ObjectManager::GetJungleRespawnCamps()
{
	std::vector<Object*> possible_targets;
	for (auto campRespawn : ObjectManager::GetMinions()) {
		if (!campRespawn) continue;
		if (campRespawn->GetCharacterData()->GetObjectTypeHash() != ObjectType::RespawnMarker) continue;
		possible_targets.push_back(campRespawn);
	}
	return possible_targets;
}

std::vector<Object*> ObjectManager::GetWards(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto ward : ObjectManager::GetMinions()) {
		if (!ward) continue;
		if (team == Alliance::Ally && !ward->IsAlly() || team == Alliance::Enemy && !ward->IsEnemy()) continue;

		if (!ward->IsWard()) continue;
		if (ward->IsAlive())
			possible_targets.push_back(ward);
	}
	return possible_targets;
}

std::vector<Object*> ObjectManager::GetTurretsAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto turret : ObjectManager::GetTurrets()) {
		if (!turret) continue;
		if (team == Alliance::Ally && !turret->IsAlly() || team == Alliance::Enemy && !turret->IsEnemy()) continue;
		if (turret->IsAlive() and turret->IsVisible() and turret->IsTargetable() and !turret->IsInvulnerable())
			possible_targets.push_back(turret);
	}

	return possible_targets;
}

int ObjectManager::CountTurretsInRange(Alliance team, Vector3 position, float range)
{
	int turretsInRange = 0;
	for (auto turret : ObjectManager::GetMinionsAs(team)) {
		if (!turret) continue;
		if (turret->GetPosition().distanceTo(position) > range) continue;
		turretsInRange++;
	}
	return turretsInRange;
}

std::vector<Object*> ObjectManager::GetInhibitorsAs(Alliance team)
{
	std::vector<Object*> possible_targets;
	for (auto inhibitor : ObjectManager::GetInhibitors()) {
		if (!inhibitor) continue;
		if (team == Alliance::Ally && !inhibitor->IsAlly() || team == Alliance::Enemy && !inhibitor->IsEnemy()) continue;
		if (inhibitor->IsAlive() and inhibitor->IsVisible() and inhibitor->IsTargetable() and !inhibitor->IsInvulnerable())
			possible_targets.push_back(inhibitor);
	}

	return possible_targets;
}

int ObjectManager::CountInhibitorsInRange(Alliance team, Vector3 position, float range)
{
	int inhibitorsInRange = 0;
	for (auto inhibitor : ObjectManager::GetInhibitorsAs(team)) {
		if (!inhibitor) continue;
		if (inhibitor->GetPosition().distanceTo(position) > range) continue;
		inhibitorsInRange++;
	}
	return inhibitorsInRange;
}

Object* ObjectManager::GetNexusAs(Alliance team)
{
	Object* nexusToReturn = nullptr;
	for (auto nexus : ObjectManager::GetNexuses()) {
		if (!nexus) continue;
		if (team == Alliance::Ally && !nexus->IsAlly() || team == Alliance::Enemy && !nexus->IsEnemy()) continue;
		if (nexus->IsAlive() and nexus->IsVisible() and nexus->IsTargetable() and !nexus->IsInvulnerable())

			if (!nexusToReturn) {
				nexusToReturn = nexus;
				break;
			}
	}

	return nexusToReturn;
}