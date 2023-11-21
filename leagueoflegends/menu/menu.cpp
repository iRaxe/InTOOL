#include "../Awareness.h"
#include "../stdafx.h"
#include "../NewMenu.h" // THIS IS U S E L E S S NO GOOD
#include "../MenuRenderer.h"
#include "menu.h"

namespace menu
{

	int sub_tabs = 0;
	int active_tab = 0;
	float tab_alpha = 0.f;
	float tab_add;
	int tabs = 0;
	std::string activeSubTab = "Champion Settings";

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	
	ID3D11ShaderResourceView* platformLogo = nullptr;
	ImGuiStyle* style;

	float nextSaveTime = 10.0f;

	void Init()
	{
		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(12.0f, 14.0f);
		style->WindowRounding = 0.0f;
		style->FramePadding = ImVec2(10.0f, 4.0f);
		style->FrameRounding = 0.0f;
		style->ItemSpacing = ImVec2(12.0f, 16.0f);
		style->ItemInnerSpacing = ImVec2(14.0f, 4.0f);
		style->IndentSpacing = 0.0f;
		style->ScrollbarSize = 4.0f;
		style->ScrollbarRounding = 2.0f;
		style->GrabMinSize = 4.0f;
		style->GrabRounding = 0.0f;
		style->WindowBorderSize = 0.0f;
		style->PopupBorderSize = 1.0f;
		style->GrabMinSize = 14.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.85f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.80f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.60f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.61f, 0.61f, 0.61f, 0.54f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.85f, 0.85f, 0.85f, 0.54f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 0.75f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.42f, 1.0f, 0.93f, 0.75f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 1.0f, 0.93f, 0.92f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

		globals::menuSize = ImVec2(3000, 1280);

		ImGui::SetNextWindowSize(globals::menuSize, ImGuiCond_FirstUseEver);

		ImGui::GetIO().MouseDrawCursor = false;
		UPasta::SDK::Menu::Initialize();
		LOG("Menu initialized");
	}

	void InitNewMenu()
	{
		ImGui::GetIO().MouseDrawCursor = false;
		globals::menuSize = ImVec2(1980, 1280);
		UPasta::SDK::Menu::Initialize();
		LOG("Menu initialized");
	}
	
	void SaveSoon()
	{
		nextSaveTime = functions::GetGameTime() + 1.0f;
	}

	void TextCentered(std::string text)
	{
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}

	bool CustomCheckbox(const char* label, bool* isChecked)
	{
		ImGuiWindow* currentWindow = ImGui::GetCurrentWindow();
		if (currentWindow->SkipItems)
			return false;

		ImGuiContext& context = *GImGui;
		const ImGuiStyle& style = context.Style;
		const ImGuiID id = currentWindow->GetID(label);

		ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 valueSize = ImGui::CalcTextSize(SP_STRING("Off"), NULL, true);

		const float minWidgetWidth = 180.0f;
		const float widgetWidth = ImMax(ImMax(minWidgetWidth, labelSize.x + valueSize.x + 60.0f), ImGui::GetWindowSize().x);

		const float widgetHeight = ImMax(labelSize.y, valueSize.y);

		ImRect totalBoundingBox(currentWindow->DC.CursorPos, currentWindow->DC.CursorPos);

		if (!ImGui::ItemAdd(totalBoundingBox, id))
		{
			ImGui::TextColored(ImVec4(0.41f, 1.0f, 0.93f, 1.0f), std::to_string(id).c_str());
			return false;
		}

		bool isPressed = ImGui::InvisibleButton(label, ImVec2(widgetWidth - 25.0f, widgetHeight));
		if (isPressed)
			*isChecked = !(*isChecked);

		ImVec2 textPosition = totalBoundingBox.Min;
		ImGui::RenderText(textPosition, label);

		textPosition.x += widgetWidth - valueSize.x - 30.0f - ((*isChecked) ? 18.0f : 0.0f) + ((widgetWidth == ImGui::GetWindowSize().x) ? 1.0f : 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(*isChecked ? 0.2f : 8.0f, *isChecked ? 8.0f : 0.2f, 0.2f, 1.0f));
		ImGui::RenderText(textPosition, *isChecked ? SP_STRING("On") : SP_STRING("Off"));
		ImGui::PopStyleColor();

		if (*isChecked) {
			textPosition.x += valueSize.x + 5.0f;
			ImGui::RenderCheckMark(currentWindow->DrawList, textPosition, COLOR_GREEN, labelSize.y);
		}

		return isPressed;
	}
	
