#include "../ObjManager.h"
#include "../stdafx.h"
#include "../TargetSelector.h"

namespace Modules::prediction
{
	bool CheckCollision(Vector3 sourcePos, Vector3 targetPos, Object* sourceObject, Object* targetObject, Skillshot& skillshot)
	{
		if (skillshot.IsCollidableWith(CollidableObjects::Objects) && Modules::prediction::IsAnyObjectInWay(sourcePos, targetPos, sourceObject, targetObject, skillshot.GetRadius()))
			return false;
		return true;
	}

	bool IsSpecificObjectInWay(Vector3 sourcePos, Vector3 targetPos, Object* collisionObject, float projectileRadius)
	{
		auto sourceToTarget = targetPos - sourcePos;
		sourceToTarget.y = 0.0f;
		auto distance = sourceToTarget.Length();

		auto objPos = collisionObject->GetPosition();
		auto sourceToObj = objPos - sourcePos;
		sourceToObj.y = 0.0f;
		if (sourceToObj.Length() > distance) return false;

		float dot1 = sourceToObj.DotProduct(sourceToTarget);
		float dot2 = sourceToTarget.DotProduct(sourceToTarget);

		if (dot1 < 0.0f) return false;

		float t = dot1 / dot2;

		Vector3 projection = sourcePos + (sourceToTarget * t);
		projection.y = 0.0;

		Vector3 distVector = objPos - projection;
		distVector.y = 0.0;

		if (distVector.Length() <= projectileRadius + collisionObject->GetBoundingRadius())
			return true;

		return false;
	}

	int CountObjectsInWay(Vector3 sourcePos, Vector3 targetPos, Object* sourceObject, Alliance team, float pathRadius)
	{
		int nCount = 0;

		if (sourceObject->IsHero()) {
			for (auto hero : ObjectManager::GetHeroesAs(team)) {
				if (!hero) continue;
				if (IsSpecificObjectInWay(sourcePos, targetPos, sourceObject, pathRadius)) {
					nCount += 1;
				}
			}
		}
		else if (sourceObject->IsMinion()) {
			for (auto minion : ObjectManager::GetMinionsAs(team)) {
				if (!minion) continue;
				if (IsSpecificObjectInWay(sourcePos, targetPos, sourceObject, pathRadius)) {
					nCount += 1;
				}
			}
		}

		return nCount;

	}


	bool IsAnyObjectInWay(Vector3 sourcePos, Vector3 targetPos, Object* sourceObject, Object* targetObject, float projectileRadius)
	{
		auto objectInWay = [&](auto& objectManager) {
			auto sourceToTarget = targetPos - sourcePos;
			sourceToTarget.y = 0.0f;
			auto distance = sourceToTarget.Length();

			for (auto obj : objectManager)
			{
				if (obj == targetObject || obj == sourceObject) continue;
				if (!obj->IsValidTarget()) continue;

				if (IsSpecificObjectInWay(sourcePos, targetPos, obj, projectileRadius))
					return true;
			}
			return false;
			};

		return objectInWay(*ObjectManager::GetMinionList()) || objectInWay(*ObjectManager::GetHeroList());
	}

	Vector3 GetObjectPositionAfterTime(Object* obj, float time, float distanceBuffer)
	{
		const auto aiManager = obj->GetAiManager();

		float speed = obj->GetMovementSpeed();
		if (aiManager->IsDashing())
		{
			speed = aiManager->GetDashSpeed();
		}

		std::vector<Vector3> waypoints = { obj->GetAiManager()->GetPosition() };
		auto futureWaypoints = aiManager->GetFutureSegments();
		for (auto waypoint : futureWaypoints)
			waypoints.push_back(waypoint);

		const int waypointsSize = (int)waypoints.size();

		if (!waypointsSize || !time || !aiManager->IsMoving())
			return obj->GetAiManager()->GetPosition();

		float distance = (speed * time) - distanceBuffer;

		for (int i = 1; i < waypointsSize; i++)
		{
			const float waydistance = waypoints[i - 1].Distance(waypoints[i]);
			if (waydistance >= distance)
			{
				return waypoints[i - 1].Extend(waypoints[i], distance);
			}
			if (i == waypointsSize - 1)
				return waypoints[i];
			distance = distance - waydistance;
		}

		return waypoints.front();
	}

	Vector3 PredictTargetPosition2(Object* targetObj, float predictionTime) {

		const auto path = targetObj->GetAiManager()->GetFutureSegments();
		const int countSegments = static_cast<int>(path.size());
		if (countSegments <= 0)
			return targetObj->GetAiManager()->GetPosition();

		const auto aiManager = targetObj->GetAiManager();
		for (int i = 0; i < countSegments; i++)
		{
			Vector3 currentPosition = path[i];
			const float velocityMagnitude = aiManager->GetVelocity();
			const Vector3 velocityVector = currentPosition * velocityMagnitude;
			const Vector3 predictedPosition = currentPosition + (velocityVector * predictionTime);
			return predictedPosition;
		}

		return targetObj->GetAiManager()->GetPosition();
	}

