#include "../stdafx.h"
#include "../NewMenu.h"
#include "menu.h"

namespace menu
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGuiStyle* style;

	void Init()
	{
		ImGui::GetIO().MouseDrawCursor = false;
		globals::menuSize = ImVec2(1980, 1280);
		UPasta::SDK::Menu::Initialize();
		LOG("Menu initialized");
	}

	void Update()
	{
		ImGui::SetNextWindowSize(globals::menuSize, ImGuiCond_FirstUseEver);

		if (globals::menuOpen)
		{
			ImGui::Begin("menu", &globals::menuOpen, window_flags);

			UPasta::SDK::Menu::OnDraw();

			ImGui::End();
		}
	}
}