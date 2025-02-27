#pragma once

#include "MenuComponent.h"

namespace UPasta {
	namespace SDK {
		class List : public MenuComponent {
		public:
			List(const char* name, const char* displayName, std::vector<std::string> items, int defaultIndex, std::function<void(List*, unsigned int)> callback);

			std::vector<std::string> Items;
			int Value;
			std::function<void(List*, unsigned int)> Callback;

			void AddTooltip(const char* tooltip);
			void GetSave(json& j);
			Vector2 GetPosition();
			float GetWidth();
			float NeededWidth();
			void Draw();
			void WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos);
		};
	}
}