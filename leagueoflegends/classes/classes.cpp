#include "../Awareness.h"
#include "../ListManager.h"
#include "../stdafx.h"
#include "../TargetSelector.h"
#include "../ObjectTypeHolder.h"

CombatType Object::GetCombatType()
{
	return *(CombatType*)((QWORD)this + UPasta::Offsets::GameObject::CombatType);
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
	const auto TextureInfoPtr = this + UPasta::Offsets::SpellSlot::Info::TexturePath;
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
			// Controlla se l'oggetto è già stato aggiunto da un altro slot
			auto found = itemToSlotMap.find(itemPtr->GetId());
			if (found != itemToSlotMap.end())
			{
				// Se l'oggetto è già stato trovato in un altro slot, scegli se sovrascrivere o ignorare
				// Esempio: sovrascrivi l'index precedente con il nuovo
				rs[found->second] = itemPtr;
			}
			else
			{
				// Aggiungi l'oggetto alla lista e registra il suo slot
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
		UPasta::Offsets::ItemManager::Instance);
	if (IsNotZeroPtr(heroInventory) && IsValidPtr(heroInventory))
	{
		return heroInventory;
	}

	LOG("GetHeroInventory Doesnt work");
	return nullptr;
}

PerkID Perk::GetId()
{
	const auto perkId = *(PerkID*)((QWORD)this + UPasta::Offsets::GameObject::Perks::PerkID);
	return perkId;
}

std::string Perk::GetName()
{
	const auto perkname = *(char**)((QWORD)this + UPasta::Offsets::GameObject::Perks::PerkName);
	if (IsValidPtr(perkname))
		return perkname;

	LOG("GetName Doesnt work");
	return "";
}

std::string Perk::GetRawName()
{
	const auto perkRawName = *(char**)((QWORD)this + UPasta::Offsets::GameObject::Perks::PerkRawName);
	if (IsValidPtr(perkRawName))
		return perkRawName;

	LOG("GetRawName Doesnt work");
	return "";
}

std::string Perk::GetRawDescription()
{
	const auto perkRawDescription = *(char**)(*(QWORD*)this + UPasta::Offsets::GameObject::Perks::PerkRawDescription);
	if (IsValidPtr(perkRawDescription))
		return perkRawDescription;

	LOG("GetRawDescription Doesnt work");
	return "";
}

Perk* Perks::GetPerkByIndex(int index)
{
	auto perkSlot = (Perk*)(*(QWORD*)(*(QWORD*)this + (UPasta::Offsets::GameObject::Perks::Size * index)));

	if (IsNotZeroPtr(perkSlot))
		return perkSlot;

	return nullptr;
}

Perks* Object::GetHeroPerks()
{
	const auto heroPerks = (Perks*)(this + UPasta::Offsets::GameObject::Perks::Manager);
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

Vector3 AiManager::GetTargetPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::GameObject::AIManager::TargetPos);
}

bool AiManager::IsMoving()
{
	return *(bool*)((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::IsMoving);
}

int AiManager::GetCurrentSegment()
{
	return *(int*)((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::CurrentSegment);
}

Vector3 AiManager::GetPathStart()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::PathStart);
}

Vector3 AiManager::GetPathEnd()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::PathEnd);
}

int AiManager::GetSegmentsCount()
{
	return *(int*)((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::SegmentsCount);
}

float AiManager::GetDashSpeed()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::AIManager::DashSpeed);
}

bool AiManager::IsDashing()
{
	return *(bool*)((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::IsDashing);
}

Vector3 AiManager::GetPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::GameObject::AIManager::ServerPosition);
}

Vector3 AiManager::GetSegment(int index)
{
	return Engine::ReadVector3(
		*(QWORD*)((QWORD)this + UPasta::Offsets::GameObject::AIManager::Path::Segments) + (min(
			index, this->GetSegmentsCount() - 1) * sizeof(Vector3)));
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
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharData::Size);
}

ChampionID CharacterData::GetHeroID()
{
	return *(ChampionID*)((QWORD)this + UPasta::Offsets::GameObject::CharData::HeroID);
}

QWORD CharacterData::GetObjectTypeHash()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::GameObject::CharData::Type));
}

QWORD CharacterData::GetObjectTypeHashDetailed()
{
	return *(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::GameObject::CharData::Type) +
		UPasta::Offsets::GameObject::CharData::TypeDetailed);
}

