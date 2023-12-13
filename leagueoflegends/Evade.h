#pragma once
#include "../stdafx.h"
#include "NewMenu.h"

namespace UPasta::SDK::EvadeConfig
{


    inline CheckBox* status;
    inline CheckBox* showMissiles;
    inline CheckBox* showSpells;    
    inline CheckBox* showAnimation;

    inline CheckBox* statusComboMode;
    inline CheckBox* statusLaneClearMode;
    inline CheckBox* statusFastClearMode;
    inline CheckBox* statusLastHitMode;
    inline CheckBox* statusHarassMode;
    inline CheckBox* statusFleeMode;

    inline KeyBind* statusToggle;
    inline KeyBind* onlyIfPressKey;
}
class Evade2
{
public:
    enum EvadeState {
        DODGING,
        IDLE
    };
private:

    static bool CanEvade();
	static void InitializeMenu();

public:
    static inline EvadeState _state = IDLE;

    static void Init();
    static void OnTick();
    static void OnDraw();
    static void TryMove(Missile* missile);
    static void DrawRange(Object* obj);
    static void OnCastSound(uintptr_t state, SpellCast* cast);
    static void OnCombo();
    static void OnHarass();
    static void OnClear();
    static void OnWndProc(UINT msg, WPARAM param);
};