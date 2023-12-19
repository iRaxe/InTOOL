#include <stdbool.h>

#include "../ListManager.h"
#include "../Orbwalker.h"
#include "../stdafx.h"

namespace Engine
{
	float lastRefreshTime = 0.0f;
	
	void* spoof_trampoline = 0x0;

	void Init()
	{
		spoof_trampoline = (void*)mem::ScanModInternal((char*)"\xFF\x23", (char*)"xx", (char*)globals::moduleBase);
	}

	template <typename T, typename U>
	T Read(U addr) {
		return *reinterpret_cast<T*>((uintptr_t)addr);
	}

	template<typename T>
	void PrintChat(T value)
	{
		std::ostringstream stream;
		stream << CHAT_COLOR_DT("#7ce9ff", "[" + ConvertTime(GetGameTime()) + "] ") << value;
		call_function<void>(RVA(UPasta::Offsets::Functions::HUD::Chat::PrintChat), RVA(UPasta::Offsets::Instance::HUD::Chat::Client), stream.str().c_str(), 4);
	}

	void PrintChat(void* address) {
		PrintChat(GetHexString(reinterpret_cast<QWORD>(address)));
	}

	template <>
	std::vector<int> getCooldownData<std::vector<int>>(const std::string& playerNameToFind, const std::string& spellNameToFind) {
		for (const auto& playerData : UPasta::SDK::ListManager::Functions::playerSpells) {
			if (playerData.first == playerNameToFind) {
				for (const auto& spellData : playerData.second) {
					for (const auto& spell : spellData.second) {
						if (spell.first == spellNameToFind) {
							return spell.second;
						}
					}
				}
			}
		}
		return std::vector<int>();
	}

	template <>
	std::string getCooldownData<std::string>(const std::string& playerNameToFind, const std::string& spellNameToFind) {
		for (const auto& playerData : UPasta::SDK::ListManager::Functions::playerSpells) {
			for (const auto& spellData : playerData.second) {
				for (const auto& spell : spellData.second) {
					if (spell.first == spellNameToFind) {
						return playerData.first;
					}
				}
			}
		}
		return "";
	}

	template <>
	int getCooldownData<int>(const std::string& playerNameToFind, const std::string& spellNameToFind) {
		for (const auto& playerData : UPasta::SDK::ListManager::Functions::playerSpells) {
			if (playerData.first == playerNameToFind) {
				for (const auto& spellData : playerData.second) {
					for (const auto& spell : spellData.second) {
						if (spell.first == spellNameToFind) {
							return spellData.first;
						}
					}
				}
			}
		}
		return -1;
	}

	bool isShopOpen()	{
		return Engine::Read<bool>(RVA(UPasta::Offsets::Instance::HUD::Shop::ShopInstance)), UPasta::Offsets::Instance::HUD::Shop::IsOpen;
	}

	std::string GetHexString(QWORD hexNumber)
	{
		std::stringstream ss;
		ss << std::hex << hexNumber;
		return ss.str();
	}

	std::string CapitalizeFirstLetter(std::string str)
	{
		if (!str.empty()) {
			str[0] = std::toupper(static_cast<unsigned char>(str[0]));
		}
		return str;
	}

	Vector3 ReadVector3(QWORD offset)
	{
		Vector3 result;
		result.x = *(float*)(offset);
		result.y = *(float*)(offset + 0x4);
		result.z = *(float*)(offset + 0x8);
		return result;
	}

	Vector2 ReadVector2(QWORD offset)
	{
		Vector2 result;
		result.x = *(float*)(offset);
		result.y = *(float*)(offset + 0x4);
		return result;
	}

	void WriteVector3(QWORD offset, Vector3 vector)
	{
		*(float*)(offset) = vector.x;
		*(float*)(offset + 0x4) = vector.y;
		*(float*)(offset + 0x8) = vector.z;
	}

	void WriteVector2(QWORD offset, Vector2 vector)
	{
		*(float*)(offset) = vector.x;
		*(float*)(offset + 0x4) = vector.y;
	}


	GameState GetGameState() {
		auto addr = Read<uintptr_t>(RVA(UPasta::Offsets::Instance::Game::GameState));
		return addr ? Read<GameState>(addr + 0xC) : Loading;
	}