std::string SpellData::GetName()
{
	return *(char**)((QWORD)this + UPasta::Offsets::SpellSlot::Name);
}


float SpellData::GetManaCostByLevel(int level)
{
	return *(float*)((QWORD)this + UPasta::Offsets::SpellSlot::Info::DataManaCost + ((level - 1) * sizeof(float)));
}

SpellData* SpellInfo::GetSpellData()
{
	return *(SpellData**)((QWORD)this + UPasta::Offsets::SpellSlot::Info::Data);
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
	*reinterpret_cast<Vector3*>((QWORD)this + UPasta::Offsets::SpellSlot::Input::StartPos) = pos;
}

void SpellInput::SetEndPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + UPasta::Offsets::SpellSlot::Input::EndPos) = pos;
}

void SpellInput::SetClickedPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + UPasta::Offsets::SpellSlot::Input::EndPos + sizeof(Vector3)) = pos;
}

void SpellInput::SetUnkPos(Vector3 pos)
{
	*reinterpret_cast<Vector3*>((QWORD)this + UPasta::Offsets::SpellSlot::Input::EndPos + sizeof(Vector3) * 2) = pos;
}

void SpellInput::SetReleasePos(Vector3 pos)
{
	auto mouseInstance = globals::moduleBase + UPasta::Offsets::Instance::HUD::Mouse::Instance;
	auto mouseScreenPosition = UPasta::Offsets::Instance::HUD::Mouse::Position;
}

int Spell::GetLevel()
{
	return *(int*)((QWORD)this + UPasta::Offsets::SpellSlot::Level);
}

float Spell::GetCooldownTimer()
{
	return *(float*)((QWORD)this + UPasta::Offsets::SpellSlot::Cooldown);
}

float Spell::GetCooldown()
{
	float cooldownTimer = this->GetCooldownTimer();
	if (cooldownTimer == 0.0f) return cooldownTimer;
	return max(cooldownTimer - Engine::GetGameTime(), 0.0f);
}

int Spell::GetStacks()
{
	return *(int*)((QWORD)this + UPasta::Offsets::SpellSlot::Stacks);
}

float Spell::GetTotalCooldown()
{
	return *(float*)((QWORD)this + UPasta::Offsets::SpellSlot::TotalCooldown);
}

float Cooldown::GetBaseCooldown()
{
	return *(float*)((QWORD)this);
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
	return *(SpellInput**)((QWORD)this + UPasta::Offsets::SpellSlot::Input::SpellInput);
}

SpellInfo* Spell::GetSpellInfo()
{
	return *(SpellInfo**)((QWORD)this + UPasta::Offsets::SpellSlot::Info::SpellInfo);
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
	return *(SpellInfo**)((QWORD)this + UPasta::Offsets::GameObject::ActiveCast::Info);
}

SpellInfo* SpellCast::GetProcessSpellInfo()
{
	return *(SpellInfo**)((QWORD)this);
}

int SpellCast::GetSpellId()
{
	return *(int*)((QWORD)this + UPasta::Offsets::SpellCast::SlotID);
}

bool SpellCast::IsAutoAttack()
{
	return *(bool*)((QWORD)this + UPasta::Offsets::SpellCast::IsBasicAttack);
}

bool SpellCast::IsSpell()
{
	return *(bool*)((QWORD)this + UPasta::Offsets::SpellCast::IsSpell);
}

float SpellCast::GetCastTime()
{
	return *(float*)((QWORD)this + UPasta::Offsets::SpellCast::CastedAtTime);
}

std::string SpellCast::GetCasterName()
{
	return *(LolString*)((QWORD)this + UPasta::Offsets::SpellCast::CasterName);
}

Vector3 SpellCast::GetStartPosition()
{
	//auto test = Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::StartPosition);
	//LOG("START %f | %f | %f", test.x, test.y, test.z);
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::StartPosition);
}

Vector3 SpellCast::GetEndPosition()
{
	//auto test = Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::EndPosition);
	//LOG("END %f | %f | %f", test.x, test.y, test.z);
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::EndPosition);
}

Vector3 SpellCast::GetMousePosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::SpellCast::MousePosition);
}

