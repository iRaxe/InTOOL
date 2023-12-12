#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace AmumuConfig
{

    namespace AmumuCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline Slider* rMinEnemies;

    }

    namespace AmumuHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace AmumuJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace AmumuClear {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMinion;
        inline Slider* minMana;
    }

    namespace AmumuKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
    }

    namespace AmumuFlee {
        inline CheckBox* UseW;
    }


    namespace AmumuSpellsSettings {
        inline Slider* qRange;
        inline CheckBox* qDraw;

        inline Slider* wRange;

        inline Slider* eRange;
        inline List* eMode;

        inline Slider* rRange;
        inline CheckBox* rDraw;

        inline CheckBox* DrawIfReady;
    }

}

namespace AmumuDamages
{
    namespace QSpell {
        inline float dmgSkillArray[6] = { 0, 70, 95, 120, 145, 170 };
        inline float additionalPercentageAP = 0.85f;
    }

    namespace WSpell {
        inline float dmgSkillArray[2] = { 0, 7 };
        inline float additionalPercentageAP = 0.85f;
    }

    namespace RSpell {
        inline float dmgSkillArray[4] = { 0, 275, 400, 525 };
        inline float additionalPercentageAP = 1.0f;
    }
}