#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace AsheConfig
{
    namespace AsheCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseR;

        inline Slider* enemiesInRange;
    }

    namespace AsheHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace AsheClear {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minWMinions;
        inline Slider* minMana;
    }

    namespace AsheJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace AsheLastHit {
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace AsheSpellsSettings {

        inline CheckBox* saveMana;

        inline List* qCastMode;

        inline List* wCastMode;
        inline CheckBox* UseWIfInAARange;
        inline CheckBox* UseWIfFullAASpeed;        
        inline Slider* wRange;
        inline CheckBox* DrawW;

        inline List* targetMode;
        inline KeyBind* rTapKey;
        inline Slider* maxRDistance;
        inline Slider* minRDistance;

        inline CheckBox* DrawIfReady;
        
    }

    namespace AsheKillsteal {
        inline CheckBox* UseW;
        inline CheckBox* UseR;
    }

    namespace AsheAntiGapCloser {
        inline CheckBox* UseW;
        inline CheckBox* UseR;
        inline std::vector<Object*> whitelist;
    }

    namespace AsheAntiMelee {
        inline CheckBox* UseW;
        inline std::vector<Object*> whitelist;
    }

    namespace AsheFlee {
        inline CheckBox* UseW;
    }

}

namespace AsheDamages
{
    namespace WSpell {
        inline float dmgSkillArray[6] = { 0, 20, 35, 50, 65, 80 };
        inline float widthArray[6] = { 0, 75.00f, 100.00f, 100.00f, 123.00f, 123.00f };
        inline float additionalPercentageAD = 1.00f;
    }

    namespace RSpell {
        inline float dmgSkillArray[4] = { -9999, 200, 400, 600 };
        inline float dmgSkillArrayMonster[6] = { -9999, 175, 250, 325 };
        inline float additionalPercentageAP = 1.20f;
    }
}
