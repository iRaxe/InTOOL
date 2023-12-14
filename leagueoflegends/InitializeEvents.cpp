#include "stdafx.h"
#include "Plugins.h"
#include "InitializeEvents.h"

#include "ListManager.h"
#include "OnProcessSpellCast.h"

namespace UPasta
{
	namespace EventsManager
	{
		namespace Initialization
		{
			void OnInject()
			{
				__try { Skillshot::PopulateSpellsDB(); }
				__except (1) { LOG("[Event Handler - Add] Error in populating spells database"); }

				__try { Engine::Init(); }
				__except (1) { LOG("[Event Handler - Add] Error in initializing functions"); }

				__try { SDK::Menu::Initialize(); }
				__except (1) { LOG("[Event Handler - Add] Error in initializing menu"); }

				__try { render::Init(); }
				__except (1) { LOG("[Event Handler - Add] Error in initializing render features"); }

				__try { Modules::Init(); }
				__except (1) { LOG("[Event Handler - Add] Error in initializing scripts"); }

				__try { SDK::ListManager::Functions::Initialize(); }
				__except (1) { LOG("[Event Handler - Add] Error in initializing sdk functions"); }

				__try { HookOnProcessSpellCast(); }
				__except (1) { LOG("[Event Handler - Add] Error in hooking OnProcessSpellSound"); }

				__try { Event::Subscribe(Event::OnProcessSpell, &OnProcessManagement); }
				__except (1) { LOG("[Event Handler - Add] Error in hooking OnProcessSpellSound"); }
			}

			void OnProcessManagement(int state, SpellCast* spellCastInfo) {
				if (spellCastInfo == nullptr) return;
				if (spellCastInfo->IsAutoAttack())
				{
					switch (state) {
						case 12:
							Event::Publish(Event::OnBeforeAttack, spellCastInfo);
						break;

						case 17:
							Event::Publish(Event::OnAfterAttack, spellCastInfo);
						break;
					}
				}
				else
				{
					switch (state) {
						case 12:
							Event::Publish(Event::OnCreateSpell, spellCastInfo);
						break;

						case 13:
							Event::Publish(Event::OnImpactSpell, spellCastInfo);
						break;

						case 17:
							Event::Publish(Event::OnDeleteSpell, spellCastInfo);
						break;
					}
				}
			}

			void OnDraw()
			{
				// Render OnDraw logic of champion modules
				__try { Event::Subscribe(Event::OnDraw, Modules::Champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }
			}

			void OnGameTick()
			{
				//Sono placeholder
				__try { Event::Subscribe(Event::OnGameTick, Modules::Champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }

			}

			void OnMenu()
			{
				//Sono placeholder
				__try { Event::Subscribe(Event::OnGameTick, Modules::Champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }
			}

		}
		

		void EventsManager::Dispose()
		{

			UnHookOnProcessSpellCast();

		}
	}
}
