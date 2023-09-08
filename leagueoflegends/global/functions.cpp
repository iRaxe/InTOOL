#include "../stdafx.h"

namespace functions
{
	float lastRefreshTime = 0.0f;
	
	void* spoof_trampoline = 0x0;

	void Init()
	{
		spoof_trampoline = (void*)mem::ScanModInternal((char*)"\xFF\x23", (char*)"xx", (char*)globals::moduleBase);
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

	void PrintChat(std::string text)
	{
		typedef void(__thiscall* fnPrintChat)(QWORD* chatClient, const char* message, int colorId);
		fnPrintChat _fnSendChat = (fnPrintChat)(globals::moduleBase + oPrintChat);
		std::string timeMarkString = "[" + ConvertTime(GetGameTime()) + "] ";
		std::string coloredTimeMarkString = CHAT_COLOR_DT("#7ce9ff", timeMarkString);
		std::string formattedText = coloredTimeMarkString + text;
		_fnSendChat((QWORD*)(*(QWORD*)(globals::moduleBase + oChatInstance)), formattedText.c_str(), 4);
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

	float GetGameTime()
	{
		return *(float*)(globals::moduleBase + oGameTime);
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
		return *(bool*)(*(QWORD*)(globals::moduleBase + oChatClient) + oChatClientChatOpen);
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
		return ReadVector3((QWORD)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceInput) + oHudInstanceInputMouseWorldPos));
	}

	Vector2 WorldToScreen(Vector3 in)
	{
		typedef bool(__fastcall* fnWorldToScreen)(QWORD* viewport, Vector3* in, Vector3* out);
		fnWorldToScreen _fnWorldToScreen = (fnWorldToScreen)(globals::moduleBase + oWorldToScreen);
		QWORD* viewport = *(QWORD**)(globals::moduleBase + oViewport);
		Vector3 out;
		_fnWorldToScreen((QWORD*)((QWORD)viewport + oViewportW2S), &in, &out);
		return Vector2(out.x, out.y);
	}

