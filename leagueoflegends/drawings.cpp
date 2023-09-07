#include "stdafx.h"
#include "global/structs.h"


namespace drawings
{
    void DrawAARange(Object* obj)
    {
        Vector2 screenPos = functions::WorldToScreen(obj->GetPosition());
        if (!render::IsOnScreen(screenPos)) return;

        render::RenderCircleWorld(obj->GetPosition(), 20, obj->GetAttackRange(), COLOR_WHITE, 1.0f);
    }

    void DrawEnemyListNames()
    {
        Vector2 screenPos = functions::WorldToScreen(globals::localPlayer->GetPosition());
        float baseDraw = 0;
        for (int i = 0; i < globals::heroManager->GetListSize(); i++)
        {
            auto obj = globals::heroManager->GetIndex(i);

            render::RenderText(obj->GetName(), (screenPos - Vector2(0.0f, 22.0f + baseDraw)).ToImVec(), 18.0f, COLOR_WHITE, true);

            baseDraw += 22.0f;
        }
    }

    void Update()
    {
        if (SETTINGS_BOOL("Drawings", "Draw EnemyList"))
        {
            DrawEnemyListNames();
        }
    }
}