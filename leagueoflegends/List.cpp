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

		void List::Draw()
		{
			const auto arraySize = this->Items.size();
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