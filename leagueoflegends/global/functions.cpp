#include "../Orbwalker.h"
#include "../stdafx.h"

namespace functions
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

	template <typename ReturnType, typename... Args>
	ReturnType call_function(uintptr_t func, Args... args) {
		using func_t = ReturnType(__fastcall*)(Args...);
		return spoof_call(spoof_trampoline, (func_t)func, std::forward<Args>(args)...);
	}

	template <size_t Index, typename ReturnType, typename... Args>
	ReturnType call_virtual(void* instance, Args... args) {
		using fn = ReturnType(__fastcall*)(void*, Args...);
		auto function = (*static_cast<fn**>(instance))[Index];
		return function(instance, std::forward<Args>(args)...);
	}
	template <typename Type>
	Type RPM(const QWORD address)
	{
		if (!address)
			return Type();

		Type buffer;

		return NT_SUCCESS(ReadProcessMemory(GetCurrentProcess(), (LPVOID)address, &buffer, sizeof(Type), NULL)) ? buffer : Type();
	};

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

	void PrintChat(std::string text)
	{
		std::string timeMarkString = "[" + ConvertTime(GetGameTime()) + "] ";
		std::string coloredTimeMarkString = CHAT_COLOR_DT("#7ce9ff", timeMarkString);
		std::string formattedText = coloredTimeMarkString + text;
		call_function<void>(RVA(oPrintChat), RVA(oChatClient), formattedText.c_str(), 4);
	}

	void PrintChat(int number)
	{
		PrintChat(std::to_string(number));
	}

	void PrintChat(float number)
	{
		PrintChat(std::to_string(number));
	}

	void PrintChat(void* address)
	{
		PrintChat(GetHexString((QWORD)address));
	}

	std::string GetServerIP()
	{
		auto bho = Read<char*>((Read<uintptr_t>(RVA(oGameMap)) + 0x140));
		auto funziona = *(char**)(*(QWORD*)(globals::moduleBase + oGameMap) + 0x140);

		char nameobj[18];
		memcpy(nameobj, reinterpret_cast<void*>(*(reinterpret_cast<QWORD*>(globals::moduleBase + oGameMap) + 0x140)), sizeof(nameobj));
		return funziona;
	}

	float GetGameTime()
	{
		return Read<float>(RVA(oGameTime));
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
		return Read<bool>(Read<uintptr_t>(RVA(oChatClient)) + oChatClientChatOpen);
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
		return ReadVector3(Read<uintptr_t>(Read<uintptr_t>(RVA(oHudInstance)) + oHudInstanceInput) + oHudInstanceInputMouseWorldPos);
	}

	Vector2 WorldToScreen(Vector3 in)
	{
		Vector3 out;
		call_function<uintptr_t>(RVA(oWorldToScreen), Read<uintptr_t>(RVA(oViewport)) + oViewportW2S, &in, &out);
		return Vector2(out.x, out.y);
	}

	float GetHeightAtPosition(Vector3 in)
	{
		float World_y;
		typedef float(__fastcall* fnpointHeight)(float, float, float*);
		fnpointHeight _fnpointHeight = reinterpret_cast<fnpointHeight>(globals::moduleBase + fGetHeightAtPosition);
		_fnpointHeight(in.x, in.z, &World_y);
		return _fnpointHeight(in.x, in.z, &World_y);
	}

	Vector2 GetMinimapPos()
	{
		return ReadVector2(Read<uintptr_t>(Read<uintptr_t>(RVA(oMinimapObject)) + MinimapObjectHud) + MinimapHudPos);
	}

	float GetMinimapSize()
	{
		return Read<float>(Read<uintptr_t>(Read<uintptr_t>(RVA(oMinimapObject)) + MinimapObjectHud) + MinimapHudSize);
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
		call_function<bool>(RVA(oGetBaseDrawPosition), (QWORD*)obj, &out);
		return out;
	}
	
	Vector2 GetHpBarPosition(Object* obj)
	{
		Vector3 hpBarPos = obj->GetPosition();
		const float hpBarHeight = Read<float>(obj->GetCharacterData() + oObjCharDataDataSize) * obj->GetScale();
		hpBarPos.y += hpBarHeight;

		auto screenPos = WorldToScreen(hpBarPos);
		const float maxZoom = Read<float>(Read<uintptr_t>(RVA(oZoomInstance)) + oZoomInstanceMaxZoom);
		const float currentZoom = Read<float>(Read<uintptr_t>(Read<uintptr_t>(RVA(oHudInstance)) + oHudInstanceCamera) + oHudInstanceCameraZoom);
		const float zoomDelta = maxZoom / currentZoom;

		screenPos.y -= (((globals::windowHeight) * 0.00083333335f * zoomDelta) * hpBarHeight);
		
		return screenPos;
	}

	Object* GetOwner(Object* obj)
	{
		return call_function<Object*>(RVA(fGetOwner), obj);
	}

	Object* GetObjectFromNetId(int netId)
	{
		return call_function<Object*>(RVA(oGetObjectFromNetId), Read<uintptr_t>(RVA(oGetObjectFromNetIdParam)), netId);
	}

	Object* GetSelectedObject()
	{
		const auto targetNetId = Read<unsigned int>(Read<uintptr_t>(Read<uintptr_t>(RVA(oHudInstance)) + oHudInstanceUserData) + oHudInstanceUserDataSelectedObjectNetId);
		if (!targetNetId)
			return nullptr;

		return call_function<Object*>(RVA(oGetObjectFromNetId), Read<uintptr_t>(RVA(oGetObjectFromNetIdParam)), targetNetId);
	}

	unsigned int GetCollisionFlags(Vector3 pos)
	{
		return call_function<unsigned int>(RVA(oGetCollisionFlags), pos);
	}

	float GetRespawnTimer(Object* obj)
	{
		return call_function<float>(RVA(fGetRespawnTimeRemaining), obj);
	}


	int GetSpellState(int slotId)
	{
		// Ready = 0 || NotAvailable = 4 || Supressed = 8 || NotLearned = 12 || Disabled = 16 || Processing = 24 || Cooldown = 32 || NoMana = 64 || Unknown = 96
		return call_function<int>(RVA(fGetSpellState), globals::localPlayer + oObjSpellBook, slotId, NULL);
	}

	float GetSpellRange(int level)
	{
		//Not working, need find spellDataSpellRange oppure provare a passare spellboox o spellslot
		return call_function<float>(RVA(fGetSpellRange), globals::localPlayer + oObjSpellBook, 0, NULL);
	}

	QWORD GetZoomAddress()
	{
		return (*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceCamera) + 0x2B0);
	}

	bool IsNotLocalPlayer(Object* obj)
	{
		return call_function<bool>(RVA(fIsNotLocalPlayer), obj);
	}

	bool IsAlive(Object* obj)
	{
		return call_function<bool>(RVA(fIsAlive), obj);
	}

	bool IsHero(Object* obj)
	{
		return call_function<bool>(RVA(fIsHero), obj);
	}

	bool IsMinion(Object* obj)
	{
		return call_function<bool>(RVA(fIsMinion), obj);
	}

	bool IsTurret(Object* obj)
	{
		return call_function<bool>(RVA(fIsTurret), obj);
	}

	bool IsMissile(Missile* obj)
	{
		return call_function<bool>(RVA(fIsMissile), obj);
	}

	bool IsInhibitor(Object* obj)
	{
		return call_function<bool>(RVA(fIsInhibitor), obj);
	}

	bool IsNexus(Object* obj)
	{
		return call_function<bool>(RVA(fIsNexus), obj);
	}

	bool IsDead(Object* obj)
	{
		return call_function<bool>(RVA(fIsDead), obj);
	}

	bool IsCanSee(Object* obj)
	{
		return call_function<bool>(RVA(fIsCanSee), obj);
	}

	bool IsTargetable(Object* obj)
	{
		return call_function<bool>(RVA(fIsTargetable), obj);
	}

	bool IsVisible(Object* obj)
	{
		return call_function<bool>(RVA(fIsVisible), obj);
	}

	bool IsVisible(Missile* obj)
	{
		return call_function<bool>(RVA(fIsVisible), obj);
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

	void TryRightClick(Vector2 pos)
	{
		EventManager::TriggerProcess(EventManager::EventType::OnIssueOrder, pos);

		float floatCheck1 = *(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1);
		float floatCheck2 = *(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2);
		DWORD check = *(DWORD*)((QWORD)globals::localPlayer + oObjIssueClickCheck);

		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1) = 0.0f;
		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2) = 0.0f;
		*(DWORD*)((QWORD)globals::localPlayer + oObjIssueClickCheck) = 0x0;

		unsigned int* params = new unsigned int[20];
		params[17] = (int)pos.x;
		params[18] = (int)pos.y;
		params[19] = 2;

		call_function<bool>(RVA(oTryRightClick), (QWORD*)globals::localPlayer, params);

		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1) = floatCheck1;
		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2) = floatCheck2;
		*(DWORD*)((QWORD)globals::localPlayer + oObjIssueClickCheck) = check;
	}

	void IssueClick(Vector2 pos)
	{
		unsigned int* params = new unsigned int[20];
		params[17] = (int)pos.x;
		params[18] = (int)pos.y;
		params[19] = 2;

		call_function<bool>(RVA(oIssueClick), (QWORD*)globals::localPlayer, params[19], 0, 0, params[17], params[18], 0);
	}

	void IssueMove(Vector2 pos)
	{
		EventManager::TriggerProcess(EventManager::EventType::OnIssueMove, pos);

		call_function<bool>(RVA(oIssueMove), (QWORD*)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceInput)), (int)pos.x, (int)pos.y, false, 0, 0);
	}

	bool CastSpell(int spellId, Object* Target)
	{
		if (UPasta::SDK::Orbwalker::Functions::shouldWait) 
			return false;

		EventManager::Trigger(EventManager::EventType::OnCastSpell, spellId);
		Object* me = globals::localPlayer;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();

		if (!TargetInfo || !Target) return false;
		uintptr_t* InputLogic = *(uintptr_t**)(*(uintptr_t*)(globals::moduleBase + oHudInstance) + oHudInstanceSpellInfo);

		// set spell position
		TargetInfo->SetCaster(me->GetNetId());
		TargetInfo->SetTarget(Target->GetNetId());
		TargetInfo->SetStartPos(me->GetPosition());
		TargetInfo->SetEndPos(Target->GetPosition());
		TargetInfo->SetClickedPos(Target->GetPosition());
		TargetInfo->SetUnkPos(Target->GetPosition());
		SpellInfo* spellInfo = spell->GetSpellInfo();

		call_function<void>(RVA(oCastSpellWrapper), InputLogic, spellInfo);

		return true;
	}

	bool CastSpell(int spellId)
	{
		if (UPasta::SDK::Orbwalker::Functions::shouldWait) return false;

		EventManager::Trigger(EventManager::EventType::OnCastSpell, spellId);

		Object* me = globals::localPlayer;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();

		if (!TargetInfo) return false;
		uintptr_t* InputLogic = *(uintptr_t**)(*(uintptr_t*)(globals::moduleBase + oHudInstance) + oHudInstanceSpellInfo);

		// set spell position
		TargetInfo->SetCaster(me->GetNetId());
		TargetInfo->SetTarget(0);
		TargetInfo->SetStartPos(me->GetPosition());
		TargetInfo->SetEndPos(me->GetPosition());
		TargetInfo->SetClickedPos(me->GetPosition());
		TargetInfo->SetUnkPos(me->GetPosition());
		SpellInfo* spellInfo = spell->GetSpellInfo();

		call_function<void>(RVA(oCastSpellWrapper), InputLogic, spellInfo);


		return true;
	}

	bool CastSpell(int spellId, Vector3 pos)
	{
		if (UPasta::SDK::Orbwalker::Functions::shouldWait) return false;

		EventManager::Trigger(EventManager::EventType::OnCastSpell, spellId);

		Object* me = globals::localPlayer;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();

		if (!TargetInfo) return false;
		uintptr_t* InputLogic = *(uintptr_t**)(*(uintptr_t*)(globals::moduleBase + oHudInstance) + oHudInstanceSpellInfo);

		// set spell position
		TargetInfo->SetCaster(me->GetNetId());
		TargetInfo->SetTarget(0);
		TargetInfo->SetStartPos(pos);
		TargetInfo->SetEndPos(pos);
		TargetInfo->SetClickedPos(pos);
		TargetInfo->SetUnkPos(pos);
		SpellInfo* spellInfo = spell->GetSpellInfo();

		call_function<void>(RVA(oCastSpellWrapper), InputLogic, spellInfo);
		return true;
	}

	bool ReleaseSpell(int spellId, Vector3 pos)
	{
		typedef char(__fastcall* HudReleaseSpellFn)(QWORD* spellLogic, QWORD* idc);
		HudReleaseSpellFn _HudReleaseSpellFn = (HudReleaseSpellFn)(globals::moduleBase + oReleaseSpell);

		if (spellId < 0 || spellId >= 14) return false;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();
		if (!TargetInfo) return false;

		auto castScreenPosition = *(QWORD*)(globals::moduleBase + oMouseInstance) + oMousePosition;
		auto mouseScreenPosition = Vector2(*(int*)castScreenPosition, *(int*)(castScreenPosition + 0x4));

		if (pos.x || pos.y)
		{
			auto posw2s = WorldToScreen(pos);
			*(int*)castScreenPosition = posw2s.x;
			*(int*)(castScreenPosition + 0x4) = posw2s.y;
		}

		QWORD* InputLogic = *(QWORD**)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceSpellInfo);
		spoof_call(spoof_trampoline, _HudReleaseSpellFn, InputLogic, (QWORD*)0);
		*(int*)castScreenPosition = mouseScreenPosition.x;
		*(int*)(castScreenPosition + 0x4) = mouseScreenPosition.y;

		return true;
	}

	void OldCastSpell(int spellId, Vector3 pos)
	{
		EventManager::Trigger(EventManager::EventType::OnCastSpell, spellId, pos);

		typedef bool(__fastcall* fnCastSpellWrapper)(QWORD* hudSpellInfo, QWORD* spellInfo);
		fnCastSpellWrapper _fnCastSpellWrapper = (fnCastSpellWrapper)(globals::moduleBase + oCastSpellWrapper);

		if (spellId < 0 || spellId >= 14) return;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInfo* spellInfo = spell->GetSpellInfo();
		
		QWORD spellInput = (QWORD)spell->GetSpellInput();

		auto spellInputStartPos = ReadVector3(spellInput + oSpellInputStartPos);
		auto spellInputEndPos = ReadVector3(spellInput + oSpellInputEndPos);
		auto spellInputEndPos2 = ReadVector3(spellInput + oSpellInputEndPos + sizeof(Vector3));
		auto spellInputEndPos3 = ReadVector3(spellInput + oSpellInputEndPos + sizeof(Vector3) * 0x2);

		if (pos.x || pos.y || pos.z)
		{
			WriteVector3((spellInput + oSpellInputStartPos), globals::localPlayer->GetPosition());
			WriteVector3((spellInput + oSpellInputEndPos), pos);
			WriteVector3((spellInput + oSpellInputEndPos + sizeof(Vector3)), pos);
			WriteVector3((spellInput + oSpellInputEndPos + sizeof(Vector3) * 0x2), pos);
		}
		
		spoof_call(spoof_trampoline, _fnCastSpellWrapper, (QWORD*)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceSpellInfo)), (QWORD*)spellInfo);
		
		WriteVector3((spellInput + oSpellInputStartPos), spellInputStartPos);
		WriteVector3((spellInput + oSpellInputEndPos), spellInputEndPos);
		WriteVector3((spellInput + oSpellInputEndPos + sizeof(Vector3)), spellInputEndPos2);
		WriteVector3((spellInput + oSpellInputEndPos + sizeof(Vector3) * 0x2), spellInputEndPos3);
	}

	bool CanSendInput()
	{
		return globals::localPlayer->IsAlive() && IsGameFocused() && !IsChatOpen();
	}

	void AttackObject(Vector3 objPos)
	{
		if (!CanSendInput() || !objPos.IsValid()) return;

		auto screenPos = WorldToScreen(objPos);
		EventManager::TriggerProcess(EventManager::EventType::OnAfterAttack, objPos);

		TryRightClick(screenPos);
	}

	void AttackObject(Object* obj)
	{
		if (!CanSendInput() || obj == nullptr) return;

		Vector3 headPos = obj->GetPosition();
		const float objectHeight = *(float*)(obj->GetCharacterData() + oObjCharDataDataSize) * obj->GetScale();
		headPos.y += objectHeight - 50.0f;

		auto screenPos = WorldToScreen(headPos);
		EventManager::TriggerProcess(EventManager::EventType::OnAfterAttack, headPos);

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