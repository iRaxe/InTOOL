#include "Experience.h"
#include "Awareness.h"
#include "Cooldowns.h"

float Awarenesss::HPBar::Experience::CalculateProgress(float expAttuale, int livelloCorrente)
{
	const float progresso = (expAttuale - levelExp[livelloCorrente - 1]) /
		(expNeeded[livelloCorrente]) * 100;

	return livelloCorrente == 18 ? 100.0f : progresso;
}

void Awarenesss::HPBar::Experience::Draw(Object* obj, const Vector2& screenPos) {
    const float expProgress = CalculateProgress(obj->GetExperience(), obj->GetLevel());
    const float filledWidth = max(0, 108.0f * (expProgress / 100));

    render::RenderRect(
        ImVec2(screenPos.x + ExpXOffset, screenPos.y - ExpYOffset - ExpHeight),
        ImVec2(screenPos.x + ExpXOffset + filledWidth, screenPos.y - ExpYOffset - ExpHeight + 2),
        COLOR_WHITE, 4.0f, ImDrawFlags_RoundCornersAll, 1.0f, true
    );
}

void Awarenesss::HPBar::Experience::InitializeMenu()
{
    const auto ExperienceDrawingsMenu = Configs::HealthbarOverlayMenu->AddMenu("ExperienceDrawingsMenu", "Experience Tracker");
    Configs::EnemyTracker::showExperience = ExperienceDrawingsMenu->AddCheckBox("showExperience", "Show experience", true);
    Configs::EnemyTracker::showExperienceSelf = ExperienceDrawingsMenu->AddCheckBox("showExperienceSelf", "Show your experience", false);
    Configs::EnemyTracker::showExperienceAllies = ExperienceDrawingsMenu->AddCheckBox("showExperienceAllies", "Show experience for allies", false);
    Configs::EnemyTracker::showExperienceEnemies = ExperienceDrawingsMenu->AddCheckBox("showExperienceEnemies", "Show experience for enemies", true);
}


