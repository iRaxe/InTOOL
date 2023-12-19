#include "Evade.h"

#include "Awareness.h"
#include "Damage.h"
#include "TargetSelector.h"

static bool is_kalista = false;

float GetSpellHitTime(Missile* missile, Vector3 pos) {
	auto spellPos = missile->GetSpellPos();
	auto castRadius = pos.distanceTo(spellPos);
	if (missile->GetSpellSpeed() == 0) {
		return max(0.0f, castRadius);
	}
	return 1000 * castRadius / missile->GetSpellSpeed();
}

static bool CanHeroEvade(Missile* missile, Vector3 evadePos) {
	auto me = ObjectManager::GetLocalPlayer();
	if (!me)
		return false;

	auto heroPos = me->GetPosition();

	float evadeTime = 0;
	float spellHitTime = 0;
	float speed = me->GetMovementSpeed();
	float delay = 0.0;
	auto castRadius = missile->GetSpellStartPos().distanceTo(missile->GetSpellEndPos());
	auto evadeRadius = heroPos.distanceTo(evadePos);
	auto missileDistance = missile->GetSpellPos().distanceTo(me->GetPosition());

	evadeTime = (castRadius - evadeRadius + me->GetBoundingRadius() + 10) /
		(missileDistance != 0 ? missileDistance : speed);
	spellHitTime = GetSpellHitTime(missile, evadePos);
	return (spellHitTime - delay) > evadeTime;
}

Vector3 getEvadePos(const Vector3& current, float width, Missile* missile) {
	auto self = ObjectManager::GetLocalPlayer();
	if (!self)
		return Vector3(0, 0, 0);

	Vector3 startPos = missile->GetSpellStartPos();
	Vector3 endPos = missile->GetSpellEndPos();
	Vector3 direction = endPos - startPos;
	Vector3 pos3 = endPos + Vector3(-direction.z, direction.y, direction.x * 1.0);
	Vector3 pos4 = endPos + Vector3(direction.z * 1.0, direction.y, -direction.x);

	Vector3 direction2 = pos3 - pos4;
	direction2.Scale(width);

	Vector3 direction3(0, 0, 0);
	direction3.x = -direction2.x;
	direction3.y = -direction2.y;
	direction3.z = -direction2.z;

	std::vector<Vector3> points;

	for (int k = -8; k < 8; k += 2) {
		if (isLeft(startPos, endPos, self->GetPosition())) {
			Vector3 test_pos = current + (direction3 + (direction.Normalized().Scale(k * 40) + (Vector3(40, 0, 40))));
			if (!Engine::IsWall(test_pos)) {
				points.push_back(test_pos);
			}
		}
		else {
			Vector3 test_pos = current + (direction2 + (direction.Normalized().Scale(k * 40) + (Vector3(40, 0, 40))));
			if (!Engine::IsWall(test_pos)) {
				points.push_back(test_pos);
			}
		}
	}

	if (!points.empty()) {
		std::sort(points.begin(), points.end(), [&](const Vector3& a, const Vector3& b) {
			return self->GetPosition().distanceTo(a) < self->GetPosition().distanceTo(b);
			});
		return points[0];
	}

	return Vector3(0, 0, 0); // You may want to return a special value indicating no valid point.
}

void Evade2::InitializeMenu()
{
	using namespace UPasta::SDK::EvadeConfig;

	const auto EvadeMenu = UPasta::SDK::Menu::CreateMenu("vezEvade", "Evade Settings");

	const auto StatusMenu = EvadeMenu->AddMenu("statusSection", "Status settings");
	statusComboMode = StatusMenu->AddCheckBox("statusComboMode", "Enable in Combo mode", true);
	statusLaneClearMode = StatusMenu->AddCheckBox("statusLaneClearMode", "Enable in LaneClear mode", true);
	statusFastClearMode = StatusMenu->AddCheckBox("statusFastClearMode", "Enable in FastClear mode", true);
	statusLastHitMode = StatusMenu->AddCheckBox("statusLastHitMode", "Enable in LastHit mode", true);
	statusHarassMode = StatusMenu->AddCheckBox("statusHarassMode", "Enable in Harass mode", true);
	statusFleeMode = StatusMenu->AddCheckBox("statusFleeMode", "Enable in Flee mode", true);

	const auto KeyBindingsMenu = EvadeMenu->AddMenu("keybindingsSection", "KeyBindings settings");
	statusToggle = KeyBindingsMenu->AddKeyBind("statusToggle", "Toggle Key", ' ', false, true);
	onlyIfPressKey = KeyBindingsMenu->AddKeyBind("onlyIfPressKey", "Only if pressing Key", 'C', false, false);

	const auto DrawingsMenu = EvadeMenu->AddMenu("drawingsSection", "Drawings settings");
	status = DrawingsMenu->AddCheckBox("status", "Show Range Drawings", true);
	showMissiles = DrawingsMenu->AddCheckBox("showMissiles", "Show Missiles Drawings", true);
	showSpells = DrawingsMenu->AddCheckBox("showSpells", "Show Spells Drawings", true);
	showAnimation = DrawingsMenu->AddCheckBox("showAnimation", "Show Position Animation", true);

}

