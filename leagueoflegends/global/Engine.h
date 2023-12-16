#pragma once

namespace Engine
{
	extern void* spoof_trampoline;

	template <typename ReturnType, typename... Args>
	ReturnType call_function(uintptr_t func, auto... args) {
		using func_t = ReturnType(__fastcall*)(decltype(args)...);
		return spoof_call(spoof_trampoline, (func_t)func, std::forward<decltype(args)>(args)...);
	}

	template <size_t Index, typename ReturnType>
	ReturnType call_virtual(void* instance, auto... args) {
		using fn = ReturnType(__fastcall*)(void*, decltype(args)...);
		auto function = (*std::bit_cast<fn**>(instance))[Index];
		return function(instance, std::forward<decltype(args)>(args)...);
	}

	template <typename T, typename U>
	T Read(U addr);

	template<typename T>
	void PrintChat(T value);
	void PrintChat(void* address);

	template <typename T>
	T getCooldownData(const std::string& playerNameToFind, const std::string& spellNameToFind);

	template<typename Function> Function CallVirtual(PVOID base, QWORD index)
	{
		QWORD** VTablePointer = (QWORD**)base;
		QWORD* VTableFunctionBase = *VTablePointer;
		QWORD dwAddress = VTableFunctionBase[index];
		return (Function)(dwAddress);
	}

	void Init();
	bool isShopOpen();
	std::string GetHexString(QWORD hexNumber);
	std::string CapitalizeFirstLetter(std::string str);

	Vector3 ReadVector3(QWORD offset);
	void WriteVector3(QWORD offset, Vector3 vector);
	void WriteVector2(QWORD offset, Vector2 vector);


	Vector2 ReadVector2(QWORD offset);
	GameState GetGameState();
	std::string GetServerIP();
	float GetGameTime();
	int GetGameTick();	std::string ConvertTime(float seconds);

	bool IsGameFocused();
	bool IsChatOpen();

	Vector2 GetMousePos();
	Vector3 GetMouseWorldPos();

	Vector2 WorldToScreen(Vector3 in);
	float GetHeightAtPosition(Vector3 in);
	void ReadOnce();
	float GetHeight3D_Recreated(Vector2 pos);
	Vector2 GetMinimapPos();
	float GetMinimapSize();
	Vector2 WorldToMinimap(Object* objectToShow);
	Vector2 WorldToMinimap(Vector3 posToShow);
	Vector3 GetBaseDrawPosition(Object* obj);
	Vector2 GetHpBarPosition(Object* obj);
	Object* GetOwner(Object* obj);
	Object* GetObjectFromNetId(int netId);
	Object* GetSelectedObject();
	Object* GetPlayerPointer(const std::string& playerNameToFind);

	bool IsNotLocalPlayer(Object* obj);
	int IsAttackingLocalPlayer(Object* obj);
	bool IsAlive(Object* obj);
	bool IsHero(Object* obj);
	bool IsMinion(Object* obj);
	bool IsTurret(Object* obj);
	bool IsMissile(Missile* obj);
	bool IsInhibitor(Object* obj);
	bool IsNexus(Object* obj);
	bool IsDead(Object* obj);
	bool IsCanSee(Object* obj);
	bool SellItem(InventorySlot* slotID);
	bool IsTargetable(Object* obj);
	bool IsVisible(Object* obj);
	bool IsVisible(Missile* obj);

	void SendPing(int pingType, Vector3 pos);
	unsigned int GetCollisionFlags(Vector3 pos);
	float GetRespawnTimer(Object* obj);
	int GetSpellState(int slotId);
	float GetSpellRange(Spell* spellID);
	QWORD GetZoomAddress();
	bool IsBrush(Vector3 pos);
	bool IsWall(Vector3 pos);
	bool IsAttackWindupSpell(int spellId);
	float GetStat(Object* obj, StatEnum Stat, StatType Type);
	int GetTeam(Object* obj);
	bool IsTargetableToTeam(Object* obj);
	bool IsCastingSpell(Object* obj);
	bool IsValid(Object* obj);
	bool CanUpgradeSpell(int spellId);
	void UpgradeSpell(int spellId);
	void GlowObject(Object* obj);

	void TryRightClick(Vector2 pos);
	void IssueClick(Vector2 pos);
	void IssueMove(Vector2 pos);

	uintptr_t GetMouseInstance();
	Vector2 GetMouseInstancePosition();
	void UpdateMouseInstancePosition(Vector2 newPosition);

	bool CastSelf(SpellIndex slotID);
	bool CastTargeted(SpellIndex slotID, Object* target);
	bool CastToPosition(SpellIndex slotID, Vector3 worldPos);
	
	bool ReleaseSpell(int spellId, Vector3 pos);

	bool CanSendInput();

	void AttackObject(Object* obj);
	void AttackObject(Vector3 objPos);

	void MoveToMousePos();

	bool MenuItemContains(const std::vector<Object*>& words, const std::string& targetWord);
}
