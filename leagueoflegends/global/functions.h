#pragma once

namespace functions
{
	extern void* spoof_trampoline;

	template<typename Function> Function CallVirtual(PVOID base, QWORD index)
	{
		QWORD** VTablePointer = (QWORD**)base;
		QWORD* VTableFunctionBase = *VTablePointer;
		QWORD dwAddress = VTableFunctionBase[index];
		return (Function)(dwAddress);
	}

	void Init();

	std::string GetHexString(QWORD hexNumber);
	std::string CapitalizeFirstLetter(std::string str);

	Vector3 ReadVector3(QWORD offset);
	void WriteVector3(QWORD offset, Vector3 vector);

	Vector2 ReadVector2(QWORD offset);


	void PrintChat(std::string text);
	void PrintChat(int number);
	void PrintChat(float number);
	void PrintChat(void* address);

	std::string GetServerIP();
	float GetGameTime();
	int GetGameTick();
	std::string ConvertTime(float seconds);

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
	Vector3 GetBaseDrawPosition(Object* obj);
	Vector2 GetHpBarPosition(Object* obj);

	Object* GetSelectedObject();
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
	bool IsTargetable(Object* obj);
	bool IsVisible(Object* obj);
	bool IsVisible(Missile* obj);

	unsigned int GetCollisionFlags(Vector3 pos);
	float GetRespawnTimer(Object* obj);
	int GetSpellState(int slotId);
	float GetSpellRange(int slotid);
	QWORD GetZoomAddress();
	bool IsBrush(Vector3 pos);
	bool IsWall(Vector3 pos);
	bool IsAttackWindupSpell(int spellId);

	void TryRightClick(Vector2 pos);
	void IssueClick(Vector2 pos);
	void IssueMove(Vector2 pos);
	bool CastSpell(int spellId, Object* Target);
	bool CastSpell(int spellId);
	bool CastSpell(int spellId, Vector3 pos);
	void OldCastSpell(int spellId, Vector3 pos);

	bool CanSendInput();

	void AttackObject(Object* obj);
	void AttackObject(Vector3 objPos);

	void MoveToMousePos();
}