#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace VayneConfig
{
    namespace VayneCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline CheckBox* UseR;

        inline Slider* enemiesInRange;
    }

    namespace VayneHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace VayneClear {
        inline CheckBox* UseQ;
        inline Slider* minMana;
    }

    namespace VayneJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace VayneSpellsSettings {

        inline CheckBox* saveMana;

        inline List* qCastMode;
        inline Slider* wStacksRequired;
        inline Slider* qRange;
        inline CheckBox* DrawQ;


        inline List* eCastMode;
        inline CheckBox* UseEOnlyStun;
        inline Slider* eRange;
        inline Slider* wallCheckRange;
        inline CheckBox* DrawE;

        inline Slider* maxRDistance;
        inline Slider* minRDistance;

        inline CheckBox* DrawIfReady;

    }

    namespace VayneKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
    }

    namespace VayneAntiGapCloser {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline std::vector<Object*> whitelist;
    }

    namespace VayneAntiMelee {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace VayneFlee {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
    }

}

namespace VayneDamages
{
    namespace QSpell {
        inline float additionalPercentageAD[6] = { 0, 0.75f, 0.85f, 0.95f, 1.05f, 1.15f };
        inline float additionalPercentageAP = 0.50f;
    }

    namespace WSpell {
        inline float dmgSkillArray[6] = { 0, 50, 65, 80, 95, 110 };
        inline float additionalPercentageMaxHP[6] = { 0, 0.06f, 0.07f, 0.08f, 0.09f, 0.10f };
    }

    namespace ESpell {
        inline float dmgSkillArray[6] = { 0, 50, 85, 120, 155, 190 };
        inline float additionalPercentageAD = 0.50f;
        inline float bonusdmgSkillArray[6] = { 0, 75, 127.50, 180, 232.50, 285 };
        inline float bonusadditionalPercentageAd = 0.75f;
    }
}