#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace NautilusConfig
{
    namespace NautilusCombo {
        CheckBox* UseQ;
        CheckBox* UseW;
        CheckBox* UseE;
        CheckBox* UseR;

        Slider* enemiesInRange;

        KeyBind* engageModeKey;
    }

    namespace NautilusHarass {
        CheckBox* UseQ;
        CheckBox* UseQDive;
        CheckBox* UseE;

        Slider* minMana;
    }

    namespace NautilusJungle {
        CheckBox* UseQ;
        CheckBox* UseW;
        Slider* minMana;
    }

    namespace NautilusKillsteal {
        CheckBox* UseQ;
        CheckBox* UseE;
        CheckBox* UseR;
    }

    namespace NautilusAntiGapCloser {
        CheckBox* UseE;
    }

    namespace NautilusInterrupter {
        CheckBox* UseQ;
        CheckBox* UseR;
    }

    namespace NautilusFlee {
        CheckBox* UseQ;
    }

    namespace NautilusDrawings {
        CheckBox* DrawQ;
        CheckBox* DrawE;
        CheckBox* DrawR;
        CheckBox* DrawIfReady;
    }
}

namespace NautilusDamages
{
    namespace QSpell
    {
        inline float dmgSkillQ[6] = { 0, 70, 115, 160, 205, 250 };
        inline float additionalPercentageAP = 0.90f;

        inline float range = 1122.0f;
        inline float speed = 2000.0f;
        inline float delay = 0.25f;
        inline float width = 180.0f;
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

        inline float range = 465.0f;
        inline float speed = 2000.0f;
        inline float delay = 0.25f;
    } 

    namespace RSpell
    {
        inline float dmgSkillR[4] = { -9999, 150, 275, 400 };
        inline float additionalPercentageAP = 0.80f;
        inline float range = 825.0f;
        inline float speed = 2500.0f;
        inline float delay = 0.46f;
    }
}