std::string Buff::GetName()
{
	auto namePtr = (QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::Buff::BuffNamePtr));
	if (!IsValidPtr(namePtr)) return "";
	return *(char**)((QWORD)namePtr + UPasta::Offsets::Buff::BuffName);
}

BuffType Buff::GetType()
{
	return *(BuffType*)((QWORD)this + UPasta::Offsets::Buff::BuffType);
}

float Buff::GetStartTime()
{
	return *(float*)((QWORD)this + UPasta::Offsets::Buff::BuffEntryBuffStartTime);
}

float Buff::GetEndTime()
{
	return *(float*)((QWORD)this + UPasta::Offsets::Buff::BuffEntryBuffEndTime);
}

int Buff::GetStacksAlt()
{
	return *(int*)((QWORD)this + UPasta::Offsets::Buff::BuffEntryBuffCountAlt);
}

int Buff::GetStacks()
{
	return *(int*)((QWORD)this + UPasta::Offsets::Buff::BuffEntryBuffCount);
}

int Buff::GetMaxStacks()
{
	return max(this->GetStacksAlt(), this->GetStacks());
}

bool Buff::isActive()
{
	return *(BYTE*)(this + UPasta::Offsets::Buff::BuffEntryBuffCountAlt) && *(int64_t*)(this +
		UPasta::Offsets::Buff::BuffNamePtr) || *(BYTE*)(this + 0x88);
}

Buff* BuffEntry::GetBuff()
{
	auto buff = (Buff*)(*(QWORD*)((QWORD)this + UPasta::Offsets::Buff::BuffEntry));
	if (!IsValidPtr(buff)) return nullptr;
	return buff;
}

BuffEntry* BuffManager::GetBuffEntryByIndex(int index)
{
	auto address = (BuffEntry*)(*(QWORD*)((QWORD)this + (sizeof(QWORD) * index)));
	if (!IsValidPtr(address)) return nullptr;
	return address;
}

unsigned int Object::GetNetId()
{
	return *(int*)((QWORD)this + UPasta::Offsets::GameObject::Index);
}

DWORD Object::GetHandle()
{
	return *(DWORD*)((QWORD)this + UPasta::Offsets::GameObject::Handle);
}

DWORD SpellCast::GetCasterHandle()
{
	return *(DWORD*)((QWORD)this + UPasta::Offsets::SpellCast::CasterHandle);
}

int Object::GetTeam()
{
	return *(int*)((QWORD)this + UPasta::Offsets::GameObject::Team);
}

int Object::GetLevel()
{
	return *(int*)((QWORD)this + UPasta::Offsets::GameObject::Level);
}


float Object::GetExperience()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Experience);
}

Vector3 Object::GetPosition()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::GameObject::Position);
}

bool Object::IsVisible()
{
	//return Functions::IsCanSee(this);
	return *(bool*)((QWORD)this + UPasta::Offsets::GameObject::Visibility);
}

bool Object::IsAlive()
{
	return !Engine::IsDead(this);
	//return !(*(int*)((QWORD)this + oObjAlive) % 2);
}

float Object::GetMana()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Mana);
}

float Object::GetMaxMana()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MaxMana);
}

float Object::GetPercentMana()
{
	return 100 * this->GetMana() / this->GetMaxMana() > 100 ? 100 : 100 * this->GetMana() / this->GetMaxMana();
}

bool Object::IsTargetable()
{
	//return Functions::IsTargetable(this);
	return *(bool*)((QWORD)this + UPasta::Offsets::GameObject::Targetable);
}

bool Object::IsInvulnerable()
{
	return *(bool*)((QWORD)this + UPasta::Offsets::GameObject::Invulnerability);
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
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Health);
}

float Object::GetMaxHealth()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MaxHealth);
}

float Object::GetPercentHealth()
{
	return 100 * this->GetHealth() / this->GetMaxHealth() > 100 ? 100 : 100 * this->GetHealth() / this->GetMaxHealth();
}

unsigned short Object::GetActionState()
{
	return *(unsigned short*)((QWORD)this + UPasta::Offsets::GameObject::State);
}

float Object::GetBaseAttackDamage()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::BaseAtk);
}

float Object::GetAbilityPower()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::AbilityPower);
}

float Object::GetBonusAttackDamage()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::BonusAtk);
}

float Object::GetScale()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Scale);
}

float Object::GetShield()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Shield);
}

