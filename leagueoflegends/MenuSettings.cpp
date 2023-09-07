#include "stdafx.h"
#include "MenuSettings.h"
#include "NewMenu.h"

namespace UPasta {
	namespace SDK {
		int MenuSettings::BackgroundOpacity = 160;
		bool MenuSettings::DrawMenu = false;
		int MenuSettings::TicksPerSecond = 30;
		bool MenuSettings::AntiAFK = true;
		bool MenuSettings::Zoom = true;
		bool MenuSettings::DeveloperMode = false;
	}
}