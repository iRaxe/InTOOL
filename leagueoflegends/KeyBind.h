#pragma once

#include "MenuComponent.h"

namespace UPasta {
	namespace SDK {
		class KeyBind : public MenuComponent {
		public:
			KeyBind(const char* name, const char* displayName, int key, bool defaultValue, int mode, std::function<void(KeyBind*, bool)> callback);

			int Key;
			int Mode;
			bool Value;
			std::function<void(KeyBind*, bool)> Callback;

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