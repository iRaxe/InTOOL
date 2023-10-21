#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace YorickConfig
{
    namespace YorickCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
    }

    namespace YorickHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minGraves;

        inline Slider* minMana;
    }

    namespace YorickClear {
        inline CheckBox* UseQOnTowers;
        inline CheckBox* UseE;
        inline Slider* minMinions;

        inline Slider* minMana;
    }

    namespace YorickJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;

        inline Slider* minMana;
    }

    namespace YorickLastHit {
        inline CheckBox* UseQ;
        inline Slider* minMana;
    }

    namespace YorickKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
    }

    namespace YorickAntiGapCloser {
        inline CheckBox* UseW;
        inline std::vector<Object*> whitelist;
    }

    namespace YorickAntiMelee {
        inline CheckBox* UseW;
        inline std::vector<Object*> whitelist;
    }

    namespace YorickFlee {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
    }

    namespace YorickDrawings {
        inline CheckBox* DrawW;
        inline CheckBox* DrawE;
        inline CheckBox* DrawR;
        inline CheckBox* DrawIfReady;
    }
}

namespace YorickDamages
{
    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 30, 55, 80, 105, 130 };
        inline float additionalPercentageAD = 0.40f;
    }

    namespace ESpell
    {
        inline float dmgSkillArray[6] = { 0, 70, 105, 140, 175, 210 };
        inline float additionalPercentageAP = 0.70f;
    }

}
