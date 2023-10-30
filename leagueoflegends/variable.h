
namespace var {

	namespace player_esp {

		float color_edit0[4] = { 255 / 255.f, 77 / 255.f, 77 / 255.f, 200 / 255.f };

		const char* items[]{ "GAYWEND", "SMG", "LR90", "BOW" };
		int selectedItem = 0;

		const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };
		bool multi_items_count[5];

		bool checkbox = false;
		bool multibox = true;

		int slider_int = 300;
		int slider_int1 = 100;
		float slider_float = 1.f;
	}

	namespace world_esp {

	}

	namespace player_chams {

		static float color_edit0[4] = { 1.00f, 0.00f, 0.00f, 0.700f };
		static float color_edit1[4] = { 0.00f, 1.00f, 0.00f, 0.700f };
		static float color_edit2[4] = { 0.00f, 0.00f, 1.00f, 0.700f };
	}

	namespace aimbot {


	}
}