	std::string GetServerIP()
	{
		auto bho = Read<char*>((Read<uintptr_t>(RVA(UPasta::Offsets::Instance::Game::MapInfo::GameMap)) + 0x140));
		auto funziona = *(char**)(*(QWORD*)(globals::moduleBase + UPasta::Offsets::Instance::Game::MapInfo::GameMap) + 0x140);

		char nameobj[18];
		memcpy(nameobj, reinterpret_cast<void*>(*(reinterpret_cast<QWORD*>(globals::moduleBase + UPasta::Offsets::Instance::Game::MapInfo::GameMap) + 0x140)), sizeof(nameobj));
		return funziona;
	}

	float GetGameTime()
	{
		return Read<float>(RVA(UPasta::Offsets::Instance::Game::GameTime));
	}

	int GetGameTick()
	{
		return (int)(GetGameTime() * 1000);
	}

	std::string ConvertTime(float seconds)
	{
		int minutes = (int)(seconds / 60);
		seconds = (float)fmod(seconds, 60);
		int secs = (int)seconds;
		int milliseconds = (int)round((seconds - secs) * 1000);

		std::stringstream ss;
		ss << std::setw(2) << minutes << ":";
		ss << std::setw(2) << std::setfill('0') << secs;
		//ss << std::setw(3) << std::setfill('0') << milliseconds;

		return ss.str();
	}

	bool IsGameFocused()
	{
		return GetActiveWindow() == hooks::impl::windowDX;
	}

