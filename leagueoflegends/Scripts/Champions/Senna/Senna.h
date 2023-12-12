#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace SennaConfig
{

    namespace SennaCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline CheckBox* UseQAlly;
        inline Slider* enemiesInRange;

        //inline Slider* MinHealth;
    }

    namespace SennaHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace SennaClear {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace SennaLastHit {
        inline CheckBox* UseQ;
        inline Slider* minMana;
    }

    namespace SennaJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace SennaAuto {
        inline CheckBox* AutoHeal;
        inline CheckBox* AutoSoul;
        inline CheckBox* NoSoulInCombo;
        inline Slider* MinHealth;
        inline Slider* MinManaHeal;
        inline CheckBox* UseW;
    }

    namespace SennaKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseR;

    }

    namespace SennaFlee {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
    }

    namespace SennaHPBAR {
        inline CheckBox* DrawRDamage;
    }

    namespace SennaSpellsSettings {

        inline CheckBox* saveMana;

        inline List* qCastMode;
        inline Slider* qRange;
        inline CheckBox* DrawQ;

        inline List* wCastMode;
        inline Slider* wRange;
        inline CheckBox* DrawW;

        inline Slider* eRange;
        inline CheckBox* DrawE;

        inline List* targetMode;
        inline Slider* minRDistance;
        inline Slider* maxRDistance;

        inline CheckBox* DrawR;

        inline CheckBox* DrawIfReady;
    }


}

namespace SennaDamages
{
    static constexpr float barWidth = 100.0f;
    static constexpr float yOffset = 23.5f;
    static constexpr float xOffset = -46.0f;
    static constexpr float widthMultiplier = 105;

    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 30, 60, 90, 120, 150 };
        inline float additionalPercentageAD = 0.3f;
        inline float lethalityMultiplier = 1.6f;
    }

    namespace WSpell
    {
        inline float dmgSkillArray[6] = { 0, 70, 115, 160, 205, 250 };
        inline float additionalPercentageAD = 0.7f;
    }

    namespace RSpell
    {
        inline float dmgSkillArray[4] = { -9999, 250, 400 ,550 };
        inline float additionalPercentageAD = 1.15f;
        inline float additionalPercentageAP = 0.7f;
    }
}