float Object::GetPhysicalShield()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::PhysicalShield);
}

float Object::GetMagicalShield()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MagicalShield);
}

float Object::GetMovementSpeed()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MoveSpeed);
}

float Object::GetAttackSpeed()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::AtkSpeedMulti);
}

float Object::GetLethality()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Lethality);
}

float Object::GetArmor()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::Armor);
}

float Object::GetBonusArmor()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::BonusArmor);
}

float Object::GetTotalArmor()
{
	return this->GetArmor() + this->GetBonusArmor();
}

float Object::GetArmorPenetration()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::ArmorPen);
}

float Object::GetMagicResist()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MagicRes);
}

float Object::GetMagicPenetration()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MagicPen);
}

float Object::GetMagicPenetrationMulti()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::MagicPenMulti);
}


float Object::GetTotalMagicPenetration()
{
	return this->GetMagicPenetration() + this->GetMagicPenetrationMulti();
}

float Object::GetAttackRange()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::AttackRange);
}

float Object::GetAbilityHaste()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::AbilityHaste);
}

std::string Object::GetName()
{
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + UPasta::Offsets::GameObject::AiName);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

std::string Object::GetClassicName()
{
	char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(this) + UPasta::Offsets::GameObject::Name);
	if (!IsValidPtr(name))
	{
		LOG("Error in getting Object Name");
		return "";
	}
	return name;
}

BuffManager* Object::GetBuffManager()
{
	return (BuffManager*)(*(QWORD*)((QWORD)this + UPasta::Offsets::Buff::BuffManager));
}

QWORD* Object::GetBuffManagerEntriesEnd()
{
	return *(QWORD**)((QWORD)this + UPasta::Offsets::Buff::BuffManager + UPasta::Offsets::Buff::BuffEntriesEnd);
}

SpellCast* Object::GetActiveSpellCast()
{
	QWORD* activeSpellCastOffset = *(QWORD**)((QWORD)this + UPasta::Offsets::GameObject::ActiveCast::ActiveCast);
	if (!IsValidPtr(activeSpellCastOffset)) return nullptr;
	return (SpellCast*)activeSpellCastOffset;
}

CharacterData* Object::GetCharacterData()
{
	return (CharacterData*)(*(QWORD*)(*(QWORD*)((QWORD)this + UPasta::Offsets::GameObject::CharData::Instance) +
		UPasta::Offsets::GameObject::CharData::Array));
}

AiManager* Object::GetAiManager()
{
	LeagueObfuscation<QWORD> aiManagerObf = *(LeagueObfuscation<QWORD>*)((QWORD)this +
		UPasta::Offsets::GameObject::AIManager::AiManager);
	return (AiManager*)(*(QWORD*)(Decrypt(aiManagerObf) + 0x10));
}

Spell* Object::GetSpellBySlotId(int slotId)
{
	return *(Spell**)((QWORD)this + UPasta::Offsets::GameObject::SpellBook +
		UPasta::Offsets::SpellSlot::SpellBookSpellSlot + (sizeof(QWORD) * slotId));
}

bool isASCII(const std::string& s)
{
	return !std::any_of(s.begin(), s.end(), [](char c)
	{
		return static_cast<unsigned char>(c) > 0x80;
	});
}

std::string MissileData::GetMissileName()
{
	const auto missileName = (char*)((QWORD)this + UPasta::Offsets::Missile::MissileName);
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
		UPasta::Offsets::Missile::SpellName);
	if (IsValidPtr(missileDataPtr))
	{
		const char* textToReturnPtr = reinterpret_cast<char*>(missileDataPtr);
		return textToReturnPtr;
	}

	return "";
}

MissileData* Missile::GetMissileData()
{
	return *(MissileData**)((QWORD)this + UPasta::Offsets::Missile::SpellInfo::SpellInfo);
}

Missile* Object::GetMissileByIndex()
{
	return *(Missile**)((QWORD)this + UPasta::Offsets::Missile::Index);
}

int Missile::GetMissileSrcId()
{
	return *(int*)((QWORD)this + UPasta::Offsets::Missile::SrcIdx);
}

Vector3 Missile::GetSpellStartPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::Missile::StartPosition);
}

Vector3 Missile::GetSpellPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::Missile::Position);
}

Vector3 Missile::GetSpellEndPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::Missile::EndPosition);
}

