#include "stdafx.h"
#include "List.h"
#include "NewMenu.h"
#include "MenuSettings.h"
#include "MenuRenderer.h"

namespace UPasta {
	namespace SDK {

		List::List(const char* name, const char* displayName, std::vector<std::string> items, int defaultIndex, std::function<void(List*, unsigned int)> callback) {
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->Items = items;
			this->Value = min(defaultIndex, items.size() - 1);
			this->Callback = callback;
			this->Tooltip[0] = 0;
		}

		void List::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void List::GetSave(json& j) {
			j[this->Name] = json{ {"value", this->Value} };
		}

		Vector2 List::GetPosition() {
			auto& components = this->Parent->Components;
			for (auto i = 0; i < components.size(); i++) {
				auto component = components[i];
				if (component == this) {
					return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
				}
			}
		}

		float List::GetWidth() {
			auto value = 0.0f;

			for (auto child : this->Parent->Children) {
				value = max(value, child->NeededWidth());
			}
			for (auto component : this->Parent->Components) {
				value = max(value, component->NeededWidth());
			}

			return max(MenuComponent::Width, value);
		}

		float List::NeededWidth() {
			std::string longestItem;
			auto len = 0;
			for (auto item : this->Items) {
				auto itemLen = item.length();
				if (!len || itemLen > len) {
					longestItem = item;
					len = itemLen;
				}
			}
			return 10.0f + render::imFont->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, this->DisplayName).x + 10.0f + render::imFont->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, longestItem.c_str()).x + 5.0f + MenuComponent::Height * 2.0f;
		}

		void List::Draw() {
			if (!this->Visible) {
				return;
			}
			/*const auto arraySize = this->Items.size();
			if (arraySize <= 0)
				return;

			const char* combo_preview_value = this->Items[this->Value].c_str();
			if (ImGui::BeginCombo(this->DisplayName, combo_preview_value, 0))
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					const bool is_selected = (this->Value == static_cast<int>(i));
					if (ImGui::Selectable(this->Items[i].c_str(), is_selected))
					{
						this->Value = static_cast<int>(i);
					}
				}
				ImGui::EndCombo();
			}*/
			auto position = this->GetPosition();
			auto rect = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);
			auto leftBox = Rect(rect.Position.x + rect.Width - rect.Height * 2.0f, rect.Position.y, rect.Height, rect.Height);
			auto rightBox = Rect(rect.Position.x + rect.Width - rect.Height, rect.Position.y, rect.Height, rect.Height);

			Renderer::AddRectangleFilled(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangleFilled(leftBox, IM_COL32(160, 0, 0, 255));
			Renderer::AddRectangle(leftBox, IM_COL32(0, 0, 0, 255));
			Renderer::AddText("<<", 14.0f, leftBox, DT_CENTER | DT_VCENTER, IM_COL32(255, 255, 255, 255));
			Renderer::AddRectangleFilled(rightBox, IM_COL32(160, 0, 0, 255));
			Renderer::AddRectangle(rightBox, IM_COL32(0, 0, 0, 255));
			Renderer::AddText(">>", 14.0f, rightBox, DT_CENTER | DT_VCENTER, IM_COL32(255, 255, 255, 255));
			Renderer::AddText(this->Items[this->Value].c_str(), 14.0f, Rect(rect.Position.x, rect.Position.y, leftBox.Position.x - rect.Position.x - 5.0f, rect.Height), DT_VCENTER | DT_RIGHT, IM_COL32(255, 255, 255, 255));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.x + 10.0f, rect.Position.y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));

			//TODO
			if (this->Tooltip[0] != 0)
			{
				auto textWidth = 10.0f + render::imFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->DisplayName).x;
				auto mousePos = Engine::GetMousePos();
				auto iconRect = Rect(rect.Position.x + textWidth + 5, rect.Position.y + Height * 0.5f - 10.0f, 20, 20);
				Renderer::AddText("(?)", 16.0f, iconRect, DT_VCENTER, IM_COL32(255, 30, 30, 255));

				if (iconRect.Contains(mousePos))
				{
					auto alpha = min(MenuSettings::BackgroundOpacity + 70, 255);
					auto black = IM_COL32(0, 0, 0, alpha);
					auto width = 20.0f + render::imFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->Tooltip).x;
					auto tooltipRect = Rect(mousePos.x + 20, mousePos.y - Height * 0.5f, width, Height);
					Renderer::AddRoundedRectangleFilled(tooltipRect, black, 4, ImDrawCornerFlags_All);
					Renderer::AddRoundedRectangle(tooltipRect, black, 1.1f, 4, ImDrawCornerFlags_All);
					Renderer::AddText(this->Tooltip, 14.0f, Rect(tooltipRect.Position.x + 10.0f, tooltipRect.Position.y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
				}
			}
		}

		void List::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos) {
			if (msg != WM_LBUTTONUP || !this->Visible || !this->Parent->IsVisible()) {
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);
			auto leftBox = Rect(rect.Position.x + rect.Width - rect.Height * 2.0f, rect.Position.y, rect.Height, rect.Height);
			auto rightBox = Rect(rect.Position.x + rect.Width - rect.Height, rect.Position.y, rect.Height, rect.Height);

			if (leftBox.Contains(cursorPos)) {
				this->Value = this->Value == 0 ? this->Items.size() - 1 : this->Value - 1;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}
			else if (rightBox.Contains(cursorPos)) {
				this->Value = this->Value == this->Items.size() - 1 ? 0 : this->Value + 1;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}
		}
	}
}