	Vector3 PredictTargetPosition(Object* targetObj, float predictionTime)
	{

		const auto aiManager = targetObj->GetAiManager();
		Vector3 currentPosition = aiManager->GetPosition();
		float velocityMagnitude = aiManager->GetVelocity(); // Velocity magnitude
		Vector3 direction = aiManager->GetDirection().Normalized();
		Vector3 velocityVector = direction * velocityMagnitude;
		Vector3 predictedPosition = currentPosition + (velocityVector * predictionTime);
		return predictedPosition;
	}

	bool AoeCalc(Object* sourceObj, Skillshot skillshot, Modules::prediction::PredictionOutput& out) {
		std::vector<Vector3> predictedPositions;

		// Get the predicted future positions of all targets within the skillshot range.
		for (auto hero : ObjectManager::GetHeroesAs(Alliance::Enemy)) {
			if (hero == nullptr) continue;
			if (sourceObj->GetDistanceTo(hero) > skillshot.GetRange() + hero->GetBoundingRadius() / 2) continue;

			Modules::prediction::PredictionOutput heroPrediction;
			if (GetPrediction(sourceObj, hero, skillshot, heroPrediction)) {
				predictedPositions.push_back(heroPrediction.position);
			}
}

		// If no targets are predicted within range.
		if (predictedPositions.empty()) {
			return false;
		}

		// Calculate the centroid of the predicted positions.
		Vector3 aggregatePosition = std::accumulate(predictedPositions.begin(), predictedPositions.end(), Vector3()) / static_cast<float>(predictedPositions.size());

		// Check if the centroid is within the skillshot's radius of enough targets.
		int count = 0;
		for (const auto& pos : predictedPositions) {
			if (aggregatePosition.Distance(pos) <= skillshot.GetRadius()) {
				count++;
			}
		}

		// Not within SpellShot range
		if (count == 0) {
			return false;
		}

		// Set the centroid as the optimal AoE position and return true.
		out.position = aggregatePosition;
		return true;
	}

	bool GetPrediction(Skillshot& skillshot, Modules::prediction::PredictionOutput& out)
	{

		auto target = TargetSelector::FindBestTarget(ObjectManager::GetLocalPlayer()->GetPosition(), skillshot.GetMaxRange());
		if (!target)
			return false;

		return GetPrediction(ObjectManager::GetLocalPlayer(), target, skillshot, out);
	}

	bool GetPrediction(Object* sourceObj, Object* targetObj, Skillshot& skillshot, Modules::prediction::PredictionOutput& out)
	{
		const auto sourcePos = sourceObj->GetAiManager()->GetPosition();
		const auto targetPos = targetObj->GetPosition();
		const auto targetAiManager = targetObj->GetAiManager();
		const float spellRadius = skillshot.GetRadius();

		float distance = sourcePos.Distance(targetObj->GetAiManager()->GetPosition());
		

		if (distance > skillshot.GetMaxRange())
			return false;

		if (!skillshot.GetSpeed())
		{
			float travelTime = (distance) + skillshot.GetCastTime();
			auto predictedPos = PredictTargetPosition(targetObj, travelTime);
			
			Vector3 directionToTarget = (targetPos - predictedPos).Normalized();
			Vector3 adjustment = directionToTarget * (spellRadius / 2.0f);
			predictedPos = predictedPos + adjustment;
			out.position = PredictTargetPosition(targetObj, travelTime);
			return CheckCollision(sourcePos, out.position, sourceObj, targetObj, skillshot);
		}

		auto waypoints = targetAiManager->GetFutureSegments();
		const int waypointsSize = (int)waypoints.size();

		if (!waypointsSize || !targetAiManager->IsMoving())
		{
			out.position = targetObj->GetAiManager()->GetPosition();
			return CheckCollision(sourcePos, out.position, sourceObj, targetObj, skillshot);
		}

		float travelTime = (distance / skillshot.GetSpeed()) + skillshot.GetCastTime();
		auto predictedPos = PredictTargetPosition(targetObj, travelTime);
		Vector3 directionToTarget = (targetPos - predictedPos).Normalized();
		Vector3 adjustment = directionToTarget * (spellRadius / 2.0f);
		predictedPos = predictedPos + adjustment;

		distance = predictedPos.Distance(sourcePos);
		float missileTime = (distance / skillshot.GetSpeed()) + skillshot.GetCastTime();

		while (std::abs(travelTime - missileTime) > 0.01f)
		{
			travelTime = missileTime;
			predictedPos = PredictTargetPosition(targetObj, travelTime);
			Vector3 directionToTarget = (targetPos - predictedPos).Normalized();
			Vector3 adjustment = directionToTarget * (spellRadius / 2.0f);
			predictedPos = predictedPos + adjustment;

			distance = predictedPos.Distance(sourcePos);
			if (distance > skillshot.GetMaxRange())
			{
				return false;
			}

			missileTime = (distance / skillshot.GetSpeed()) + skillshot.GetCastTime();
		}

		out.position = predictedPos;

		if (Engine::IsWall(out.position))
		{
			return false;
		}

		return CheckCollision(sourcePos, out.position, sourceObj, targetObj, skillshot);
	}
}