	void DrawMenu(std::pair<std::string, settings::SettingsGroup> group, std::pair<std::string, std::vector<std::string>> groupOrder)
	{
		std::string groupName = functions::CapitalizeFirstLetter(group.first);

		int id = 0;
		for (const auto& key : groupOrder.second)
		{
			auto it = group.second.find(key);
			if (it != group.second.end())
			{
				std::pair<std::string, settings::SettingValue> setting = *it;

				const std::string& key = setting.first;
				const settings::SettingValue& value = setting.second;

				if (std::holds_alternative<bool>(value))
				{
					bool boolValue = std::get<bool>(value);
					if (ImGui::Checkbox(key.c_str(), &boolValue))
						SaveSoon();
					settings::Set(group.first, setting.first, boolValue);
				}
				else if (std::holds_alternative<int>(value))
				{
					int intValue = std::get<int>(value);
					const auto bounds = settings::GetBoundsInt(group.first, key, std::pair<int, int>(0, 1));
					
					if (ImGui::SliderInt(key.c_str(), &intValue, bounds.first, bounds.second, "%d"))
						SaveSoon();

					settings::Set(group.first, setting.first, intValue);
				}
				else if (std::holds_alternative<float>(value))
				{
					float floatValue = std::get<float>(value);
					const auto bounds = settings::GetBoundsFloat(group.first, key, std::pair<float, float>(0.0f, 1.0f));

					if (ImGui::SliderFloat(("##" + key).c_str(), &floatValue, bounds.first, bounds.second, key.c_str())) 
						SaveSoon();

					settings::Set(group.first, setting.first, floatValue);
				}				

				if (!id && groupOrder.second.size() > 1 && key == SP_STRING("enabled"))
				{
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
					ImGui::Separator();
				}

				++id;
			}
		}

		if (groupName == SP_STRING("Debug"))
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.0f);
			ImGui::Separator();
		}
	}

	void DynamicTabs()
	{
		std::set<std::string> seenGroups; // Set per tenere traccia dei gruppi visti

		int counter = 1; // Contatore per tener traccia delle iterazioni

		ImGui::SetCursorPos(ImVec2(300, 20));
		ImGui::BeginGroup();
		{
			for (const auto& group : scripts::parentOrder)
			{
				// Verifica se il gruppo è già stato visto
				if (seenGroups.find(group.first) != seenGroups.end())
					continue;

				// Il gruppo non si è ancora ripetuto
				seenGroups.insert(group.first);
				if (ImGui::SubTab(group.first.c_str(), counter == sub_tabs, ImVec2(120, 50)))
				{
					sub_tabs = counter;
					activeSubTab = group.first.c_str();
				}

				counter++;
			}
		}
		ImGui::EndGroup();
	}

	void DynamicSettings()
	{
		std::set<std::string> seenGroups; // Set per tenere traccia dei gruppi visti
		int counter = 1; // Contatore per tener traccia delle iterazioni
		ImGui::SetCursorPos(ImVec2(280, 90));

		auto it2 = scripts::parentToGroupMap.find(activeSubTab.c_str());
		if (it2 == scripts::parentToGroupMap.end())
			return;

		const std::vector<std::string>& groups = it2->second;

		for (const std::string& group2 : groups)
		{
			auto it = settings::data.find(group2.c_str());
			if (it == settings::data.end())
				continue;

			// Trova la corrispondenza tra group2 e settingsOrder una volta
			auto groupIt = std::find_if(scripts::settingsOrder.begin(),
				scripts::settingsOrder.end(),
				[&](const auto& group)
				{
					return group2.length() == group.first.length();
				});

			if (groupIt != scripts::settingsOrder.end() && seenGroups.find(group2) == seenGroups.end())
			{
				seenGroups.insert(group2);

				ImGui::BeginGroup();
				{
					ImGui::BeginChild(group2.c_str(), ImVec2(320, 190), true);
					{
						DrawMenu(*it, *groupIt);
					}
					ImGui::EndChild();
				}
				ImGui::EndGroup();
				counter++;

				if (counter % 2 == 0)
					ImGui::SameLine();
				else
					ImGui::SetCursorPos(ImVec2(280, 90 + (counter / 2) * 210));
			}
		}
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

	void Update2()
	{
		ImGui::SetNextWindowSize(globals::menuSize);
		if (globals::menuOpen)
		{
			ImGui::Begin("UCPasta", &globals::menuOpen, window_flags);
			{
				const auto& p = ImGui::GetWindowPos();

				UPasta::SDK::Menu::DrawTabs();

				tab_alpha = ImClamp(tab_alpha + (7.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
				tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
				ImGui::SetCursorPos(ImVec2(280 - tab_alpha * 40, 88));
			}
			ImGui::End();
		}

	}
}