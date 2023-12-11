#pragma once
#include "../NewMenu.h"
using namespace UPasta::SDK;

namespace KogMawConfig
{

    namespace KogMawCombo {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseR;
        inline List* RMode;
        inline CheckBox* UseROnlyInBetweenAAS;
    }

    namespace KogMawHarass {
        inline Slider* minMana;
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        inline CheckBox* UseE;
        inline CheckBox* UseEOnlyWhenEnemyAttacking;
    }

    namespace KogMawClear {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        //inline CheckBox* UseE;
        inline Slider* minMana;
        //inline Slider*   eMinMinion;
    }

    namespace KogMawJungle {
        inline CheckBox* UseQ;
        inline CheckBox* UseW;
        //inline CheckBox* UseE;
        inline Slider* minMana;
        //inline Slider*   eMinMinion;
    }

    namespace KogMawLastHit {
        inline CheckBox* UseQ;
        //inline CheckBox* UseE;
        inline Slider* minMana;
        //inline Slider*   eMinMinion;
    }

    namespace KogMawSpellsSettings {

        inline Slider* qMinArmor;

        inline Slider* qRange;
        inline CheckBox* qDraw;

        inline CheckBox* wDraw;
        inline Slider* wRangeOffset;

        inline Slider* eRange;
        inline CheckBox* eDraw;

        inline Slider* rRange;
        inline CheckBox* rDraw;

        inline CheckBox* DrawIfReady;
    }

    namespace KogMawKillsteal {
        inline CheckBox* UseR;
    }

    namespace KogMawFlee {
        inline CheckBox* UseE;
    }

}

namespace KogMawDamages
{
    namespace QSpell
    {
        inline float dmgSkillArray[6] = { 0, 90, 140, 190, 240, 290 };
        inline float additionalPercentageAP = 0.7f;
        inline float QWidth = 140;
    }

    namespace WSpell
    {
        inline float addtionalRangeArray[6] = { 0, 130, 150, 170, 190, 210 };
        inline float dmgSkillArray[6] = { 0, 0, 0, 0, 0, 0 };
        inline float additionalPercentageAD = 0.0f;
        inline float additionalPercentageAP = 0.0f;
    }

    namespace ESpell
    {
        inline float dmgSkillArray[6] = { 0, 75, 120, 165, 210, 255 };
        inline float additionalPercentageAD = 0.0f;
        inline float additionalPercentageAP = 0.7f;
    }

    namespace RSpell
    {
        inline float dmgSkillArray[4] = { 0, 100, 140, 180 };
        inline float multiplicationFactor = 0.83f;
        inline float additionalPercentageAD = 65.0f;
        inline float additionalPercentageAP = 35.0f;
    }
}