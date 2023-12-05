#pragma once
#include "Awarenesss.h"

using namespace UPasta::SDK;
class Awarenesss::LastPosition
{
	static float TimeHidden(Object* hero, std::unordered_map<uint64_t, bool>& previous_visibility,
		std::unordered_map<uint64_t, Vector3>& previous_position,
		std::unordered_map<uint64_t, float>& hidden_time);

	static void ShowLastMinimapPosition(Object* obj, const ImVec2& fromPos);
	static void ShowLastWorldPosition(Object* obj, const ImVec2& fromPos);
public:

	static void Draw(Object* obj);
	static void InitializeMenu();
};

