#include "JustEvade.h"

#include "ObjManager.h"
#define _DEBUGEVADE 0
#define _DEBUGMISSILES 0

ProjectionInfo::ProjectionInfo(const bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point) :
	IsOnSegment(is_on_segment), LinePoint(line_point), SegmentPoint(segment_point)
{
}

IntersectionResult::IntersectionResult(const bool intersects, Vector3 const& point) :
	Intersects(intersects), Point(point)
{
}

ProjectionInfo Vector3::ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const
{
	float rs;
	float cx = x;
	float cz = z;
	float ax = segment_start.x;
	float az = segment_start.z;
	float bx = segment_end.x;
	float bz = segment_end.z;

	float cy = y;
	float ay = segment_start.y;
	float by = segment_end.y;

	float rl = float((cx - ax) * (bx - ax) + (cz - az) * (bz - az) + (cy - ay) * (by - ay)) / (pow(bx - ax, 2) + pow(bz - az, 2) + pow(by - ay, 2));
	Vector3 point_line = Vector3(ax + rl * (bx - ax), ay + rl * (by - ay), az + rl * (bz - az));

	//	const auto rl = ((cx - ax) * (bx - ax) + (cz - az) * (bz - az)) / (pow(bx - ax, 2) + pow(bz - az, 2));
	//	const auto point_line = Vector3(ax + rl * (bx - ax), 0, az + rl * (bz - az));

	if (rl < 0)
	{
		rs = 0;
	}
	else if (rl > 1)
	{
		rs = 1;
	}
	else
	{
		rs = rl;
	}

	bool is_on_segment = rs == rl;
	Vector3 point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), ay + rs * (by - ay), az + rs * (bz - az));
	//auto const point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), 0, az + rs * (bz - az));

	return ProjectionInfo(is_on_segment, point_segment, point_line);
}

IntersectionResult Vector3::Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start,
	Vector3 const& line_segment2_end) const
{
	Vector3 side1 = Vector3(line_segment_end.x - this->x, line_segment_end.y - this->y, line_segment_end.z - this->z);
	Vector3 side2 = Vector3(line_segment2_end.x - line_segment2_start.x, line_segment2_end.y - line_segment2_start.y, line_segment2_end.z - line_segment2_start.z);

	float s = (-side1.z * (this->x - line_segment2_start.x) + side1.x * (this->z - line_segment2_start.z)) / (-side2.x * side1.z + side1.x * side2.z);
	float t = (side2.x * (this->z - line_segment2_start.z) - side2.z * (this->x - line_segment2_start.x)) / (-side2.x * side1.z + side1.x * side2.z);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return { true,{ this->x + t * side1.x,  this->y + t * side1.y, this->z + t * side1.z } };

	return { false,{} };
}

namespace Evade
{
	std::list<int> addedSpells;

	void Core::Initalize() {
		InitSpells();
		InitializeMenu();
		Subscribe();
		BoundingRadius = ObjectManager::GetLocalPlayer()->GetBoundingRadius() + 15;

		for (auto hero : *ObjectManager::GetHeroList()) {
			if (hero->IsAlly())				continue;

			for (Champ spell : SpellDB) {
				if (hero->GetName() == spell.hero)
				{
					LOG("Added %s as champion to evade", hero->GetName().c_str());
					spell.obj = hero;
					ChampsInGame.emplace_back(spell);
				}
			}
		}

		for (auto& hero : ChampsInGame) {
			if (hero.obj->GetName() == "Sylas") {
				LOG("Found Sylas in game, loopin through ally spells for add in evade list");
				for (auto teamate : *ObjectManager::GetHeroList()) {
					for (Champ cham : SpellDB) {
						if (teamate->GetName() == cham.hero) {
							for (auto spell : cham.spells) {
								if (spell.slot == _R) {
									hero.spells.push_back(spell);
								}
							}
						}
					}
				}
				break;
			}
		}
	}

	void Core::Subscribe() {
		TryCatch(Event::Subscribe(Event::OnDraw, &OnDraw), "Error subscribing to OnWndProc event");
		TryCatch(Event::Subscribe(Event::OnGameTick, &OnGameUpdate), "Error subscribing to OnGameUpdate event");
		TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
		TryCatch(Event::Subscribe(Event::OnProcessSpell, &OnProcessSpell), "Error subscribing to OnProcessSpell event");
	}

	void Core::Unsubscribe() {
		TryCatch(Event::UnSubscribe(Event::OnDraw, &OnDraw), "Error unsubscribing to OnDraw event");
		TryCatch(Event::UnSubscribe(Event::OnGameTick, &OnGameUpdate), "Error unsubscribing to OnGameTick event");
		TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing to OnWndProc event");
		TryCatch(Event::UnSubscribe(Event::OnProcessSpell, &OnProcessSpell), "Error unsubscribing to OnBeforeAttack event");
	}

	void Core::InitializeMenu()
	{
		using namespace EvadeConfig;

		const auto EvadeMenu = UPasta::SDK::Menu::CreateMenu("vezEvade", "Evade Settings");
		evadeSpells = EvadeMenu->AddCheckBox("evadeSpells", "Evade Spells", true);
		evadeMissiles = EvadeMenu->AddCheckBox("evadeMissiles", "Evade Missiles", true);
		forceEvade = EvadeMenu->AddCheckBox("forceEvade", "Force Evade", true);
	}

	float lastChanged = 0;
	void Core::OnDraw()
	{
		for (Spell spell : DetectedSkillshots)
		{
			if (spell.endPos.IsValid() && spell.startPos.IsValid()) {
				if (spell.type == SpellType::circular)
					render::RenderCircleWorld(spell.endPos, 50, spell.radius, ImColor(255, 255, 255), 1);
				else if (spell.type == SpellType::linear)
					render::RenderPolygonWorld(spell.path, ImColor(255, 255, 255), 1);
				else
					render::RenderPolygonWorld(spell.path, COLOR_RED	, 1);
			}
		}
		render::RenderCircleWorld(MyHeroPos, 50, 200, COLOR_YELLOW, 3);
		render::RenderCircleWorld(SafePos, 50, 200, COLOR_GREEN, 3);
		render::RenderCircleWorld(GetExtendedSafePos(Engine::GetMouseWorldPos()), 50, 200, COLOR_BLUE, 3);
		render::RenderCircleWorld(PreviousPos, 50, 200, COLOR_BLACK, 3);
		
		if (IsSafePos(MyHeroPos, 300))
			render::RenderTextWorld("SAFE ZONE", ObjectManager::GetLocalPlayer()->GetPosition(), 24.0f, COLOR_GREEN, true);
		
	}

	void Core::OnGameUpdate() {
		if (ObjectManager::GetLocalPlayer() == nullptr) return;
		if (!ObjectManager::GetLocalPlayer()->IsAlive()) return;

		GameTimer = Engine::GetGameTime();

		if (Evading)
		{
			Engine::PrintChat(CHAT_COLOR("#72ff72", "Loooool"));			
		}
		auto it = DetectedSkillshots.begin();
		while (it != DetectedSkillshots.end()) {
			if (!IsValid(*it)) {
				it = DetectedSkillshots.erase(it);
			}
			else {
				++it;
			}
		}

		for (int i = 0; i < DetectedSkillshots.size(); i++) {
			if (DetectedSkillshots[i].followEnemy) {
				DetectedSkillshots[i].endPos = DetectedSkillshots[i].obj->GetPosition();
			}
			else if (DetectedSkillshots[i].followEnemy2) {
			}
			else if (DetectedSkillshots[i].followEnemy3) {
				if (DetectedSkillshots[i].delayBR > 0) {
					if (DetectedSkillshots[i].startTime + DetectedSkillshots[i].delay + DetectedSkillshots[i].delayBR < GameTimer && !DetectedSkillshots[i].directionChanged) {
						DetectedSkillshots[i].startPos = DetectedSkillshots[i].endPos;
						DetectedSkillshots[i].directionChanged = true;
					}
					if (DetectedSkillshots[i].directionChanged)
						DetectedSkillshots[i].endPos = DetectedSkillshots[i].obj->GetPosition();
				}
				else {
					DetectedSkillshots[i].endPos = DetectedSkillshots[i].obj->GetPosition();
				}
			}

			DetectedSkillshots[i].path = GetPath(DetectedSkillshots[i]);
		}

		for (Spell& spell : DetectedSkillshots) {
			spell.endPos = CalculateEndPos(spell);
			spell.path = GetPath(spell);
		}


		if (!EnabledSkillshots.empty()) {
			EnabledSkillshots.clear();
		}

		if (!DangerSkillshots.empty()) {
			DangerSkillshots.clear();
		}

		for (Spell spell : DetectedSkillshots)
		{
			if (spell.path.IsInside(ObjectManager::GetLocalPlayer()->GetAiManager()->GetPosition())) {
				DangerSkillshots.emplace_back(spell);
			}
			EnabledSkillshots.emplace_back(spell);
		}


		if (DetectedSkillshots.empty())
		{
			ExtendedPos = Vector3();
			SafePos = Vector3();
			Evading = false;
		}

		if (!Evading && !IsDangerous(PreviousPos) && IsDangerous(ObjectManager::GetLocalPlayer()->GetAiManager()->GetPathEnd())) {
			SetEvading(true);
			MoveToPos(ObjectManager::GetLocalPlayer()->GetAiManager()->GetPathEnd().Extend(ObjectManager::GetLocalPlayer()->GetPosition(), BoundingRadius));
		}

		StartEvading();

		// TODO: HANDLE IS EVADING SPELL
	}

	void Core::OnWndProc(UINT msg, WPARAM param) {
		if (param == 0x4B) {
			switch (msg) {
			case WM_KEYDOWN: 			
				EvadeConfig::evadeSpells->Value = !EvadeConfig::evadeSpells->Value;
				lastChanged = GetTickCount() + 100;
				DetectedSkillshots.clear();
				Engine::PrintChat("Evade: " + std::string((EvadeConfig::evadeSpells->Value ? "ON" : "OFF")));
				break;
			case WM_KEYUP: break;
			}
		}
	}

	bool Core::IsValid(Spell& s)
	{
		if (s.spell && s.path.Points.size() > 0) {
			if ((s.startTime + s.range / s.speed + s.delay + s.time + s.delayBR) > GameTimer) {
					if (s.type == linear || s.type == threeway) {
						float rng = s.speed * (GameTimer - s.startTime - s.delay);
						Vector3 sP = s.startPos.Extend(s.endPos, rng);
						s.currPos = sP;
						s.path = Geometry::Rectangle(sP, s.endPos, s.radius).ToPolygon(BoundingRadius);
					}
			}
			else {
				return false;
			}

			return true;
		}
		return false;
	}

	bool Core::CheckPosCollision()
	{
		auto aim = ObjectManager::GetLocalPlayer()->GetAiManager();
		bool flag = aim->GetSegmentsCount() > 0;
		for (Spell spell : DetectedSkillshots) {
			if (!Evading && GetMovePath().IsValid()) {
				if (flag) {
					auto mPath = aim->GetFutureSegments();
					for (int i = 0; i < aim->GetSegmentsCount(); i++) {
						Vector3 path = mPath[i - 1];
						bool flag2 = GetMovePath().distanceTo(path) > 5 || aim->GetSegmentsCount() > 2 || spell.path.IsInside(path);
						if (flag2)
							return true;
					}
				}
			}
		}
		return false;
	}

	void Core::MoveToPos(Vector3 pos)
	{
		if (!EvadeConfig::evadeSpells->Value)
			return;

		Vector3 p = pos.Extend(MyHeroPos, -BoundingRadius);
		if (Engine::GetGameTime() > LastClick) {
			LastClick = Engine::GetGameTime();

			Engine::IssueMove(Engine::WorldToScreen(pos));
			Engine::PrintChat(CHAT_COLOR("#72ff72", "TRYING"));
		}
	}

	void Core::SetEvading(bool b)
	{
		if (b) {
			LastClick = 0.0f;
			MoveToPos(SafePos);
		}
		Evading = b;
	}

	bool Core::IsDangerous(Vector3 pos)	{
		for (Spell spell : DetectedSkillshots) {
			if (spell.path.IsInside(pos)) {
				return true;
			}
		}

		return false;
	}

	void Core::DodgeSpell()	{
		if (ExtendedPos.IsValid()) {
			MoveToPos(ExtendedPos);
			if (!EvadeConfig::forceEvade->Value)
				ExtendedPos = Vector3(0, 0, 0);
		}
	}

	std::list<Vector3> Core::FindIntersections(Geometry::Polygon poly, Vector3 p1, Vector3 p2)	{
		std::list<Vector3>intersections;
		for (size_t i = 0; i < poly.Points.size(); ++i) {
			Vector3 startPos = poly.Points[i];
			Vector3 endPos = poly.Points[(i == poly.Points.size() - 1) ? 0 : i + 1];
			Vector3 temp = startPos.Intersection(endPos, p1, p2).Point;
			if (temp.IsValid())
				intersections.emplace_back(temp);
		}
		return intersections;
	}

	Vector3 Core::PrependVector(Vector3 pos1, Vector3 pos2, float dist)	{
		return pos1 + (pos2 - pos1).Normalized() * dist;
	}

	void Core::StartEvading()	{
		MyHeroPos = ObjectManager::GetLocalPlayer()->GetPosition();

		if (PreviousPos.IsValid() && MyHeroPos.distanceTo(PreviousPos) > 200) {
			SetEvading(false);
			EvadeToPoint.x = 0;
			EvadeToPoint.y = 0;
		}

		PreviousPos = MyHeroPos;

		if (!ObjectManager::GetLocalPlayer()->IsAlive() || ObjectManager::GetLocalPlayer()->GetAiManager()->IsDashing()) {
			SetEvading(false);
			EvadeToPoint = Vector3();
			return;
		}

		if (!DetectedSkillshots.empty()) {
			int result = 0;
			for (Spell& spell : DetectedSkillshots)
				result += CoreManager(spell);

			Vector3 movePath = GetMovePath();
			if (movePath.IsValid() && !Evading) {
				auto is_my_position_safe = IsDangerous(MyHeroPos);
				auto is_my_path_safe = IsSafePos(movePath, 100);
				std::list<Vector3> ints;
				for (Spell s : DetectedSkillshots) {
					Geometry::Polygon poly = s.path;
					if (!poly.IsInside(MyHeroPos)) {
						ints = FindIntersections(poly, MyHeroPos, movePath);
					}
				}

				ints = filter(ints, [&](const Vector3& pos) { return !IsDangerous(pos) && !Engine::IsWall(pos); });
				if (!ints.empty()) {
					ints.sort([&](Vector3 const& a, Vector3 const& b)	{ return MyHeroPos.DistanceSquared(a) < MyHeroPos.DistanceSquared(b); });
					Vector3 movePos = PrependVector(MyHeroPos, ints.front(), BoundingRadius / 2);
					MoveToPos(movePos);
				}
			}

			if (Evading)
				DodgeSpell();

			if (result == 0) {
				Evading = false;
				SafePos = Vector3();
				ExtendedPos = Vector3();
			}
		}
	}

	Vector3 Core::GetExtendedSafePos(Vector3 pos)	{
		float distance = MyHeroPos.distanceTo(pos);
		std::list<Vector3>positions;
		for (auto minion : *ObjectManager::GetMinionList()) {
			if (minion == nullptr) continue;
			if (minion->IsAlly()) continue;
			if (!minion->IsTargetable()) continue;
			if (minion->GetMaxHealth() < 5) continue;

			Vector3 minionPos = minion->GetPosition();
			if (MyHeroPos.distanceTo(minionPos) <= distance) {
				positions.emplace_back(minionPos);
			}
		}

		for (int i = 1; i < 8; i++) {
			bool collision = false;

			Vector3 ext = MyHeroPos.Append(MyHeroPos, pos, BoundingRadius * i);
			if (i > 1 && !Engine::IsWall(ext) || i == 1) {
				for (Vector3& minionPos : positions) {
					if (ext.distanceTo(minionPos) <= BoundingRadius) {
						collision = true;
						break;
					}
				}

				if (!collision)
					return ext;
			}
		}
		return Vector3();
	}

	int Core::Avoid(Spell spell, Vector3 dodgePos, EvadeSpell data)	{
		return 0;
	}

	int Core::CoreManager(Spell s) {
		if (s.path.IsInside(MyHeroPos)) {
			if (OldTimer != NewTimer) {
				Vector3 safePos = GetBestEvadePos(DangerSkillshots, s.radius, 1);
				int result = 0;
				if (safePos.IsValid()) {

					ExtendedPos = GetExtendedSafePos(safePos);
					SafePos = safePos;
					Engine::PrintChat(CHAT_COLOR("#72ff72", "ASSIGNED SAFE POS"));

					Evading = true;
				}
				else {
					if (result == 0) {
						Vector3 dodgePos = GetBestEvadePos(DangerSkillshots, s.radius, 1);
						if (dodgePos.IsValid())
						{
							ExtendedPos = GetExtendedSafePos(dodgePos);
							SafePos = dodgePos;
							Engine::PrintChat(CHAT_COLOR("#72ff72", "ASSIGNED EXTENDED POS"));

							Evading = true;
						}
					}
				}

				OldTimer = NewTimer;
			}

			return 1;
		}
		return 0;
	}

	Vector3 Core::GetMovePath()	{
		if (!ObjectManager::GetLocalPlayer()->GetAiManager()->IsMoving())
			return Vector3();

		return ObjectManager::GetLocalPlayer()->GetAiManager()->GetPathEnd();
	}

