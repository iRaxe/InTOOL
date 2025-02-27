#include "LastHitDamage.h"

#include "Damage.h"
#include "TargetSelector.h"

ImVec2 Awarenesss::LastHitDamage::CalculateTopLeft(const Vector2& basePos) {
	return { basePos.x + xOffset, basePos.y - yOffset };
}

ImVec2 Awarenesss::LastHitDamage::CalculateBottomRight(const Vector2& basePos, float widthLimit) {
	return { basePos.x + xOffset + widthLimit, basePos.y - yOffset + 3.5f };
}

void Awarenesss::LastHitDamage::Draw() {
	if (!Configs::EnemyTracker::showDamagePrediction->Value)
		return;

	for (auto minion : ObjectManager::GetMinionsAs(Alliance::Enemy))
	{
		if (!minion) continue;

		if (!TargetSelector::IsValid(minion, ObjectManager::GetLocalPlayer()->GetPosition(), ObjectManager::GetLocalPlayer()->GetRealAttackRange())) continue;
		if (minion->IsJungle()) continue;

		const Vector2 screenPos = Engine::GetHpBarPosition(minion);
		const ImVec2 topLeft = CalculateTopLeft(screenPos);
		const ImVec2 bottomRight = CalculateBottomRight(screenPos, (60 * minion->GetPercentHealth()) / 100);

		const float damage = Damage::CalculateAutoAttackDamage(ObjectManager::GetLocalPlayer(), minion);
		const float minionHealth = minion->GetHealth();
		const bool canKill = damage > minionHealth;

		const auto drawColor = canKill ? COLOR_GREEN : COLOR_RED;
		render::RenderRect(topLeft, bottomRight, drawColor, 0.0f, 0, 1.0f, true);

		const float aaNeeded = ceil(minionHealth / damage);
		const float partWidth = (bottomRight.x - topLeft.x) / aaNeeded;

		for (int j = 1; j < static_cast<int>(aaNeeded); ++j)
		{
			const float xPosition = topLeft.x + j * partWidth;
			render::RenderLine(ImVec2(xPosition, topLeft.y), ImVec2(xPosition, bottomRight.y), COLOR_BLACK, 1.0f);
		}

		if (Configs::EnemyTracker::showKillableStatus->Value && canKill) {
			Functions::Radius::DrawRadius(minion->GetPosition(), 60.0f, COLOR_GREEN, 1.0f);
		}

	}
}

void Awarenesss::LastHitDamage::InitializeMenu() {
	const auto MinionDrawings = Configs::AwarenessMenu->AddMenu("MinionDrawings", "Minions Tracker");
	Configs::EnemyTracker::showDamagePrediction = MinionDrawings->AddCheckBox("showDamagePrediction", "Show damage prediction", true);
	Configs::EnemyTracker::showKillableStatus = MinionDrawings->AddCheckBox("showKillableStatus", "Show killable status", true);
}
