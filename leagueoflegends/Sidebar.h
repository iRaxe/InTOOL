#pragma once
#include "Awarenesss.h"

using namespace UPasta::SDK;
class Awarenesss::Sidebar
{
	static inline float childBaseSize = 200.0f;
	static inline float iconBaseSize = 40.0f;
	static inline float iconSize = 40.0f;
	static inline float sizeMultiplier = 40.0f;

	static void DrawBackground(const ImVec2& screenPos);
	static void DrawRespawnTimer(Object* obj, const ImVec2& screenPos);
	static void DrawHealthBar(Object* obj, const ImVec2& screenPos, float widthLimit);
	static void DrawManaBar(Object* obj, const ImVec2& screenPos, float widthLimit);
	static void DrawExperienceBar(Object* obj, const ImVec2& screenPos, float widthLimit);
	static void DrawDSpellIcon(Object* obj, const ImVec2& screenPos);
	static void DrawFSpellIcon(Object* obj, const ImVec2& screenPos);
	static void DrawRSpellIcon(Object* obj, const ImVec2& screenPos);
	static void DrawSpellTimer(Object* obj, int index, const ImVec2& topLeft, const ImVec2& centerPos,
	                           const ImVec2& bottomRight);
	static ImVec2 CalculateChildSize(int listSize);
	static ImVec2 CalculatePosition(int index, const ImVec2& basePos);
	static void DrawHeroSidebar(Object* obj, const ImVec2& position);

public:
	static void InitializeMenu();

	static void Draw(bool* p_open);
};