	void Core::OnSpellCast(Spell& spell)
	{
		if (!spell.obj || !spell.spell)
			return;

		spell.endPos = CalculateEndPos(spell);

		if (spell.followEnemy)
			spell.startPos = spell.obj->GetPosition();

		spell.path = GetPath(spell);

		if (spell.path.Points.empty())
			return;

		render::RenderPolygonWorld(spell.path, ImColor(255, 255, 255), 1);

		bool exists = false;

		for (Spell s : DetectedSkillshots) {
			if (s.displayName == spell.displayName && s.obj == spell.obj) {
				exists = true;
				break;
			}
		}

		if (!exists) {
			DetectedSkillshots.emplace_back(spell);
			if (spell.type == threeway) {
				for (int i = 0; i < 2; i++) {
					Spell tspell = spell;
					if (i == 0) {
						tspell.endPos = spell.endPos.Rotate(spell.startPos, DEG2RAD(spell.angle));
					}
					else if (i == 1) {
						tspell.endPos = spell.endPos.Rotate(spell.startPos, -DEG2RAD(spell.angle));
					}

					spell.path = Geometry::Rectangle(tspell.startPos, tspell.endPos, tspell.radius).ToPolygon(BoundingRadius);
					DetectedSkillshots.emplace_back(tspell);
				}
			}
		}

		NewTimer = GameTimer;
	}

	float Core::GetMovementSpeed(bool extra, EvadeSpell evadeSpell)	{
		Object* localPlayer = ObjectManager::GetLocalPlayer();
		if (localPlayer == nullptr) return false;

		float moveSpeed = localPlayer->GetMovementSpeed();
		if (!extra)
			return moveSpeed;

		if (evadeSpell.type == EvadeType::Blink)
			return 9999;

		return moveSpeed;
	}

	bool Core::IsAboutToHit(Spell& spell, Vector3 pos1, int extra)	{
		EvadeSpell evadeSpell;
		for (EvadeSpell es : MyEvadeSpells) {
			if (es.type == EvadeType::SpellShield && es.use)
				evadeSpell = es;
		}

		if (extra && evadeSpell.type != EvadeType::SpellShield)
			return false;

		float moveSpeed = GetMovementSpeed(extra, evadeSpell);
		Vector3 myPos = MyHeroPos;

		float diff = GameTimer - spell.startTime;
		Vector3 pos = myPos.Append(myPos, pos1, 99999);
		if (spell.speed != MathHuge && spell.type == linear || spell.type == threeway) {
			if (spell.delay > 0 && diff <= spell.delay) {
				myPos = (myPos).Extend(pos, (spell.delay - diff) * moveSpeed);
				if (!spell.path.IsInside(myPos))
					return false;

				Vector3 va = (pos - myPos).Normalized() * moveSpeed;
				Vector3 vb = (spell.endPos - spell.startPos).Normalized() * spell.speed;
				Vector3 da = (myPos - spell.startPos);
				Vector3 db = (va - vb);
				float a = db.DotProduct(db);
				float b = 2 * da.DotProduct(db);
				float c = da.DotProduct(da) - std::pow((spell.radius + BoundingRadius * 2), 2);
				float delta = float(b * b - 4 * a * c);
				if (delta >= 0) {
					float rtDelta = std::sqrtf(delta);
					float t1 = (-b + rtDelta) / (2 * a);
					float t2 = (-b - rtDelta) / (2 * a);

					return max(t1, t2) >= 0;
				}
				return false;
			}
		}

		float t = max(0, float(spell.range / spell.speed + spell.delay - diff));
		return spell.path.IsInside(myPos.Extend(pos, moveSpeed * t));
	}

	bool Core::IsSafePos(Vector3 pos, int extra)	{
		for (Spell& s : DetectedSkillshots) {
			if (s.path.IsInside(pos) || IsAboutToHit(s, pos, extra) || isNearEnemy(pos, 550) || isNearMinion(pos, 300)) {
				return false;
			}

			break;
		}
		return true;
	}

	float Core::GetDistanceToChampions(Vector3 pos)	{
		if (!pos.IsValid()) return 0;

		float minDist = FLT_MAX;

		for (auto hero : *ObjectManager::GetHeroList()) {
			if (hero == nullptr) continue;
			if (hero->IsAlly()) continue;
			if (!hero->IsTargetable()) continue;

			float dist = hero->GetPosition().distanceTo(pos);
			minDist = min(minDist, dist);
		}

		return minDist;
	}

	float Core::GetDistanceToMinions(Vector3 pos) {
		if (!pos.IsValid()) return 0;

		float minDist = FLT_MAX;

		for (auto minion : *ObjectManager::GetMinionList()) {
			if (minion == nullptr) continue;
			if (minion->IsAlly()) continue;
			if (!minion->IsTargetable()) continue;
			if (minion->GetMaxHealth() < 5) continue;

			float dist = minion->GetPosition().distanceTo(pos);
			minDist = min(minDist, dist);
		}

		return minDist;
	}

	float Core::GetDistanceToTurrets(Vector3 pos) {
		float minDist = FLT_MAX;

		for (Object* turret : *ObjectManager::GetTurretsList()) {
			if (turret == nullptr) continue;
			if (turret->IsAlly()) continue;
			if (!turret->IsTargetable()) continue;

			float distToTurret = pos.distanceTo(turret->GetPosition());
			minDist = min(minDist, distToTurret);
		}

		return minDist;
	}

	bool Core::isNearEnemy(Vector3 pos, float distance) {
		Object* localPlayer = ObjectManager::GetLocalPlayer();
		if (localPlayer == nullptr) return false;

		float curDistToEnemies = GetDistanceToChampions(localPlayer->GetPosition());
		float posDistToEnemies = GetDistanceToChampions(pos);

		return curDistToEnemies < distance && curDistToEnemies > posDistToEnemies || posDistToEnemies < distance;
	}

	bool Core::isNearMinion(Vector3 pos, float distance) {
		Object* localPlayer = ObjectManager::GetLocalPlayer();
		if (localPlayer == nullptr) return false;

		float curDistToEnemies = GetDistanceToMinions(localPlayer->GetPosition());
		float posDistToEnemies = GetDistanceToMinions(pos);

		return curDistToEnemies < distance && curDistToEnemies > posDistToEnemies || posDistToEnemies < distance;
	}

	bool Core::sortBest(Vector3 a, Vector3 b) {
		return a.DistanceSquared(MyHeroPos) < b.DistanceSquared(MyHeroPos);
	}

	Vector3 Core::GetBestEvadePos(std::list<Spell> spells, float radius, int mode) {
		std::list<Vector3> points;
		std::list<Geometry::Polygon> polygons;

		for (Spell spell : spells) {
			polygons.push_back(spell.path);
		}

		std::list<Geometry::Polygon> danger_polygons = polygons;

		for (Geometry::Polygon poly : danger_polygons) {
			for (size_t i = 0; i < poly.Points.size(); ++i) {
				auto startPos = poly.Points[i];
				auto endPos = poly.Points[(i == poly.Points.size() - 1) ? 0 : i + 1];

				auto my_position = MyHeroPos;
				auto original = my_position.ProjectOn(startPos, endPos).SegmentPoint;
				auto distSqr = original.DistanceSquared(my_position);

				if (distSqr <= 360000) {
					auto side_distance = endPos.DistanceSquared(startPos);
					auto direction = (endPos - startPos).Normalized();
					int step = (distSqr < 200 * 200 && side_distance > 90 * 90) ? 7 : 0;

					for (int j = -step; j <= step; j++) {
						auto candidate = original + direction * (j * 20);
						candidate = MyHeroPos.Append(MyHeroPos, candidate, 5);
						if (!IsDangerous(candidate) && !Engine::IsWall(candidate)) {
							points.push_back(candidate);
						}
					}
				}
			}
		}

		if (!points.empty()) {
			Vector3 tempMyHeroPos = MyHeroPos;
			float tempRB = radius + BoundingRadius;
			Vector3 tempMPos = Engine::GetMouseWorldPos();

			if (mode == 1) {
				points.sort(sortBest);
			}
			else if (mode == 2) {
				points.sort([&tempMyHeroPos, &tempRB, &tempMPos](Vector3 const& a, Vector3 const& b){
						Vector3 mPos = tempMyHeroPos.Extend(tempMPos, tempRB);
						return a.DistanceSquared(mPos) <= b.DistanceSquared(mPos);
					});
			}

			return points.front();
		}

		return Vector3();
	}

	Vector3 Core::CalculateEndPos(Spell& spellInfo)	{
		Vector3 startPos1 = spellInfo.startPos;
		Vector3 placementPos = spellInfo.endPos;
		Vector3 unitPos = spellInfo.obj->GetPosition();
		float range = spellInfo.range;
		Vector3 endPos = startPos1.Extend(placementPos, range);

		if (!spellInfo.extend) {
			if (range > 0) {
				if (unitPos.distanceTo(placementPos) < range) {
					endPos = placementPos;
				}
			}
			else {
				endPos = unitPos;
			}
		}
		else if (spellInfo.type == linear) {
			if (spellInfo.speed != MathHuge) {
				endPos = endPos.Append(startPos1, endPos, spellInfo.radiusRes);
			}
			if (spellInfo.collision || spellInfo.collisionWC) {
				Vector3 startPos = startPos1.Extend(placementPos, 45);
				std::list<Vector3>minions;
				if (spellInfo.collision) {
					for (Object* minion : *ObjectManager::GetMinionList())
					{
						if (minion == nullptr) continue;
						if (minion->IsAlly()) continue;
						if (!minion->IsTargetable()) continue;

						Vector3 minionPos = minion->GetPosition();
						if (minionPos.distanceTo(startPos) <= range && minion->GetMaxHealth() > 295 && minion->GetHealth() > 5) {
							Vector3 col = minionPos.ProjectOn(startPos, placementPos).SegmentPoint;
							if (col.IsValid() && col.distanceTo(minionPos) < ((minion->GetBoundingRadius()) / 2 + spellInfo.radius)) {
								minions.emplace_back(minionPos);
							}
						}
					}
				}

				if (!minions.empty()) {
					minions.sort([&](Vector3 const& a, Vector3 const& b){ return a.DistanceSquared(startPos) < b.DistanceSquared(startPos); });
					float range2 = startPos.distanceTo(minions.front());
					endPos = (startPos).Extend(placementPos, range2);
					spellInfo.range = range2;
				}
			}
		}
		if (!spellInfo.extend)
			spellInfo.range = startPos1.distanceTo(endPos);
		spellInfo.range = range;

		return endPos;
	}

	Geometry::Polygon Core::GetPath(Spell& spell)
	{
		if (spell.type == linear || spell.type == polygon) {
			return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon(BoundingRadius);
		}
		if (spell.type == zone) {
			return Geometry::Rectangle(spell.endPos.Extend(spell.startPos, -spell.radiusEx), spell.endPos, spell.radius).ToPolygon(BoundingRadius);
		}
		if (spell.type == rectangular) {
			Vector3 dir = (spell.endPos - spell.startPos).Perpendicular().Normalized() * (400);
			spell.startPos = spell.endPos - dir;
			spell.endPos = spell.endPos + dir;

			return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius / 2).ToPolygon(BoundingRadius);
		}
		if (spell.type == circular) {
			return Geometry::Circle(spell.endPos, spell.radius).ToPolygon();
		}
		if (spell.type == arc) {
			return Geometry::Arc(spell.startPos, spell.endPos, spell.radius).ToPolygon();
		}
		if (spell.type == ring) {
			return Geometry::Ring(spell.endPos, spell.radius, spell.radiusEx).ToPolygon();
		}
		if (spell.type == conic) {
			return Geometry::Sector(spell.startPos, spell.endPos - spell.startPos, spell.angle * M_PI / 180.0f, spell.range).ToPolygon(BoundingRadius);
		}
		if (spell.type == threeway) {
			return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon(BoundingRadius);
		}

