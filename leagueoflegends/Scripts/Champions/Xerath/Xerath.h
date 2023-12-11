#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace XerathConfig
{
    void TEST2();

    namespace XerathCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
    }

    namespace XerathHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace XerathJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace XerathLaneClear {
        inline CheckBox* UseQ;
        inline Slider* minQMinions;
        inline CheckBox* UseW;
        inline Slider* minWMinions;
        inline Slider* minMana;
    }

    namespace XerathLastHit {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace XerathUltimate {
        inline List* targetMode;
        inline KeyBind* manualCastUltimateKey;
        inline Slider* castDelay;
    }

    namespace XerathKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline Slider* enemiesKillable;
    }

    namespace XerathAntiGapCloser {
        inline CheckBox* UseE;
    }

    namespace XerathInterrupter {
        inline CheckBox* UseE;
    }

    namespace XerathFlee {
        inline CheckBox* UseE;
    }

    namespace XerathDrawings {
        inline CheckBox* DrawQ;
        inline CheckBox* DrawW;
        inline CheckBox* DrawE;
        inline CheckBox* DrawR;
        inline CheckBox* DrawIfReady;
    }
}

namespace XerathDamages
{
    namespace QSpell
    {
        inline float dmgSkillQ[6] = { 0, 70, 110, 150, 190, 230 };
        inline float additionalPercentageAP = 0.85f;
    }

    namespace WSpell
    {
        inline float dmgSkillW[6] = { 0, 60, 95, 130, 165, 200 };
        inline float additionalPercentageAP = 0.60f;
    }

    namespace ESpell
    {
        inline float dmgSkillE[6] = { 0, 80, 110, 140, 170, 200 };
        inline float additionalPercentageAP = 0.45f;
    }

    namespace RSpell
    {
        inline float dmgSkillR[4] = { -9999, 50, 125, 200 };
        inline float additionalPercentageAP = 0.45f;
    }
}
