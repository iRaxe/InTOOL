#include "stdafx.h"
#include "HealthPrediction.h"
#include "EventManager.h"
#include "Damage.h"

namespace UPasta {
	namespace SDK {
		std::vector<IncomingAttack> HealthPrediction::IncomingAttacks;

		IncomingAttack::IncomingAttack(Object* source, Object* target, Vector3 sourcePosition, float Delay, float AnimationTime, float missileSpeed, float startTime, float damage, enum CombatType type) {
			this->Source = source;
			this->Target = target;
			this->SourcePosition = sourcePosition;
			this->Delay = Delay;
			this->AnimationTime = AnimationTime;
			this->MissileSpeed = missileSpeed;
			this->StartTime = startTime;
			this->Damage = damage;
			this->Processed = false;
			this->CombatType = type;
		}

		void HealthPrediction::Initialize() {
			//Aggiungere eventi agli eventi lul
			Event::Subscribe(Event::OnGameTick, OnGameUpdate);
			Event::Subscribe(Event::OnProcessSpell, OnProcessSpell);
		}

		void HealthPrediction::OnGameUpdate() {
			auto time = Engine::GetGameTick();
			for (auto it = IncomingAttacks.begin(); it != IncomingAttacks.end();) {
				if (it->StartTime < time - 3000) 
					IncomingAttacks.erase(it);
				else
					it++;
				
			}
		}

		void HealthPrediction::OnProcessSpell(SpellInfo* castInfo, SpellData* spellData)
		{
			if (castInfo == nullptr || spellData == nullptr)
				return;

			
		}

		float HealthPrediction::GetHealthPrediction(Object* unit, int time, int delay = 70)
		{
			auto predictedDamage = 0.0f;

			if (unit == nullptr)
				return 0.0f;

		}

		float HealthPrediction::LaneClearHealthPrediction(Object* unit, int time, int delay = 70)
		{
			
		}
	}
}