		return Geometry::Rectangle(spell.startPos, spell.endPos, spell.radius).ToPolygon(BoundingRadius);
	}

	void Core::OnProcessSpell(int state, SpellCast* spellCastInfo) {
		if (spellCastInfo == nullptr) return;
		if (spellCastInfo->GetCasterHandle() == ObjectManager::GetLocalPlayer()->GetHandle()) return;

		const auto caster = ObjectManager::GetClientByHandle(spellCastInfo->GetCasterHandle());
		if (caster == nullptr) return;
		if (!caster->IsHero()) return;
		if (caster->IsAlly()) return;

		for (Champ& champ : ChampsInGame) {
			for (Spell& s : champ.spells) {
				if (Compare(spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetName(), s.name, true)) {
					addedSpells.push_back(spellCastInfo->GetCasterHandle());
					s.startTime = GameTimer;
					s.obj = champ.obj;
					s.spell = spellCastInfo->GetProcessSpellInfo();
					if ((s.type != circular && s.type != ring) && s.range == 0 && spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetMaxCastRange() != 0)
						s.range = (spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetMaxCastRange());
					s.startPos = spellCastInfo->GetStartPosition();
					s.endPos = spellCastInfo->GetEndPosition();
					s.radiusRes = spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetCastRadius();
					if (s.radius == 0 && spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetCastRadius() != 0)
						s.radius = s.radiusRes;
					if (s.type != circular)
						s.speed = spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetCastSpeed() == 0 ? MathHuge : spellCastInfo->GetProcessSpellInfo()->GetSpellData()->GetCastSpeed();

					OnSpellCast(s);
				}
			}
		}
	}
	
	//TODO
	void Core::InitSpells()
	{
		{
			Champ Items;
			Items.hero = "Items";

		}

		{
			Champ Belveth;
			Belveth.hero = "Belveth";

			Spell W;
			W.name = "BelvethW";
			W.icon = "BelvethW";
			W.displayName = "Stun";
			W.missileName = "";
			W.slot = _W;
			W.type = linear;
			W.speed = 3000000;
			W.range = 715;
			W.delay = 0.5;
			W.radius = 100;
			W.danger = 3;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Belveth.spells.emplace_back(W);

			SpellDB.emplace_back(Belveth);
		}

		{
			Champ Aatrox;
			Aatrox.hero = "Aatrox";

			Spell Q1;
			Q1.name = "AatroxQ";
			Q1.icon = "AatroxQ";
			Q1.displayName = "The Darkin Blade [First]";
			Q1.missileName = "";
			Q1.slot = _Q;
			Q1.type = linear;
			Q1.speed = MathHuge;
			Q1.range = 660;
			Q1.delay = 0.5;
			Q1.radius = 120;
			Q1.danger = 3;
			Q1.cc = true;
			Q1.collision = false;
			Q1.windwall = false;
			Q1.hitbox = false;
			Q1.fow = false;
			Q1.exception = false;
			Q1.extend = true;
			Aatrox.spells.emplace_back(Q1);

			Spell Q2;
			Q2.name = "AatroxQ2";
			Q2.icon = "AatroxQ";
			Q2.displayName = "The Darkin Blade [Second]";
			Q2.missileName = "";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = MathHuge;
			Q2.range = 510;
			Q2.delay = 0.5;
			Q2.radius = 350;
			Q2.danger = 3;
			Q2.cc = true;
			Q2.collision = false;
			Q2.windwall = false;
			Q2.hitbox = false;
			Q2.fow = false;
			Q2.exception = false;
			Q2.extend = true;
			Aatrox.spells.emplace_back(Q2);

			Spell Q3;
			Q3.name = "AatroxQ3";
			Q3.icon = "AatroxQ";
			Q3.displayName = "The Darkin Blade [Third]";
			Q3.missileName = "";
			Q3.slot = _Q;
			Q3.type = circular;
			Q3.speed = MathHuge;
			Q3.range = 200;
			Q3.delay = 0.5;
			Q3.radius = 160;
			Q3.danger = 3;
			Q3.cc = true;
			Q3.collision = false;
			Q3.windwall = false;
			Q3.hitbox = false;
			Q3.fow = false;
			Q3.exception = false;
			Q3.extend = true;
			Aatrox.spells.emplace_back(Q3);

			/*Spell Q1;
			Q1.name = "AatroxQWrapperCast";
			Q1.icon = "AatroxQ1";
			Q1.displayName = "The Darkin Blade [First]";
			Q1.missileName = "";
			Q1.slot = _Q;
			Q1.type = conic;
			Q1.speed = MathHuge;
			Q1.range = 650;
			Q1.delay = 0.5;
			Q1.radius = 150;
			Q1.danger = 3;
			Q1.cc = true;
			Q1.collision = false;
			Q1.windwall = false;
			Q1.hitbox = false;
			Q1.fow = false;
			Q1.exception = false;
			Q1.extend = false;
			Aatrox.spells.emplace_back(Q1);*/

			Spell W;
			W.name = "AatroxW";
			W.icon = "AatroxW";
			W.displayName = "Infernal Chains";
			W.missileName = "AatroxW";
			W.slot = _W;
			W.type = linear;
			W.speed = 1800;
			W.range = 825;
			W.delay = 0.25;
			W.radius = 80;
			W.angle = 0;
			W.danger = 2;
			W.cc = true;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Aatrox.spells.emplace_back(W);

			SpellDB.emplace_back(Aatrox);
		}

		{
			Champ Ahri;
			Ahri.hero = "Ahri";

			Spell Q;
			Q.name = "AhriOrbofDeception";
			Q.icon = "AhriQ";
			Q.displayName = "Orb of Deception";
			Q.missileName = "AhriOrbMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2600;
			Q.range = 900;
			Q.delay = 0.25;
			Q.radius = 100;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Ahri.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "AhriOrbReturn";
			Q2.icon = "AhriQ";
			Q2.displayName = "Orb of Deception";
			Q2.missileName = "AhriOrbReturn";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2500;
			Q2.followEnemy = true;
			Q2.range = 1000;
			Q2.delay = 0.25;
			Q2.radius = 100;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Ahri.spells.emplace_back(Q2);

			Spell E;
			E.name = "AhriSeduce";
			E.icon = "AhriE";
			E.displayName = "Seduce";
			E.missileName = "AhriSeduceMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1550;
			E.range = 1000;
			E.delay = 0.25;
			E.radius = 60;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Ahri.spells.emplace_back(E);

			SpellDB.emplace_back(Ahri);
		}

		{
			Champ Akali;
			Akali.hero = "Akali";

			Spell Q;
			Q.name = "AkaliQ";
			Q.icon = "AkaliQ";
			Q.displayName = "Five Point Strike";
			Q.missileName = "";
			Q.slot = _Q;
			Q.type = conic;
			Q.speed = 3200;
			Q.range = 550;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.angle = 45;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Akali.spells.emplace_back(Q);

			Spell E;
			E.name = "AkaliE";
			E.icon = "AkaliE";
			E.displayName = "Shuriken Flip";
			E.missileName = "AkaliEMis";
			E.slot = _E;
			E.type = linear;
			E.speed = 1800;
			E.range = 825;
			E.delay = 0.25;
			E.radius = 80;
			E.danger = 2;
			E.cc = false;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Akali.spells.emplace_back(E);

			Spell R;
			R.name = "AkaliR";
			R.icon = "AkaliR";
			R.displayName = "Perfect Execution [First]";
			R.slot = _R;
			R.type = linear;
			R.speed = 1800;
			R.range = 600;
			R.delay = 0;
			R.radius = 140;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			Akali.spells.emplace_back(R);

			Spell R2;
			R2.name = "AkaliRb";
			R2.icon = "AkaliR2";
			R2.displayName = "Perfect Execution [Second]";
			R2.missileName = "";
			R2.slot = _R;
			R2.type = linear;
			R2.speed = 3600;
			R2.range = 750;
			R2.delay = 0;
			R2.radius = 140;
			R2.angle = 0;
			R2.danger = 4;
			R2.cc = false;
			R2.collision = false;
			R2.windwall = false;
			R2.hitbox = false;
			R2.fow = false;
			R2.exception = false;
			R2.extend = true;
			Akali.spells.emplace_back(R2);

			SpellDB.emplace_back(Akali);
		}

		{
			Champ Amumu;
			Amumu.hero = "Amumu";

			Spell Q;
			Q.name = "BandageToss";
			Q.icon = "AmumuQ";
			Q.displayName = "Amumu Q";
			Q.missileName = "SadMummyBandageToss";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2000;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 80;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Amumu.spells.emplace_back(Q);

			SpellDB.emplace_back(Amumu);
		}

		{
			Champ Anivia;
			Anivia.hero = "Anivia";

			Spell Q;
			Q.name = "FlashFrost";
			Q.icon = "AniviaQ";
			Q.displayName = "Anivia Q";
			Q.missileName = "FlashFrostSpell";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 850;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 140;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Anivia.spells.emplace_back(Q);

			SpellDB.emplace_back(Anivia);
		}

		{
			Champ Annie;
			Annie.hero = "Annie";

			Spell W;
			W.name = "AnnieW";
			W.icon = "AnnieW";
			W.displayName = "Incinerate";
			W.slot = _Q;
			W.type = conic;
			W.speed = MathHuge;
			W.range = 590;
			W.delay = 0.25;
			W.radius = 250;
			W.danger = 2;
			W.angle = 50;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = true;
			Annie.spells.emplace_back(W);

			Spell R;
			R.name = "AnnieR";
			R.icon = "AnnieR";
			R.displayName = "Summon: Tibbers";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 600;
			R.delay = 0.25;
			R.radius = 250;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;
			Annie.spells.emplace_back(R);

			SpellDB.emplace_back(Annie);
		}

		{
			Champ Alistar;
			Alistar.hero = "Alistar";

			Spell Q;
			Q.name = "Pulverize";
			Q.icon = "AlistarQ";
			Q.displayName = "Pulverize";
			Q.missileName = "";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 0;
			Q.delay = 0.50;
			Q.radius = 275;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;
			Alistar.spells.emplace_back(Q);

			SpellDB.emplace_back(Alistar);
		}

		{
			Champ Ezreal;
			Ezreal.hero = "Ezreal";

			Spell Q;
			Q.name = "EzrealQ";
			Q.icon = "EzrealQ";
			Q.displayName = "Mystic Shot";
			Q.missileName = "EzrealQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2000;
			Q.range = 1200;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.useMissile = true;
			Q.extend = true;

			Spell W;
			W.name = "EzrealW";
			W.icon = "EzrealW";
			W.displayName = "Essence Flux";
			W.missileName = "EzrealW";
			W.slot = _W;
			W.type = linear;
			W.speed = 1700;
			W.range = 1200;
			W.delay = 0.25;
			W.radius = 80;
			W.danger = 2;
			W.cc = false;
			W.collision = false;
			W.collisionWC = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			W.useMissile = true;

			Spell R;
			R.name = "EzrealR";
			R.icon = "EzrealR";
			R.displayName = "Trueshot Barrage";
			R.missileName = "EzrealR";
			R.slot = _R;
			R.type = linear;
			R.speed = 2000;
			R.range = 20000;
			R.delay = 1;
			R.radius = 160;
			R.danger = 3;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			R.useMissile = true;

			Ezreal.spells.emplace_back(Q);
			Ezreal.spells.emplace_back(W);
			Ezreal.spells.emplace_back(R);

			SpellDB.emplace_back(Ezreal);
		}

		{
			Champ Ashe;
			Ashe.hero = "Ashe";
			Spell R;
			R.name = "EnchantedCrystalArrow";
			R.icon = "AsheR";
			R.displayName = "Enchanted Crystal Arrow";
			R.missileName = "EnchantedCrystalArrow";
			R.slot = _R;
			R.type = linear;
			R.speed = 1600;
			R.range = 25000;
			R.delay = 0.25;
			R.radius = 130;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.collisionWC = true;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			R.useMissile = true;
			Ashe.spells.emplace_back(R);

			/*Spell W;
			W.name = "Volley";
			W.icon = "AsheW";
			W.displayName = "Volley";
			W.missileName = "VolleyRightAttack";
			W.slot = _W;
			W.type = conic;
			W.speed = 1500;
			W.range = 1200;
			W.delay = 0.25;
			W.radius = 20;
			W.angle = 43;
			W.danger = 1;
			W.cc = true;
			W.collision = true;
			W.windwall = true;
			W.hitbox = false;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Ashe.spells.emplace_back(W);*/

			SpellDB.emplace_back(Ashe);
		}

		{
			Champ Blitzcrank;
			Blitzcrank.hero = "Blitzcrank";
			Spell Q;
			Q.name = "RocketGrab";
			Q.icon = "BlitzcrankQ";
			Q.displayName = "Rocket Grab";
			Q.missileName = "RocketGrabMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1800;
			Q.range = 1150;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.danger = 4;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Blitzcrank.spells.emplace_back(Q);

			Spell R;
			R.name = "StaticField";
			R.icon = "BlitzcrankR";
			R.displayName = "Static Field";
			R.missileName = "";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 600;
			R.delay = 0.25;
			R.radius = 600;
			R.danger = 3;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;
			Blitzcrank.spells.emplace_back(R);

			SpellDB.emplace_back(Blitzcrank);
		}

		{
			Champ Brand;
			Brand.hero = "Brand";
			Spell Q;
			Q.name = "BrandQ";
			Q.icon = "BrandQ";
			Q.displayName = "Sear";
			Q.missileName = "BrandQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1600;
			Q.range = 1050;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.angle = 0;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Brand.spells.emplace_back(Q);

			Spell W;
			W.name = "BrandW";
			W.icon = "BrandW";
			W.displayName = "Pillar of Flame";
			W.missileName = "BrandWMissile";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 900;
			W.delay = 0.85f;
			W.radius = 250;
			W.angle = 0;
			W.danger = 2;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Brand.spells.emplace_back(W);

			SpellDB.emplace_back(Brand);
		}

		{
			Champ Bard;
			Bard.hero = "Bard";
			Spell Q;
			Q.name = "BardQ";
			Q.icon = "BardQ";
			Q.displayName = "Bard Q";
			Q.missileName = "BardQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1600;
			Q.range = 950;
			Q.delay = 0.25;
			Q.radius = 65;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Bard.spells.emplace_back(Q);

			Spell R;
			R.name = "BardR";
			R.icon = "BardR";
			R.displayName = "Bard R";
			R.missileName = "BardR";
			R.slot = _R;
			R.type = circular;
			R.speed = 2100;
			R.range = 3400;
			R.delay = 0.5;
			R.radius = 350;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			Bard.spells.emplace_back(R);

			SpellDB.emplace_back(Bard);
		}

		{
			Champ Braum;
			Braum.hero = "Braum";
			Spell Q;
			Q.name = "BraumQ";
			Q.icon = "BraumQ";
			Q.displayName = "BraumQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1700;
			Q.range = 1050;
			Q.delay = 0.25;
			Q.radius = 65;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Braum.spells.emplace_back(Q);

			Spell R;
			R.name = "BraumRWrapper";
			R.icon = "BraumRWrapper";
			R.displayName = "BraumRWrapper";
			R.slot = _R;
			R.type = linear;
			R.speed = 1400;
			R.range = 1200;
			R.delay = 0.5;
			R.radius = 120;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Braum.spells.emplace_back(R);

			SpellDB.emplace_back(Braum);
		}

		{
			Champ Soraka;
			Soraka.hero = "Soraka";

			Spell Q;
			Q.name = "SorakaQ";
			Q.icon = "SorakaQ";
			Q.displayName = "Starcall";
			Q.missileName = "SorakaQMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1100;
			Q.range = 950;
			Q.delay = 0.25;
			Q.radius = 240;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Soraka.spells.emplace_back(Q);

			Spell E;
			E.name = "SorakaE";
			E.icon = "SorakaE";
			E.displayName = "Soraka E";
			E.slot = _E;
			E.type = circular;
			E.speed = MathHuge;
			E.range = 1300;
			E.delay = 1.6f;
			E.radius = 260;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = false;
			Soraka.spells.emplace_back(E);

			SpellDB.emplace_back(Soraka);
		}

		{
			Champ Illaoi;
			Illaoi.hero = "Illaoi";

			Spell Q;
			Q.name = "IllaoiQ";
			Q.icon = "IllaoiQ";
			Q.displayName = "Illaoi Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 825;
			Q.delay = 0.75;
			Q.radius = 105;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Illaoi.spells.emplace_back(Q);

			Spell E;
			E.name = "IllaoiE";
			E.icon = "IllaoiE";
			E.displayName = "Illaoi E";
			E.slot = _E;
			E.type = linear;
			E.speed = 1900;
			E.range = 950;
			E.delay = 0.25f;
			E.radius = 50;
			E.danger = 2;
			E.cc = false;
			E.collision = true;
			E.windwall = false;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Illaoi.spells.emplace_back(E);

			SpellDB.emplace_back(Illaoi);
		}

		{
			Champ Caitlyn;
			Caitlyn.hero = "Caitlyn";

			Spell Q;
			Q.name = "CaitlynQ";
			Q.icon = "CaitlynQ";
			Q.displayName = "Caitlyn Q";
			//Q.missileName = "CaitlynPiltoverPeacemaker";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2200;
			Q.range = 1300;
			Q.delay = 0.65f;
			Q.radius = 70;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			//E.useMissile = true;
			Caitlyn.spells.emplace_back(Q);

			Spell E;
			E.name = "CaitlynE";
			E.icon = "CaitlynE";
			E.displayName = "Entrapment";
			E.missileName = "CaitlynEntrapmentMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1600;
			E.range = 900;
			E.delay = 0.25f;
			E.radius = 90;
			E.danger = 1;
			E.cc = true;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Caitlyn.spells.emplace_back(E);

			Spell W;
			W.name = "CaitlynW";
			W.icon = "CaitlynW";
			W.displayName = "Yordle Trap";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 800;
			W.radius = 80;
			W.danger = 1;
			W.cc = false;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = false;
			Caitlyn.spells.emplace_back(W);

			SpellDB.emplace_back(Caitlyn);
		}

		{
			Champ Karthus;
			Karthus.hero = "Karthus";

			Spell Q1;
			Q1.name = "KarthusLayWasteA1";
			Q1.icon = "KarthusQ";
			Q1.displayName = "Lay Waste [1]";
			Q1.missileName = "";
			Q1.slot = _Q;
			Q1.type = circular;
			Q1.speed = MathHuge;
			Q1.range = 875;
			Q1.delay = 0.9f;
			Q1.radius = 175;
			Q1.angle = 0;
			Q1.danger = 1;
			Q1.cc = false;
			Q1.collision = false;
			Q1.windwall = false;
			Q1.hitbox = false;
			Q1.fow = false;
			Q1.exception = false;
			Q1.extend = false;

			Spell Q2;
			Q2.name = "KarthusLayWasteA2";
			Q2.icon = "KarthusQ";
			Q2.displayName = "Lay Waste [2]";
			Q2.missileName = "";
			Q2.slot = _Q;
			Q2.type = circular;
			Q2.speed = MathHuge;
			Q2.range = 875;
			Q2.delay = 0.9f;
			Q2.radius = 175;
			Q2.angle = 0;
			Q2.danger = 1;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = false;
			Q2.hitbox = false;
			Q2.fow = false;
			Q2.exception = false;
			Q2.extend = false;

			Spell Q3;
			Q3.name = "KarthusLayWasteA3";
			Q3.icon = "KarthusQ";
			Q3.displayName = "Lay Waste [3]";
			Q3.missileName = "";
			Q3.slot = _Q;
			Q3.type = circular;
			Q3.speed = MathHuge;
			Q3.range = 875;
			Q3.delay = 0.9f;
			Q3.radius = 175;
			Q3.angle = 0;
			Q3.danger = 1;
			Q3.cc = false;
			Q3.collision = false;
			Q3.windwall = false;
			Q3.hitbox = false;
			Q3.fow = false;
			Q3.exception = false;
			Q3.extend = false;
			Karthus.spells.emplace_back(Q1);
			Karthus.spells.emplace_back(Q2);
			Karthus.spells.emplace_back(Q3);

			SpellDB.emplace_back(Karthus);
		}

		{
			Champ Cassiopeia;
			Cassiopeia.hero = "Cassiopeia";

			Spell Q;
			Q.name = "CassiopeiaQ";
			Q.icon = "CassiopeiaQ";
			Q.displayName = "Noxious Blast";
			Q.missileName = "";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 850;
			Q.delay = 0.75;
			Q.radius = 150;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;
			Cassiopeia.spells.emplace_back(Q);

			Spell W;
			W.name = "CassiopeiaW";
			W.icon = "CassiopeiaW";
			W.displayName = "Miasma";
			W.missileName = "";
			W.slot = _W;
			W.type = circular;
			W.speed = 2500;
			W.range = 800;
			W.delay = 0.75;
			W.radius = 160;
			W.danger = 2;
			W.cc = true;
			W.collision = false;
			W.windwall = true;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Cassiopeia.spells.emplace_back(W);

			Spell R;
			/*R.name = "CassiopeiaR";
			R.icon = "CassiopeiaR";
			R.displayName = "Petrifying Gaze";
			R.missileName = "";
			R.slot = _R;
			R.type = conic;
			R.speed = MathHuge;
			R.range = 825;
			R.delay = 0.5;
			R.radius = 790;
			R.angle = 80;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			R.FromEnemy = true;
			Cassiopeia.spells.emplace_back(R);*/

			R.name = "CassiopeiaR";
			R.icon = "CassiopeiaR";
			R.displayName = "Petrifying Gaze";
			R.missileName = "";
			R.slot = _R;
			R.type = linear;
			R.speed = MathHuge;
			R.range = 825;
			R.delay = 0.5;
			R.radius = 790;
			R.angle = 80;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			R.FromEnemy = true;
			Cassiopeia.spells.emplace_back(R);

			SpellDB.emplace_back(Cassiopeia);
		}

		{
			Champ Yorick;
			Yorick.hero = "Yorick";

			Spell E;

			E.name = "YorickE";
			E.icon = "YorickE";
			E.displayName = "Yorick";
			E.missileName = "YorickEMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1800;
			E.range = 1200;
			E.delay = 0.10;
			E.radius = 100;
			E.angle = 0;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			//E.useMissile = true;
			Yorick.spells.emplace_back(E);


			SpellDB.emplace_back(Yorick);
		}

		{
			Champ Diana;
			Diana.hero = "Diana";

			Spell Q;
			Q.name = "DianaQ";
			Q.icon = "DianaQ";
			Q.displayName = "Crescent Strike";
			Q.missileName = "DianaQ";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1400;
			Q.range = 800;
			Q.delay = 0.25;
			Q.radius = 195;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Diana.spells.emplace_back(Q);

			/*Spell Q2;
			Q2.name = "DianaQ";
			Q2.icon = "DianaQ";
			Q2.displayName = "Diana Q Line";
			Q2.slot = _Q;
			Q2.type = arc;
			Q2.speed = 1400;
			Q2.range = 1300;
			Q2.delay = 0.25;
			Q2.radius = 195;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = false;
			Diana.spells.emplace_back(Q2);*/

			SpellDB.emplace_back(Diana);
		}

		{
			Champ Fizz;
			Fizz.hero = "Fizz";
			Spell R;
			R.name = "FizzR";
			R.icon = "FizzR";
			R.displayName = "Chum the Waters";
			R.missileName = "FizzRMissile";
			R.slot = _R;
			R.type = linear;
			R.speed = 1300;
			R.range = 1300;
			R.delay = 0.25;
			R.radius = 150;
			R.angle = 0;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Fizz.spells.emplace_back(R);

			Spell R2;
			R2.name = "FizzR";
			R2.icon = "FizzR";
			R2.displayName = "Chum the Waters";
			R2.missileName = "FizzRMissile";
			R2.slot = _R;
			R2.type = circular;
			R2.speed = MathHuge;
			R2.range = 1300;
			R2.delay = 0.95f;
			R2.radius = 250;
			R2.danger = 5;
			R2.cc = true;
			R2.collision = false;
			R2.windwall = true;
			R2.hitbox = true;
			R2.fow = true;
			R2.exception = false;
			R2.extend = false;
			Fizz.spells.emplace_back(R2);

			SpellDB.emplace_back(Fizz);
		}

		{
			Champ Jhin;
			Jhin.hero = "Jhin";
			Spell W;
			W.name = "JhinW";
			W.icon = "JhinW";
			W.displayName = "Deadly Flourish";
			W.missileName = "JhinAnchorDragMissile";
			W.slot = _W;
			W.type = linear;
			W.speed = MathHuge;
			W.range = 2500;
			W.delay = 0.25;
			W.radius = 80;
			W.danger = 1;
			W.cc = true;
			W.collision = false;
			W.windwall = true;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = true;
			Jhin.spells.emplace_back(W);

			Spell R;
			R.name = "JhinRShot";
			R.icon = "JhinR";
			R.displayName = "Curtain Call";
			R.missileName = "JhinRShotMis";
			R.slot = _R;
			R.type = linear;
			R.speed = 5000;
			R.range = 3500;
			R.delay = 0.25;
			R.radius = 80;
			R.angle = 0;
			R.danger = 2;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			//R.useMissile = true;
			Jhin.spells.emplace_back(R);

			Spell R2;
			R2.name = "JhinRShot";
			R2.icon = "JhinR";
			R2.displayName = "Curtain Call";
			R2.missileName = "JhinRShotMis4";
			R2.slot = _R;
			R2.type = linear;
			R2.speed = 5000;
			R2.range = 3500;
			R2.delay = 0.25;
			R2.radius = 80;
			R2.angle = 0;
			R2.danger = 2;
			R2.cc = true;
			R2.collision = false;
			R2.windwall = true;
			R2.hitbox = true;
			R2.fow = true;
			R2.exception = false;
			R2.extend = true;
			//R2.useMissile = true;
			Jhin.spells.emplace_back(R2);

			SpellDB.emplace_back(Jhin);
		}

		{
			Champ Nami;
			Nami.hero = "Nami";
			Spell Q;
			Q.name = "NamiQ";
			Q.icon = "NamiQ";
			Q.displayName = "Nami Q";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 1650;
			Q.delay = 0.95f;
			Q.radius = 190;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;
			Nami.spells.emplace_back(Q);

			Spell R;
			R.name = "NamiR";
			R.icon = "NamiR";
			R.displayName = "Nami R";
			R.slot = _R;
			R.type = linear;
			R.speed = 850;
			R.range = 2750;
			R.delay = 0.5f;
			R.radius = 260;
			R.danger = 3;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			Nami.spells.emplace_back(R);

			SpellDB.emplace_back(Nami);
		}

		{
			Champ Kaisa;
			Kaisa.hero = "Kaisa";
			Spell W;
			W.name = "KaisaW";
			W.icon = "KaisaW";
			W.displayName = "Void Seeker";
			W.missileName = "KaisaW";
			W.slot = _W;
			W.type = linear;
			W.speed = 1750;
			W.range = 3000;
			W.delay = 0.4f;
			W.radius = 100;
			W.angle = 0;
			W.danger = 1;
			W.cc = false;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Kaisa.spells.emplace_back(W);

			SpellDB.emplace_back(Kaisa);
		}

		{
			Champ Khazix;
			Khazix.hero = "Khazix";

			Spell W1;
			W1.name = "KhazixW";
			W1.icon = "KhazixW1";
			W1.displayName = "Void Spike [Standard]";
			W1.missileName = "KhazixWMissile";
			W1.slot = _W;
			W1.type = linear;
			W1.speed = 1700;
			W1.range = 1025;
			W1.delay = 0.25;
			W1.radius = 70;
			W1.danger = 2;
			W1.cc = false;
			W1.collision = false;
			W1.windwall = true;
			W1.hitbox = true;
			W1.fow = true;
			W1.exception = false;
			W1.extend = true;
			Khazix.spells.emplace_back(W1);

			Spell W2;
			W2.name = "KhazixWLong";
			W2.icon = "KhazixW2";
			W2.displayName = "Void Spike [Evolved]";
			W2.missileName = "KhazixWMissile";
			W2.slot = _W;
			W2.type = threeway;
			W2.speed = 1700;
			W2.range = 1025;
			W2.delay = 0.25;
			W2.radius = 70;
			W2.angle = 23;
			W2.danger = 2;
			W2.cc = true;
			W2.collision = true;
			W2.windwall = true;
			W2.hitbox = true;
			W2.fow = false;
			W2.exception = false;
			W2.extend = true;
			Khazix.spells.emplace_back(W2);

			SpellDB.emplace_back(Khazix);
		}

		{
			Champ Lux;
			Lux.hero = "Lux";
			Spell Q;
			Q.name = "LuxLightBinding";
			Q.icon = "LuxQ";
			Q.displayName = "Light Binding";
			Q.missileName = "LuxLightBindingDummy";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1200;
			Q.range = 1300;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Lux.spells.emplace_back(Q);

			Spell E;
			E.name = "LuxLightStrikeKugel";
			E.icon = "LuxE";
			E.displayName = "Light Strike Kugel";
			E.missileName = "LuxLightStrikeKugel";
			E.slot = _E;
			E.type = circular;
			E.speed = 1200;
			E.range = 1100;
			E.delay = 0.25;
			E.radius = 295;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;
			Lux.spells.emplace_back(E);

			Spell E2;
			E2.name = "LuxE";
			E2.icon = "LuxE";
			E2.displayName = "Light Strike Kugel [After]";
			E2.missileName = "LuxLightStrikeKugel";
			E2.slot = _E;
			E2.type = circular;
			E2.speed = 1200;
			E2.range = 1100;
			E2.delay = 0.25;
			E2.radius = 295;
			E2.danger = 2;
			E2.cc = true;
			E2.collision = false;
			E2.windwall = true;
			E2.hitbox = false;
			E2.fow = true;
			E2.exception = false;
			E2.extend = false;
			Lux.spells.emplace_back(E2);

			Spell R;
			R.name = "LuxR";
			R.icon = "LuxR";
			R.displayName = "Malice Cannon";
			R.missileName = "LuxRMis";
			R.slot = _R;
			R.type = linear;
			R.speed = MathHuge;
			R.range = 3340;
			R.delay = 1;
			R.radius = 190;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Lux.spells.emplace_back(R);

			SpellDB.emplace_back(Lux);
		}

		{
			Champ Morgana;
			Morgana.hero = "Morgana";
			Spell Q;
			Q.name = "MorganaQ";
			Q.icon = "MorganaQ";
			Q.displayName = "Dark Binding";
			Q.missileName = "MorganaQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1200;
			Q.range = 1300;
			Q.delay = 0.25;
			Q.radius = 80;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Morgana.spells.emplace_back(Q);

			SpellDB.emplace_back(Morgana);
		}

		{
			Champ Senna;
			Senna.hero = "Senna";

			Spell Q;
			Q.name = "SennaQ";
			Q.icon = "SennaQ";
			Q.displayName = "Senna Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 1300;
			Q.delay = 0.245f;
			Q.radius = 70;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Senna.spells.emplace_back(Q);

			Spell W;
			W.name = "SennaW";
			W.icon = "SennaW";
			W.displayName = "Senna W";
			W.missileName = "SennaW";
			W.slot = _W;
			W.type = linear;
			W.speed = 1200;
			W.range = 1250;
			W.delay = 0.25;
			W.radius = 80;
			W.danger = 2;
			W.cc = true;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Senna.spells.emplace_back(W);

			Spell WC;
			WC.name = "SennaW";
			WC.icon = "SennaW";
			WC.displayName = "Senna W Circle";
			WC.slot = _W;
			WC.type = circular;
			WC.speed = 1200;
			WC.range = 1350;
			WC.delay = 0.25;
			WC.radius = 240;
			WC.danger = 2;
			WC.cc = true;
			WC.collision = true;
			WC.windwall = true;
			WC.hitbox = true;
			WC.fow = true;
			WC.exception = false;
			WC.extend = false;
			Senna.spells.emplace_back(WC);

			Spell R;
			R.name = "SennaR";
			R.icon = "SennaR";
			R.displayName = "Senna R";
			R.missileName = "SennaRWarningMis";
			R.slot = _R;
			R.type = linear;
			R.speed = MathHuge;
			R.range = 18000;
			R.delay = 1.25;
			R.radius = 180;
			R.danger = 5;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Senna.spells.emplace_back(R);

			SpellDB.emplace_back(Senna);
		}

		{
			Champ Karma;
			Karma.hero = "Karma";

			Spell Q;
			Q.name = "KarmaQ";
			Q.icon = "KarmaQ";
			Q.displayName = "Karma Q";
			Q.missileName = "KarmaQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1700;
			Q.range = 950;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Karma.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "KarmaMantra";
			Q2.icon = "KarmaQ";
			Q2.displayName = "Karma Q Mantra";
			Q2.missileName = "KarmaQMissileMantra";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 1700;
			Q2.range = 950;
			Q2.delay = 0.25;
			Q2.radius = 80;
			Q2.danger = 2;
			Q2.cc = true;
			Q2.collision = true;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Karma.spells.emplace_back(Q2);

			SpellDB.emplace_back(Karma);
		}

		{
			Champ Jayce;
			Jayce.hero = "Jayce";

			Spell Q;
			Q.name = "JayceShockBlast";
			Q.icon = "JayceQ";
			Q.displayName = "Shock Blast [Standard]";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1450;
			Q.range = 1050;
			Q.delay = 0.21f;
			Q.radius = 70;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Jayce.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "JayceShockBlastWall";
			Q2.icon = "JayceQAccel";
			Q2.displayName = "Shock Blast [Accelerated]";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2350;
			Q2.range = 1600;
			Q2.delay = 0.21f;
			Q2.radius = 115;
			Q2.danger = 1;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Jayce.spells.emplace_back(Q2);

			SpellDB.emplace_back(Jayce);
		}

		{
			Champ Mordekaiser;
			Mordekaiser.hero = "Mordekaiser";

			Spell Q;
			Q.name = "MordekaiserQ";
			Q.icon = "MordekaiserQ";
			Q.displayName = "Mordekaiser Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 700;
			Q.delay = 0.50;
			Q.radius = 150;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Mordekaiser.spells.emplace_back(Q);

			Spell E;
			E.name = "MordekaiserE";
			E.icon = "MordekaiserE";
			E.displayName = "Mordekaiser E";
			E.slot = _E;
			E.type = linear;
			E.speed = MathHuge;
			E.range = 1000;
			E.delay = 0.70f;
			E.radius = 150;
			E.danger = 1;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Mordekaiser.spells.emplace_back(E);

			SpellDB.emplace_back(Mordekaiser);
		}

		{
			Champ Darius;
			Darius.hero = "Darius";

			Spell Q;
			Q.name = "DariusCleave";
			Q.icon = "DariusQ";
			Q.displayName = "Darius Q";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 420;
			Q.delay = 0.75;
			Q.radius = 425;
			Q.danger = 3;
			Q.followEnemy = true;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Darius.spells.emplace_back(Q);

			Spell E;
			E.name = "DariusAxeGrabCone";
			E.icon = "DariusE";
			E.displayName = "Darius E";
			E.slot = _E;
			E.type = conic;
			E.speed = MathHuge;
			E.range = 535;
			E.delay = 0.25;
			E.radius = 230;
			E.angle = 50;
			E.danger = 1;
			E.cc = true;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Darius.spells.emplace_back(E);

			SpellDB.emplace_back(Darius);
		}

		{
			Champ Nautilus;
			Nautilus.hero = "Nautilus";
			Spell Q;
			Q.name = "NautilusAnchorDrag";
			Q.icon = "NautilusQ";
			Q.displayName = "Dredge Line";
			Q.missileName = "NautilusAnchorDragMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2000;
			Q.range = 1200;
			Q.delay = 0.25;
			Q.radius = 90;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Nautilus.spells.emplace_back(Q);

			SpellDB.emplace_back(Nautilus);
		}

		{
			Champ Leblanc;
			Leblanc.hero = "Leblanc";

			Spell E;
			E.name = "LeblancE";
			E.icon = "LeblancE";
			E.displayName = "Ethereal Chains [Standard]";
			E.missileName = "LeblancEMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1750;
			E.range = 950;
			E.delay = 0.25;
			E.radius = 55;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Leblanc.spells.emplace_back(E);

			Spell ER;
			ER.name = "LeblancRE";
			ER.icon = "LeblancRE";
			ER.displayName = "Ethereal Chains [Ultimate]";
			ER.missileName = "LeblancREMissile";
			ER.slot = _E;
			ER.type = linear;
			ER.speed = 1750;
			ER.range = 950;
			ER.delay = 0.25;
			ER.radius = 55;
			ER.danger = 1;
			ER.cc = true;
			ER.collision = false;
			ER.windwall = true;
			ER.hitbox = true;
			ER.fow = true;
			ER.exception = false;
			ER.extend = true;
			Leblanc.spells.emplace_back(ER);

			SpellDB.emplace_back(Leblanc);
		}

		{
			Champ Neeko;
			Neeko.hero = "Neeko";

			Spell Q;
			Q.name = "NeekoQ";
			Q.icon = "NeekoQ";
			Q.displayName = "NeekoQ";
			Q.missileName = "Blooming Burst";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1800;
			Q.range = 800;
			Q.delay = 0.25;
			Q.radius = 225;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Neeko.spells.emplace_back(Q);

			Spell E;
			E.name = "NeekoE";
			E.icon = "NeekoE";
			E.displayName = "NeekoE";
			E.missileName = "Tangle-Barbs";
			E.slot = _E;
			E.type = linear;
			E.speed = 1400;
			E.range = 1050;
			E.delay = 0.25;
			E.radius = 85;
			E.angle = 0;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Neeko.spells.emplace_back(E);

			SpellDB.emplace_back(Neeko);
		}

		{
			Champ Nidalee;
			Nidalee.hero = "Nidalee";
			Spell Q;
			Q.name = "JavelinToss";
			Q.icon = "NidaleeQ";
			Q.displayName = "Javelin Toss";
			Q.missileName = "JavelinToss";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1300;
			Q.range = 1500;
			Q.delay = 0.25;
			Q.radius = 40;
			Q.danger = 3;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Nidalee.spells.emplace_back(Q);

			SpellDB.emplace_back(Nidalee);
		}

		{
			Champ Jinx;
			Jinx.hero = "Jinx";

			Spell W;
			W.name = "JinxW";
			W.icon = "JinxW";
			W.displayName = "Jinx W";
			W.missileName = "JinxWMissile";
			W.slot = _W;
			W.type = linear;
			W.speed = 3300;
			W.range = 1500;
			W.delay = 0.35f;
			W.radius = 75;
			W.danger = 2;
			W.cc = true;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Jinx.spells.emplace_back(W);

			Spell E;
			E.name = "JinxE";
			E.icon = "JinxE";
			E.displayName = "Jinx E";
			E.missileName = "JinxEHit";
			E.slot = _E;
			E.type = zone;
			E.speed = MathHuge;
			E.range = 900;
			E.delay = 0.25;
			E.radius = 130;
			E.radiusEx = 130 / 3;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;
			Jinx.spells.emplace_back(E);

			Spell R;
			R.name = "JinxR";
			R.icon = "JinxR";
			R.displayName = "Jinx R";
			R.missileName = "JinxR";
			R.slot = _R;
			R.type = linear;
			R.speed = 1700;
			R.range = 25000;
			R.delay = 0.60f;
			R.radius = 140;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.collisionWC = true;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			R.useMissile = true;
			Jinx.spells.emplace_back(R);

			SpellDB.emplace_back(Jinx);
		}

		{
			Champ LeeSin;
			LeeSin.hero = "LeeSin";
			Spell Q;
			Q.name = "BlindMonkQOne";
			Q.icon = "LeesinQ";
			Q.displayName = "Sonic Wave";
			Q.missileName = "BlindMonkQOne";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1800;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 65;
			Q.danger = 3;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			LeeSin.spells.emplace_back(Q);

			SpellDB.emplace_back(LeeSin);
		}

		{
			Champ Quinn;
			Quinn.hero = "Quinn";
			Spell Q;
			Q.name = "QuinnQ";
			Q.icon = "QuinnQ";
			Q.displayName = "Quinn Q";
			Q.missileName = "QuinnQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1550;
			Q.range = 1050;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 3;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Quinn.spells.emplace_back(Q);

			SpellDB.emplace_back(Quinn);
		}

		{
			Champ Galio;
			Galio.hero = "Galio";
			Spell E;
			E.name = "GalioE";
			E.icon = "GalioE";
			E.displayName = "Galio E";
			E.slot = _E;
			E.type = linear;
			E.speed = 2300;
			E.range = 650;
			E.delay = 0.50;
			E.radius = 160;
			E.danger = 2;
			E.cc = false;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Galio.spells.emplace_back(E);

			SpellDB.emplace_back(Galio);
		}

		{
			Champ Gnar;
			Gnar.hero = "Gnar";

			Spell Q;
			Q.name = "GnarQ";
			Q.icon = "GnarQ";
			Q.displayName = "Gnar Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2500;
			Q.range = 1125;
			Q.delay = 0.25;
			Q.radius = 55;
			Q.danger = 3;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Gnar.spells.emplace_back(Q);

			Spell QM;
			QM.name = "GnarQMissile";
			QM.icon = "GnarQ";
			QM.displayName = "Gnar Q Missile";
			QM.slot = _Q;
			QM.type = linear;
			QM.speed = 2500;
			QM.range = 1125;
			QM.delay = 0.25;
			QM.radius = 55;
			QM.danger = 3;
			QM.cc = false;
			QM.collision = false;
			QM.windwall = true;
			QM.hitbox = true;
			QM.fow = true;
			QM.exception = false;
			QM.extend = true;
			Gnar.spells.emplace_back(Q);

			Spell Q3;
			Q3.name = "GnarBigQ";
			Q3.icon = "GnarBigQ";
			Q3.displayName = "Gnar Q Big";
			Q3.slot = _Q;
			Q3.type = linear;
			Q3.speed = 2100;
			Q3.range = 1125;
			Q3.delay = 0.50;
			Q3.radius = 90;
			Q3.danger = 2;
			Q3.cc = false;
			Q3.collision = false;
			Q3.windwall = true;
			Q3.hitbox = true;
			Q3.fow = true;
			Q3.exception = false;
			Q3.extend = true;
			Gnar.spells.emplace_back(Q3);

			Spell Q2;
			Q2.name = "GnarQReturn";
			Q2.icon = "GnarQReturn";
			Q2.displayName = "Gnar Q Return";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2500;
			Q2.range = 2995;
			Q2.delay = 0.25;
			Q2.radius = 75;
			Q2.danger = 3;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Gnar.spells.emplace_back(Q2);

			Spell R;
			R.name = "GnarR";
			R.icon = "GnarR";
			R.displayName = "Gnar R";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 0;
			R.delay = 0.25;
			R.radius = 450;
			R.danger = 5;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			Gnar.spells.emplace_back(R);

			Spell W;
			W.name = "GnarBigW";
			W.icon = "GnarBigW";
			W.displayName = "Gnar W Big";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 525;
			W.delay = 0.60f;
			W.radius = 80;
			W.danger = 3;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Gnar.spells.emplace_back(W);

			Spell E;
			E.name = "GnarE";
			E.icon = "GnarE";
			E.displayName = "Gnar E";
			E.slot = _E;
			E.type = circular;
			E.speed = 900;
			E.range = 475;
			E.delay = 0.25;
			E.radius = 150;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = false;
			Gnar.spells.emplace_back(E);

			Spell E2;
			E2.name = "GnarBigE";
			E2.icon = "GnarBigE";
			E2.displayName = "Gnar E Big";
			E2.slot = _E;
			E2.type = circular;
			E2.speed = 1000;
			E2.range = 600;
			E2.delay = 0.25;
			E2.radius = 350;
			E2.danger = 2;
			E2.cc = false;
			E2.collision = false;
			E2.windwall = true;
			E2.hitbox = true;
			E2.fow = true;
			E2.exception = false;
			E2.extend = false;
			Gnar.spells.emplace_back(E2);

			SpellDB.emplace_back(Gnar);
		}

		{
			Champ Lillia;
			Lillia.hero = "Lillia";

			Spell W;
			W.name = "LilliaW";
			W.icon = "LilliaW";
			W.displayName = "Lillia W";
			W.slot = _W;
			W.type = circular;
			W.speed = 1200;
			W.range = 500;
			W.delay = 0.50;
			W.radius = 250;
			W.danger = 3;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = false;
			Lillia.spells.emplace_back(W);

			Spell E;
			E.name = "LilliaE";
			E.icon = "LilliaE";
			E.displayName = "Lillia E";
			E.slot = _E;
			E.type = linear;
			E.speed = 1750;
			E.range = 25000;
			E.delay = 0.25;
			E.radius = 120;
			E.danger = 3;
			E.cc = false;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Lillia.spells.emplace_back(E);

			SpellDB.emplace_back(Lillia);
		}

		{
			Champ Fiora;
			Fiora.hero = "Fiora";
			Spell W;
			W.name = "FioraW";
			W.icon = "FioraW";
			W.displayName = "Fiora W";
			W.missileName = "FioraWMissile";
			W.slot = _W;
			W.type = linear;
			W.speed = 3200;
			W.range = 800;
			W.delay = 0.55f;
			W.radius = 70;
			W.danger = 3;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			W.followEnemy = false;
			Fiora.spells.emplace_back(W);

			SpellDB.emplace_back(Fiora);
		}

		{
			Champ Sona;
			Sona.hero = "Sona";
			Spell R;
			R.name = "SonaR";
			R.icon = "SonaR";
			R.displayName = "Sona R";
			R.missileName = "SonaR";
			R.slot = _R;
			R.type = linear;
			R.speed = 2400;
			R.range = 1000;
			R.delay = 0.25;
			R.radius = 140;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Sona.spells.emplace_back(R);

			SpellDB.emplace_back(Sona);
		}

		{
			Champ Kayle;
			Kayle.hero = "Kayle";
			Spell Q;
			Q.name = "KayleQ";
			Q.icon = "KayleQ";
			Q.displayName = "Kayle Q";
			Q.missileName = "KayleQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2000;
			Q.range = 850;
			Q.delay = 0.50;
			Q.radius = 75;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Kayle.spells.emplace_back(Q);

			Spell R;
			R.name = "KayleR";
			R.icon = "KayleR";
			R.displayName = "Kayle R";
			R.missileName = "KayleR";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 850;
			R.delay = 1.50;
			R.radius = 450;
			R.danger = 2;
			R.followEnemy = true;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Kayle.spells.emplace_back(R);

			SpellDB.emplace_back(Kayle);
		}

		{
			Champ Kalista;
			Kalista.hero = "Kalista";

			Spell Q;
			Q.name = "KalistaMysticShot";
			Q.icon = "KalistaQ";
			Q.displayName = "Kalista Q";
			Q.missileName = "kalistamysticshotmis";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1700;
			Q.range = 1200;
			Q.delay = 0.25;
			Q.radius = 40;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Kalista.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "KalistaMysticShot";
			Q2.icon = "KalistaQ";
			Q2.displayName = "Kalista Q2";
			Q2.missileName = "kalistamysticshotmistrue";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 1700;
			Q2.range = 1200;
			Q2.delay = 0.25;
			Q2.radius = 40;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = true;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = false;
			Q2.exception = false;
			Q2.extend = true;
			Kalista.spells.emplace_back(Q2);

			SpellDB.emplace_back(Kalista);
		}

		{
			Champ Chogath;
			Chogath.hero = "Chogath";

			Spell Q;
			Q.name = "Rupture";
			Q.icon = "ChogathQ";
			Q.displayName = "Rupture";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 950;
			Q.delay = 1.2f;
			Q.radius = 230;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;
			Chogath.spells.emplace_back(Q);

			Spell W;
			W.name = "FeralScream";
			W.icon = "ChogathW";
			W.displayName = "Feral Scream";
			W.slot = _W;
			W.type = conic;
			W.speed = MathHuge;
			W.range = 650;
			W.delay = 0.5;
			W.radius = 0;
			W.angle = 56;
			W.danger = 2;
			W.cc = true;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = true;
			Chogath.spells.emplace_back(W);

			SpellDB.emplace_back(Chogath);
		}

		{
			Champ Rumble;
			Rumble.hero = "Rumble";

			Spell E;
			E.name = "RumbleGrenade";
			E.icon = "RumbleE";
			E.displayName = "Rumble E";
			E.missileName = "RumbleGrenadeMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 2000;
			E.range = 950;
			E.delay = 0.25;
			E.radius = 60;
			E.danger = 2;
			E.cc = true;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Rumble.spells.emplace_back(E);

			Spell R;
			R.name = "RumbleCarpetBomb";
			R.icon = "RumbleR";
			R.displayName = "Rumble R";
			R.missileName = "RumbleCarpetBombMissile";
			R.slot = _R;
			R.type = linear;
			R.speed = 1600;
			R.range = 1200;
			R.delay = 0.58f;
			R.radius = 200;
			R.danger = 2;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			//R.useMissile = true;
			Rumble.spells.emplace_back(R);

			SpellDB.emplace_back(Rumble);
		}

		{
			Champ Ryze;
			Ryze.hero = "Ryze";
			Spell Q;
			Q.name = "RyzeQWrapper";
			Q.icon = "RyzeQ";
			Q.displayName = "Overload";
			Q.missileName = "RocketGrabMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1700;
			Q.range = 1000;
			Q.delay = 0.25;
			Q.radius = 55;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Ryze.spells.emplace_back(Q);

			SpellDB.emplace_back(Ryze);
		}

		{
			Champ Janna;
			Janna.hero = "Janna";

			Spell Q;
			Q.name = "HowlingGale";
			Q.icon = "JannaQ";
			Q.displayName = "Howling Gale";
			Q.missileName = "HowlingGaleSpell";
			/*Q.extraMissileNames.emplace_back("HowlingGaleSpell1");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell2");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell3");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell4");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell5");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell6");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell7");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell8");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell9");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell10");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell11");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell12");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell13");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell14");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell15");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell16");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell17");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell18");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell19");
			Q.extraMissileNames.emplace_back("HowlingGaleSpell20");*/
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 900;
			Q.range = 1700;
			Q.delay = 0.25;
			Q.radius = 120;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Janna.spells.emplace_back(Q);

			SpellDB.emplace_back(Janna);
		}

		{
			Champ Syndra;
			Syndra.hero = "Syndra";
			Spell Q;
			Q.name = "SyndraQ";
			Q.icon = "SyndraQ";
			Q.displayName = "Dark Sphere";
			Q.missileName = "SyndraQSpell";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 800;
			Q.delay = 0.625;
			Q.radius = 200;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;
			Syndra.spells.emplace_back(Q);

			SpellDB.emplace_back(Syndra);
		}

		{
			Champ TahmKench;
			TahmKench.hero = "TahmKench";

			Spell Q;
			Q.name = "TahmKenchQ";
			Q.icon = "TahmKenchQ";
			Q.displayName = "Tongue Lash";
			Q.missileName = "TahmKenchQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2800;
			Q.range = 900;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			TahmKench.spells.emplace_back(Q);
			SpellDB.emplace_back(TahmKench);
		}

		{
			Champ Kennen;
			Kennen.hero = "Kennen";

			Spell Q;
			Q.name = "KennenShurikenHurlMissile1";
			Q.icon = "KennenShurikenHurlMissile1";
			Q.displayName = "Kennen Q";
			Q.missileName = "KennenShurikenHurlMissile1";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1700;
			Q.range = 1050;
			Q.delay = 0.25;
			Q.radius = 50;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Kennen.spells.emplace_back(Q);
			SpellDB.emplace_back(Kennen);
		}

		{
			Champ Taliyah;
			Taliyah.hero = "Taliyah";

			Spell Q;
			Q.name = "TaliyahQ";
			Q.icon = "TaliyahQ";
			Q.displayName = "Threaded Volley";
			Q.missileName = "TaliyahQMis";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 3600;
			Q.range = 1000;
			Q.delay = 0;
			Q.time = 2;
			Q.radius = 100;
			Q.angle = 0;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = true;
			Q.extend = true;
			//Q.followEnemy = true;
			Q.followEnemy2 = true;
			//Q.useMissile = true;
			Taliyah.spells.emplace_back(Q);

			Spell W;
			W.name = "TaliyahWVC";
			W.icon = "TaliyahW";
			W.displayName = "Seismic Shove";
			W.missileName = "";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 900;
			W.delay = 0.85f;
			W.radius = 150;
			W.angle = 0;
			W.danger = 1;
			W.cc = true;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Taliyah.spells.emplace_back(W);

			Spell E;
			E.name = "TaliyahE";
			E.icon = "TaliyahE";
			E.displayName = "Unraveled Earth";
			E.missileName = "";
			E.slot = _E;
			E.type = conic;
			E.speed = 2000;
			E.range = 800;
			E.delay = 0.45f;
			E.radius = 0;
			E.angle = 60;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = true;
			//E.useMissile = false;
			Taliyah.spells.emplace_back(E);

			Spell R;
			R.name = "TaliyahR";
			R.icon = "TaliyahR";
			R.displayName = "Weaver's Wall";
			R.missileName = "TaliyahRMis";
			R.slot = _R;
			R.type = linear;
			R.speed = 1700;
			R.range = 3000;
			R.delay = 1;
			R.radius = 120;
			R.angle = 0;
			R.danger = 1;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			//R.useMissile = false;
			Taliyah.spells.emplace_back(R);

			SpellDB.emplace_back(Taliyah);
		}

		{
			Champ Talon;
			Talon.hero = "Talon";

			Spell W;
			W.name = "TalonW";
			W.icon = "TalonW";
			W.displayName = "Rake";
			W.missileName = "TalonWMissileOne";
			W.slot = _W;
			W.type = conic;
			W.speed = 2350;
			W.range = 900;
			W.delay = 0.25;
			W.radius = 75;
			W.angle = 40;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = false;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Talon.spells.emplace_back(W);
			SpellDB.emplace_back(Talon);
		}

		{
			Champ Thresh;
			Thresh.hero = "Thresh";

			Spell Q;
			Q.name = "ThreshQ";
			Q.icon = "ThreshQ";
			Q.displayName = "Death Sentence";
			Q.missileName = "ThreshQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1900;
			Q.range = 1100;
			Q.delay = 0.35f;
			Q.radius = 70;
			Q.danger = 3;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Thresh.spells.emplace_back(Q);

			Spell E;
			E.name = "ThreshE";
			E.icon = "ThreshE";
			E.displayName = "Flay";
			E.missileName = "ThreshEMissile1";
			E.slot = _E;
			E.type = linear;
			E.speed = 2000;
			E.range = 0;
			E.delay = 0.389f;
			E.radius = 110;
			E.danger = 3;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = true;
			Thresh.spells.emplace_back(E);

			SpellDB.emplace_back(Thresh);
		}

		{
			Champ Leona;
			Leona.hero = "Leona";

			Spell E;
			E.name = "LeonaZenithBlade";
			E.icon = "LeonaE";
			E.displayName = "Zenith Blade";
			E.missileName = "LeonaZenithBladeMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 2000;
			E.range = 905;
			E.delay = 0.25;
			E.radius = 75;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Spell R;
			R.name = "LeonaSolarFlare";
			R.icon = "LeonaR";
			R.displayName = "Solar Flare";
			//R.missileName = "";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 1200;
			R.delay = 0.90f;
			R.radius = 300;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			//R.useMissile = false;

			Leona.spells.emplace_back(E);
			Leona.spells.emplace_back(R);
			SpellDB.emplace_back(Leona);
		}

		{
			Champ Tristana;
			Tristana.hero = "Tristana";

			Spell W;
			W.name = "TristanaW";
			W.icon = "TristanaW";
			W.displayName = "Rocket Jump";
			W.slot = _W;
			W.type = circular;
			W.speed = 1100;
			W.range = 900;
			W.delay = 0.25;
			W.radius = 270;
			W.danger = 2;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Tristana.spells.emplace_back(W);
			SpellDB.emplace_back(Tristana);
		}

		{
			Champ Tryndamere;
			Tryndamere.hero = "Tryndamere";

			Spell E;
			E.name = "TryndamereE";
			E.icon = "TryndamereE";
			E.displayName = "Spinning Slash";
			E.missileName = "";
			E.slot = _E;
			E.type = linear;
			E.speed = 1300;
			E.range = 660;
			E.delay = 0;
			E.radius = 225;
			E.angle = 0;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = true;
			Tryndamere.spells.emplace_back(E);
			SpellDB.emplace_back(Tryndamere);
		}

		{
			Champ Irelia;
			Irelia.hero = "Irelia";

			Spell E;
			E.name = "IreliaE2";
			E.icon = "IreliaE";
			E.displayName = "Irelia E";
			E.missileName = "IreliaEParticleMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = MathHuge;
			E.range = 5000;
			E.delay = 0.6f;
			E.radius = 40;
			E.danger = 4;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = true;
			Irelia.spells.emplace_back(E);

			Spell W;
			W.name = "IreliaW";
			W.icon = "IreliaW";
			W.displayName = "Irelia W";
			W.missileName = "IreliaW2";
			W.slot = _W;
			W.type = linear;
			W.speed = MathHuge;
			W.range = 775;
			W.delay = 0.25;
			W.radius = 120;
			W.danger = 4;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = true;
			Irelia.spells.emplace_back(W);

			Spell W2;
			W2.name = "IreliaW2";
			W2.icon = "IreliaW2";
			W2.displayName = "Irelia W2";
			W2.slot = _W;
			W2.type = linear;
			W2.speed = MathHuge;
			W2.range = 775;
			W2.delay = 0.25;
			W2.radius = 120;
			W2.danger = 4;
			W2.cc = false;
			W2.collision = false;
			W2.windwall = false;
			W2.hitbox = false;
			W2.fow = false;
			W2.exception = false;
			W2.extend = true;
			Irelia.spells.emplace_back(W2);

			Spell R;
			R.name = "IreliaR";
			R.icon = "IreliaR";
			R.displayName = "Irelia R";
			R.missileName = "IreliaR";
			R.slot = _R;
			R.type = linear;
			R.speed = 2000;
			R.range = 1050;
			R.delay = 0.5;
			R.radius = 160;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			Irelia.spells.emplace_back(R);

			SpellDB.emplace_back(Irelia);
		}

		{
			Champ TwistedFate;
			TwistedFate.hero = "TwistedFate";

			Spell Q;
			Q.name = "WildCards";
			Q.icon = "TwistedFateQ";
			Q.displayName = "Wild Cards";
			Q.missileName = "SealFateMissile";
			Q.slot = _Q;
			Q.type = threeway;
			Q.speed = 1000;
			Q.range = 1450;
			Q.delay = 0.25;
			Q.radius = 40;
			Q.angle = 28;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			TwistedFate.spells.emplace_back(Q);
			SpellDB.emplace_back(TwistedFate);
		}

		{
			Champ Urgot;
			Urgot.hero = "Urgot";
			Spell Q;
			Q.name = "UrgotQ";
			Q.icon = "UrgotQ";
			Q.displayName = "Corrosive Charge";
			Q.missileName = "UrgotQMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 3200;
			Q.range = 800;
			Q.delay = 0.25f;
			Q.radius = 180;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;

			Spell E;
			E.name = "UrgotE";
			E.icon = "UrgotE";
			E.displayName = "Disdain";
			E.slot = _E;
			E.type = linear;
			E.speed = 1530;
			E.range = 475;
			E.delay = 0.25f;
			E.radius = 100;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = true;

			Spell R;
			R.name = "UrgotR";
			R.icon = "UrgotR";
			R.displayName = "Fear Beyond Death";
			R.missileName = "UrgotR";
			R.slot = _R;
			R.type = linear;
			R.speed = 3200;
			R.range = 2500;
			R.delay = 0.5;
			R.radius = 80;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Urgot.spells.emplace_back(Q);
			Urgot.spells.emplace_back(E);
			Urgot.spells.emplace_back(R);
			SpellDB.emplace_back(Urgot);
		}

		{
			Champ Varus;
			Varus.hero = "Varus";

			Spell Q;
			Q.name = "VarusQ1";
			Q.icon = "VarusQ";
			Q.displayName = "Piercing Arrow";
			Q.missileName = "VarusQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.delay = 0.25;
			Q.speed = 1900;
			Q.range = 1525;
			Q.radius = 70;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;

			Spell E;
			E.name = "VarusE";
			E.icon = "VarusE";
			E.displayName = "Hail of Arrows";
			E.missileName = "VarusE";
			E.slot = _E;
			E.type = circular;
			E.speed = 1500;
			E.range = 925;
			E.delay = 0.242f;
			E.radius = 260;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;

			Spell R;
			R.name = "VarusR";
			R.icon = "VarusR";
			R.displayName = "Chain of Corruption";
			R.missileName = "VarusRMissile";
			R.slot = _R;
			R.type = linear;
			R.speed = 1950;
			R.range = 1200;
			R.delay = 0.25;
			R.radius = 120;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = true;

			Varus.spells.emplace_back(Q);
			Varus.spells.emplace_back(E);
			Varus.spells.emplace_back(R);
			SpellDB.emplace_back(Varus);
		}

		{
			Champ Veigar;
			Veigar.hero = "Veigar";

			Spell Q;
			Q.name = "VeigarBalefulStrike";
			Q.icon = "VeigarQ";
			Q.displayName = "Baleful Strike";
			Q.missileName = "VeigarBalefulStrikeMis";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2200;
			Q.range = 900;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;

			Spell W;
			W.name = "VeigarDarkMatterCastLock";
			W.icon = "VeigarW";
			W.displayName = "Dark Matter";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 900;
			W.delay = 1.25;
			W.radius = 210;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;

			Spell E;
			E.name = "VeigarEventHorizon";
			E.icon = "VeigarE";
			E.displayName = "Veigar E Ring In";
			E.slot = _E;
			E.type = ring;
			E.speed = MathHuge;
			E.range = 700;
			E.delay = 4.f;
			E.radius = 80;
			E.radiusEx = 350;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;

			Spell E2;
			E2.name = "VeigarEventHorizon";
			E2.icon = "VeigarE";
			E2.displayName = "Veigar E Ring Out";
			E2.slot = _E;
			E2.type = ring;
			E2.speed = MathHuge;
			E2.range = 700;
			E2.delay = 1.f;
			E2.radius = 140;
			E2.radiusEx = 240;
			E2.danger = 3;
			E2.cc = true;
			E2.collision = false;
			E2.windwall = false;
			E2.hitbox = false;
			E2.fow = true;
			E2.exception = false;
			E2.extend = false;

			Veigar.spells.emplace_back(Q);
			Veigar.spells.emplace_back(W);
			Veigar.spells.emplace_back(E);
			Veigar.spells.emplace_back(E2);
			SpellDB.emplace_back(Veigar);
		}

		{
			Champ Velkoz;
			Velkoz.hero = "Velkoz";

			Spell Q;
			Q.name = "VelkozQ";
			Q.icon = "VelkozQ";
			Q.missileName = "VelkozQMissile";
			Q.displayName = "Plasma Fission";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1300;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 55;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = true;
			Q.collisionWC = true;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;

			Spell Q2;
			Q2.name = "VelkozQSplitActivate";
			Q2.icon = "VelkozQ";
			Q2.displayName = "Plasma Fission Two Line";
			Q2.missileName = "VelkozQMissileSplit";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2100;
			Q2.range = 1100;
			Q2.delay = 0.25f;
			Q2.radius = 50;
			Q2.danger = 2;
			Q2.cc = true;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = false;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;

			Spell W;
			W.name = "VelkozW";
			W.icon = "VelkozW";
			W.displayName = "Void Rift";
			W.missileName = "VelkozWMissile";
			W.slot = _W;
			W.type = linear;
			W.speed = 1700;
			W.range = 1050;
			W.delay = 0.25;
			W.radius = 87;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;

			Spell E;
			E.name = "VelkozE";
			E.icon = "VelkozE";
			E.displayName = "Tectonic Disruption";
			E.slot = _E;
			E.type = circular;
			E.speed = MathHuge;
			E.range = 800;
			E.delay = 0.8f;
			E.radius = 185;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = false;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = false;

			Velkoz.spells.emplace_back(Q);
			Velkoz.spells.emplace_back(Q2);
			Velkoz.spells.emplace_back(W);
			Velkoz.spells.emplace_back(E);
			SpellDB.emplace_back(Velkoz);
		}

		/*{
			Champ Vi;
			Vi.hero = "Vi";

			Spell Q;
			Q.name = "ViQ";
			Q.icon = "ViQ";
			Q.displayName = "Vault Breaker";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1500;
			Q.range = 725;
			Q.delay = 0;
			Q.radius = 90;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;

			Vi.spells.emplace_back(Q);
			SpellDB.emplace_back(Vi);
		}*/

		{
			Champ Vex;
			Vex.hero = "Vex";

			Spell Q;
			Q.name = "VexQ";
			Q.icon = "VexQ";
			Q.displayName = "VexQ";
			Q.slot = _Q;
			Q.radius = 180;
			Q.type = linear;
			//	Q.range = 1200;
			Q.extend = true;
			Q.collision = false;
			Q.danger = 2;
			Q.windwall = true;
			Q.speed = 0;
			Q.delay = 0;
			Vex.spells.push_back(Q);

			Spell E;
			E.name = "VexE";
			E.icon = "VexE";
			E.displayName = "VexE";
			E.slot = _E;
			E.type = circular;
			//	E.range = 800;
			E.collision = false;
			E.danger = 1;
			E.windwall = false;
			E.speed = 0;
			E.delay = 0;
			E.radius = 0;
			Vex.spells.push_back(E);

			Spell R;
			R.name = "VexR";
			R.icon = "VexR";
			R.displayName = "VexR";
			R.slot = _R;
			R.type = linear;
			R.range = 3000;
			R.radius = 130;
			R.collisionWC = true;
			R.danger = 2;
			//R.radius = 130;
			R.windwall = true;
			R.speed = 0;
			R.delay = 0;
			Vex.spells.push_back(R);

			SpellDB.push_back(Vex);
		}

		{
			Champ Viktor;
			Viktor.hero = "Viktor";

			Spell W;
			W.name = "ViktorGravitonField";
			W.icon = "ViktorW";
			W.displayName = "Graviton Field";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 800;
			W.delay = 1.75;
			W.radius = 270;
			W.danger = 1;
			W.cc = true;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;

			Spell E;
			E.name = "ViktorDeathRayMissile";
			E.icon = "ViktorE";
			E.displayName = "Death Ray";
			E.missileName = "ViktorDeathRayMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1050;
			E.range = 700;
			E.radius = 80;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.fow = true;
			E.exception = true;
			E.extend = true;
			Viktor.spells.emplace_back(W);
			Viktor.spells.emplace_back(E);
			SpellDB.emplace_back(Viktor);
		}

		/*{
			Champ Vladimir;
			Vladimir.hero = "Vladimir";
			Spell R;
			R.name = "VladimirHemoplague";
			R.icon = "VladimirR";
			R.displayName = "Hemoplague";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 700;
			R.delay = 0.389;
			R.radius = 350;
			R.danger = 3;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;

			Vladimir.spells.emplace_back(R);
			SpellDB.emplace_back(Vladimir);
		}*/

		{
			Champ Warwick;
			Warwick.hero = "Warwick";

			Spell R;
			R.name = "WarwickR";
			R.icon = "WarwickR";
			R.displayName = "Infinite Duress";
			R.slot = _R;
			R.type = circular;
			R.speed = 2000;
			R.range = 3000;
			R.delay = 0.5f;
			R.radius = 180;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;
			Warwick.spells.emplace_back(R);
			SpellDB.emplace_back(Warwick);
		}

		{
			Champ Nocturne;
			Nocturne.hero = "Nocturne";

			Spell Q;
			Q.name = "NocturneDuskbringer";
			Q.icon = "NocturneQ";
			Q.displayName = "Nocturne Q";
			Q.missileName = "NocturneDuskbringer";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1600;
			Q.range = 1200;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Nocturne.spells.emplace_back(Q);
			SpellDB.emplace_back(Nocturne);
		}

		{
			Champ Draven;
			Draven.hero = "Draven";

			Spell E;
			E.name = "DravenDoubleShot";
			E.icon = "DravenDoubleShot";
			E.displayName = "Draven E";
			E.missileName = "DravenDoubleShotMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1400;
			E.range = 1100;
			E.delay = 0.25;
			E.radius = 130;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = false;
			E.exception = false;
			E.extend = true;
			Draven.spells.emplace_back(E);
			SpellDB.emplace_back(Draven);
		}

		{
			Champ Xayah;
			Xayah.hero = "Xayah";
			Spell Q;
			Q.name = "XayahQ";
			Q.icon = "XayahQ";
			Q.displayName = "Double Daggers";
			Q.missileName = "XayahQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2075;
			Q.range = 1100;
			Q.delay = 0.5;
			Q.radius = 50;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Xayah.spells.emplace_back(Q);
			Spell Q1;
			Q1.name = "XayahQ1";
			Q1.icon = "XayahQ1";
			Q1.displayName = "Double Daggers 1";
			Q1.missileName = "XayahQ1";
			Q1.slot = _Q;
			Q1.type = linear;
			Q1.speed = 2075;
			Q1.range = 1100;
			Q1.delay = 0.5;
			Q1.radius = 59;
			Q1.danger = 1;
			Q1.cc = false;
			Q1.collision = false;
			Q1.windwall = true;
			Q1.hitbox = false;
			Q1.fow = true;
			Q1.exception = false;
			Q1.extend = true;
			Xayah.spells.emplace_back(Q1);
			Spell Q2;
			Q2.name = "XayahQ2";
			Q2.icon = "XayahQ2";
			Q2.displayName = "Double Daggers 2";
			Q2.missileName = "XayahQ2";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2075;
			Q2.range = 1100;
			Q2.delay = 0.5;
			Q2.radius = 45;
			Q2.danger = 1;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = false;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Xayah.spells.emplace_back(Q2);

			Spell E;
			E.name = "XayahE";
			E.icon = "XayahE";
			E.displayName = "Xayah E";
			E.missileName = "XayahEMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 4000;
			E.range = 1100;
			E.delay = 0.25;
			E.radius = 80;
			E.danger = 1;
			E.cc = false;
			E.followEnemy = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Xayah.spells.emplace_back(E);

			SpellDB.emplace_back(Xayah);
		}

		{
			Champ Xerath;
			Xerath.hero = "Xerath";
			Spell Q;
			Q.name = "XerathArcanopulse2";
			Q.icon = "XerathQ";
			Q.displayName = "Arcanopulse";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 1550;
			Q.delay = 0.6f;
			Q.radius = 95;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;

			Spell W;
			W.name = "XerathArcaneBarrage2";
			W.icon = "XerathW";
			W.displayName = "Arcane Barrage";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 1100;
			W.delay = 0.8f;
			W.radius = 260;
			W.danger = 3;
			W.cc = true;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;

			Spell E;
			E.name = "XerathMageSpear";
			E.icon = "XerathE";
			E.displayName = "Mage Spear";
			E.missileName = "XerathMageSpearMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1400;
			E.range = 1125;
			E.delay = 0.2f;
			E.radius = 70;
			E.danger = 1;
			E.cc = true;
			E.collision = true;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Xerath.spells.emplace_back(Q);
			Xerath.spells.emplace_back(W);
			Xerath.spells.emplace_back(E);
			SpellDB.emplace_back(Xerath);
		}

		{
			Champ XinZhao;
			XinZhao.hero = "XinZhao";
			Spell W;
			W.name = "XinZhaoW";
			W.icon = "XinZhaoW";
			W.displayName = "Wind Becomes Lightning";
			W.slot = _W;
			W.type = linear;
			W.speed = 1600;
			W.range = 1000;
			W.delay = 0.25;
			W.radius = 70;
			W.danger = 3;
			W.cc = true;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = false;
			W.exception = false;
			W.extend = true;

			XinZhao.spells.emplace_back(W);
			SpellDB.emplace_back(XinZhao);
		}

		{
			Champ Yone;
			Yone.hero = "Yone";

			Spell Q;
			Q.name = "YoneQ";
			Q.icon = "YoneQ";
			Q.displayName = "Mortal Steel [Sword]";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 475;
			Q.delay = 0.25;
			Q.radius = 100;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;

			Spell Q3;
			Q3.name = "YoneQ3";
			Q3.icon = "YoneQ3";
			Q3.displayName = "Mortal Steel [Storm]";
			Q3.missileName = "YoneQ3Missile";
			Q3.slot = _Q;
			Q3.type = linear;
			Q3.speed = 1500;
			Q3.range = 1000;
			Q3.delay = 0.35f;
			Q3.radius = 100;
			Q3.danger = 2;
			Q3.cc = true;
			Q3.collision = false;
			Q3.windwall = false;
			Q3.hitbox = false;
			Q3.fow = false;
			Q3.exception = false;
			Q3.extend = true;

			Spell W;
			W.name = "YoneW";
			W.icon = "YoneW";
			W.displayName = "Spirit Cleave";
			W.slot = _W;
			W.type = conic;
			W.speed = MathHuge;
			W.range = 600;
			W.delay = 0.35f;
			W.radius = 0;
			W.angle = 80;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = true;

			Spell R;
			R.name = "YoneR";
			R.icon = "YoneR";
			R.displayName = "Fate Sealed";
			R.slot = _R;
			R.type = linear;
			R.speed = MathHuge;
			R.range = 1000;
			R.delay = 0.75f;
			R.radius = 150;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = false;
			R.exception = false;
			R.extend = true;

			Yone.spells.emplace_back(Q);
			Yone.spells.emplace_back(Q3);
			Yone.spells.emplace_back(W);
			Yone.spells.emplace_back(R);
			SpellDB.emplace_back(Yone);
		}

		{
			Champ Zac;
			Zac.hero = "Zac";
			Spell Q;
			Q.name = "ZacQ";
			Q.icon = "ZacQ";
			Q.displayName = "Stretching Strikes";
			Q.missileName = "ZacQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2800;
			Q.range = 800;
			Q.delay = 0.25;
			Q.radius = 120;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;

			Zac.spells.emplace_back(Q);

			Spell E;
			E.name = "ZacE";
			E.icon = "ZacE";
			E.displayName = "Zac E";
			E.slot = _E;
			E.type = circular;
			E.speed = 1200;
			E.range = 1800;
			E.delay = 0.25;
			E.radius = 250;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;
			Zac.spells.emplace_back(E);

			SpellDB.emplace_back(Zac);
		}

		{
			Champ Swain;
			Swain.hero = "Swain";

			Spell Q;
			Q.name = "SwainQ";
			Q.icon = "SwainQ";
			Q.displayName = "Death's Hand";
			Q.missileName = "SwainQ";
			Q.slot = _Q;
			Q.type = conic;
			Q.speed = 5000;
			Q.range = 600;
			Q.delay = 0.33f;
			Q.radius = 30;
			Q.danger = 2;
			Q.angle = 30;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;

			Swain.spells.emplace_back(Q);

			Spell W;
			W.name = "SwainW";
			W.icon = "SwainW";
			W.displayName = "Death's Hand";
			W.missileName = "SwainW";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 3500;
			W.delay = 1.5;
			W.radius = 300;
			W.danger = 2;
			W.cc = true;
			W.collision = false;
			W.windwall = true;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = false;

			Swain.spells.emplace_back(W);

			Spell E;
			E.name = "SwainE";
			E.icon = "SwainE";
			E.displayName = "Nevermove";
			E.missileName = "SwainE";
			E.slot = _E;
			E.type = linear;
			E.speed = 935;
			E.range = 900;
			E.delay = 0.25;
			E.radius = 95;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Swain.spells.emplace_back(E);

			Spell ER;
			ER.name = "SwainEReturn";
			ER.icon = "SwainEReturn";
			ER.displayName = "Swain E Return";
			ER.missileName = "SwainEReturnMissile";
			ER.slot = _E;
			ER.type = linear;
			ER.speed = 600;
			ER.range = 950;
			ER.delay = 0.25;
			ER.radius = 95;
			ER.danger = 2;
			ER.cc = true;
			ER.followEnemy = true;
			ER.collision = false;
			ER.windwall = true;
			ER.hitbox = false;
			ER.fow = false;
			ER.exception = false;
			ER.extend = true;

			Swain.spells.emplace_back(ER);

			SpellDB.emplace_back(Swain);
		}

		{
			Champ DrMundo;
			DrMundo.hero = "DrMundo";

			Spell Q;
			Q.name = "DrMundoQ";
			Q.icon = "DrMundoQ";
			Q.displayName = "Mundo Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2000;
			Q.range = 1050;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 3;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			DrMundo.spells.emplace_back(Q);

			SpellDB.emplace_back(DrMundo);
		}

		{
			Champ Rengar;
			Rengar.hero = "Rengar";

			Spell E;
			E.name = "RengarE";
			E.icon = "RengarE";
			E.displayName = "Rengar E";
			E.slot = _E;
			E.type = linear;
			E.speed = 1500;
			E.range = 1000;
			E.delay = 0.25;
			E.radius = 70;
			E.danger = 3;
			E.cc = false;
			E.collision = true;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Rengar.spells.emplace_back(E);

			SpellDB.emplace_back(Rengar);
		}

		{
			Champ Pyke;
			Pyke.hero = "Pyke";

			Spell Q;
			Q.name = "PykeQMelee";
			Q.icon = "PykeQ";
			Q.displayName = "Pyke Q Melee";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 400;
			Q.delay = 0.25;
			Q.radius = 100;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Pyke.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "PykeQRange";
			Q2.icon = "PykeQ";
			Q2.displayName = "Pyke Q Long";
			Q2.missileName = "PykeQRange";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2000;
			Q2.range = 1100;
			Q2.delay = 0.20f;
			Q2.radius = 70;
			Q2.danger = 3;
			Q2.cc = true;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = false;
			Pyke.spells.emplace_back(Q2);

			Spell E;
			E.name = "PykeE";
			E.icon = "PykeE";
			E.displayName = "PykeE";
			E.missileName = "PykeEMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 3000;
			E.range = 550;
			E.delay = 1.0f;
			E.radius = 110;
			E.danger = 3;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.followEnemy3 = true;
			E.extend = false;
			Pyke.spells.emplace_back(E);

			Spell R;
			R.name = "PykeR";
			R.icon = "PykeR";
			R.displayName = "Pyke R";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 750;
			R.delay = 0.5;
			R.radius = 150;
			R.danger = 3;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			Pyke.spells.emplace_back(R);

			SpellDB.emplace_back(Pyke);
		}

		{
			Champ Pantheon;
			Pantheon.hero = "Pantheon";

			Spell Q;
			Q.name = "PantheonQTap";
			Q.icon = "PantheonQ";
			Q.displayName = "Pantheon QTap";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 600;
			Q.delay = 0.3f;
			Q.radius = 80;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Pantheon.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "PantheonQMissile";
			Q2.icon = "PantheonQ";
			Q2.displayName = "Pantheon Q";
			Q2.missileName = "PantheonQMissile";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2500;
			Q2.range = 1200;
			Q2.delay = 0.25;
			Q2.radius = 65;
			Q2.danger = 3;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Pantheon.spells.emplace_back(Q2);

			Spell R;
			R.name = "PantheonR";
			R.icon = "PantheonR";
			R.displayName = "Pantheon R";
			R.slot = _R;
			R.type = linear;
			R.speed = 2250;
			R.range = 1350;
			R.delay = 4;
			R.radius = 250;
			R.danger = 3;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Pantheon.spells.emplace_back(R);

			SpellDB.emplace_back(Pantheon);
		}

		{
			Champ Malzahar;
			Malzahar.hero = "Malzahar";

			Spell Q;
			Q.name = "MalzaharQ";
			Q.icon = "MalzaharQ";
			Q.displayName = "Malzahar Q";
			Q.missileName = "MalzaharQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 900;
			Q.delay = 0.75;
			Q.radius = 85 * 2;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Malzahar.spells.emplace_back(Q);

			SpellDB.emplace_back(Malzahar);
		}

		{
			Champ Yasuo;
			Yasuo.hero = "Yasuo";

			Spell Q;
			Q.name = "YasuoQ1Wrapper";
			Q.icon = "YasuoQ";
			Q.displayName = "Yasuo Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 3200;
			Q.range = 475;
			Q.delay = 0.25;
			Q.radius = 100;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Yasuo.spells.emplace_back(Q);

			Spell Q1;
			Q1.name = "YasuoQ2Wrapper";
			Q1.icon = "YasuoQ1";
			Q1.displayName = "Yasuo Q1";
			Q1.slot = _Q;
			Q1.type = polygon;
			Q1.speed = MathHuge;
			Q1.range = 475;
			Q1.delay = 0.25;
			Q1.radius = 100;
			Q1.danger = 1;
			Q1.cc = false;
			Q1.collision = false;
			Q1.windwall = false;
			Q1.hitbox = true;
			Q1.fow = true;
			Q1.exception = false;
			Q1.extend = true;
			Yasuo.spells.emplace_back(Q1);

			Spell Q3;
			Q3.name = "YasuoQ3Wrapper";
			Q3.icon = "YasuoQ3";
			Q3.displayName = "Yasuo Q3";
			Q3.slot = _Q;
			Q3.type = linear;
			Q3.speed = 1200;
			Q3.range = 1150;
			Q3.delay = 0.25;
			Q3.radius = 100;
			Q3.danger = 1;
			Q3.cc = false;
			Q3.collision = false;
			Q3.windwall = false;
			Q3.hitbox = true;
			Q3.fow = true;
			Q3.exception = false;
			Q3.extend = true;
			Yasuo.spells.emplace_back(Q3);

			SpellDB.emplace_back(Yasuo);
		}

		{
			Champ Seraphine;
			Seraphine.hero = "Seraphine";

			Spell Q;
			Q.name = "SeraphineQ";
			Q.icon = "SeraphineQ";
			Q.displayName = "Seraphine Q";
			Q.missileName = "SeraphineQInitialMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1300;
			Q.range = 900;
			Q.delay = 0.6f;
			Q.radius = 290;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Seraphine.spells.emplace_back(Q);

			Spell E;
			E.name = "SeraphineE";
			E.icon = "SeraphineE";
			E.displayName = "Seraphine E";
			E.missileName = "SeraphineEMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1200;
			E.range = 1350;
			E.delay = 0.25;
			E.radius = 80;
			E.danger = 3;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Seraphine.spells.emplace_back(E);

			Spell R;
			R.name = "SeraphineR";
			R.icon = "SeraphineR";
			R.displayName = "Seraphine R";
			R.missileName = "SeraphineR";
			R.slot = _R;
			R.type = linear;
			R.speed = 1600;
			R.range = 2500;
			R.delay = 0.5;
			R.radius = 160;
			R.danger = 5;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Seraphine.spells.emplace_back(R);

			Spell R2;
			R2.name = "SeraphineRfow";
			R2.icon = "SeraphineR";
			R2.displayName = "Seraphine R Fow";
			R2.slot = _R;
			R2.type = linear;
			R2.speed = MathHuge;
			R2.range = 1300;
			R2.delay = 0.5;
			R2.radius = 160;
			R2.danger = 5;
			R2.cc = true;
			R2.collision = false;
			R2.windwall = true;
			R2.hitbox = true;
			R2.fow = true;
			R2.exception = false;
			R2.extend = true;
			Seraphine.spells.emplace_back(R2);

			SpellDB.emplace_back(Seraphine);
		}

		{
			Champ Sylas;
			Sylas.hero = "Sylas";

			Spell E2;
			E2.name = "SylasE2";
			E2.icon = "SylasE2";
			E2.displayName = "Sylas E2";
			E2.missileName = "SylasE2";
			E2.slot = _E;
			E2.type = linear;
			E2.speed = 1600;
			E2.range = 850;
			E2.delay = 0.25;
			E2.radius = 65;
			E2.danger = 4;
			E2.cc = false;
			E2.collision = false;
			E2.windwall = true;
			E2.hitbox = true;
			E2.fow = true;
			E2.exception = false;
			E2.extend = true;
			//Q.useMissile = false;
			Sylas.spells.emplace_back(E2);

			Spell Q;
			Q.name = "SylasQ";
			Q.icon = "SylasQ";
			Q.displayName = "Sylas Q";
			Q.slot = _Q;
			Q.type = polygon;
			Q.speed = MathHuge;
			Q.range = 725;
			Q.delay = 1.15f;
			Q.radius = 150;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			//Q.useMissile = false;
			Sylas.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "SylasQLine";
			Q2.icon = "SylasQ";
			Q2.displayName = "Sylas Q Line";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = MathHuge;
			Q2.range = 725;
			Q2.delay = 0.4f;
			Q2.radius = 55;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			//Q.useMissile = false;
			Sylas.spells.emplace_back(Q2);

			SpellDB.emplace_back(Sylas);
		}

		{
			Champ Ekko;
			Ekko.hero = "Ekko";

			Spell Q;
			Q.name = "EkkoQ";
			Q.icon = "EkkoQ";
			Q.displayName = "Ekko Q";
			Q.missileName = "EkkoQMis";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1650;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 160;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Ekko.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "EkkoQReturn";
			Q2.icon = "EkkoQ";
			Q2.displayName = "Ekko Q Return";
			Q2.missileName = "EkkoQReturn";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2300;
			Q2.range = 1100;
			Q2.delay = 0.25;
			Q2.radius = 160;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			Ekko.spells.emplace_back(Q2);

			Spell Q3;
			Q3.name = "EkkoQball";
			Q3.icon = "EkkoQ";
			Q3.displayName = "Ekko Q Ball";
			Q3.missileName = "EkkoQMis";
			Q3.slot = _Q;
			Q3.type = circular;
			Q3.speed = MathHuge;
			Q3.range = 950;
			Q3.delay = 0.6f;
			Q3.radius = 150;
			Q3.danger = 4;
			Q3.cc = false;
			Q3.collision = false;
			Q3.windwall = true;
			Q3.hitbox = true;
			Q3.fow = true;
			Q3.exception = false;
			Q3.extend = true;
			Ekko.spells.emplace_back(Q3);

			Spell W;
			W.name = "EkkoW";
			W.icon = "EkkoW";
			W.displayName = "Ekko W";
			W.missileName = "EkkoWMis";
			W.slot = _W;
			W.type = circular;
			W.speed = 1650;
			W.range = 1600;
			W.delay = 3.75f;
			W.radius = 375;
			W.danger = 3;
			W.cc = true;
			W.collision = false;
			W.windwall = false;
			W.hitbox = false;
			W.fow = true;
			W.exception = false;
			W.extend = false;
			Ekko.spells.emplace_back(W);

			Spell R;
			R.name = "EkkoR";
			R.icon = "EkkoR";
			R.displayName = "Ekko R";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 25000;
			R.delay = 0.8f;
			R.radius = 375;
			R.danger = 3;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			Ekko.spells.emplace_back(R);

			SpellDB.emplace_back(Ekko);
		}

		{
			Champ Samira;
			Samira.hero = "Samira";

			Spell Q;
			Q.name = "SamiraQGun";
			Q.icon = "SamiraQ";
			Q.displayName = "Samira Q";
			Q.missileName = "SamiraQGun";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 2600;
			Q.range = 950;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Samira.spells.emplace_back(Q);

			SpellDB.emplace_back(Samira);
		}

		{
			Champ Gragas;
			Gragas.hero = "Gragas";

			Spell Q;
			Q.name = "GragasQ";
			Q.icon = "GragasQ";
			Q.displayName = "Gragas Q";
			Q.missileName = "GragasQMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1300;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 275;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Gragas.spells.emplace_back(Q);

			Spell E;
			E.name = "GragasE";
			E.icon = "GragasE";
			E.displayName = "Gragas E";
			E.missileName = "GragasE";
			E.slot = _E;
			E.type = linear;
			E.speed = 1200;
			E.range = 800;
			E.delay = 0;
			E.radius = 200;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			Gragas.spells.emplace_back(E);

			Spell R;
			R.name = "GragasR";
			R.icon = "GragasR";
			R.displayName = "Gragas R";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 1050;
			R.delay = 1;
			R.radius = 375;
			R.danger = 5;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			Gragas.spells.emplace_back(R);

			Spell R2;
			R2.name = "GragasRfow";
			R2.icon = "GragasR";
			R2.displayName = "Gragas R";
			R.missileName = "GragasRBoom";
			R2.slot = _R;
			R2.type = circular;
			R2.speed = 1300;
			R2.range = 1050;
			R2.delay = 1;
			R2.radius = 375;
			R2.danger = 5;
			R2.cc = false;
			R2.collision = false;
			R2.windwall = false;
			R2.hitbox = true;
			R2.fow = true;
			R2.exception = false;
			R2.extend = false;
			Gragas.spells.emplace_back(R2);

			SpellDB.emplace_back(Gragas);
		}

		{
			Champ Graves;
			Graves.hero = "Graves";

			Spell Q;
			Q.name = "GravesQLineSpell";
			Q.icon = "GravesQ";
			Q.displayName = "Graves Q";
			Q.missileName = "GravesQLineMis";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 3000;
			Q.range = 808;
			Q.delay = 0.25;
			Q.radius = 90;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			//Q.useMissile = false;
			Graves.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "GravesQLineSpell";
			Q2.icon = "GravesQ";
			Q2.displayName = "Graves Q Retuen";
			Q2.missileName = "GravesQReturn";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 3000;
			Q2.range = 808;
			Q2.delay = 0.25;
			Q2.radius = 40;
			Q2.danger = 1;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			//Q.useMissile = false;
			Graves.spells.emplace_back(Q2);

			Spell W;
			W.name = "GravesSmokeGrenade";
			W.icon = "GravesQ";
			W.displayName = "Graves W";
			W.missileName = "GravesSmokeGrenadeBoom";
			W.slot = _W;
			W.type = circular;
			W.speed = 1500;
			W.range = 950;
			W.delay = 1.0f;
			W.radius = 220;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = false;
			//Q.useMissile = false;
			Graves.spells.emplace_back(W);

			Spell R;
			R.name = "GravesChargeShot";
			R.icon = "GravesR";
			R.displayName = "Graves R";
			R.missileName = "GravesChargeShotShot";
			R.slot = _R;
			R.type = linear;
			R.speed = 2100;
			R.range = 1100;
			R.delay = 0.25;
			R.radius = 100;
			R.danger = 1;
			R.cc = false;
			R.collision = true;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;
			Graves.spells.emplace_back(R);

			Spell R2;
			R2.name = "GravesChargeShotFxMissile2";
			R2.icon = "GravesR";
			R2.displayName = "Graves R";
			R2.missileName = "GravesChargeShotFxMissile2";
			R2.slot = _R;
			R2.type = conic;
			R2.speed = 2100;
			R2.range = 800;
			R2.delay = 0.25;
			R2.radius = 10;
			R2.danger = 5;
			R2.cc = false;
			R2.collision = true;
			R2.windwall = true;
			R2.hitbox = true;
			R2.fow = true;
			R2.exception = false;
			R2.extend = true;
			Graves.spells.emplace_back(R2);

			SpellDB.emplace_back(Graves);
		}

		{
			Champ Volibear;
			Volibear.hero = "Volibear";

			Spell E;
			E.name = "VolibearE";
			E.icon = "VolibearE";
			E.displayName = "Volibear E";
			E.slot = _E;
			E.type = circular;
			E.speed = MathHuge;
			E.range = 1250;
			E.delay = 0.25;
			E.radius = 325;
			E.danger = 1;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = true;
			E.fow = false;
			E.exception = false;
			E.extend = false;
			//Q.useMissile = false;
			Volibear.spells.emplace_back(E);

			SpellDB.emplace_back(Volibear);
		}

		{
			Champ Riven;
			Riven.hero = "Riven";

			Spell R;
			R.name = "RivenIzunaBlade";
			R.icon = "RivenIzunaBlade";
			R.displayName = "Riven R";
			R.slot = _R;
			R.type = conic;
			R.speed = 1600;
			R.range = 1100;
			R.delay = 0.25;
			R.radius = 100;
			R.danger = 4;
			R.angle = 60;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			//Q.useMissile = false;
			Riven.spells.emplace_back(R);

			SpellDB.emplace_back(Riven);
		}

		{
			Champ Shen;
			Shen.hero = "Shen";

			Spell E;
			E.name = "ShenE";
			E.icon = "ShenE";
			E.displayName = "Shen E";
			E.slot = _E;
			E.type = linear;
			E.speed = 1300;
			E.range = 659;
			E.delay = 0.0f;
			E.radius = 90;
			E.danger = 1;
			E.cc = false;
			E.collision = false;
			E.windwall = false;
			E.hitbox = true;
			E.fow = false;
			E.exception = false;
			E.extend = true;
			Shen.spells.emplace_back(E);

			SpellDB.emplace_back(Shen);
		}

		{
			Champ Zeri;
			Zeri.hero = "Zeri";

			Spell W;
			W.name = "ZeriW";
			W.icon = "ZeriW";
			W.displayName = "Zeri W";
			W.slot = _W;
			W.type = circular;
			W.speed = 2500;
			W.range = 750;
			W.delay = 0.25;
			W.radius = 240;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Zeri.spells.emplace_back(W);

			SpellDB.emplace_back(Zeri);
		}

		{
			Champ Zed;
			Zed.hero = "Zed";

			Spell Q;
			Q.name = "ZedQ";
			Q.icon = "ZedQ";
			Q.displayName = "Zed Q";
			Q.missileName = "ZedQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1700;
			Q.range = 925;
			Q.delay = 0.25;
			Q.radius = 65;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Zed.spells.emplace_back(Q);

			Spell W;
			W.name = "ZedW";
			W.icon = "ZedW";
			W.displayName = "Zed W";
			W.slot = _W;
			W.type = circular;
			W.speed = 2500;
			W.range = 750;
			W.delay = 0.25;
			W.radius = 240;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = false;
			W.exception = false;
			W.extend = false;
			Zed.spells.emplace_back(W);

			SpellDB.emplace_back(Zed);
		}

		{
			Champ Rell;
			Rell.hero = "Rell";

			Spell Q;
			Q.name = "RellQ";
			Q.icon = "RellQ";
			Q.displayName = "Rell Q";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 900;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Rell.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "RellR";
			Q2.icon = "RellR";
			Q2.displayName = "Rell R";
			Q2.slot = _R;
			Q2.type = circular;
			Q2.speed = MathHuge;
			Q2.range = 1000;
			Q2.delay = 0.25;
			Q2.radius = 0;
			Q2.danger = 1;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = false;
			Q2.hitbox = true;
			Q2.fow = false;
			Q2.exception = false;
			Q2.extend = false;
			Rell.spells.emplace_back(Q2);

			SpellDB.emplace_back(Rell);
		}

		{
			Champ Ivern;
			Ivern.hero = "Ivern";

			Spell Q;
			Q.name = "IvernQ";
			Q.icon = "IvernQ";
			Q.displayName = "Ivern Q";
			Q.missileName = "IvernQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1300;
			Q.range = 1100;
			Q.delay = 0.25;
			Q.radius = 65;
			Q.danger = 3;
			Q.cc = true;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Ivern.spells.emplace_back(Q);

			SpellDB.emplace_back(Ivern);
		}

		{
			Champ Malphite;
			Malphite.hero = "Malphite";

			Spell R;
			R.name = "UFSlash";
			R.icon = "MalphiteR";
			R.displayName = "Malphite R";
			R.missileName = "UFSlash";
			R.slot = _R;
			R.type = circular;
			R.speed = 1500;
			R.range = 1000;
			R.delay = 1;
			R.radius = 270;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = false;
			Malphite.spells.emplace_back(R);

			SpellDB.emplace_back(Malphite);
		}

		{
			Champ Rakan;
			Rakan.hero = "Rakan";

			Spell Q;
			Q.name = "RakanQ";
			Q.icon = "RakanQ";
			Q.displayName = "Rakan Q";
			Q.missileName = "RakanQMis";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1850;
			Q.range = 900;
			Q.delay = 0.25;
			Q.radius = 65;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Rakan.spells.emplace_back(Q);

			Spell W;
			W.name = "RakanWCast";
			W.icon = "RakanW";
			W.displayName = "Rakan W";
			W.slot = _W;
			W.type = circular;
			W.speed = MathHuge;
			W.range = 650;
			W.delay = 0.3f;
			W.radius = 275;
			W.danger = 1;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = false;
			Rakan.spells.emplace_back(W);

			SpellDB.emplace_back(Rakan);
		}

		{
			Champ Heimerdinger;
			Heimerdinger.hero = "Heimerdinger";

			Spell W;
			W.name = "HeimerdingerWAttack2";
			W.icon = "HeimerdingerWAttack2";
			W.displayName = "Heimerdinger W";
			W.slot = _W;
			W.type = linear;
			W.speed = 3000;
			W.range = 1350;
			W.delay = 0.3f;
			W.radius = 40;
			W.danger = 1;
			W.cc = false;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Heimerdinger.spells.emplace_back(W);

			SpellDB.emplace_back(Heimerdinger);
		}

		{
			Champ Sivir;
			Sivir.hero = "Sivir";

			Spell Q;
			Q.name = "SivirQ";
			Q.icon = "SivirQ";
			Q.displayName = "Sivir Q";
			Q.missileName = "SivirQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1350;
			Q.range = 1000;
			Q.delay = 0.25;
			Q.delayBR = 0.8;
			Q.radius = 100;
			Q.danger = 2;
			Q.followEnemy3 = true;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			Sivir.spells.emplace_back(Q);

			/*Spell QR;
			QR.name = "SivirQ";
			QR.icon = "SivirQ";
			QR.displayName = "Sivir Q Return";
			QR.missileName = "SivirQMissileReturn";
			QR.slot = _Q;
			QR.type = linear;
			QR.speed = 1350;
			QR.range = 1250;
			QR.delay = 0.25;
			QR.radius = 100;
			QR.danger = 2;
			QR.followEnemy = true;
			QR.cc = false;
			QR.collision = false;
			QR.windwall = true;
			QR.hitbox = true;
			QR.fow = true;
			QR.exception = false;
			QR.extend = true;
			Sivir.spells.emplace_back(QR);*/

			SpellDB.emplace_back(Sivir);
		}

		{
			Champ Lulu;
			Lulu.hero = "Lulu";

			Spell Q;
			Q.name = "LuluQ";
			Q.icon = "LuluQ";
			Q.displayName = "Razor Shuriken";
			Q.missileName = "LuluQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1450;
			Q.range = 950;
			Q.delay = 0.25;
			Q.radius = 60;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			//Q.useMissile = false;
			Lulu.spells.emplace_back(Q);

			Spell Q2;
			Q2.name = "LuluQPix";
			Q2.icon = "LuluQPix";
			Q2.displayName = "Razor Shuriken";
			Q2.missileName = "LuluQMissileTwo";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 1450;
			Q2.range = 950;
			Q2.delay = 0.25;
			Q2.radius = 60;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = false;
			Q2.windwall = true;
			Q2.hitbox = true;
			Q2.fow = true;
			Q2.exception = false;
			Q2.extend = true;
			//Q.useMissile = false;
			Lulu.spells.emplace_back(Q2);

			SpellDB.emplace_back(Lulu);
		}

		{
			Champ Kayn;
			Kayn.hero = "Kayn";

			Spell Q;
			Q.name = "KaynQ";
			Q.icon = "KaynQ";
			Q.displayName = "Kayn Q";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 2400;
			Q.range = 350;
			Q.delay = 0.25;
			Q.radius = 100;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;
			//Q.useMissile = false;
			Kayn.spells.emplace_back(Q);

			Spell W;
			W.name = "KaynW";
			W.icon = "KaynW";
			W.displayName = "Kayn W";
			W.slot = _W;
			W.type = linear;
			W.speed = MathHuge;
			W.range = 700;
			W.delay = 0.55f;
			W.radius = 100;
			W.danger = 3;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;
			Kayn.spells.emplace_back(W);

			Spell W2;
			W2.name = "KaynAssW";
			W2.icon = "KaynAssW";
			W2.displayName = "Kayn Ass W";
			W2.slot = _W;
			W2.type = linear;
			W2.speed = MathHuge;
			W2.range = 900;
			W2.delay = 0.60f;
			W2.radius = 100;
			W2.danger = 3;
			W2.cc = false;
			W2.collision = false;
			W2.windwall = false;
			W2.hitbox = false;
			W2.fow = false;
			W2.exception = false;
			W2.extend = true;
			Kayn.spells.emplace_back(W2);

			SpellDB.emplace_back(Kayn);
		}

		{
			Champ Ziggs;
			Ziggs.hero = "Ziggs";

			Spell Q;
			Q.name = "ZiggsQ";
			Q.icon = "ZiggsQ";
			Q.displayName = "Bouncing Bomb";
			Q.missileName = "ZiggsQSpell";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = 1700;
			Q.range = 850;
			Q.delay = 0.25;
			Q.radius = 125;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;

			Spell W;
			W.name = "ZiggsW";
			W.icon = "ZiggsW";
			W.displayName = "Satchel Charge";
			W.missileName = "ZiggsW";
			W.slot = _W;
			W.type = circular;
			W.speed = 1750;
			W.range = 1000;
			W.delay = 0.25;
			W.radius = 275;
			W.danger = 3;
			W.cc = true;
			W.collision = false;
			W.windwall = true;
			W.hitbox = false;
			W.fow = true;
			W.exception = false;
			W.extend = false;

			Spell E;
			E.name = "ZiggsE";
			E.icon = "ZiggsE";
			E.displayName = "Hexplosive Minefield";
			E.missileName = "ZiggsE2";
			E.slot = _E;
			E.type = circular;
			E.speed = 1550;
			E.range = 900;
			E.delay = 0.25;
			E.radius = 235;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = false;

			Spell R;
			R.name = "ZiggsR";
			R.icon = "ZiggsR";
			R.displayName = "Mega Inferno Bomb";
			R.missileName = "ZiggsRBoom";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 5000;
			R.delay = 0.38f;
			R.radius = 500;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = true;
			R.exception = false;
			R.extend = false;

			Ziggs.spells.emplace_back(Q);
			Ziggs.spells.emplace_back(W);
			Ziggs.spells.emplace_back(E);
			Ziggs.spells.emplace_back(R);

			SpellDB.emplace_back(Ziggs);
		}

		{
			Champ Zilean;
			Zilean.hero = "Zilean";

			Spell Q;
			Q.name = "ZileanQ";
			Q.icon = "ZileanQ";
			Q.displayName = "Time Bomb";
			Q.missileName = "ZileanQMissile";
			Q.slot = _Q;
			Q.type = circular;
			Q.speed = MathHuge;
			Q.range = 900;
			Q.delay = 0.25f;
			Q.radius = 150;
			Q.danger = 2;
			Q.cc = true;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = true;
			Q.exception = false;
			Q.extend = false;
			Zilean.spells.emplace_back(Q);
			SpellDB.emplace_back(Zilean);
		}

		{
			Champ Viktor;
			Viktor.hero = "Viktor";

			Spell E;
			E.name = "ViktorDeathRay";
			E.icon = "ViktorDeathRay";
			E.displayName = "Viktor E";
			E.slot = _Q;
			E.type = linear;
			E.speed = 1050;
			E.range = 550;
			E.delay = 0.25f;
			E.radius = 80;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = true;
			E.extend = true;
			Viktor.spells.emplace_back(E);

			Spell E2;
			E2.name = "ViktorDeathRay";
			E2.icon = "ViktorDeathRay";
			E2.displayName = "Viktor E";
			E2.slot = _E;
			E2.type = linear;
			E2.speed = 1500;
			E2.range = 500;
			E2.delay = 0.25f;
			E2.radius = 80;
			E2.danger = 2;
			E2.cc = false;
			E2.collision = false;
			E2.windwall = true;
			E2.hitbox = false;
			E2.fow = true;
			E2.exception = true;
			E2.extend = true;
			Viktor.spells.emplace_back(E2);
			SpellDB.emplace_back(Viktor);
		}

		{
			Champ MissFortune;
			MissFortune.hero = "MissFortune";

			Spell R;
			R.name = "MissFortuneBulletTime";
			R.icon = "MissFortuneR";
			R.displayName = "Bullet Time";
			R.missileName = "";
			R.slot = _R;
			R.type = conic;
			R.speed = 2000;
			R.range = 1400;
			R.delay = 0.25;
			R.radius = 100;
			R.angle = 34;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = true;
			MissFortune.spells.emplace_back(R);
			SpellDB.emplace_back(MissFortune);
		}

		{
			Champ Viego;
			Viego.hero = "Viego";

			Spell Q;
			Q.name = "ViegoQ";
			Q.icon = "ViegoQ";
			Q.displayName = "ViegoQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.range = 600;
			Q.delay = 0.4f;
			Q.radius = 70;
			Q.danger = 4;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = true;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = true;
			Viego.spells.emplace_back(Q);

			Spell W;
			W.name = "ViegoW1";
			W.icon = "ViegoW1";
			W.displayName = "ViegoWCast";
			W.missileName = "ViegoWMis";
			W.slot = _W;
			W.type = linear;
			W.speed = 1500;
			W.range = 1000;
			W.delay = 0.25;
			W.radius = 70;
			W.danger = 3;
			W.cc = false;
			W.collision = false;
			W.windwall = true;
			W.hitbox = false;
			W.fow = false;
			W.exception = false;
			W.extend = true;
			Viego.spells.emplace_back(W);

			Spell R;
			R.name = "ViegoR";
			R.icon = "ViegoR";
			R.displayName = "ViegoR";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 500;
			R.delay = 0.6f;
			R.radius = 280;
			R.danger = 3;
			R.cc = false;
			R.collision = false;
			R.windwall = true;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;
			Viego.spells.emplace_back(R);
			SpellDB.emplace_back(Viego);
		}

		{
			Champ Lucian;
			Lucian.hero = "Lucian";

			Spell Q;
			Q.name = "LucianQ";
			Q.icon = "LucianQ";
			Q.displayName = "Piercing Light";
			//Q.missileName = "ZileanQMissile";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = MathHuge;
			Q.delay = 0.4f;
			Q.range = 1000;
			Q.radius = 60;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;

			Spell W;
			W.name = "LucianW";
			W.icon = "LucianW";
			W.displayName = "Ardent Blaze";
			W.missileName = "LucianWMissile";
			W.slot = _W;
			W.type = linear;
			W.speed = 1600;
			W.range = 1350;
			W.delay = 0.25;
			W.radius = 80;
			W.danger = 2;
			W.cc = false;
			W.collision = true;
			W.windwall = true;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;

			Spell R;
			R.name = "LucianR";
			R.icon = "LucianR";
			R.displayName = "Lucian R";
			R.missileName = "LucianRMissile";
			R.slot = _R;
			R.type = linear;
			R.speed = MathHuge;
			R.range = 1400;
			R.radius = 120;
			R.danger = 2;
			R.cc = false;
			R.collision = true;
			R.windwall = true;
			R.hitbox = true;
			R.fow = true;
			R.exception = false;
			R.extend = true;

			Lucian.spells.emplace_back(Q);
			Lucian.spells.emplace_back(W);
			Lucian.spells.emplace_back(R);
			SpellDB.emplace_back(Lucian);
		}

		{
			Champ Sett;
			Sett.hero = "Sett";

			Spell W;
			W.name = "SettW";
			W.icon = "SettW";
			W.displayName = "Piercing Light";
			W.missileName = "SettW";
			W.slot = _W;
			W.type = linear;
			W.speed = MathHuge;
			W.delay = 0.75f;
			W.range = 785;
			W.radius = 80;
			W.danger = 2;
			W.cc = false;
			W.collision = false;
			W.windwall = false;
			W.hitbox = true;
			W.fow = true;
			W.exception = false;
			W.extend = true;

			Spell E;
			E.name = "SettE";
			E.icon = "SettE";
			E.displayName = "SettE";
			E.missileName = "SettE";
			E.slot = _E;
			E.type = linear;
			E.speed = MathHuge;
			E.range = 490;
			E.delay = 0.5f;
			E.radius = 160;
			E.danger = 2;
			E.cc = false;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Sett.spells.emplace_back(E);
			Sett.spells.emplace_back(W);
			SpellDB.emplace_back(Sett);
		}

		{
			Champ Zoe;
			Zoe.hero = "Zoe";

			Spell Q1;
			Q1.name = "ZoeQ";
			Q1.icon = "ZoeQ1";
			Q1.displayName = "Paddle Star [First]";
			Q1.missileName = "ZoeQMissile";
			Q1.slot = _Q;
			Q1.type = linear;
			Q1.speed = 1200;
			Q1.range = 800;
			Q1.delay = 0.25;
			Q1.radius = 50;
			Q1.danger = 1;
			Q1.cc = false;
			Q1.collision = true;
			Q1.windwall = true;
			Q1.hitbox = false;
			Q1.fow = true;
			Q1.exception = false;
			Q1.extend = true;

			Spell Q2;
			Q2.name = "ZoeQ2";
			Q2.icon = "ZoeQ2";
			Q2.displayName = "Paddle Star [Second]";
			Q2.missileName = "ZoeQMis2";
			Q2.slot = _Q;
			Q2.type = linear;
			Q2.speed = 2500;
			Q2.range = 10000;
			Q2.delay = 0;
			Q2.radius = 70;
			Q2.danger = 2;
			Q2.cc = false;
			Q2.collision = true;
			Q2.windwall = true;
			Q2.hitbox = false;
			Q2.fow = true;
			Q2.exception = true;
			Q2.extend = true;

			Spell E;
			E.name = "ZoeE";
			E.icon = "ZoeE";
			E.displayName = "Sleepy Trouble Bubble";
			E.missileName = "ZoeEMis";
			E.slot = _E;
			E.type = linear;
			E.speed = 1850;
			E.range = 800;
			E.delay = 0.3f;
			E.radius = 50;
			E.danger = 2;
			E.cc = true;
			E.collision = true;
			E.windwall = true;
			E.hitbox = false;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Zoe.spells.emplace_back(Q1);
			Zoe.spells.emplace_back(Q2);
			Zoe.spells.emplace_back(E);

			SpellDB.emplace_back(Zoe);
		}

		{
			Champ Zyra;
			Zyra.hero = "Zyra";
			Spell Q;
			Q.nameStrictly = true;
			Q.name = "ZyraQ";
			Q.icon = "ZyraQ";
			Q.displayName = "Deadly Spines";
			Q.slot = _Q;
			Q.type = rectangular;
			Q.speed = MathHuge;
			Q.range = 800;
			Q.delay = 0.85f;
			Q.radius = 140;
			Q.danger = 2;
			Q.cc = false;
			Q.collision = false;
			Q.windwall = false;
			Q.hitbox = false;
			Q.fow = false;
			Q.exception = false;
			Q.extend = false;

			Spell E;
			E.name = "ZyraE";
			E.icon = "ZyraE";
			E.displayName = "Grasping Roots";
			E.missileName = "ZyraE";
			E.slot = _E;
			E.type = linear;
			E.speed = 1150;
			E.range = 1150;
			E.delay = 0.25;
			E.radius = 70;
			E.danger = 1;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;
			//E.useMissile = true;

			Spell R;
			R.name = "ZyraR";
			R.icon = "ZyraR";
			R.displayName = "Stranglethorns";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 700;
			R.delay = 2.25;
			R.radius = 500;
			R.danger = 4;
			R.cc = true;
			R.collision = false;
			R.windwall = false;
			R.hitbox = true;
			R.fow = false;
			R.exception = false;
			R.extend = false;

			Zyra.spells.emplace_back(Q);
			Zyra.spells.emplace_back(E);
			Zyra.spells.emplace_back(R);

			SpellDB.emplace_back(Zyra);
		}

		{
			Champ KogMaw;
			KogMaw.hero = "KogMaw";
			Spell Q;
			Q.name = "KogMawQ";
			Q.icon = "KogMawQ";
			Q.displayName = "Caustic Spittle";
			Q.missileName = "KogMawQ";
			Q.slot = _Q;
			Q.type = linear;
			Q.speed = 1650;
			Q.range = 1175;
			Q.delay = 0.25;
			Q.radius = 70;
			Q.danger = 1;
			Q.cc = false;
			Q.collision = true;
			Q.windwall = true;
			Q.hitbox = true;
			Q.fow = true;
			Q.exception = false;
			Q.extend = true;

			Spell E;
			E.name = "KogMawVoidOozeMissile";
			E.icon = "KogMawE";
			E.displayName = "Void Ooze";
			E.missileName = "KogMawVoidOozeMissile";
			E.slot = _E;
			E.type = linear;
			E.speed = 1400;
			E.range = 1360;
			E.delay = 0.25;
			E.radius = 120;
			E.danger = 2;
			E.cc = true;
			E.collision = false;
			E.windwall = true;
			E.hitbox = true;
			E.fow = true;
			E.exception = false;
			E.extend = true;

			Spell R;
			R.name = "KogMawLivingArtillery";
			R.icon = "KogMawR";
			R.displayName = "Living Artillery";
			R.slot = _R;
			R.type = circular;
			R.speed = MathHuge;
			R.range = 1800;
			R.delay = 1;
			R.radius = 240;
			R.danger = 4;
			R.cc = false;
			R.collision = false;
			R.windwall = false;
			R.hitbox = false;
			R.fow = false;
			R.exception = false;
			R.extend = false;

			KogMaw.spells.emplace_back(Q);
			KogMaw.spells.emplace_back(E);
			KogMaw.spells.emplace_back(R);

			SpellDB.emplace_back(KogMaw);
		}
	}
};