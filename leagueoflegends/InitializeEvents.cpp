#include "stdafx.h"
#include "Plugins.h"
#include "InitializeEvents.h"

namespace UPasta
{
	namespace EventsManager
	{
		namespace Initialization
		{
			void Initialization::OnInject()
			{
				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, scripts::champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }

				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, render::scriptsrender::drawings::Update); }
				__except (1) { LOG("[Event Handler - Add] Error in utilities drawings"); }
			}

			void Initialization::OnDraw()
			{
				// Render OnDraw logic of champion modules
				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, scripts::champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }


				__try { EventManager::AddEventHandler(EventManager::EventType::OnDraw, render::scriptsrender::drawings::Update); }
				__except (1) { LOG("[Event Handler - Add] Error in utilities drawings"); }

			}

			void Initialization::OnGameTick()
			{
				//Sono placeholder
				__try { EventManager::AddEventHandler(EventManager::EventType::OnGameTick, scripts::champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }

				__try { EventManager::AddEventHandler(EventManager::EventType::OnGameTick, render::scriptsrender::drawings::Update); }
				__except (1) { LOG("[Event Handler - Add] Error in utilities drawings"); }
			}

			void Initialization::OnMenu()
			{
				//Sono placeholder
				__try { EventManager::AddEventHandler(EventManager::EventType::OnGameTick, scripts::champions::RenderUpdate); }
				__except (1) { LOG("[Event Handler - Add] Error in champions drawings"); }

				__try { EventManager::AddEventHandler(EventManager::EventType::OnGameTick, render::scriptsrender::drawings::Update); }
				__except (1) { LOG("[Event Handler - Add] Error in utilities drawings"); }
			}

		}
		

		void EventsManager::Dispose()
		{


		}
	}
}