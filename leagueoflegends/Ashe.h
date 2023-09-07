#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace AsheConfig
{
    namespace AsheMisc {
        CheckBox* UseQOnObjectives;
    }

    namespace AsheCombo {
        CheckBox* UseQ;
        CheckBox* UseW;
        CheckBox* UseWIfInAARange;
        CheckBox* UseR;

       //Slider* enemiesInRange;
    }

    namespace AsheHarass {
        CheckBox* UseW;
        Slider* minMana;
    }

    namespace AsheClear {
        CheckBox* UseQ;
        CheckBox* UseW;
        Slider* minMana;
    }

    namespace AsheJungle {
        CheckBox* UseQ;
        CheckBox* UseW;
        Slider* minMana;
    }

    namespace AsheKillsteal {
        CheckBox* UseW;
        CheckBox* UseR;
    }

    namespace AsheDrawings {
        CheckBox* DrawW;
    }
}

namespace AsheDamages
{
    namespace WSpell {
        inline float dmgSkillArray[6] = { 0, 20, 35, 50, 65, 80 };
        inline float widthArray[6] = { 0, 75.00f, 100.00f, 100.00f, 123.00f, 123.00f };
        inline float additionalPercentageAD = 1.00f;
        inline float range = 1200.0f;
        inline float speed = 2000.0f;
        inline float delay = 0.25f;
    }

    namespace RSpell {
        inline float dmgSkillArray[4] = { -9999, 350, 500, 650 };
        inline float dmgSkillArrayMonster[6] = { -9999, 175, 250, 325 };
        inline float additionalPercentageAD = 1.00f;
        inline float additionalPercentageAP = 0.90f;
        inline float width = 120.0f;
        inline float speed = 2000.0f;
        inline float delay = 0.25f;
    }
}
