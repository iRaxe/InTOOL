#pragma once
#include "../stdafx.h"
class TargetSelector
{
	static inline Object* _last_target;
	static inline Object* _last_minion;
	static inline Object* _last_jungle;
	static inline Object* _last_turret;
	static inline Object* _override_target;

public:
	static bool IsValid(Object* target, Vector3 from, float range);

	static void Initialize();
	static void OnWndProc(UINT msg, WPARAM param);
	static Object* FindBestTarget(Vector3 from, float range);
	static Object* FindFarestBestTarget(Vector3 from, float range);
	static Object* FindBestMinion(Vector3 from, float range, Alliance team);
	static Object* FindBestJungle(Vector3 from, float range);
	static Object* FindTurret(Vector3 from, float range, Alliance team);
	static Object* FindInhibitor(Vector3 from, float range, Alliance team);
	static Object* FindBestLaneClear(Vector3 from, float range);
	static Object* GetOverrideTarget() { return _override_target; }
};

