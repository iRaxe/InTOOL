#pragma once
#include "../imgui/imgui.h"
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0xff000000
#define COLOR_YELLOW 0xFFFFFF00
#define COLOR_RED 0xffff1f1f
#define COLOR_GREEN 0xff0c9d00
#define COLOR_BLUE 0xff7ea6ff
#define COLOR_TURQUOISE 0xff6BFFEE
#define COLOR_PURPLE 0xff9C26FC

#define COLOR_GRAY 0xff4C4A4C
#define COLOR_DARK 0xff090509
#define COLOR_DARK_TRANSPARENT 0x80090509
#define COLOR_ORANGE 0xffde7642
#define COLOR_LIGHT_GREEN 0xff6BDB63

namespace colors {

	namespace utility {
		inline ImVec4 circle_user = ImColor(37, 37, 37, 255);
		inline ImVec4 logo_color = ImColor(164, 41, 42, 150);
	}

	namespace background {
		inline ImVec4 background_image = ImColor(14, 14, 14, 0);
		inline ImVec4 background = ImColor(0, 0, 0, 0);
		inline ImVec4 background_1 = ImColor(0, 0, 0, 0);
		inline ImVec4 shadow = ImColor(50, 50, 50, 255);
		inline ImVec4 border = ImColor(30, 30, 30, 255);

	}

	namespace child {

		inline ImVec4 text = ImColor(255, 255, 255, 255);
		inline ImVec4 child = ImColor(0, 0, 0, 0);
		inline ImVec4 border = ImColor(30, 30, 30, 255);
		inline ImVec4 border_line = ImColor(57, 72, 153, 255);

	}

	namespace scroll {

		inline ImVec4 background = ImColor(0, 0, 0, 0);
		inline ImVec4 scroll = ImColor(57, 72, 153, 255);

	}

	namespace button {

		inline ImVec4 background_active = ImColor(57, 72, 153, 255);
		inline ImVec4 background_hov = ImColor(20, 25, 29, 0);
		inline ImVec4 background = ImColor(20, 25, 29, 0);

		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(24, 0, 255, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);
	}

	namespace checkbox {

		inline ImVec4 background = ImColor(23, 24, 29, 255);

		inline ImVec4 circle_active = ImColor(57, 72, 153, 255);
		inline ImVec4 circle_inactive = ImColor(52, 58, 57, 255);

		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(75, 75, 77, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);

	}

	namespace slider {

		inline ImVec4 background = ImColor(23, 24, 29, 255);

		inline ImVec4 gradient_start = ImColor(57, 72, 153, 255);
		inline ImVec4 gradient_end = ImColor(57, 72, 153, 255);

		inline ImVec4 circle_active = ImColor(57, 72, 153, 255);
		inline ImVec4 circle_inactive = ImColor(57, 72, 153, 255);

		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(75, 75, 77, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);

	}

	namespace picker {

		inline ImVec4 background = ImColor(20, 20, 20, 255);

		inline ImVec4 shadow = ImColor(8, 8, 8, 255);
		inline ImVec4 border = ImColor(5, 5, 5, 255);

		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(75, 75, 77, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);

	}

	namespace combo {

		inline ImVec4 background = ImColor(20, 20, 20, 255);
		inline ImVec4 circle = ImColor(57, 72, 153, 255);

		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(75, 75, 77, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);

		namespace selectable {

			inline ImVec4 mark_active = ImColor(57, 72, 153, 255);

			inline ImVec4 text_active = ImColor(255, 255, 255, 255);
			inline ImVec4 text_hov = ImColor(75, 75, 77, 255);
			inline ImVec4 text = ImColor(255, 255, 255, 255);

		}

	}

	namespace keybind {

		inline ImVec4 key_text = ImColor(107, 107, 107, 255);
		inline ImVec4 color_rect = ImColor(24, 25, 27, 255);
		inline ImVec4 color_outline = ImColor(24, 25, 27, 255);

		inline ImVec4 key_rect = ImColor(24, 25, 27, 255);
		inline ImVec4 key_outline = ImColor(24, 25, 27, 255);

	}

}