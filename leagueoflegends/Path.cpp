#include "Path.h"

void Awarenesss::PathTracker::DrawPlayerPaths(Object* obj)
{
	const auto path = obj->GetAiManager()->GetFutureSegments();
	const int countSegments = static_cast<int>(path.size());
	if (countSegments <= 0)
		return;

	ImVec2 lastScreenPos = Engine::WorldToScreen(obj->GetPosition()).ToImVec();

	for (int i = 0; i < countSegments; i++)
	{
		const ImVec2 currentScreenPos = Engine::WorldToScreen(path[i]).ToImVec();

		render::RenderLine(lastScreenPos, currentScreenPos, COLOR_WHITE, 1.0f);

		lastScreenPos = currentScreenPos;
	}
}

void Awarenesss::PathTracker::Draw(Object* obj)
{
	const bool shouldDrawPath = (Configs::EnemyTracker::showPathsSelf->Value && obj->GetNetId() == globals::localPlayer->GetNetId()) ||
		(Configs::EnemyTracker::showPathsAllies->Value && obj->IsAlly() && obj->GetNetId() != globals::localPlayer->GetNetId()) ||
		(Configs::EnemyTracker::showPathsEnemies->Value && obj->IsEnemy());

	if (shouldDrawPath) {
		DrawPlayerPaths(obj);
	}
}

void Awarenesss::PathTracker::InitializeMenu()
{
	const auto PathDrawingsMenu = Configs::AwarenessMenu->AddMenu("PathDrawingsMenu", "Path Tracker");
	Configs::EnemyTracker::showPaths = PathDrawingsMenu->AddCheckBox("showPaths", "Show paths", true);
	Configs::EnemyTracker::showPathsSelf = PathDrawingsMenu->AddCheckBox("showExperienceSelf", "Show your path", false);
	Configs::EnemyTracker::showPathsAllies = PathDrawingsMenu->AddCheckBox("showExperienceAllies", "Show path for allies", false);
	Configs::EnemyTracker::showPathsEnemies = PathDrawingsMenu->AddCheckBox("showExperienceEnemies", "Show path for enemies", true);
}
