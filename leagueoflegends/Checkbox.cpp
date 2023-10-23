#include "stdafx.h"
#include "CheckBox.h"
#include "NewMenu.h"
#include "MenuSettings.h"
#include "MenuRenderer.h"
#include "notification.h"

namespace UPasta
{
	namespace SDK
	{
		CheckBox::CheckBox(const char* name, const char* displayName, bool defaultValue, std::function<void(CheckBox*, bool)> callback)
		{
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->Value = defaultValue;
			this->Callback = callback;
			this->Tooltip[0] = 0;
		}

		void CheckBox::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void CheckBox::GetSave(json& j)
		{
			j[this->Name] = json{ {"value", this->Value} };
		}

		Vector2 CheckBox::GetPosition()
		{
			auto& components = this->Parent->Components;
			for (auto i = 0; i < components.size(); i++)
			{
				auto component = components[i];
				if (component == this)
				{
					return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
				}
			}
		}

		float CheckBox::GetWidth()
		{
			auto value = 0.0f;

			for (auto child : this->Parent->Children)
			{
				value = max(value, child->NeededWidth());
			}
			for (auto component : this->Parent->Components)
			{
				value = max(value, component->NeededWidth());
			}

			return max(MenuComponent::Width, value);
		}

		float CheckBox::NeededWidth()
		{
			return 10.0f + render::imFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + MenuComponent::Height;
		}

		void CheckBox::Draw()
		{
			if (!this->Visible)
			{
				return;
			}

			if (ImGui::Button("Button", ImVec2(ImGui::GetContentRegionMax().x - GetStyle().WindowPadding.x, 40)))
			{
				auto number = 0;
				if (number >= 4) number = 1; else number++;

				if (number == 1) ImGui::Notification({ ImGuiToastType_Success, 4000, "The design of the notification was made by PAST OWL\nLeaked on 19.10.2023" });
				if (number == 2) ImGui::Notification({ ImGuiToastType_Info, 4000, "The design of the notification was made by PAST OWL\nLeaked on 19.10.2023" });
				if (number == 3) ImGui::Notification({ ImGuiToastType_Warning, 4000, "The design of the notification was made by PAST OWL\nLeaked on 19.10.2023" });
				if (number == 4) ImGui::Notification({ ImGuiToastType_Error, 4000, "The design of the notification was made by PAST OWL\nLeaked on 19.10.2023" });
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);
			auto box = Rect(rect.Position.x + rect.Width - rect.Height, rect.Position.y, rect.Height, rect.Height);

			Renderer::AddRectangleFilled(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.x + 10.0f, rect.Position.y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
			Renderer::AddRectangleFilled(box, this->Value ? IM_COL32(160, 0, 0, 255) : IM_COL32(37, 37, 37, 255));
			Renderer::AddRectangle(box, IM_COL32(0, 0, 0, 255));
			Renderer::AddText(this->Value ? "ON" : "OFF", 14.0f, box, DT_CENTER | DT_VCENTER, IM_COL32(255, 255, 255, 255));

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
		}

		void CheckBox::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos)
		{
			if (msg != WM_LBUTTONUP || !this->Visible || !this->Parent->IsVisible())
			{
				return;
			}

			auto position = this->GetPosition();
			auto box = Rect(position.x + this->GetWidth() - MenuComponent::Height, position.y, MenuComponent::Height, MenuComponent::Height);

			if (box.Contains(cursorPos)) {
				this->Value = !this->Value;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}
		}
	}
}
