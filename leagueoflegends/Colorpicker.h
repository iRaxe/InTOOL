#pragma once

#include "MenuComponent.h"

namespace UPasta {
	namespace SDK {
		class ColorPicker : public MenuComponent {
		public:
			ColorPicker(const char* name, const char* displayName, float col[4], std::function<void(ColorPicker*, bool)> callback);

			float R;
			float G;
			float B;
			float A;
			float* RGBA;

			std::function<void(ColorPicker*, bool)> Callback;
			void AddTooltip(const char* tooltip);

			Vector2 GetPosition();
			float GetWidth();
			float NeededWidth();
			void GetSave(json& j);
			void Draw();
			void WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos);
		};
	}
}