	Vector2 GetMinimapPos()
	{
		return ReadVector2((QWORD)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oMinimapObject) + MinimapObjectHud) + MinimapHudPos));
	}

	float GetMinimapSize()
	{
		return *(float*)((QWORD)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oMinimapObject) + MinimapObjectHud) + MinimapHudSize));
	}

	Vector2 WorldToMinimap(Object* objectToShow)
	{
		Vector3 objDrawPos = GetBaseDrawPosition(objectToShow);
		Vector2 objMinimapDrawPos(objDrawPos.x, objDrawPos.z);
		float scale_factor = GetMinimapSize() / 14800.0f;

		const float minimap_x = (objMinimapDrawPos.x * scale_factor) + (GetMinimapPos().x - 12.0f);
		const float minimap_y = (GetMinimapPos().y - 13.0f) + GetMinimapSize() - (objMinimapDrawPos.y * scale_factor);

		return { minimap_x, minimap_y };
	}


	Vector3 GetBaseDrawPosition(Object* obj)
	{
		typedef bool(__fastcall* fnGetBaseDrawPosition)(QWORD* obj, Vector3* out);
		fnGetBaseDrawPosition _fnGetBaseDrawPosition = (fnGetBaseDrawPosition)(globals::moduleBase + oGetBaseDrawPosition);
		Vector3 out;
		_fnGetBaseDrawPosition((QWORD*)obj, &out);
		return out;
	}
	
	Vector2 GetHpBarPosition(Object* obj)
	{
		Vector3 hpBarPos = obj->GetPosition();
		const float hpBarHeight = *(float*)(obj->GetCharacterData() + oObjCharDataDataSize) * obj->GetScale();
		hpBarPos.y += hpBarHeight;

		auto screenPos = WorldToScreen(hpBarPos);
		const auto zoomInstance = *(QWORD*)(globals::moduleBase + oZoomInstance);
		const float maxZoom = *(float*)(zoomInstance + oZoomInstanceMaxZoom);
		float currentZoom = *(float*)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceCamera) + oHudInstanceCameraZoom);
		float zoomDelta = maxZoom / currentZoom;

		screenPos.y -= (((globals::windowHeight) * 0.00083333335f * zoomDelta) * hpBarHeight);
		
		return screenPos;
	}

	Object* GetSelectedObject()
	{
		typedef Object* (__fastcall* fnGetObjectFromNetId)(QWORD* a1, unsigned int netId);
		fnGetObjectFromNetId _fnGetObjectFromNetId = (fnGetObjectFromNetId)(globals::moduleBase + oGetObjectFromNetId);

		QWORD* hudInstance = (QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance));
		unsigned int targetNetId = *(unsigned int*)(*(QWORD*)((QWORD)hudInstance + oHudInstanceUserData) + oHudInstanceUserDataSelectedObjectNetId);
		if (!targetNetId) return 0;

		return _fnGetObjectFromNetId((QWORD*)(*(QWORD*)(globals::moduleBase + oGetObjectFromNetIdParam)), targetNetId);
	}

	unsigned int GetCollisionFlags(Vector3 pos)
	{
		typedef unsigned int(__fastcall* fnGetCollisionFlags)(Vector3 pos);
		fnGetCollisionFlags _fnGetCollisionFlags = (fnGetCollisionFlags)(globals::moduleBase + oGetCollisionFlags);

		return _fnGetCollisionFlags(pos);
	}

	float GetRespawnTimer(Object* obj)
	{
		typedef float(__fastcall* fnGetRespawnTimer)(Object* obj);
		fnGetRespawnTimer _fnGetRespawnTimer = (fnGetRespawnTimer)(globals::moduleBase + fGetRespawnTimeRemaining);
		return _fnGetRespawnTimer(obj);
	}

	float GetSpellRange(int level)
	{
		typedef float(__fastcall* fnGetSpellRange)(int level);
		//Not working, need find spellDataSpellRange 
		fnGetSpellRange _fnGetSpellRange = (fnGetSpellRange)(globals::moduleBase + fGetSpellRange);
		return _fnGetSpellRange(level);
	}

	QWORD GetZoomAddress()
	{
		return (*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceCamera) + 0x2B0);
	}

	bool IsAlive(Object* obj)
	{
		typedef bool(__fastcall* fnIsAlive)(Object* obj);
		fnIsAlive _fnIsAlive = (fnIsAlive)(globals::moduleBase + fIsAlive);

		return _fnIsAlive(obj);
	}

	bool IsHero(Object* obj)
	{
		typedef bool(__fastcall* fnIsHero)(Object* obj);
		fnIsHero _fnIsHero = (fnIsHero)(globals::moduleBase + fIsHero);

		return _fnIsHero(obj);
	}

	bool IsMinion(Object* obj)
	{
		typedef bool(__fastcall* fnIsMinion)(Object* obj);
		fnIsMinion _fnIsMinion = (fnIsMinion)(globals::moduleBase + fIsMinion);

		return _fnIsMinion(obj);
	}

	bool IsTurret(Object* obj)
	{
		typedef bool(__fastcall* fnIsTurret)(Object* obj);
		fnIsTurret _fnIsTurret = (fnIsTurret)(globals::moduleBase + fIsTurret);

		return _fnIsTurret(obj);
	}

	bool IsMissile(Object* obj)
	{
		typedef bool(__fastcall* fnIsMissile)(Object* obj);
		fnIsMissile _fnIsMissile = (fnIsMissile)(globals::moduleBase + fIsMissile);

		return _fnIsMissile(obj);
	}

	bool IsInhibitor(Object* obj)
	{
		typedef bool(__fastcall* fnIsInhibitor)(Object* obj);
		fnIsInhibitor _fnIsInhibitor = (fnIsInhibitor)(globals::moduleBase + fIsInhibitor);

		return _fnIsInhibitor(obj);
	}

	bool IsNexus(Object* obj)
	{
		typedef bool(__fastcall* fnIsNexus)(Object* obj);
		fnIsNexus _fnIsNexus = (fnIsNexus)(globals::moduleBase + fIsNexus);

		return _fnIsNexus(obj);
	}

	bool IsDead(Object* obj)
	{
		typedef bool(__fastcall* fnIsDead)(Object* obj);
		fnIsDead _fnIsDead = (fnIsDead)(globals::moduleBase + fIsDead);

		return _fnIsDead(obj);
	}

	bool IsCanSee(Object* obj)
	{
		typedef bool(__fastcall* fnIsCanSee)(Object* obj);
		fnIsCanSee _fnIsCanSee = (fnIsCanSee)(globals::moduleBase + fIsCanSee);

		return _fnIsCanSee(obj);
	}

	bool IsTargetable(Object* obj)
	{
		typedef bool(__fastcall* fnIsTargetable)(Object* obj);
		fnIsTargetable _fnIsTargetable = (fnIsTargetable)(globals::moduleBase + fIsTargetable);

		return _fnIsTargetable(obj);
	}

	bool IsVisible(Object* obj)
	{
		typedef bool(__fastcall* fnIsVisible)(Object* obj);
		fnIsVisible _fnIsVisible = (fnIsVisible)(globals::moduleBase + fIsVisible);

		return _fnIsVisible(obj);
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
		DWORD check = *(DWORD*)((QWORD)globals::localPlayer + 0xD4);

		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1) = 0.0f;
		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2) = 0.0f;
		*(DWORD*)((QWORD)globals::localPlayer + 0xD4) = 0x0;

		typedef bool(__fastcall* fnTryRightClick)(QWORD* player, unsigned int* params);
		fnTryRightClick _fnTryRightClick = (fnTryRightClick)(globals::moduleBase + oTryRightClick);

		unsigned int* params = new unsigned int[20];
		params[17] = (int)pos.x;
		params[18] = (int)pos.y;
		params[19] = 2;

		spoof_call(spoof_trampoline, _fnTryRightClick, (QWORD*)globals::localPlayer, params);

		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1) = floatCheck1;
		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2) = floatCheck2;
		*(DWORD*)((QWORD)globals::localPlayer + 0xD4) = check;
	}

	void IssueClick(Vector2 pos)
	{
		float floatCheck1 = *(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1);
		float floatCheck2 = *(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2);
		DWORD check = *(DWORD*)((QWORD)globals::localPlayer + oObjIssueClickCheck);

		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1) = 0.0f;
		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2) = 0.0f;
		*(DWORD*)((QWORD)globals::localPlayer + oObjIssueClickCheck) = 0x0;

		typedef bool(__fastcall* fnIssueClick)(QWORD* player, unsigned int* params);
		fnIssueClick _fnIssueClick = (fnIssueClick)(globals::moduleBase + oIssueClick);
		unsigned int* params = new unsigned int[20];
		params[17] = (int)pos.x;
		params[18] = (int)pos.y;
		params[19] = 2;

		spoof_call(spoof_trampoline, _fnIssueClick, (QWORD*)globals::localPlayer, params);

		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck1) = floatCheck1;
		*(float*)((QWORD)globals::localPlayer + oObjIssueClickFloatCheck2) = floatCheck2;
		*(DWORD*)((QWORD)globals::localPlayer + oObjIssueClickCheck) = check;
	}

	void IssueMove(Vector2 pos)
	{
		EventManager::TriggerProcess(EventManager::EventType::OnIssueMove, pos);

		typedef bool(__fastcall* fnIssueMove)(QWORD* hudInput, int screenX, int screenY, bool isAttackMove, int zeroOrOne, int order);
		fnIssueMove _fnIssueMove = (fnIssueMove)(globals::moduleBase + oIssueMove);

		spoof_call(spoof_trampoline, _fnIssueMove, (QWORD*)(*(QWORD*)(*(QWORD*)(globals::moduleBase + oHudInstance) + oHudInstanceInput)), (int)pos.x, (int)pos.y, false, 0, 0);
	}

	bool CastSpell(int spellId, Object* Target)
	{
		Object* me = globals::localPlayer;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();

		if (!TargetInfo || !Target) return false;
		uintptr_t* InputLogic = *(uintptr_t**)(*(uintptr_t*)(globals::moduleBase + oHudInstance) + 0x68);

		// set spell position
		TargetInfo->SetCaster(me->GetNetId());
		TargetInfo->SetTarget(Target->GetNetId());
		TargetInfo->SetStartPos(me->GetPosition());
		TargetInfo->SetEndPos(Target->GetPosition());
		TargetInfo->SetClickedPos(Target->GetPosition());
		TargetInfo->SetUnkPos(Target->GetPosition());

		typedef void(__fastcall* HudCastSpellFn)(uintptr_t* HudInput, SpellInfo* SpellData);
		HudCastSpellFn _HudCastSpellFn = (HudCastSpellFn)(globals::moduleBase + oCastSpellWrapper);
		SpellInfo* spellInfo = spell->GetSpellInfo();

		_HudCastSpellFn(InputLogic, spellInfo);

		return true;
	}

	bool CastSpell(int spellId)
	{
		Object* me = globals::localPlayer;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();

		if (!TargetInfo) return false;
		uintptr_t* InputLogic = *(uintptr_t**)(*(uintptr_t*)(globals::moduleBase + oHudInstance) + 0x68);

		// set spell position
		TargetInfo->SetCaster(me->GetNetId());
		TargetInfo->SetTarget(0);
		TargetInfo->SetStartPos(me->GetPosition());
		TargetInfo->SetEndPos(me->GetPosition());
		TargetInfo->SetClickedPos(me->GetPosition());
		TargetInfo->SetUnkPos(me->GetPosition());

		typedef void(__fastcall* HudCastSpellFn)(uintptr_t* HudInput, SpellInfo* SpellData);
		HudCastSpellFn _HudCastSpellFn = (HudCastSpellFn)(globals::moduleBase + oCastSpellWrapper);
		SpellInfo* spellInfo = spell->GetSpellInfo();

		_HudCastSpellFn(InputLogic, spellInfo);

		return true;
	}

	bool CastSpell(int spellId, Vector3 pos)
	{
		Object* me = globals::localPlayer;
		Spell* spell = globals::localPlayer->GetSpellBySlotId(spellId);
		SpellInput* TargetInfo = spell->GetSpellInput();

		if (!TargetInfo) return false;
		uintptr_t* InputLogic = *(uintptr_t**)(*(uintptr_t*)(globals::moduleBase + oHudInstance) + 0x68);

		// set spell position
		TargetInfo->SetCaster(me->GetNetId());
		TargetInfo->SetTarget(0);
		TargetInfo->SetStartPos(pos);
		TargetInfo->SetEndPos(pos);
		TargetInfo->SetClickedPos(pos);
		TargetInfo->SetUnkPos(pos);

		typedef void(__fastcall* HudCastSpellFn)(uintptr_t* HudInput, SpellInfo* SpellData);
		HudCastSpellFn _HudCastSpellFn = (HudCastSpellFn)(globals::moduleBase + oCastSpellWrapper);
		SpellInfo* spellInfo = spell->GetSpellInfo();

		_HudCastSpellFn(InputLogic, spellInfo);

		return true;
	}

	void OldCastSpell(int spellId, Vector3 pos)
	{
		EventManager::TriggerProcess(EventManager::EventType::OnCastSpell, spellId, pos);

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
		if (!CanSendInput()) return;

		auto screenPos = WorldToScreen(objPos);
		EventManager::TriggerProcess(EventManager::EventType::OnIssueOrder, objPos);

		TryRightClick(screenPos);
	}

	void AttackObject(Object* obj)
	{
		if (!CanSendInput()) return;

		Vector3 headPos = obj->GetPosition();
		const float objectHeight = *(float*)(obj->GetCharacterData() + oObjCharDataDataSize) * obj->GetScale();
		headPos.y += objectHeight - 50.0f;

		auto screenPos = WorldToScreen(headPos);
		EventManager::TriggerProcess(EventManager::EventType::OnIssueOrder, headPos);

		TryRightClick(screenPos);
	}

	void MoveToMousePos()
	{
		if (!CanSendInput()) return;
		IssueMove(GetMousePos());
	}


}