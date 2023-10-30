#include "stdafx.h"
#include "NewMenu.h"
#include "MenuSettings.h"
#include "MenuRenderer.h"
#include "Keybind.h"
#include "List.h"
#include "Slider.h"
#include "imgui.h"
#include <d3d11.h>
#include <tchar.h>
#pragma comment(lib, "D3DX11.lib")
#include <iostream>
#include <imgui_internal.h>
#include <string>

#include "Awareness.h"
#include "Colorpicker.h"
#include "uxtheme.h"
#include "Vayne.h"

#define WIDTH 850
#define HEIGHT 400
ID3D11ShaderResourceView* platformLogo = nullptr;
int tabs = 0;
int sub_tabs = 0;
int active_tab = 0;
float tab_alpha = 0.f;
float tab_add;
namespace UPasta {
	namespace SDK {
		Vector2 Menu::BasePosition = Vector2(30.0f, 30.0f);
		Vector2 Menu::DragPosition = Vector2();
		bool Menu::IsDragging = false;
		std::vector<Menu*> Menu::RootMenus;
		json Menu::MenuSave;

		Menu::Menu(const char* name, const char* displayName)
		{
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->Tooltip[0] = 0;
		}

		Menu::~Menu() {
			for (auto child : this->Children) {
				delete child;
			}

			for (auto component : this->Components) {
				delete component;
			}
		}

		void Menu::SetVisible(bool value) {
			this->Visible = value;

			if (!value) {
				this->Interacting = false;
				for (auto menu : this->Children) {
					menu->SetVisible(false);
				}
				for (auto component : this->Components) {
					component->Visible = false;
					component->Interacting = false;
				}
			}
		}

