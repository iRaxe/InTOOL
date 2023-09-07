#pragma once
#include "EventManager.h"
#include "NewMenu.h"
#include "classes/classes.h"

#define PI 3.14159265f

namespace UPasta
{
	namespace Plugins
	{
		//change champ name
		namespace ChampionName
		{ 
			// When need set starting variables , events etc
			void Initialize();

			// When need destroy all (when game finish or unload)
			void Dispose();

			// Things that need to run together with the game logic
			void OnGameUpdate();

			// Things that need to run together with the draw logic
			void OnDraw();

			// Event that will be triggered once a missile will be created from memory
			void OnCreateMissile(Object* unit);

			// Event that will be triggered once a missile will be deleted from memory
			void OnDeleteMissile(Object* unit);

			// Event that will be triggered once a object will be created from memory
			void OnCreateObject(Object* unit);

			// Event that will be triggered once a object will be deleted from memory
			void OnDeleteObject(Object* unit);

			/*// Event that will be triggered once a specific unit does a specific action
			void OnIssueOrder(Object* unit, GameObjectOrder order, Vector3* position, Object* target);

			// Event that will be triggered once our player unit casts a specific spell
			void OnCastSpell(SpellbookClient* spellbook, SpellDataInst* pSpellInfo, kSpellSlot slot, Vector3* _end_position, Vector3* _start_position, DWORD netId);

			// Event that will be triggered once someone casts a specific spell
			void OnSpellCast(kSpellSlot slot);

			void OnDoCast(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnDoCastDelayed(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnProcessSpell(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnPlayAnimation(Object* ptr, char* name, float animationTime);

			void OnFinishCast(SpellCastInfo* castInfo, Object* object);

			void OnStopCast(Object* unit, StopCast args);

			void OnGapCloserSpell(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnInterruptibleSpell(SpellInfo* castInfo, SpellDataResource* spellData);

			void OnNewPath(NewPath args);*/
		}
	}
}