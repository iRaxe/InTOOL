#pragma once
#include "Cooldowns.h"
#include "Awarenesss.h"

class Awarenesss::HPBar::Experience
{
    inline static float levelExp[18] = { 0.f, 280.f, 660.f, 1140.f, 1720.f, 2400.f, 3180.f, 4060.f,
             5040.f, 6120.f, 7300.f, 8580.f, 9960.f, 11440.f, 13020.f,
             14700.f, 16480.f, 18360.f };

    inline static float expNeeded[18] = { 0.f, 280.f, 380.f, 480.f, 580.f, 680.f, 780.f, 880.f,
                                                   980.f, 1080.f, 1180.f, 1280.f, 1380.f, 1480.f, 1580.f,
                                                   1680.f, 1780.f, 1880.f };


public:
	static float CalculateProgress(float expAttuale, int livelloCorrente);

	static void Draw(Object* obj, const Vector2& screenPos);
    static void InitializeMenu();

};

