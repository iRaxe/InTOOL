#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace BlitzcrankConfig
{

    namespace BlitzcrankCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline Slider* rMinEnemies;

    }

    namespace BlitzcrankHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace BlitzcrankJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace BlitzcrankKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
    }

    namespace BlitzcrankFlee {
        inline CheckBox* UseW;
    }


    namespace BlitzcrankSpellsSettings {
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

namespace BlitzcrankDamages
{
    namespace QSpell {
        inline float dmgSkillArray[6] = { 0, 105, 150, 195, 240, 285 };
        inline float additionalPercentageAP = 1.20f;
    }

    namespace RSpell {
        inline float dmgSkillArray[4] = { 0, 275, 400, 525 };
        inline float additionalPercentageAP = 1.0f;
    }
}