void Evade2::Init() {
	InitializeMenu();
	Event::Subscribe(Event::OnWndProc, &OnWndProc);
	Event::Subscribe(Event::OnDraw, &OnDraw);
	Event::Subscribe(Event::OnGameTick, &OnTick);
	Event::Subscribe(Event::OnProcessSpell, &OnCastSound);
}

void Evade2::OnWndProc(UINT msg, WPARAM param) {

}
Vector3 test = Vector3(0, 0, 0);
Vector3 test1 = Vector3(0, 0, 0);
void Evade2::TryMove(Missile* missile)
{
	Vector3 textPos = Vector3(ObjectManager::GetLocalPlayer()->GetPosition().x, ObjectManager::GetLocalPlayer()->GetPosition().y + 30, ObjectManager::GetLocalPlayer()->GetPosition().z);
	Vector3 pos2 = getEvadePos(ObjectManager::GetLocalPlayer()->GetPosition(), 70.0f, missile);
	bool canEvade = CanHeroEvade(missile, pos2);
	render::RenderTextWorld(canEvade ? "GG" : "NOTGG", textPos, 24, canEvade ? COLOR_GREEN : COLOR_RED, true);

	_state = DODGING;
	render:render::RenderLineWorld(ObjectManager::GetLocalPlayer()->GetPosition(), pos2, COLOR_YELLOW, 5.0F);
	Engine::IssueMove(Engine::WorldToScreen(pos2));

	if (ObjectManager::GetLocalPlayer()->GetPosition().distanceTo(pos2) < 10.0f)
		_state = IDLE;

	return;
}

void Evade2::OnDraw() {
	auto me = ObjectManager::GetLocalPlayer();
	if (!me) return;

	if (!UPasta::SDK::EvadeConfig::status->Value) return;

	render::RenderTextWorld(UPasta::SDK::EvadeConfig::statusToggle->Value ? "EVADE: ON" : "EVADE: OFF", ObjectManager::GetLocalPlayer()->GetPosition(),24, UPasta::SDK::EvadeConfig::statusToggle->Value ? COLOR_GREEN : COLOR_RED, true);
	
	if (UPasta::SDK::EvadeConfig::showMissiles->Value) {
		for (auto& missileClient : ObjectManager::GetMissileList()->missile_map)
		{
			uintptr_t network_id = missileClient.first;
			Missile* missile = missileClient.second;

			if (IsNotZeroPtr(missile) && IsValidPtr(missile) && missile && !missile->GetMissileData()->IsAutoAttack())
			{
				auto missilePos = missile->GetSpellPos().ToGround();
				if (missilePos.Distance(ObjectManager::GetLocalPlayer()->GetPosition()) > 1500.0f)
					return;

				auto startPos = missile->GetSpellStartPos().ToGround();
				auto endPos = missile->GetSpellEndPos().ToGround();
				TryMove(missile);
				
				Geometry::Polygon poly = Geometry::Rectangle(startPos, endPos, 70.f).ToPolygon();
				render::RenderPolygonWorld(poly, COLOR_WHITE, 1.0f);

				if (UPasta::SDK::EvadeConfig::showAnimation->Value == true)
				{
					Geometry::Polygon poly2 = Geometry::Rectangle(startPos, missilePos, 70.f).ToPolygon();
					render::RenderPolygonWorld(poly2, 0x40FFFFFF, 1.0f, true, 0x40FFFFFF);
				}
			}
		}
	}

	if (test.IsValid() && test1.IsValid())	{
		Geometry::Polygon poly = Geometry::Rectangle(test, test1, 70.f).ToPolygon();
		render::RenderPolygonWorld(poly, COLOR_WHITE, 1.0f);
	}

}

void Evade2::OnTick() {
	auto me = ObjectManager::GetLocalPlayer();
	if (!me) return;
	if (!me->IsAlive()) return;
}

void Evade2::OnCastSound(uintptr_t state, SpellCast* cast) {
	if (cast->GetCasterHandle() == ObjectManager::GetLocalPlayer()->GetHandle()) return;
	if (cast->IsAutoAttack()) return;
	const auto caster = ObjectManager::GetClientByHandle(cast->GetCasterHandle());
	if (caster->IsAlly()) return;

	if (UPasta::SDK::EvadeConfig::showSpells->Value) {
		auto startPos = cast->GetStartPosition().ToGround();
		auto endPos = cast->GetEndPosition().ToGround();

		if (startPos.IsValid() && endPos.IsValid())	{
			test = startPos;
			test1 = endPos;
		}
		return;
	}
}