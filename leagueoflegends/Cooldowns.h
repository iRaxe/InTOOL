#pragma once
#include "Awarenesss.h"
#include "stdafx.h"
#include "NewMenu.h"
using namespace UPasta::SDK;

class Awarenesss::HPBar
{
	static constexpr float CdWidth = 26.5f;

	static constexpr float YOffset = 3.0f;
	static constexpr float XOffset = -64.0f + 18.0f;

	static constexpr float ExpHeight = 30.0f;
	static constexpr float ExpYOffset = 1.0f;
	static constexpr float ExpXOffset = -64.0f + 18.0f;

	static constexpr float OffsetMultiplierD = -2.00f;
	static constexpr float OffsetMultiplierF = 4.0f;
	static constexpr float DFOverlayYOffset = -24.0f;
	static constexpr float DFOverlayXIncrement = CdWidth + 1.0f;

	static ImVec2 CalculateTopLeft(const Vector2& basePos, int index, float width);
	static ImVec2 CalculateBottomRight(const Vector2& basePos, int index, float width);
	static ImVec2 CalculateCenter(const Vector2& basePos, int index, float width);

	static void DrawDFIcons(Object* obj, const Vector2& basePos, int slotId);
	static void DrawCooldowns(Object* obj, const Vector2& basePos);

public:
	class Experience;
	static void DrawSpellTimer(Object* obj, int index, const ImVec2& topLeft, const ImVec2& centerPos,
		const ImVec2& bottomRight);
	static void DrawSpellIcon(Object* obj, int index, const ImVec2& topLeft, const ImVec2& bottomRight);
	static void Draw(Object* obj);
	static void InitializeMenu();
};

