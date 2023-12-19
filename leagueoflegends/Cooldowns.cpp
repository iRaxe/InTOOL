#include "Cooldowns.h"
#include "Awareness.h"
#include "Awarenesss.h"
#include "Experience.h"
#include "ListManager.h"

ImVec2 Awarenesss::HPBar::CalculateTopLeft(const Vector2& basePos, int index, float width) {
	return {basePos.x + index * (width + 1.0f) + 1.0f, basePos.y + 1.0f};
}

ImVec2 Awarenesss::HPBar::CalculateBottomRight(const Vector2& basePos, int index, float width) {
    return { basePos.x + (index + 1) * (width + 1.0f), basePos.y + width };
}

ImVec2 Awarenesss::HPBar::CalculateCenter(const Vector2& basePos, int index, float width) {
	const ImVec2 topLeft = CalculateTopLeft(basePos, index, width);
    const ImVec2 bottomRight = CalculateBottomRight(basePos, index, width);
    return { (topLeft.x + bottomRight.x) * 0.5f, topLeft.y - 13.0f };
}

void Awarenesss::HPBar::DrawSpellTimer(Object* obj, int index, const ImVec2& topLeft, const ImVec2& centerPos, const ImVec2& bottomRight) {
    const float relativeCooldown = obj->IsEnemy() ? (ListManager::Functions::GetCooldown(obj, index)) : obj->GetSpellBySlotId(index)->GetCooldown();
    if (relativeCooldown == 0.0f) return;

    render::RenderRect(topLeft, bottomRight, COLOR_RED * 0.50f, 4.0f, ImDrawFlags_RoundCornersAll, 1.0f, true);
    if (relativeCooldown < 100.0f) {
        render::RenderText(std::to_string(static_cast<int>(ceil(relativeCooldown))), centerPos, 16.0f, COLOR_RED, true);
    }
}

void Awarenesss::HPBar::DrawSpellIcon(Object* obj, int index, const ImVec2& topLeft, const ImVec2& bottomRight) {
    const auto spellName = str_tolower(std::string(obj->GetSpellBySlotId(index)->GetTextureName()));
    const auto it = ChampionSpells::_spell_map.find(spellName);
    if (it != ChampionSpells::_spell_map.end()) 
        render::Texture2D(it->second, topLeft, bottomRight, true);
}

void Awarenesss::HPBar::DrawDFIcons(Object* obj, const Vector2& basePos, int slotId) {

    if (slotId == 4)
    {
        const auto topLeft = ImVec2(basePos.x + OffsetMultiplierD * DFOverlayXIncrement + 1.0f, basePos.y + DFOverlayYOffset);
        const auto bottomRight = ImVec2(basePos.x + (OffsetMultiplierD + 1) * DFOverlayXIncrement, basePos.y - 1);
        const auto center = ImVec2((topLeft.x + bottomRight.x) * 0.5f, topLeft.y - 13.0f);

	    DrawSpellIcon(obj, 4, topLeft, bottomRight);
        DrawSpellTimer(obj, 4, topLeft, center, bottomRight);
    }

    else if(slotId == 5)
    {
        const auto topLeft = ImVec2(basePos.x + OffsetMultiplierF * DFOverlayXIncrement + 1.0f, basePos.y + DFOverlayYOffset);
        const auto bottomRight = ImVec2(basePos.x + (OffsetMultiplierF + 1) * DFOverlayXIncrement, basePos.y - 1);
        const auto center = ImVec2((topLeft.x + bottomRight.x) * 0.5f, topLeft.y - 13.0f);

	    DrawSpellIcon(obj, 5, topLeft, bottomRight);
        DrawSpellTimer(obj, 5, topLeft, center, bottomRight);
    }
}

void Awarenesss::HPBar::DrawCooldowns(Object* obj, const Vector2& basePos) {

    for (int i = 0; i < 4; ++i) {
        ImVec2 topLeft = CalculateTopLeft(basePos, i, CdWidth);
        ImVec2 center = CalculateCenter(basePos, i, CdWidth);
        ImVec2 bottomRight = CalculateBottomRight(basePos, i, CdWidth);

        DrawSpellIcon(obj, i, topLeft, bottomRight);
        DrawSpellTimer(obj, i, topLeft, center, bottomRight);
    }

    DrawDFIcons(obj, basePos, 4);
    DrawDFIcons(obj, basePos, 5);

}

void Awarenesss::HPBar::Draw(Object* obj) {
    const Vector2 screenPos = Engine::GetHpBarPosition(obj);
    const Vector2 basePos(screenPos.x + XOffset, screenPos.y - YOffset);

    const bool shouldDrawExperience = Configs::EnemyTracker::showExperience->Value && 
        ((Configs::EnemyTracker::showExperienceSelf->Value && obj->GetNetId() == ObjectManager::GetLocalPlayer()->GetNetId()) ||
        (Configs::EnemyTracker::showExperienceAllies->Value && obj->IsAlly() && obj->GetNetId() != ObjectManager::GetLocalPlayer()->GetNetId()) ||
        (Configs::EnemyTracker::showExperienceEnemies->Value && obj->IsEnemy()));

    if (shouldDrawExperience) {
        Experience::Draw(obj, screenPos);
    }

    const bool shouldDrawCooldowns = Configs::EnemyTracker::showCooldowns->Value &&
        ((Configs::EnemyTracker::showCooldownsSelf->Value && obj->GetNetId() == ObjectManager::GetLocalPlayer()->GetNetId()) ||
            (Configs::EnemyTracker::showCooldownsAllies->Value && obj->IsAlly() && obj->GetNetId() != ObjectManager::GetLocalPlayer()->GetNetId()) ||
            (Configs::EnemyTracker::showCooldownsEnemies->Value && obj->IsEnemy()));

    if (shouldDrawCooldowns) {
        DrawCooldowns(obj, basePos);
    }
}

void Awarenesss::HPBar::InitializeMenu()
{
    const auto CooldownDrawingsMenu = Configs::HealthbarOverlayMenu->AddMenu("CooldownDrawingsMenu", "Cooldown Tracker");
    Configs::EnemyTracker::showCooldowns = CooldownDrawingsMenu->AddCheckBox("showCooldowns", "Show cooldowns", true);
    Configs::EnemyTracker::showCooldownsSelf = CooldownDrawingsMenu->AddCheckBox("showCooldownsSelf", "Show your cooldowns", false);
    Configs::EnemyTracker::showCooldownsAllies = CooldownDrawingsMenu->AddCheckBox("showCooldownsAllies", "Show cooldowns for allies", false);
    Configs::EnemyTracker::showCooldownsEnemies = CooldownDrawingsMenu->AddCheckBox("showCooldownsEnemies", "Show cooldowns for enemies", true);
}
