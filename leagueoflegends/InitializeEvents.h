#pragma once

namespace UPasta
{
	namespace EventsManager
	{
		namespace Initialization
		{
			void OnInject();
			void OnProcessManagement(int state, SpellCast* spellCastInfo);
			void OnDraw();
			void OnGameTick();
			void OnMenu();
		}

		void InitializeOnInject();
		void InitializeOnDraw();
		void InitializeOnGameTick();
		void InitializeOnMenu();

		void Dispose();
	}
}