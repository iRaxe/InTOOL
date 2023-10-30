#include "NewMenu.h"

using namespace UPasta::SDK;
using namespace scripts;

namespace EzrealConfig
{
    void SaveJsonFile();
    namespace EzrealCombo {
	    inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseR;

        inline Slider* enemiesInRange;
    }

    namespace EzrealHarass {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace EzrealClear {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace EzrealJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline Slider* minMana;
    }

    namespace EzrealLastHit {
        inline CheckBox* UseQ;
        inline Slider* minMana;
    }

    namespace EzrealSpellsSettings {

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
        
        inline CheckBox* DrawIfReady;
    }

    namespace EzrealKillsteal {
        inline CheckBox* UseQ;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
    }

    namespace EzrealAntiGapCloser {
        inline List* eMode;
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace EzrealAntiMelee {
        inline List* eMode;
        inline CheckBox* UseE;
        inline std::vector<Object*> whitelist;
    }

    namespace EzrealFlee {
        inline CheckBox* UseE;
    }

}

namespace EzrealDamages
{
    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 20, 45, 70, 95, 120 };
        inline float additionalPercentageAD = 1.30f;
        inline float additionalPercentageAP = 0.15f;
    }

    namespace WSpell
    {
        inline float dmgSkillArray[6] = { 0, 80, 135, 190, 245, 300 };
        inline float additionalPercentageAD = 0.60f;
        inline float additionalPercentageAP[6] = { 0, 0.70f, 0.75f, 0.80f, 0.85f, 0.90f };
    }

    namespace ESpell
    {
        inline float dmgSkillArray[6] = { 0, 80, 130, 180, 230, 280 };
        inline float additionalPercentageAD = 0.50f;
        inline float additionalPercentageAP = 0.75f;
    }

    namespace RSpell
    {
        inline float dmgSkillArray[4] = { -9999, 350, 500, 650 };
        inline float dmgSkillArrayMonster[6] = { -9999, 175, 250, 325 };
        inline float additionalPercentageAD = 0.85f;
        inline float additionalPercentageAP = 1.10f;
    }
}
