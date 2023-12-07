#pragma once
#include "../stdafx.h"
#include "NewMenu.h"

namespace UPasta::SDK::OrbwalkerConfig
{
	inline KeyBind* comboKey;
    inline KeyBind* laneClearKey;
    inline KeyBind* fastClearKey;
    inline KeyBind* lastHitKey;
    inline KeyBind* harassKey;
    inline KeyBind* fleeKey;

    inline CheckBox* statusFollowMouse;
    inline CheckBox* statusComboMode;
    inline CheckBox* statusLaneClearMode;
    inline CheckBox* statusFastClearMode;
    inline CheckBox* statusLastHitMode;
    inline CheckBox* statusHarassMode;
    inline CheckBox* statusFleeMode;
}
class Orbwalker
{
public:

    enum STATE {
        IDLE, // MOVING
        ATTACKING, //  CANT MOVE OR CANCELS AUTO
        CASTING, // NORMAL CAST, CAN MOVE
        CHARGING, // XERATHQ ,CAN MOVE 
        CHANNELING, // KATA R, CANT MOVE
        DODGING // CANT MOVE
    };

private:

    static inline STATE _state = IDLE;
    static inline OrbwalkState _mode = Off;
    static inline float _last_aa = 0;
    static inline float _last_action = 0;

    static bool CanAttack();
    static bool CanMove();

    static int GetLatency(int extra = 0);
    static void InitializeMenu();

public:
    static void Init();
    static void OnTick();
    static void OnDraw();
    static void OnCastSound(uintptr_t state, SpellCast* cast);
    static void OnCombo();
    static void OnHarass();
    static void OnClear();
    static void OnWndProc(UINT msg, WPARAM param);
    static inline void Reset() { _last_aa = 0; }

};