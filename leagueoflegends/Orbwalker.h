#pragma once
#include "../stdafx.h"
#include "NewMenu.h"

namespace UPasta::SDK::OrbwalkerConfig
{
    inline CheckBox* statusFollowMouse;
    inline CheckBox* statusComboMode;
    inline CheckBox* statusLaneClearMode;
    inline CheckBox* statusFastClearMode;
    inline CheckBox* statusLastHitMode;
    inline CheckBox* statusHarassMode;
    inline CheckBox* statusFleeMode;

    inline KeyBind* comboKey;
    inline KeyBind* laneClearKey;
    inline KeyBind* fastClearKey;
    inline KeyBind* lastHitKey;
    inline KeyBind* harassKey;
    inline KeyBind* fleeKey;

    inline CheckBox* status;
    inline CheckBox* showHeroes;
    inline CheckBox* showSelf;
    inline CheckBox* showAllies;
    inline CheckBox* showEnemies;

    inline CheckBox* showTurrets;
    inline CheckBox* showAlliesTurrets;
    inline CheckBox* showEnemiesTurrets;
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
    static inline float _last_aa = 0;
    static inline float _last_action = 0;
    static inline float _draw_range = 1500.0f;


    static bool CanAttack();
    static bool CanMove();

    static int GetLatency(int extra = 0);
    static void InitializeMenu();

public:

    static inline STATE State = IDLE;
    static inline OrbwalkState Mode = Off;

    static void Init();
    static void OnTick();
    static void OnDraw();
    static void DrawRange(Object* obj);
    static void OnCastSound(uintptr_t state, SpellCast* cast);
    static void OnCombo();
    static void OnHarass();
    static void OnClear();
    static void OnWndProc(UINT msg, WPARAM param);
    static void AttackTarget(Object* obj);
    static bool CanCastAfterAttack();

    static inline void Reset() { _last_aa = 0; }

};