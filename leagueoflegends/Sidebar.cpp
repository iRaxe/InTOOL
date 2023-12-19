#include "Sidebar.h"

#include "Cooldowns.h"
#include "Experience.h"
#include "ListManager.h"
#include "TargetSelector.h"

void Awarenesss::Sidebar::DrawBackground(const ImVec2& screenPos) {
	const ImVec2 backgroundMin = ImVec2(screenPos.x + 10 * sizeMultiplier, screenPos.y + 10 * sizeMultiplier);
	const ImVec2 backgroundMax = ImVec2(backgroundMin.x + 180 * sizeMultiplier, backgroundMin.y + 200 * sizeMultiplier);
	render::RenderRect(backgroundMin, backgroundMax, COLOR_DARK_TRANSPARENT, 4.f, ImDrawFlags_RoundCornersAll, 1.0f, true);
}

void Awarenesss::Sidebar::DrawRespawnTimer(Object* obj, const ImVec2& screenPos) {
	if (obj->IsAlive()) return;

	const ImVec2 circleCenter = ImVec2(screenPos.x + 100 * sizeMultiplier, screenPos.y + 90 * sizeMultiplier);
	const float circleRadius = 60 * sizeMultiplier;
	render::RenderCircle(circleCenter, circleRadius, COLOR_DARK_TRANSPARENT, 0, 30, true);

	const float respawnTime = Engine::GetRespawnTimer(obj) + 1;
	const ImVec2 respawnTextPos = ImVec2(screenPos.x + 100 * sizeMultiplier, screenPos.y + 50 * sizeMultiplier);
	render::RenderText(Engine::ConvertTime(respawnTime), respawnTextPos, 24 * sizeMultiplier, COLOR_RED, true);

}

void Awarenesss::Sidebar::DrawHealthBar(Object* obj, const ImVec2& screenPos, float widthLimit) {
	const float larghezzaHpBar = obj->IsAlive() ? widthLimit * (obj->GetPercentHealth() / 100) : widthLimit;
	const ImVec2 hudHpMin = ImVec2(screenPos.x, screenPos.y + 5 * sizeMultiplier);
	const ImVec2 hudHpMax = ImVec2(screenPos.x + larghezzaHpBar, hudHpMin.y + 10 * sizeMultiplier);
	render::RenderRect(hudHpMin, hudHpMax, obj->IsAlive() ? COLOR_GREEN : COLOR_RED, 4.f, ImDrawFlags_RoundCornersAll, 1.0f, true);
}

void Awarenesss::Sidebar::DrawManaBar(Object* obj, const ImVec2& screenPos, float widthLimit) {
	if (obj->GetMana() == 0.0f) return;
	const float larghezzaManaBar = widthLimit * (obj->GetPercentMana() / 100);
	const ImVec2 hudManaMin = ImVec2(screenPos.x, screenPos.y + 18 * sizeMultiplier);
	const ImVec2 hudManaMax = ImVec2(screenPos.x + larghezzaManaBar, hudManaMin.y + 10 * sizeMultiplier);
	render::RenderRect(hudManaMin, hudManaMax, COLOR_BLUE, 4.f, ImDrawFlags_RoundCornersAll, 1.0f, true);
}

void Awarenesss::Sidebar::DrawExperienceBar(Object* obj, const ImVec2& screenPos, float widthLimit) {
	const float progresso = max(0, HPBar::Experience::CalculateProgress(obj->GetExperience(), obj->GetLevel()));
	if (progresso == 0.0f) return;
	float larghezzaExpBar = widthLimit * (progresso / 100);
	const ImVec2 hudExpMin = ImVec2(screenPos.x, screenPos.y + 31 * sizeMultiplier);
	const ImVec2 hudExpMax = ImVec2(screenPos.x + larghezzaExpBar, hudExpMin.y + 10 * sizeMultiplier);
	render::RenderRect(hudExpMin, hudExpMax, COLOR_WHITE, 4.f, ImDrawFlags_RoundCornersAll, 1.0f, true);
}

void Awarenesss::Sidebar::DrawDSpellIcon(Object* obj, const ImVec2& screenPos) {
	const ImVec2 SummSpell1Min = ImVec2(screenPos.x, screenPos.y - iconSize);
	const ImVec2 SummSpell1Center = ImVec2(screenPos.x + (iconSize / 2), screenPos.y - iconSize );
	const ImVec2 SummSpell1Max = ImVec2(SummSpell1Min.x + iconSize, screenPos.y);

	HPBar::DrawSpellIcon(obj, 4, SummSpell1Min, SummSpell1Max);
	DrawSpellTimer(obj, 4, SummSpell1Min, SummSpell1Center, SummSpell1Max);
}

void Awarenesss::Sidebar::DrawFSpellIcon(Object* obj, const ImVec2& screenPos) {
	const ImVec2 SummSpell2Min = ImVec2(screenPos.x - iconSize, screenPos.y - iconSize);
	const ImVec2 SummSpell2Center = ImVec2(screenPos.x - (iconSize / 2), screenPos.y - iconSize);
	const ImVec2 SummSpell2Max = ImVec2(screenPos.x, screenPos.y);

	HPBar::DrawSpellIcon(obj, 5, SummSpell2Min, SummSpell2Max);
	DrawSpellTimer(obj, 5, SummSpell2Min, SummSpell2Center, SummSpell2Max);
}

void Awarenesss::Sidebar::DrawRSpellIcon(Object* obj, const ImVec2& screenPos) {
	if (obj->GetLevel() < 6) return;

	const ImVec2 SummUltimatePosMin = ImVec2(screenPos.x, screenPos.y);
	const ImVec2 SummUltimatePosCenter = ImVec2(screenPos.x + (iconSize / 2), screenPos.y);
	const ImVec2 SummUltimatePosMax = ImVec2(screenPos.x + iconSize, screenPos.y + iconSize);

	HPBar::DrawSpellIcon(obj, 3, SummUltimatePosMin, SummUltimatePosMax);
	DrawSpellTimer(obj, 3, SummUltimatePosMin, SummUltimatePosCenter, SummUltimatePosMax);
}