float Object::GetBoundingRadius()
{
	using fnGetBoundingRadius = float(__fastcall*)(Object* obj);
	auto _fnGetBoundingRadius = (fnGetBoundingRadius)(globals::moduleBase +
		UPasta::Offsets::Functions::Stats::GetBoundingRadius);
	return _fnGetBoundingRadius(this);
}

float Object::GetAttackDelay()
{
	using fnGetAttackDelay = float(__cdecl*)(Object* obj);
	auto _fnGetAttackDelay = (fnGetAttackDelay)(globals::moduleBase +
		UPasta::Offsets::Functions::Stats::GetAttackDelay);
	return _fnGetAttackDelay(this);
}

float Object::GetAttackWindup()
{
	using fnGetAttackWindup = float(__cdecl*)(Object* obj, int flags);
	auto _fnGetAttackWindup = (fnGetAttackWindup)(globals::moduleBase +
		UPasta::Offsets::Functions::Stats::GetAttackCastDelay);
	return _fnGetAttackWindup(this, 0x40);
}

bool Object::IsCastingSpell()
{
	auto spellCast = this->GetActiveSpellCast();
	return spellCast != nullptr && !spellCast->IsAutoAttack();
}

bool Object::CanAttack()
{
	return this->GetActionState() & ActionState::CanAttack && !globals::localPlayer->GetBuffByName("KaisaE");
}

bool Object::CanCast()
{
	return this->GetActionState() & ActionState::CanCast;
}

bool Object::CanMove()
{
	return this->GetActionState() & ActionState::CanMove;
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
	return this->GetCharacterData()->GetObjectTypeHash() == RespawnMarker;
}

bool Object::IsMelee()
{
	return this->GetCombatType() == CombatType::Melee;
}

bool Object::IsRanged()
{
	return this->GetCombatType() == CombatType::Ranged;
}

bool Object::IsHero()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Champion;
}

bool Object::IsSpecial()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Special;
}

bool Object::IsWard()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Ward;
}

bool Object::IsMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane;
}

bool Object::IsSiegeMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Siege;
}

bool Object::IsVoidMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Special_Void;
}

bool Object::IsRangedMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Ranged;
}

bool Object::IsMeleeMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Melee;
}

bool Object::IsSuperMinion()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Minion_Lane_Super;
}

bool Object::IsMonster()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Monster;
}

bool Object::IsDragon()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Monster_Dragon;
}

bool Object::IsEpicMonster()
{
	return this->GetCharacterData()->GetObjectTypeHash() == Monster_Epic;
}

bool Object::IsTurret()
{
	//Doesnt work idk why 
	return this->GetCharacterData()->GetObjectTypeHash() == Structure_Turret;
}

bool Object::IsBuilding()
{
	return Engine::GetCollisionFlags(this->GetPosition()) == Tower;
}

float Object::CharGetAttackDamage()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::BaseAtk);
}

float Object::GetAttackDamage()
{
	return this->GetBaseAttackDamage() + this->GetBonusAttackDamage();
}

