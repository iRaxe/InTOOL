#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace EzrealConfig
{
    namespace EzrealCombo {
        CheckBox* UseQ;
        CheckBox* UseW;
        CheckBox* UseR;

        Slider* enemiesInRange;
    	Slider* maxTargetDistance;
	    KeyBind* manualCastUltimateKey;
    }

    namespace EzrealHarass {
        CheckBox* UseQ;
        CheckBox* UseW;

        Slider* minMana;
    }

    namespace EzrealClear {
        CheckBox* UseQ;
        CheckBox* UseW;
        Slider* minMana;
    }

    namespace EzrealJungle {
        CheckBox* UseQ;
        CheckBox* UseW;
        Slider* minMana;
    }

    namespace EzrealLastHit {
        CheckBox* UseQ;
        Slider* minMana;
    }

    namespace EzrealKillsteal {
        CheckBox* UseQ;
        CheckBox* UseE;
        CheckBox* UseR;
    }

    namespace EzrealAntiGapCloser {
        CheckBox* UseE;
    }

    namespace EzrealDrawings {
        CheckBox* DrawQ;
        CheckBox* DrawW;
        CheckBox* DrawIfReady;
    }
}

namespace EzrealDamages
{
    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 20, 45, 70, 95, 120 };
        inline float additionalPercentageAD = 1.30f;
        inline float additionalPercentageAP = 0.15f;

        inline float range = 1200.0f;
        inline float width = 120.0f;
        inline float speed = 2000.0f;
        inline float delay = 0.25f;
    }

    namespace WSpell
    {
        inline float dmgSkillArray[6] = { 0, 80, 135, 190, 245, 300 };
        inline float additionalPercentageAD = 0.60f;
        inline float additionalPercentageAP[6] = { 0, 0.70f, 0.75f, 0.80f, 0.85f, 0.90f };

        inline float range = 1200.0f;
        inline float width = 160.0f;
        inline float speed = 1700.0f;
        inline float delay = 0.25f;
    }

    namespace ESpell
    {
        inline float dmgSkillArray[6] = { 0, 80, 130, 180, 230, 280 };
        inline float additionalPercentageAD = 0.50f;
        inline float additionalPercentageAP = 0.75f;

        inline float range = 475.0f;
    }

    namespace RSpell
    {
        inline float dmgSkillArray[4] = { -9999, 350, 500, 650 };
        inline float dmgSkillArrayMonster[6] = { -9999, 175, 250, 325 };
        inline float additionalPercentageAD = 0.85f;
        inline float additionalPercentageAP = 1.10f;

        inline float width = 180.0f;
        inline float speed = 2000.0f;
        inline float delay = 0.25f;
    }
}
