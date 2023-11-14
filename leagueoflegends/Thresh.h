#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace ThreshConfig
{
    namespace ThreshCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;

        inline CheckBox* UseR;

        inline Slider* enemiesInRange;
    }

    namespace ThreshHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline List* eMode;

        inline Slider* minMana;
    }

    namespace ThreshClear {
        inline CheckBox* UseE;
        inline Slider* minionsInRange;
        inline Slider* minMana;
    }

    namespace ThreshJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace ThreshLastHit {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline Slider* minionsInRange;
        inline Slider* minMana;
    }

    namespace ThreshSpellsSettings {

        inline CheckBox* saveMana;

        inline List* qCastMode;
        inline Slider* qRange;
        inline CheckBox* DrawQ;

        inline List* wCastMode;
        inline Slider* wRange;
        inline CheckBox* DrawW;

        inline List* eCastMode;
        inline Slider* eRange;
        inline CheckBox* DrawE;

        inline Slider* rRange;
        inline CheckBox* DrawR;

        inline CheckBox* DrawIfReady;
    }

    namespace ThreshKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
    }

    namespace ThreshAntiGapCloser {
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace ThreshAntiMelee {
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace ThreshFlee {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
    }

}

namespace ThreshDamages
{
    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 100, 150, 200, 250, 300 };
        inline float additionalPercentageAP = 0.90f;
    }

    namespace WSpell
    {
        inline float shieldSkillArray[6] = { 0, 50, 70, 90, 110, 130 };
        inline int soulCalculationMin = 2;
    }

    namespace ESpell
    {
        inline float dmgSkillArray[6] = { 0, 75, 115, 155, 195, 235 };
        inline float soulCalculationMin = 1.5;
        inline float dmgSoulSkillArray[6] = { 0, 0.80f, 1.10f, 1.40f, 1.70f, 2.00f };

        inline float additionalPercentageAP = 0.70f;
    }

    namespace RSpell
    {
        inline float dmgSkillArray[4] = { -9999, 250, 400, 550 };
        inline float additionalPercentageAP = 1.00f;
    }
}