		void Menu::Initialize()
		{
			std::ifstream file("MenuComponent.save");
			if (file.is_open()) 
			{
				file >> MenuSave;
				auto& savedBasePositionX = MenuSave["Core settings"]["MenuSettings"]["BasePosition"]["X"];
				auto& savedBasePositionY = MenuSave["Core settings"]["MenuSettings"]["BasePosition"]["Y"];
				if (!savedBasePositionX.is_null() && !savedBasePositionY.is_null()) {
					BasePosition = Vector2(savedBasePositionX, savedBasePositionY);
				}
			}

			auto menu = Menu::CreateMenu("Core settings", "Core settings");

			auto menuSettings = menu->AddMenu("MainSettings", "Main Background Colors");
			menuSettings->AddColorPicker("background_image", "Logo Color", (float*)&colors::background::background_image);
			menuSettings->AddColorPicker("background", "Content Color", (float*)&colors::background::background);
			menuSettings->AddColorPicker("background_1", "Tabs Color", (float*)&colors::background::background_1);
			menuSettings->AddColorPicker("shadow", "Shadows Color", (float*)&colors::background::shadow);
			menuSettings->AddColorPicker("border", "Border Color", (float*)&colors::background::border);
			menuSettings->AddKeyBind("MenuKey", "Menu Key", VK_SHIFT, false, false, [](KeyBind*, bool value) 
				{
				MenuSettings::DrawMenu = value;
				});

			auto menuSettings2 = menu->AddMenu("ChildSettings", "Child Styling");
			menuSettings2->AddColorPicker("childText", "Text Color", (float*)&colors::child::text);
			menuSettings2->AddColorPicker("childBackground", "Background Color", (float*)&colors::child::child);
			menuSettings2->AddColorPicker("childBorder", "Border Color", (float*)&colors::child::border);
			menuSettings2->AddColorPicker("childSeparator", "Separator Color", (float*)&colors::child::border_line);

			auto menuSettings3 = menu->AddMenu("ScrollSettings", "Scrollbar Styling");
			menuSettings3->AddColorPicker("scrollBackground", "Background Color", (float*)&colors::scroll::background);
			menuSettings3->AddColorPicker("scrollColor", "Scrollbar Color", (float*)&colors::scroll::scroll);

			auto menuSettings4 = menu->AddMenu("CheckboxSettings", "Checkbox Styling");
			menuSettings4->AddColorPicker("checkboxBackground", "Background Color", (float*)&colors::checkbox::background);
			menuSettings4->AddColorPicker("checkboxActiveFlag", "Active Flag Color", (float*)&colors::checkbox::circle_active);
			menuSettings4->AddColorPicker("checkboxActiveText", "Active Text Color", (float*)&colors::checkbox::text_active);
			menuSettings4->AddColorPicker("checkboxHoverText", "Hover Text Color", (float*)&colors::checkbox::text_hov);
			menuSettings4->AddColorPicker("checkboxStandardText", "Text Color", (float*)&colors::checkbox::text);
			menuSettings4->AddColorPicker("checkboxInactiveFlag", "Inactive Flag Color", (float*)&colors::checkbox::circle_inactive);

			auto menuSettings5 = menu->AddMenu("sliderSettings", "Slider Styling");
			menuSettings5->AddColorPicker("sliderBackground", "Background Color", (float*)&colors::slider::background);
			menuSettings5->AddColorPicker("sliderStartGradient", "Start Gradient Color", (float*)&colors::slider::gradient_start);
			menuSettings5->AddColorPicker("sliderEndGradient", "End Gradient Color", (float*)&colors::slider::gradient_end);
			menuSettings5->AddColorPicker("sliderActiveFlag", "Active Flag Color", (float*)&colors::slider::circle_active);
			menuSettings5->AddColorPicker("sliderActiveText", "Active Text Color", (float*)&colors::slider::text_active);
			menuSettings5->AddColorPicker("sliderHoverText", "Hover Text Color", (float*)&colors::slider::text_hov);
			menuSettings5->AddColorPicker("sliderStandardText", "Text Color", (float*)&colors::slider::text);
			menuSettings5->AddColorPicker("sliderInactiveFlag", "Inactive Flag Color", (float*)&colors::slider::circle_inactive);

			auto menuSettings6 = menu->AddMenu("pickerSettings", "ColorPicker Styling");
			menuSettings6->AddColorPicker("pickerBackground", "Background Color", (float*)&colors::picker::background);
			menuSettings6->AddColorPicker("pickerShadow", "Shadow Color", (float*)&colors::picker::shadow);
			menuSettings6->AddColorPicker("pickerBorder", "Border Color", (float*)&colors::picker::border); 
			menuSettings6->AddColorPicker("pickerActiveText", "Active Text Color", (float*)&colors::picker::text_active);
			menuSettings6->AddColorPicker("pickerHoverText", "Hover Text Color", (float*)&colors::picker::text_hov);
			menuSettings6->AddColorPicker("pickerStandardText", "Text Color", (float*)&colors::picker::text);

			auto menuSettings7 = menu->AddMenu("comboSettings", "Combo Styling");
			menuSettings7->AddColorPicker("comboBackground", "Background Color", (float*)&colors::combo::background);
			menuSettings7->AddColorPicker("comboActiveFlag", "Active Flag Color", (float*)&colors::slider::circle_active);
			menuSettings7->AddColorPicker("comboActiveText", "Active Text Color", (float*)&colors::combo::text_active);
			menuSettings7->AddColorPicker("comboSelectableActiveText", "Selectable Active Text Color", (float*)&colors::combo::selectable::text_active);
			menuSettings7->AddColorPicker("comboHoverText", "Hover Text Color", (float*)&colors::combo::text_hov);
			menuSettings7->AddColorPicker("comboSelectableHoverText", "Selectable Hover Text Color", (float*)&colors::combo::selectable::text_hov);
			menuSettings7->AddColorPicker("comboStandardText", "Text Color", (float*)&colors::combo::text);
			menuSettings7->AddColorPicker("comboSelectableStandardText", "Selectable Text Color", (float*)&colors::combo::selectable::text);

			auto menuSettings8 = menu->AddMenu("keybindSettings", "Keybind Styling");
			menuSettings8->AddColorPicker("keybindBackground", "Background Color", (float*)&colors::keybind::color_rect);
			menuSettings8->AddColorPicker("keybindBorder", "Border Color", (float*)&colors::keybind::color_outline);
			menuSettings8->AddColorPicker("keybindStandardText", "Text Color", (float*)&colors::keybind::key_text);
			menuSettings8->AddColorPicker("keybindColor", "Key Color", (float*)&colors::keybind::key_rect);
			menuSettings8->AddColorPicker("keybindKeyBorder", "Key Border Color", (float*)&colors::keybind::key_outline);
		}

		void Menu::Dispose()
		{
			for (auto menu : RootMenus) 
			{
				menu->GetSave(MenuSave);
				delete menu;
			}
			RootMenus.clear();

			MenuSave["Core settings"]["MenuSettings"]["BasePosition"]["X"] = BasePosition.x;
			MenuSave["Core settings"]["MenuSettings"]["BasePosition"]["Y"] = BasePosition.y;

			std::ofstream file("MenuComponent.save");
			if (file.is_open()) 
			{
				file << MenuSave.dump(4);
			}
		}
		Menu* activeMenu = nullptr;
		//ImGui::GetColorU32(colors::background::border)

