#include "NewMenu.h"

using namespace UPasta::SDK;

namespace JinxConfig
{
    namespace JinxCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;

        inline Slider* enemiesInRange;
    }

    namespace JinxHarass {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
    }

    namespace JinxJungle {
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace JinxLastHit {
    }

    namespace JinxSpellsSettings {

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
        inline KeyBind* rTapKey;
        inline Slider* maxRDistance;
        inline Slider* minRDistance;
        inline CheckBox* DrawR;

        inline CheckBox* DrawIfReady;
    }

    namespace JinxKillsteal {
        inline CheckBox* UseW;
    	inline CheckBox* UseE;
        inline CheckBox* UseR;
    }


    namespace JinxAntiGapCloser {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace JinxAntiMelee {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace JinxFlee {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
    }

}

namespace JinxDamages
{
    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 0, 0, 0, 0, 0 };
        inline float additionalPercentageAD = 0.0f;
        inline float additionalPercentageAP = 0.0f;
    }

    namespace WSpell
    {
        inline float dmgSkillArray[6] = { 0, 0, 0, 0, 0, 0 };
        inline float additionalPercentageAD = 0.0f;
        inline float additionalPercentageAP = 0.0f;
    }

    namespace ESpell
    {
        inline float dmgSkillArray[6] = { 0, 20, 30, 40, 50, 60 };
        inline float perStackDamage[6] = { 0, 15, 20, 25, 30, 35 };
        inline float additionalPercentageAD = 0.35f;
        inline float additionalPercentageAP = 0.0f;
    }

    namespace RSpell
    {
        inline float dmgSkillArray[4] = { -9999, 40, 55 ,70 };
        inline float additionalPercentageAD = 0.0f;
        inline float additionalPercentageAP = 0.0f;
    }
}
