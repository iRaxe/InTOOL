#include "LastPosition.h"

std::unordered_map<uint64_t, bool> previous_visibility;
std::unordered_map<uint64_t, Vector3> previous_position;
std::unordered_map<uint64_t, float> hidden_time;

float Awarenesss::LastPosition::TimeHidden(Object* hero,
	std::unordered_map<uint64_t, bool>& previous_visibility,
	std::unordered_map<uint64_t, Vector3>& previous_position,
	std::unordered_map<uint64_t, float>& hidden_time)
{
	const auto netId = hero->GetNetId();
	const bool is_currently_visible = hero->IsValidTarget();

	if (!is_currently_visible)	{
		previous_visibility[netId] = false;
		previous_position[netId] = hero->GetPosition();
	}
	else if (previous_visibility[netId] == false) {
		previous_visibility[netId] = true;
		hidden_time[netId] = Engine::GetGameTime();
	}
	else {
		previous_visibility[netId] = true;
		hidden_time[netId] = Engine::GetGameTime();
	}

	return !is_currently_visible ? hidden_time[netId] : 0.0f;
}

void Awarenesss::LastPosition::ShowLastMinimapPosition(Object* obj, const ImVec2& fromPos)
{
	const float minimapSize = Engine::GetMinimapSize();
	const float scaleFactor = 35 * ((minimapSize - 192) / 193);
	const float defScale = max(25.46f, scaleFactor);

	const ImVec2 heroIconMin = ImVec2(fromPos.x, fromPos.y);
	const ImVec2 heroIconMax = ImVec2(fromPos.x + defScale, fromPos.y + defScale);
	const ImVec2 heroIconCenter = ImVec2(fromPos.x + (defScale / 2), fromPos.y + (defScale / 2));

	DrawChampIcon(obj, heroIconMin, heroIconMax);
	render::RenderCircle(heroIconCenter, defScale / 2, COLOR_DARK_TRANSPARENT, 0, 30, true);
	render::RenderCircle(heroIconCenter, defScale / 2, COLOR_RED, 1.0f, 30);
}

void Awarenesss::LastPosition::ShowLastWorldPosition(Object* obj, const ImVec2& fromPos)
{
	if (Configs::EnemyTracker::showLastPosition->Value)
	{
		const ImVec2 heroIconMin = ImVec2(fromPos.x - 25, fromPos.y - 25);
		const ImVec2 heroIconMax = ImVec2(fromPos.x + 25, fromPos.y + 25);
		const ImVec2 heroWorldIconCenter = ImVec2(fromPos.x, fromPos.y);

		DrawChampIcon(obj, heroIconMin, heroIconMax);
		render::RenderCircle(heroWorldIconCenter, 25, COLOR_DARK_TRANSPARENT, 0, 30, true);
	}
}

void Awarenesss::LastPosition::Draw(Object* obj) {
	const uint64_t objNetId = obj->GetNetId();
	const auto itPos = previous_position.find(objNetId);
	const Vector3& wasPreviousPosition = (itPos != previous_position.end()) ? itPos->second : obj->GetPosition();

	if (Configs::EnemyTracker::showLastPosition->Value) {
		const ImVec2 prevPosToWorldVec = Engine::WorldToScreen(wasPreviousPosition).ToImVec();
		ShowLastWorldPosition(obj, prevPosToWorldVec);
	}

	if (Configs::EnemyTracker::showLastPositionMiniMap->Value) {
		const ImVec2 prevPosToMapVec = Engine::WorldToMinimap(wasPreviousPosition).ToImVec();
		ShowLastMinimapPosition(obj, prevPosToMapVec);
	}
}

void Awarenesss::LastPosition::InitializeMenu()
{
	const auto LastPosMenu = Configs::AwarenessMenu->AddMenu("LastPosMenu", "Last Position Tracker");
	Configs::EnemyTracker::showLastPosition = LastPosMenu->AddCheckBox("showLastPosition", "Show last position", true);
	Configs::EnemyTracker::showLastPositionWorld = LastPosMenu->AddCheckBox("showLastPositionWorld", "Show on world", true);
	Configs::EnemyTracker::showLastPositionMiniMap = LastPosMenu->AddCheckBox("showLastPositionMiniMap", "Show on minimap", true);
}