		void Menu::OnWndProc(UINT msg, WPARAM wparam)
		{
			if (IsDragging) {
				if (msg == WM_MOUSEMOVE) {
					BasePosition = DragPosition + functions::GetMousePos();
				}
				else if (msg == WM_LBUTTONUP) {
					IsDragging = false;
				}
			}

			for (auto menu : RootMenus) {
				menu->WndProc(msg, wparam, functions::GetMousePos());
			}
		}

		Menu* Menu::CreateMenu(const char* name, const char* displayName)
		{
			auto menu = new Menu(name, displayName);
			menu->Save = MenuSave[name];
			RootMenus.push_back(menu);
			return menu;
		}

		Menu* Menu::AddMenu(const char* name, const char* displayName) {
			auto menu = new Menu(name, displayName);
			menu->Save = this->Save[name];
			menu->Parent = this;
			this->Children.push_back(menu);
			return menu;
		}

		CheckBox* Menu::AddCheckBox(const char* name, const char* displayName, bool defaultValue, std::function<void(CheckBox*, bool)> callback) {
			auto component = new CheckBox(name, displayName, defaultValue, callback);
			component->Save = this->Save[name];
			auto& savedValue = component->Save["value"];
			if (!savedValue.is_null()) {
				component->Value = savedValue;
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		ColorPicker* Menu::AddColorPicker(const char* name, const char* displayName, float col[4], std::function<void(ColorPicker*, bool)> callback)
		{
			auto component = new ColorPicker(name, displayName, col, callback);
			component->Save = this->Save[name]; // Assegna la sezione di salvataggio appropriata
			auto& savedKeyR = component->Save["R"];
			if (!savedKeyR.is_null()) { component->RGBA[0] = savedKeyR; }

			auto& savedKeyG = component->Save["G"];
			if (!savedKeyG.is_null()) { component->RGBA[1] = savedKeyG; }

			auto& savedKeyB = component->Save["B"];
			if (!savedKeyB.is_null()) { component->RGBA[2] = savedKeyB; }

			auto& savedKeyA = component->Save["A"];
			if (!savedKeyA.is_null()) { component->RGBA[3] = savedKeyA; }
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		Info* Menu::AddInfo(const char* name, const char* displayName) {
			auto component = new Info(name, displayName);
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		KeyBind* Menu::AddKeyBind(const char* name, const char* displayName, unsigned char key, bool defaultValue, bool isToggle, std::function<void(KeyBind*, bool)> callback) {
			auto component = new KeyBind(name, displayName, key, defaultValue, isToggle, callback);
			component->Save = this->Save[name];
			auto& savedKey = component->Save["key"];
			if (!savedKey.is_null()) {
				component->Key = savedKey;
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		List* Menu::AddList(const char* name, const char* displayName, std::vector<std::string> items, unsigned int defaultIndex, std::function<void(List*, unsigned int)> callback) {
			auto component = new List(name, displayName, items, defaultIndex, callback);
			component->Save = this->Save[name];
			auto& savedValue = component->Save["value"];
			if (!savedValue.is_null()) {
				component->Value = min((unsigned int)savedValue, items.size() - 1);
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		Slider* Menu::AddSlider(const char* name, const char* displayName, int defaultValue, int minimumValue, int maximumValue, int step, std::function<void(Slider*, int)> callback) {
			auto component = new Slider(name, displayName, defaultValue, minimumValue, maximumValue, step, callback);
			component->Save = this->Save[name];
			auto& savedValue = component->Save["value"];
			if (!savedValue.is_null()) {
				component->Value = min(component->MaximumValue, max(component->MinimumValue, (int)savedValue));
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		bool Menu::IsVisible() {
			return MenuSettings::DrawMenu && (!this->Parent || this->Visible);
		}

		void Menu::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void Menu::GetSave(json& j)
		{
			j[this->Name] = {};

			for (auto child : this->Children) 
			{
				child->GetSave(j[this->Name]);
			}

			for (auto component : this->Components) {
				component->GetSave(j[this->Name]);
			}

			if (j[this->Name].is_null()) {
				j.erase(this->Name);
			}
		}

		Vector2 Menu::GetPosition()
		{
			if (this->Parent) {
				auto& children = this->Parent->Children;
				for (auto i = 0; i < children.size(); i++) 
				{
					auto child = children[i];
					if (child == this) {
						return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * i);
					}
				}
			}
			else {
				for (auto i = 0; i < RootMenus.size(); i++) {
					auto menu = RootMenus[i];
					if (menu == this) {
						return BasePosition + Vector2(0, MenuComponent::Height * i);
					}
				}
			}
		}

		int Menu::GetIndex()
		{
			if (this->Parent) {
				auto& children = this->Parent->Children;
				for (auto i = 0; i < children.size(); i++)
				{
					auto child = children[i];
					if (child == this)
						return i + 1;
				}
			}
			else {
				for (auto i = 0; i < RootMenus.size(); i++) {
					auto menu = RootMenus[i];
					if (menu == this) 
						return i + 1;
				}
			}
			return 0;
		}

		float Menu::GetWidth()
		{
			auto value = 0.0f;

			if (this->Parent) {
				for (auto child : this->Parent->Children) {
					value = max(value, child->NeededWidth());
				}
				for (auto component : this->Parent->Components) {
					value = max(value, component->NeededWidth());
				}
			}
			else {
				for (auto menu : RootMenus) {
					value = max(value, menu->NeededWidth());
				}
			}

			return max(MenuComponent::Width, value);
		}

		float Menu::NeededWidth() {
			return 10.0f + render::imFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + MenuComponent::Height * 0.45f;
		}
		ImVec2 menu_size = { 900, 400 };

		void Menu::DrawTabs()
		{
			if (!MenuSettings::DrawMenu)
				return;

			int counter = 1;
			const auto& p = ImGui::GetWindowPos();
			const ImVec2 startPos(260 + p.x, p.y);
			const ImVec2 endPos(1290 + p.x, 90 + p.y);
			const ImVec2 borderStart(280 + p.x, p.y + 20);
			const ImVec2 borderEnd(1270 + p.x, 70 + p.y);

			ImGui::GetWindowDrawList()->AddRectFilled(startPos, endPos,ImGui::GetColorU32(colors::background::background_1), 7.f);
			ImGui::GetWindowDrawList()->AddRect(borderStart, borderEnd, ImGui::GetColorU32(colors::background::border), 4.f, 0, 2.f);

			ImGui::SetCursorPos(ImVec2(300, 20));

			for (auto menu : RootMenus)
			{
				if (ImGui::SubTab(menu->DisplayName, counter == sub_tabs, ImVec2(menu->GetWidth(), 50)))
				{
					sub_tabs = counter;
					activeMenu = menu;
				}
				counter++;
			}

			if (activeMenu != nullptr)
				activeMenu->DrawTabContent();
		}
		
		void Menu::DrawTabContent() const
		{
			if (!MenuSettings::DrawMenu)
				return;

			if (!this->Children.empty())
			{
				const auto& p = ImGui::GetWindowPos();
				const float tabStartY = 110 + p.y;
				constexpr float contentOffset = 20.0f;
				const float contentHeight = this->Children.size() > 6 ? 660.0f : this->Children.size() > 3 ? 460.0f : 260.0f;
				const ImVec2 contentStart(260 + p.x, tabStartY);
				const ImVec2 borderStart(contentStart.x + contentOffset, contentStart.y + contentOffset);

				const ImVec2 contentEnd(1290 + p.x, tabStartY + contentHeight);
				const ImVec2 borderEnd(contentEnd.x - contentOffset, contentEnd.y - contentOffset);

				ImGui::GetWindowDrawList()->AddRectFilled(contentStart, contentEnd, ImGui::GetColorU32(colors::background::background), 7.f);
				ImGui::GetWindowDrawList()->AddRect(borderStart, borderEnd, ImGui::GetColorU32(colors::background::border), 4.f, 0, 2.f);
				
				ImGui::SetCursorPos(ImVec2(300 - tab_alpha * 40, 150.0f));
				
				for (const auto child : this->Children)
				{
					child->DrawChild();
					if (child->GetIndex() < 3 || child->GetIndex() > 3 && child->GetIndex() < 6 || child->GetIndex() > 6)
						ImGui::SameLine();
					else if (child->GetIndex() == 3)
						ImGui::SetCursorPos(ImVec2(300 - tab_alpha * 40, 350.0f));
					else if (child->GetIndex() == 6)
						ImGui::SetCursorPos(ImVec2(300 - tab_alpha * 40, 550.0f));

				}
			}
		}

		void Menu::DrawChild() const
		{
			const int columnsQty = min(3, this->Parent->Children.size());
			const float areaWidth = 965.00f - (10.0f * columnsQty);

			ImGui::BeginGroup();
			{
				ImGui::BeginChild(this->DisplayName, ImVec2(areaWidth / columnsQty, 180.0f), true);
				{
					if (!this->Components.empty())
					{
						for (auto component : this->Components)
						{
							component->Draw();
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndGroup();
		}

		void Menu::Draw()
		{
			/*
			if (!this->Children.empty())
			{
				for (auto child : this->Children)
				{
					child->DrawChild();
				}
			}
			*/
		}

		/*void Menu::Draw()
		{
			auto position = this->GetPosition();
			auto rect = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);

			Renderer::AddRectangleFilled(rect, this->Interacting ? IM_COL32(160, 0, 0, MenuSettings::BackgroundOpacity) : IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.x + 10.0f, rect.Position.y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
			ImGui::SetCursorPos(ImVec2(300 - tab_alpha * 40, 150.0f));
			if (!this->Children.empty() || !this->Components.empty()) {
				auto p1 = Vector2(rect.Position.x + rect.Width - (int)(rect.Height * 0.2f), rect.Position.y + (int)(rect.Height * 0.5f));
				auto p2 = Vector2(rect.Position.x + rect.Width - (int)(rect.Height * 0.45f), rect.Position.y + (int)(rect.Height * 0.65f));
				auto p3 = Vector2(rect.Position.x + rect.Width - (int)(rect.Height * 0.45f), rect.Position.y + (int)(rect.Height * 0.35f));

				Renderer::AddTriangleFilled(p1, p2, p3, this->Interacting ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255));

				for (auto child : this->Children) 
				{
					child->DrawChild(this->Children.size());
				}

				for (auto component : this->Components) 
				{
					component->Draw();
				}
			}

			//TODO
			if (this->Tooltip[0] != 0)
			{
				auto textWidth = 10.0f + render::imFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->DisplayName).x;
				auto mousePos = functions::GetMousePos();
				auto iconRect = Rect(rect.Position.x + textWidth + 5, rect.Position.y + Height * 0.5f - 10.0f, 20, 20);
				Renderer::AddText("(?)", 16.0f, iconRect, DT_VCENTER, IM_COL32(255, 30, 30, 255));

				if (iconRect.Contains(mousePos))
				{
					auto alpha = min(MenuSettings::BackgroundOpacity + 70, 255);
					auto black = IM_COL32(0, 0, 0, alpha);
					auto width = 20.0f + render::imFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->Tooltip).x;
					auto tooltipRect = Rect(mousePos.x + 20, mousePos.y - Height * 0.5f, width, Height);
					Renderer::AddRoundedRectangleFilled(tooltipRect, black, 4, ImDrawFlags_RoundCornersAll);
					Renderer::AddRoundedRectangle(tooltipRect, black, 1.1f, 4, ImDrawFlags_RoundCornersAll);
					Renderer::AddText(this->Tooltip, 14.0f, Rect(tooltipRect.Position.x + 10.0f, tooltipRect.Position.y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
				}
			}
		}*/

		void Menu::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos)
		{
			for (auto child : this->Children) 
			{
				child->WndProc(msg, wparam, cursorPos);
			}

			for (auto component : this->Components) {
				component->WndProc(msg, wparam, cursorPos);
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);

			if (msg != WM_LBUTTONDOWN || !this->IsVisible() || !rect.Contains(cursorPos)) {
				return;
			}

			if (!IsDragging) 
			{
				IsDragging = true;
				DragPosition = BasePosition - cursorPos;
			}

			if (!this->Parent) 
			{
				for (auto menu : RootMenus) 
				{
					if (menu != this) 
					{
						menu->SetVisible(true);
					}
				}
			}
			else {
				for (auto menu : this->Parent->Children) {
					if (menu != this) {
						menu->Interacting = true;
						for (auto child : menu->Children) {
							child->SetVisible(true);
						}
						for (auto component : menu->Components) {
							component->Visible = true;
							component->Interacting = true;
						}
					}
				}
				for (auto component : this->Parent->Components) {
					component->Interacting = true;
				}
			}

			if (!this->Children.empty() || !this->Components.empty()) {
				this->Interacting = !this->Interacting;
			}

			for (auto child : this->Children) {
				child->SetVisible(!child->Visible);
			}

			for (auto component : this->Components) {
				component->Visible = !component->Visible;
				component->Interacting = true;
			}
		}

		MenuComponent* Menu::operator[](std::string key) {
			for (auto child : this->Children) {
				if (child->Name == key) {
					return child;
				}
			}

			for (auto component : this->Components) 
			{
				if (component->Name == key) {
					return component;
				}
			}

			return nullptr;
		}
	}
}