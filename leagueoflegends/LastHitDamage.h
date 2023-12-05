#pragma once
#include "Awarenesss.h"

using namespace UPasta::SDK;
class Awarenesss::LastHitDamage
{
	static constexpr float barWidth = 100.0f;
	static constexpr float yOffset = 5.5f;
	static constexpr float xOffset = -barWidth / 3.3f;

	static ImVec2 CalculateTopLeft(const Vector2& basePos);
	static ImVec2 CalculateBottomRight(const Vector2& basePos, float widthLimit);
public:

	static void Draw();
	static void InitializeMenu();
};

