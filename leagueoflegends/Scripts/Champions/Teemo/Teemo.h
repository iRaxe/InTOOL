#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace TeemoConfig
{

    namespace TeemoCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseR;

    }

    namespace TeemoHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
        inline Slider* minMana;
    }

    namespace TeemoClear {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
        inline Slider* minRMinions;
    }

    namespace TeemoJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
        inline Slider* minMana;
    }

    namespace TeemoLastHit {
        inline CheckBox* UseQ;
        inline Slider* minMana;
    }

    namespace TeemoKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
    }

    namespace TeemoFlee {
        inline CheckBox* UseW;
    }


    namespace TeemoSpellsSettings {
        inline Slider* qRange;
        inline CheckBox* qDraw;

        inline Slider* wRange;

        inline Slider* rRange;
        inline CheckBox* rDraw;
        inline CheckBox* rShowSpecialPlaces;

        inline CheckBox* DrawIfReady;
    }

}

namespace TeemoDamages
{
    namespace QSpell {
        inline float dmgSkillArray[6] = { 0, 80, 125, 170, 215, 260 };
        inline float additionalPercentageAP = 0.80f;
    }

    namespace RSpell {
        inline float dmgSkillArray[4] = { 0, 275, 400, 525 };
        inline float additionalPercentageAP = 1.0f;
    }
}