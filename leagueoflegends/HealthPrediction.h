#pragma once
#include "global/structs.h"

namespace UPasta {
	namespace SDK {
		class IncomingAttack {
		public:
			Object* Source;
			Object* Target;
			Vector3 SourcePosition;
			float Delay;
			float AnimationTime;
			float MissileSpeed;
			float StartTime;
			float Damage;
			bool Processed;
			CombatType CombatType;

			IncomingAttack(Object* source, Object* target, Vector3 sourcePosition, float Delay, float AnimationTime, float missileSpeed, float startTime, float damage, enum CombatType type);
		};

		namespace HealthPrediction {
			extern std::vector<IncomingAttack> IncomingAttacks;

			void Initialize();
			void OnGameUpdate();
			void OnDeleteMissile(Object* unit);
			void OnProcessSpell(SpellInfo* unit, SpellData* castInfo);

			float GetHealthPrediction(Object* unit, int time, int delay);
			float LaneClearHealthPrediction(Object* unit, int time, int delay);
		}
	}
}
