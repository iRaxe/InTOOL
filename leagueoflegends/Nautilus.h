#include "NewMenu.h"

using namespace UPasta::SDK;

namespace NautilusConfig
{
    namespace NautilusCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;

        inline Slider* enemiesInRange;

        inline KeyBind* engageModeKey;
    }

    namespace NautilusHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseQDive;
        inline CheckBox* UseE;

        inline Slider* minMana;
    }

    namespace NautilusJungle {
        inline  CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace NautilusKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
    }

    namespace NautilusAntiGapCloser {
        inline CheckBox* UseE;
    }

    namespace NautilusInterrupter {
        inline CheckBox* UseQ;
        inline CheckBox* UseR;
    }

    namespace NautilusFlee {
        inline  CheckBox* UseQ;
    }

    inline std::vector<Object*> q_whitelist;
    inline std::vector<Object*> r_whitelist;

    namespace NautilusDrawings {
        inline CheckBox* DrawQ;
        inline CheckBox* DrawE;
        inline CheckBox* DrawR;
        inline CheckBox* DrawIfReady;
    }
}

namespace NautilusDamages
{
    namespace QSpell
    {
        inline float dmgSkillQ[6] = { 0, 70, 115, 160, 205, 250 };
        inline float additionalPercentageAP = 0.90f;
    } 

    namespace WSpell
    {
        inline float dmgSkillW[6] = { 0, 30, 40, 50, 60, 70 };
        inline float additionalPercentageAP = 0.40f;
    } 

    namespace ESpell
    {
        inline float dmgSkillE[6] = { 0, 110, 180, 250, 320, 390 };
        inline float additionalPercentageAP = 1.00f;
    } 

    namespace RSpell
    {
        inline float dmgSkillR[4] = { -9999, 150, 275, 400 };
        inline float additionalPercentageAP = 0.80f;
    }
}
