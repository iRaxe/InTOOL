#pragma once

#include <list>

#include "../stdafx.h"

namespace Modules
{

	void Init();
	void Update();

	namespace prediction
	{
		struct PredictionOutput
		{
			Vector3 position;
			float hitChance = 100.0f;
		};

		bool CheckCollision(Vector3 sourcePos, Vector3 targetPos, Object* sourceObject, Object* targetObject, Skillshot& skillshot);
		bool IsSpecificObjectInWay(Vector3 sourcePos, Vector3 targetPos, Object* collisionObject, float projectileRadius);
		int CountObjectsInWay(Vector3 sourcePos, Vector3 targetPos, Object* sourceObject, Alliance team, float pathRadius);

		bool IsAnyObjectInWay(Vector3 sourcePos, Vector3 targetPos, Object* sourceObject, Object* targetObject, float projectileRadius);

		Vector3 GetObjectPositionAfterTime(Object* obj, float time, float distanceBuffer = 0.0f);

		bool AoeCalc(Object* sourceObj, Skillshot skillshot, Modules::prediction::PredictionOutput& out);
		bool GetPrediction(Skillshot &spell, PredictionOutput& out);
		bool GetPrediction(Object* sourceObj, Object* targetObj, Skillshot &spell, PredictionOutput &out);
		Vector3 PredictTargetPosition(Object* targetObj, float predictionTime);

	}

	namespace Utilities
	{
		void Init();
	}

	namespace Champions
	{
		void Init();
		void Update();
		void RenderUpdate();
		void DoBeforeAttack();
		void DoAfterAttack();
		void DoCreateMissile();
		void DoDeleteMissile();
	}

}
