#include "NewMenu.h"

using namespace UPasta::SDK;

namespace TwitchConfig
{

    namespace TwitchCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline CheckBox* UseEWithStacks;
        inline Slider* minStacks;
        inline Slider* enemiesInRange;
    }

    namespace TwitchHarass {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseEWithStacks;
        inline Slider* minStacks;
        inline Slider* minMana;

    }

    namespace TwitchClear {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline Slider* minMana;
        inline Slider* minMinion;
    }

    namespace TwitchJungle {
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseEWithStacks;
        inline Slider* minStacks;
        inline Slider* minMana;
    }

    namespace TwitchLastHit {
    }

    namespace TwitchSpellsSettings {

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
        inline CheckBox* DrawR;

        inline CheckBox* DrawIfReady;
    }

    namespace TwitchKillsteal {
        inline CheckBox* UseE;
    }

    namespace TwitchFlee {
        inline CheckBox* UseQ;
    }

}

namespace TwitchDamages
{
    static constexpr float barWidth = 100.0f;
    static constexpr float yOffset = 23.5f;
    static constexpr float xOffset = -46.0f;
    static constexpr float widthMultiplier = 105;

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