	bool IsChatOpen()
	{
		return Read<bool>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::Chat::Client)) + UPasta::Offsets::Instance::HUD::Chat::IsOpen);
	}

	Vector2 GetMousePos()
	{
		if (!IsGameFocused()) return Vector2();
		POINT curMouse;
		bool getMouse = GetCursorPos(&curMouse);
		return Vector2((float)curMouse.x, (float)curMouse.y);
	}

	Vector3 GetMouseWorldPos()
	{
		return ReadVector3(Read<uintptr_t>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::HudInstance)) + UPasta::Offsets::Instance::HUD::Input) + UPasta::Offsets::Instance::HUD::Mouse::InputMouseWorldPos);
	}

	Vector2 WorldToScreen(Vector3 in)
	{
		Vector3 out;
		call_function<uintptr_t>(RVA(UPasta::Offsets::Functions::Drawings::WorldToScreen), Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::Viewport::ViewPort)) + UPasta::Offsets::Instance::HUD::Viewport::W2S, &in, &out);
		return Vector2(out.x, out.y);
	}

	float GetHeightAtPosition(Vector3 in)
	{
		float World_y;
		typedef float(__fastcall* fnpointHeight)(float, float, float*);
		fnpointHeight _fnpointHeight = reinterpret_cast<fnpointHeight>(globals::moduleBase + UPasta::Offsets::Functions::HUD::GetHeightAtPosition);
		_fnpointHeight(in.x, in.z, &World_y);
		return _fnpointHeight(in.x, in.z, &World_y);
	}

	Vector2 GetMinimapPos()
	{
		return ReadVector2(Read<uintptr_t>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::Minimap::MinimapInstance)) + UPasta::Offsets::Instance::HUD::Minimap::HudInstance) + UPasta::Offsets::Instance::HUD::Minimap::Position);
	}

	float GetMinimapSize()
	{
		return Read<float>(Read<uintptr_t>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::Minimap::MinimapInstance)) + UPasta::Offsets::Instance::HUD::Minimap::HudInstance) + UPasta::Offsets::Instance::HUD::Minimap::Size);
	}

	Vector2 WorldToMinimap(Object* objectToShow)
	{
		const Vector3 objDrawPos = GetBaseDrawPosition(objectToShow);
		const Vector2 objMinimapDrawPos(objDrawPos.x, objDrawPos.z);
		const float scale_factor = GetMinimapSize() / 14800.0f;

		const float minimap_x = (objMinimapDrawPos.x * scale_factor) + (GetMinimapPos().x - 12.0f);
		const float minimap_y = (GetMinimapPos().y - 13.0f) + GetMinimapSize() - (objMinimapDrawPos.y * scale_factor);

		return { minimap_x, minimap_y };
	}

	Vector2 WorldToMinimap(Vector3 posToShow)
	{
		const Vector2 objMinimapDrawPos(posToShow.x, posToShow.z);
		const float scale_factor = GetMinimapSize() / 14800.0f;

		const float minimap_x = (objMinimapDrawPos.x * scale_factor) + (GetMinimapPos().x - 12.0f);
		const float minimap_y = (GetMinimapPos().y - 13.0f) + GetMinimapSize() - (objMinimapDrawPos.y * scale_factor);

		return { minimap_x, minimap_y };
	}


	Vector3 GetBaseDrawPosition(Object* obj)
	{
		Vector3 out;
		call_function<bool>(RVA(UPasta::Offsets::Functions::Drawings::GetBaseDrawPosition), (QWORD*)obj, &out);
		return out;
	}
	
	Vector2 GetHpBarPosition(Object* obj)
	{
		Vector3 hpBarPos = obj->GetPosition();
		const float hpBarHeight = Read<float>(obj->GetCharacterData() + UPasta::Offsets::CharData::Size) * obj->GetScaleMulti();
		hpBarPos.y += hpBarHeight;

		auto screenPos = WorldToScreen(hpBarPos);
		const float maxZoom = Read<float>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::Camera::CameraInstance)) + UPasta::Offsets::Instance::HUD::Camera::Limit);
		const float currentZoom = Read<float>(Read<uintptr_t>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::HudInstance)) + UPasta::Offsets::Instance::HUD::Camera::Camera) + UPasta::Offsets::Instance::HUD::Camera::Value);
		const float zoomDelta = maxZoom / currentZoom;

		screenPos.y -= (((globals::windowHeight) * 0.00083333335f * zoomDelta) * hpBarHeight);
		
		return screenPos;
	}

	Object* GetPlayerPointer(const std::string& playerNameToFind)
	{
		for (Object* obj : *ObjectManager::GetHeroList())
		{
			//if (obj->IsAlly()) continue;
			if (obj->GetName() == playerNameToFind)
				return obj;
		}
		return nullptr;
	}

	Object* GetOwner(Object* obj)
	{
		return call_function<Object*>(RVA(UPasta::Offsets::Functions::Stats::GetOwner), obj);
	}

	Object* GetObjectFromNetId(int netId)
	{
		return call_function<Object*>(RVA(UPasta::Offsets::Functions::Stats::GetObjectFromNetID), Read<uintptr_t>(RVA(UPasta::Offsets::Instance::Lists::ObjManager)), netId);
	}

	Object* GetSelectedObject()
	{
		const auto targetNetId = Read<unsigned int>(Read<uintptr_t>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::HudInstance)) + UPasta::Offsets::Instance::HUD::UserData) + UPasta::Offsets::Instance::HUD::UserDataSelectedNetId);
		if (!targetNetId)
			return nullptr;


		return call_function<Object*>(RVA(UPasta::Offsets::Functions::Stats::GetObjectFromNetID), Read<uintptr_t>(RVA(UPasta::Offsets::Instance::Lists::ObjManager)), targetNetId);
	}

	void SendPing(int pingType, Vector3 pos)
	{
		call_function<void>(RVA(UPasta::Offsets::Functions::HUD::SendPing), pingType, &pos);
	}

	unsigned int GetCollisionFlags(Vector3 pos)
	{
		return call_function<unsigned int>(RVA(UPasta::Offsets::Functions::Drawings::GetCollisionFlags), pos);
	}

	float GetRespawnTimer(Object* obj)
	{
		return call_function<float>(RVA(UPasta::Offsets::Events::Objects::GetRespawnTimeRemaining), obj);
	}

	int GetSpellSlot(Object* hero, SpellInfo* spellInfo)
	{
		return call_function<int>(RVA(UPasta::Offsets::Functions::Spells::GetSpellSlot), hero, spellInfo);
	}

	int GetSpellState(int slotId)
	{
		// Ready = 0 || NotAvailable = 4 || Supressed = 8 || NotLearned = 12 || Disabled = 16 || Processing = 24 || Cooldown = 32 || NoMana = 64 || Unknown = 96
		typedef int(__fastcall* fnGetSpellState)(void*, int, const QWORD&);
		fnGetSpellState _fnGetSpellState = (fnGetSpellState)(globals::moduleBase + UPasta::Offsets::Functions::Spells::GetSpellState);
		return _fnGetSpellState(ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::SpellBookInstance, slotId, NULL);
	}

	float GetSpellRange(Spell* spellID)
	{
		return call_function<float>(RVA(UPasta::Offsets::Functions::Spells::GetSpellRange), spellID);
	}

	QWORD GetZoomAddress()
	{
		return (*(QWORD*)(*(QWORD*)(globals::moduleBase + UPasta::Offsets::Instance::HUD::HudInstance) + UPasta::Offsets::Instance::HUD::Camera::Camera) + 0x2B8);
	}

	bool IsNotLocalPlayer(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsNotLocalPlayer), obj);
	}

	bool IsAlive(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsAlive), obj);
	}

	bool IsHero(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsHero), obj);
	}

	bool IsMinion(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsMinion), obj);
	}

	bool IsTurret(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsTurret), obj);
	}

	bool IsMissile(Missile* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsMissile), obj);
	}

	bool IsInhibitor(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsInhibitor), obj);
	}

	bool IsNexus(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsNexus), obj);
	}

	bool IsDead(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsDead), obj);
	}

	bool IsCanSee(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsCanSee), obj);
	}

	bool SellItem(InventorySlot* slotID)
	{
		auto test = ObjectManager::GetLocalPlayer()->GetSpellBySlotId(SpellIndex::Item1);
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Shop::SellItem), test);
	}

	bool IsTargetable(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsTargetable), obj);
	}

	bool IsVisible(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsVisible), obj);
	}

	bool IsVisible(Missile* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsVisible), obj);
	}

	bool IsBrush(Vector3 pos)
	{
		return GetCollisionFlags(pos) & CollisionFlags::Grass;
	}

	bool IsWall(Vector3 pos)
	{
		return GetCollisionFlags(pos) & CollisionFlags::Wall;
	}

	bool IsAttackWindupSpell(int spellId)
	{
		return spellId >= 14;
	}

	float GetStat(Object* obj, StatEnum Stat, StatType Type)
	{
		auto v2 = (float*)obj + 0xA4;
		return call_function<float>(RVA(UPasta::Offsets::Functions::Stats::GetStatTotal), v2, Stat, Type);
	}

	int GetTeam(Object* obj)
	{
		return call_function<int>(RVA(UPasta::Offsets::Functions::Stats::GetTeam), obj);
	}

	bool IsTargetableToTeam(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsTargetableToTeam), obj);
	}

	bool IsCastingSpell(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsCasting), obj);
	}

	bool IsValid(Object* obj)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsValid), obj);
	}

	bool CanUpgradeSpell(int spellId)
	{
		return call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::IsSpellslotUpgradable), ObjectManager::GetLocalPlayer(), spellId);
	}

	void UpgradeSpell(int spellId)
	{
		if (ObjectManager::GetLocalPlayer() == nullptr || spellId > 3) return;
		call_function<bool>(RVA(UPasta::Offsets::Functions::Stats::HudUpgradeSpell), ObjectManager::GetLocalPlayer(), spellId);
	}

	void GlowObject(Object* obj)
	{
		if (ObjectManager::GetLocalPlayer() == nullptr) return;
		call_function<void>(RVA(UPasta::Offsets::Functions::D3DX::Functions::Riot_RenderPipelineLOL_RenderMouseOvers), obj);
	}



	void TryRightClick(Vector2 pos)
	{
		float floatCheck1 = *(float*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickFloatCheck1);
		float floatCheck2 = *(float*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickFloatCheck2);
		DWORD check = *(DWORD*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickCheck);

		*(float*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickFloatCheck1) = 0.0f;
		*(float*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickFloatCheck2) = 0.0f;
		*(DWORD*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickCheck) = 0x0;

		unsigned int* params = new unsigned int[20];
		params[17] = (int)pos.x;
		params[18] = (int)pos.y;
		params[19] = 2;

		call_function<bool>(RVA(UPasta::Offsets::Functions::Orders::IssueRClick), (QWORD*)ObjectManager::GetLocalPlayer(), params);

		*(float*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickFloatCheck1) = floatCheck1;
		*(float*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickFloatCheck2) = floatCheck2;
		*(DWORD*)((QWORD)ObjectManager::GetLocalPlayer() + UPasta::Offsets::Client::IssueClickCheck) = check;
		Event::Publish(Event::OnAfterAttack);

	}

	void IssueClick(Vector2 pos)
	{
		unsigned int* params = new unsigned int[20];
		params[17] = (int)pos.x;
		params[18] = (int)pos.y;
		params[19] = 2;

		call_function<bool>(RVA(UPasta::Offsets::Functions::Orders::IssueClick), (QWORD*)ObjectManager::GetLocalPlayer(), params[19], 0, 0, params[17], params[18], 0);
	}

	void IssueMove(Vector2 pos)
	{
		call_function<bool>(RVA(UPasta::Offsets::Functions::Orders::IssueMove), (QWORD*)(*(QWORD*)(*(QWORD*)(globals::moduleBase + UPasta::Offsets::Instance::HUD::HudInstance) + UPasta::Offsets::Instance::HUD::Input)), (int)pos.x, (int)pos.y, false, 0, 0);
	}

	bool IsSpellSlotValid(int spellId) {
		return spellId >= 0 && spellId < 14;
	}

	uintptr_t GetInputLogic() {
		return Read<uintptr_t>(Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::HudInstance)) + UPasta::Offsets::Instance::HUD::SpellInfo);
	}

	uintptr_t GetMouseInstance()	{
		return Read<uintptr_t>(RVA(UPasta::Offsets::Instance::HUD::Mouse::MouseInstance)) + UPasta::Offsets::Instance::HUD::Mouse::Position;
	}

	Vector2 GetMouseInstancePosition() {
		const auto castScreenPosition = GetMouseInstance();
		return Vector2(*(int*)castScreenPosition, *(int*)(castScreenPosition + 0x4));
	}

	void UpdateMouseInstancePosition(Vector2 newPosition) {
		const auto castScreenPosition = GetMouseInstance();

		*(int*)(castScreenPosition) = newPosition.x;
		*(int*)(castScreenPosition + 0x4) = newPosition.y;
	}

	bool CastSpellLogic(int spellId,SpellCastMode castType, Vector2 originalMousePosition) {
		const auto InputLogic = GetInputLogic();
		call_function<void>(RVA(UPasta::Offsets::Functions::Spells::NewCastSpell), InputLogic, spellId, castType, 0);
		UpdateMouseInstancePosition(originalMousePosition);
		return true;
	}

	bool CastSelf(SpellIndex slotID)
	{
		Object* localPlayer = ObjectManager::GetLocalPlayer();
		if (localPlayer == nullptr) return false;

		Spell* spellPtr = localPlayer->GetSpellBySlotId(slotID);
		if (spellPtr == nullptr) return false;

		SpellInput* inputLogic = spellPtr->GetSpellInput();
		if (inputLogic == nullptr) return false;

		AiManager* localManager = localPlayer->GetAiManager();
		if (localManager == nullptr) return false;

		Vector3 serverPosition = localManager->GetPosition();
		if (!serverPosition.IsValid()) return false;

		Vector2 pos2WS = WorldToScreen(serverPosition);
		if (!pos2WS.IsValid()) return false;

		Vector2 mouseScreenPosition = GetMouseInstancePosition();
		if (!mouseScreenPosition.IsValid()) return false;

		UpdateMouseInstancePosition(pos2WS);
		inputLogic->SetCaster(localPlayer->GetHandle());
		inputLogic->SetTarget(0);
		inputLogic->SetStartPos(serverPosition);
		inputLogic->SetEndPos(serverPosition);
		inputLogic->SetClickedPos(serverPosition);
		inputLogic->SetUnkPos(serverPosition);


		return CastSpellLogic(slotID, SmartCast, mouseScreenPosition);
	}

	bool CastTargeted(SpellIndex slotID, Object* target)
	{
		Object* localPlayer = ObjectManager::GetLocalPlayer();
		if (localPlayer == nullptr) return false;
		if (target == nullptr) return false;

		Spell* spellPtr = localPlayer->GetSpellBySlotId(slotID);
		if (spellPtr == nullptr) return false;

		SpellInput* inputLogic = spellPtr->GetSpellInput();
		if (inputLogic == nullptr) return false;

		AiManager* myLocalManager = localPlayer->GetAiManager();
		if (myLocalManager == nullptr) return false;
		AiManager* hisLocalManager = target->GetAiManager();
		if (hisLocalManager == nullptr) return false;

		Vector3 myServerPosition = myLocalManager->GetPosition();
		if (!myServerPosition.IsValid()) return false;
		Vector3 hisServerPosition = target->GetPosition();
		if (!hisServerPosition.IsValid()) return false;

		Vector2 pos2WS = WorldToScreen(hisServerPosition);
		if (!pos2WS.IsValid()) return false;

		Vector2 mouseScreenPosition = GetMouseInstancePosition();
		if (!mouseScreenPosition.IsValid()) return false;

		UpdateMouseInstancePosition(pos2WS);
		inputLogic->SetCaster(localPlayer->GetHandle());
		inputLogic->SetTarget(target->GetHandle());
		inputLogic->SetStartPos(myServerPosition);
		inputLogic->SetEndPos(hisServerPosition);
		inputLogic->SetClickedPos(hisServerPosition);
		inputLogic->SetUnkPos(hisServerPosition);

		return CastSpellLogic(slotID, SmartCast, mouseScreenPosition);
	}

	bool CastToPosition(SpellIndex slotID, Vector3 worldPos)
	{
		Object* localPlayer = ObjectManager::GetLocalPlayer();
		if (localPlayer == nullptr) return false;
		if (!worldPos.IsValid()) return false;

		Spell* spellPtr = localPlayer->GetSpellBySlotId(slotID);
		if (spellPtr == nullptr) return false;

		SpellInput* inputLogic = spellPtr->GetSpellInput();
		if (inputLogic == nullptr) return false;

		AiManager* localManager = localPlayer->GetAiManager();
		if (localManager == nullptr) return false;

		Vector3 serverPosition = localManager->GetPosition();
		if (!serverPosition.IsValid()) return false;

		Vector2 pos2WS = WorldToScreen(worldPos);
		if (!pos2WS.IsValid()) return false;

		Vector2 mouseScreenPosition = GetMouseInstancePosition();
		if (!mouseScreenPosition.IsValid()) return false;

		UpdateMouseInstancePosition(pos2WS);
		inputLogic->SetCaster(localPlayer->GetHandle());
		inputLogic->SetTarget(0);
		inputLogic->SetStartPos(serverPosition);
		inputLogic->SetEndPos(worldPos);
		inputLogic->SetClickedPos(worldPos);
		inputLogic->SetUnkPos(worldPos);

		return CastSpellLogic(slotID, SmartCast, mouseScreenPosition);
	}

	bool ReleaseSpell(int spellId, Vector3 pos)
	{
		if (!IsSpellSlotValid(spellId))
			return false;

		Object* me = ObjectManager::GetLocalPlayer();
		Spell* spell = me->GetSpellBySlotId(spellId);
		if (!spell || !spell->GetSpellInput() || !pos.IsValid())
			return false;

		const auto mouseScreenPosition = GetMouseInstancePosition();

		if (pos.x || pos.y)
		{
			const auto posw2s = WorldToScreen(pos);
			UpdateMouseInstancePosition(posw2s);
		}

		const auto InputLogic = GetInputLogic();
		call_function<void>(RVA(UPasta::Offsets::Functions::Spells::ReleaseSpell), InputLogic, (QWORD*)0);
		UpdateMouseInstancePosition(mouseScreenPosition);
		return true;
	}

	bool CanSendInput()
	{
		return ObjectManager::GetLocalPlayer()->IsAlive() && IsGameFocused() && !IsChatOpen();
	}

	void AttackObject(Vector3 objPos)
	{
		if (!CanSendInput() || !objPos.IsValid()) return;

		auto screenPos = WorldToScreen(objPos);

		TryRightClick(screenPos);
	}

	void AttackObject(Object* obj)
	{
		if (!CanSendInput() || obj == nullptr) return;

		Vector3 headPos = obj->GetPosition();
		const float objectHeight = *(float*)(obj->GetCharacterData() + UPasta::Offsets::CharData::Size) * obj->GetScaleMulti();
		headPos.y += objectHeight - 50.0f;

		auto screenPos = WorldToScreen(headPos);

		TryRightClick(screenPos);
	}

	void MoveToMousePos()
	{
		if (!CanSendInput() || !GetMousePos().IsValid()) return;
		IssueMove(GetMousePos());
	}

	bool MenuItemContains(const std::vector<Object*>& words, const std::string& targetWord) {
		for (const auto word : words)
		{
			if (word->GetName() == targetWord)
			{
				return true;
			}
		}
		return false;
	}


}