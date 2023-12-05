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

			void OnProcessManagement(uintptr_t state, SpellCast* cast)
			{
				if (cast->GetCasterHandle() != globals::localPlayer->GetHandle()) return;
				if (cast->IsAutoAttack())
				{
					switch ((int)state) {
						case 0:
						case 1344:
							Event::Publish(Event::OnBeforeAttack);
						break;

						case 50:
						case 107:
						case 119:
							Event::Publish(Event::OnAfterAttack);
						break;
					}
				}
				else
				{
					switch ((int)state) {
						case 0:
						case 1344:
							Event::Publish(Event::OnCreateMissile);
						break;

						case 50:
						case 107:
						case 119:
							Event::Publish(Event::OnDeleteMissile);
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