float Object::GetEffectiveHealth(int damageType)
{
	if (damageType == True) return this->GetHealth();
	return this->GetHealth() * (1 + (damageType == Physical ? this->GetArmor() : this->GetAbilityPower()) / 100);
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


bool Vector3::IsUnderEnemyTower()
{
	const auto turret = UPasta::SDK::TargetSelector::Functions::GetEnemyTurretInRange(2000.0f);
	if (turret && turret->IsEnemy())
		return this->distanceTo(turret->GetPosition()) <= 992.0f;

	return false;
}

bool Object::IsUnderEnemyTower()
{
	return this->GetPosition().IsUnderEnemyTower();
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
	return globals::localPlayer->GetRealAttackRange() + globals::localPlayer->GetBoundingRadius() >= render::Distance(
		globals::localPlayer->GetPosition(), this->GetPosition());
}

bool Object::CanCastSpell(int slotId)
{
	auto spell = this->GetSpellBySlotId(slotId);
	return this->CanCast() && spell->IsReady() && spell->GetManaCost() <= this->GetMana();

	//return this->CanCast() && Engine::GetSpellState(slotId) == SpellState::IsReady;
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


bool Object::HasBuff(const char* buffname)
{
	return Engine::Read<bool>(RVA(UPasta::Offsets::Functions::Stats::HasBuff)), this, 0, RealNameInHash(buffname);
}

int Object::GetBuffListSize()
{
	return static_cast<int>((QWORD)this->GetBuffManagerEntriesEnd() - (QWORD)this->GetBuffManager()) / static_cast<int>(
		sizeof(QWORD));
}

Buff* Object::GetBuffByName(std::string name)
{
	for (int i = 0; i < this->GetBuffListSize(); i++)
	{
		auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= Engine::GetGameTime() && buff->GetName() == name) return buff;
	}
	return nullptr;
}

Buff* Object::GetBuffByType(BuffType type)
{
	for (int i = 0; i < this->GetBuffListSize(); i++)
	{
		auto buffEntry = this->GetBuffManager()->GetBuffEntryByIndex(i);
		if (!buffEntry) return nullptr;
		auto buff = buffEntry->GetBuff();
		if (buff && buff->GetEndTime() >= Engine::GetGameTime() && buff->GetType() == type) return buff;
	}
	return nullptr;
}

CharacterDataStack* Object::GetCharacterDataStack()
{
	return (CharacterDataStack*)((QWORD)this + UPasta::Offsets::GameObject::CharData::Instance);
}

#pragma region CharacterStateIntermediate
CharacterStateIntermediate* Object::GetCharacterStateIntermediate()
{
	return (CharacterStateIntermediate*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oCharacterStateIntermediate);
}

float CharacterStateIntermediate::GetAbilityHasteMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oAbilityHasteMod);
}

float CharacterStateIntermediate::GetPercentCooldownCapMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentCooldownCapMod);
}

float CharacterStateIntermediate::GetPassiveCooldownEndTime()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPassiveCooldownEndTime);
}

float CharacterStateIntermediate::GetPassiveCooldownTotalTime()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPassiveCooldownTotalTime);
}

float CharacterStateIntermediate::GetFlatPhysicalDamageMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatPhysicalDamageMod);
}

float CharacterStateIntermediate::GetPercentPhysicalDamageMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentPhysicalDamageMod);
}

float CharacterStateIntermediate::GetPercentBonusPhysicalDamageMod()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentBonusPhysicalDamageMod);
}

float CharacterStateIntermediate::GetPercentBasePhysicalDamageAsFlatBonusMod()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentBasePhysicalDamageAsFlatBonusMod);
}

float CharacterStateIntermediate::GetFlatMagicDamageMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatMagicDamageMod);
}

float CharacterStateIntermediate::GetPercentMagicDamageMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentMagicDamageMod);
}

float CharacterStateIntermediate::GetFlatMagicReduction()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatMagicReduction);
}

float CharacterStateIntermediate::GetPercentDamageToBarracksMinionMod()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oGetPercentDamageToBarracksMinionMod);
}

float CharacterStateIntermediate::GetFlatDamageReductionFromBarracksMinionMod()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oGetFlatDamageReductionFromBarracksMinionMod);
}

float CharacterStateIntermediate::GetPercentMagicReduction()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentMagicReduction);
}

float CharacterStateIntermediate::GetFlatCastRangeMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatCastRangeMod);
}

float CharacterStateIntermediate::GetAttackSpeedMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oAttackSpeedMod);
}

float CharacterStateIntermediate::GetPercentAttackSpeedMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentAttackSpeedMod);
}

float CharacterStateIntermediate::GetPercentMultiplicativeAttackSpeedMod()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentMultiplicativeAttackSpeedMod);
}

float CharacterStateIntermediate::GetBaseAttackDamage()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oBaseAttackDamage);
}

float CharacterStateIntermediate::GetBaseAttackDamageSansPercentScale()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oBaseAttackDamageSansPercentScale);
}

float CharacterStateIntermediate::GetFlatBaseAttackDamageMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatBaseAttackDamageMod);
}

float CharacterStateIntermediate::GetPercentBaseAttackDamageMod()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentBaseAttackDamageMod);
}

float CharacterStateIntermediate::GetBaseAbilityDamage()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oBaseAbilityDamage);
}

float CharacterStateIntermediate::GetCritDamageMultiplier()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oCritDamageMultiplier);
}

float CharacterStateIntermediate::GetScaleSkinCoef()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oScaleSkinCoef);
}