void Awarenesss::Sidebar::DrawSpellTimer(Object* obj, int index, const ImVec2& topLeft, const ImVec2& centerPos, const ImVec2& bottomRight) {
	const float relativeCooldown = obj->IsEnemy() ? (ListManager::Functions::GetCooldown(obj, index)) : obj->GetSpellBySlotId(index)->GetCooldown();
	if (relativeCooldown == 0.0f) return;

	render::RenderRect(topLeft, bottomRight, COLOR_RED * 0.50f, 4.0f, ImDrawFlags_RoundCornersAll, 1.0f, true);
	if (relativeCooldown < 100.0f) {
		render::RenderText(std::to_string(static_cast<int>(ceil(relativeCooldown))), centerPos, iconSize / 2, COLOR_BLACK, true);
	}
}

ImVec2 Awarenesss::Sidebar::CalculateChildSize(int listSize)
{
	return Configs::EnemySidebar::orientation->Value ?
		ImVec2(listSize * (childBaseSize * sizeMultiplier), childBaseSize * sizeMultiplier) :
		ImVec2(childBaseSize * sizeMultiplier, listSize * (childBaseSize * sizeMultiplier));
}

ImVec2 Awarenesss::Sidebar::CalculatePosition(int index, const ImVec2& basePos)
{
	const float spacing = childBaseSize * (index - 1) * sizeMultiplier;
	return Configs::EnemySidebar::orientation->Value ?
		ImVec2(basePos.x + spacing, basePos.y) :
		ImVec2(basePos.x, basePos.y + spacing);
}

void Awarenesss::Sidebar::DrawHeroSidebar(Object* obj, const ImVec2& position)
{
	const ImVec2 heroIconMin = ImVec2(position.x + 40 * sizeMultiplier, position.y + 30 * sizeMultiplier);
	const ImVec2 heroIconMax = ImVec2(heroIconMin.x + 120 * sizeMultiplier, heroIconMin.y + 120 * sizeMultiplier);

	DrawBackground(position);
	DrawChampIcon(obj, heroIconMin, heroIconMax);
	DrawRespawnTimer(obj, position);

	const float larghezzaIntera = heroIconMax.x - heroIconMin.x;
	DrawHealthBar(obj, ImVec2(heroIconMin.x, heroIconMax.y), larghezzaIntera);
	DrawManaBar(obj, ImVec2(heroIconMin.x, heroIconMax.y), larghezzaIntera);
	DrawExperienceBar(obj, ImVec2(heroIconMin.x, heroIconMax.y), larghezzaIntera);
	DrawDSpellIcon(obj, ImVec2(heroIconMin.x, heroIconMax.y));
	DrawFSpellIcon(obj, ImVec2(heroIconMax.x, heroIconMax.y));

	ImVec2 SummLevelPosCenter = ImVec2(heroIconMax.x - (iconSize / 2), heroIconMin.y + (iconSize / 2));
	render::RenderCircle(SummLevelPosCenter, iconSize / 2, COLOR_DARK_TRANSPARENT, 0, 20, true);
	render::RenderText(std::to_string(obj->GetLevel()), ImVec2(SummLevelPosCenter.x, SummLevelPosCenter.y - (iconSize / 1.30)), (iconSize / 2), COLOR_WHITE, true);

	DrawRSpellIcon(obj, ImVec2(heroIconMin.x, heroIconMin.y));
}


void Awarenesss::Sidebar::Draw(bool* p_open) {

	if (ImGui::Begin("Hero awareness", p_open, window_flags))
	{
		const auto heroManager = ObjectManager::GetHeroList();
		if (!heroManager) return;  // Aggiunto controllo di nullità

		const int champListSize = heroManager->GetListSize();
		sizeMultiplier = Configs::EnemySidebar::hudSize->Value / 100.0f;
		iconSize = iconBaseSize * sizeMultiplier;  // Usa costante nominativa

		const ImVec2 p2 = ImGui::GetCursorScreenPos();
		const ImVec2 childSize = CalculateChildSize(champListSize);  // Funzione ausiliaria per calcolare le dimensioni

		ImGui::BeginChildOld("Test", childSize, false, window_flags);

		for (int i = 1; i < champListSize; i++)
		{
			auto obj = heroManager->GetIndex(i);
			if (!obj || obj->GetName() == "PracticeTool_TargetDummy" || obj->IsAlly())
				continue;

			ImVec2 p = CalculatePosition(i, p2);
			DrawHeroSidebar(obj, p);
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

void Awarenesss::Sidebar::InitializeMenu()
{
	const auto EnemySidebarMenu = Configs::AwarenessMenu->AddMenu("enemySidebar", "Enemies Sidebar");
	Configs::EnemySidebar::status = EnemySidebarMenu->AddCheckBox("status", "Enable the sidebar", true);
	Configs::EnemySidebar::locked = EnemySidebarMenu->AddCheckBox("locked", "Block sidebar movements", false);
	Configs::EnemySidebar::locked->AddTooltip("Enable it if you want to avoid\nMoving the sidebar dragging it");
	Configs::EnemySidebar::orientation = EnemySidebarMenu->AddList("orientationMode", "Draw Mode", std::vector<std::string>{"Vertical", "Horizontal"}, 0);
	Configs::EnemySidebar::hudSize = EnemySidebarMenu->AddSlider("hudSize", "Sidebar Multiplier", 50.0f, 10.0f, 100.0f, 10.0f);
}