float CharacterStateIntermediate::GetDodge()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oDodge);
}

float CharacterStateIntermediate::GetCritPercent()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oCritPercent);
}

float CharacterStateIntermediate::GetFlatBaseHPPoolMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatBaseHPPoolMod);
}

float CharacterStateIntermediate::GetArmor()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oArmor);
}

float CharacterStateIntermediate::GetBonusArmor()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oBonusArmor);
}

float CharacterStateIntermediate::GetSpellBlock()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oSpellBlock);
}

float CharacterStateIntermediate::GetBonusSpellBlock()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oBonusSpellBlock);
}

float CharacterStateIntermediate::GetHPRegenRate()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oHPRegenRate);
}

float CharacterStateIntermediate::GetBaseHPRegenRate()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oBaseHPRegenRate);
}

float CharacterStateIntermediate::GetMoveSpeed()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oMoveSpeed);
}

float CharacterStateIntermediate::GetAttackRange()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oAttackRange);
}

float CharacterStateIntermediate::GetFlatBubbleRadiusMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatBubbleRadiusMod);
}

float CharacterStateIntermediate::GetPercentBubbleRadiusMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentBubbleRadiusMod);
}

float CharacterStateIntermediate::GetFlatArmorPenetration()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatArmorPenetration);
}

float CharacterStateIntermediate::GetPhysicalLethality()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPhysicalLethality);
}

float CharacterStateIntermediate::GetPercentArmorPenetration()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentArmorPenetration);
}

float CharacterStateIntermediate::GetPercentBonusArmorPenetration()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentBonusArmorPenetration);
}

float CharacterStateIntermediate::GetPercentCritBonusArmorPenetration()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentCritBonusArmorPenetration);
}

float CharacterStateIntermediate::GetPercentCritTotalArmorPenetration()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentCritTotalArmorPenetration);
}

float CharacterStateIntermediate::GetFlatMagicPenetration()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatMagicPenetration);
}

float CharacterStateIntermediate::GetMagicLethality()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oMagicLethality);
}

float CharacterStateIntermediate::GetPercentMagicPenetration()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentMagicPenetration);
}

float CharacterStateIntermediate::GetPercentBonusMagicPenetration()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentBonusMagicPenetration);
}

float CharacterStateIntermediate::GetPercentLifeStealMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentLifeStealMod);
}

float CharacterStateIntermediate::GetPercentSpellVampMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentSpellVampMod);
}

float CharacterStateIntermediate::GetPercentOmnivampMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentOmnivampMod);
}

float CharacterStateIntermediate::GetPercentPhysicalVamp()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentPhysicalVamp);
}

float CharacterStateIntermediate::GetPathfindingRadiusMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPathfindingRadiusMod);
}

float CharacterStateIntermediate::GetPercentCCReduction()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentCCReduction);
}

float CharacterStateIntermediate::GetPercentEXPBonus()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPercentEXPBonus);
}

float CharacterStateIntermediate::GetFlatBaseArmorMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatBaseArmorMod);
}

float CharacterStateIntermediate::GetFlatBaseSpellBlockMod()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oFlatBaseSpellBlockMod);
}

float CharacterStateIntermediate::GetPrimaryARBaseRegenRateRep()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oPrimaryARBaseRegenRateRep);
}

float CharacterStateIntermediate::GetSecondaryARRegenRateRep()
{
	return *(float*)((QWORD)this + UPasta::Offsets::GameObject::CharacterStateIntermediate::oSecondaryARRegenRateRep);
}

float CharacterStateIntermediate::GetSecondaryARBaseRegenRateRep()
{
	return *(float*)((QWORD)this +
		UPasta::Offsets::GameObject::CharacterStateIntermediate::oSecondaryARBaseRegenRateRep);
}

#pragma endregion

int ObjectManager::GetListSize()
{
	return *(int*)((QWORD)this + UPasta::Offsets::Instance::Lists::ManagerListSize);
}

Object* ObjectManager::GetIndex(int index)
{
	return *(Object**)(*(QWORD*)((QWORD)this + UPasta::Offsets::Instance::Lists::ManagerList) + (sizeof(QWORD) *
		index));
}

Vector3 SpellData::GetSpellEndPos()
{
	return Engine::ReadVector3((QWORD)this + UPasta::Offsets::Missile::EndPosition);
}
