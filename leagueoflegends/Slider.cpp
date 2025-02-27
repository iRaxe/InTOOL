#include "stdafx.h"
#include "Slider.h"

#include "NewMenu.h"
#include "MenuSettings.h"
#include "MenuRenderer.h"

namespace UPasta
{
	namespace SDK
	{
		Slider::Slider(const char* name, const char* displayName, int defaultValue, int minimumValue, int maximumValue, int step, std::function<void(Slider*, int)> callback) {
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->MinimumValue = min(minimumValue, maximumValue);
			this->MaximumValue = max(maximumValue, minimumValue);
			this->Value = min(this->MaximumValue, max(this->MinimumValue, defaultValue));
			this->Step = step;
			this->Callback = callback;
			this->Tooltip[0] = 0;
		}

		void Slider::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void Slider::GetSave(json& j)
		{
			j[this->Name] = json{ {"value", this->Value} };
		}

		Vector2 Slider::GetPosition() {
			auto& components = this->Parent->Components;
			for (auto i = 0; i < components.size(); i++) {
				auto component = components[i];
				if (component == this) {
					return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
				}
			}
		}

		float Slider::GetWidth() {
			auto value = 0.0f;

			for (auto child : this->Parent->Children) {
				value = max(value, child->NeededWidth());
			}
			for (auto component : this->Parent->Components) {
				value = max(value, component->NeededWidth());
			}

			return max(MenuComponent::Width, value);
		}

		float Slider::NeededWidth() {
			return 110.0f + render::imFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 15.0f + render::imFont->CalcTextSizeA(16, FLT_MAX, 0.0f, std::to_string(this->MaximumValue).c_str()).x + 5.0f;
		}

		void Slider::Draw() {
			if (!this->Visible) {
				return;
			}

			auto textWidth = 30.0f + render::imFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->DisplayName).x;

			auto position = this->GetPosition();
			auto rect = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);
			auto box = Rect(rect.Position.x + this->GetWidth() - 120, rect.Position.y + 8, 20, rect.Height / 2);

			auto rectSfondo = Rect(position.x, position.y, this->GetWidth(), MenuComponent::Height);
			Renderer::AddRectangleFilled(rectSfondo, IM_COL32(0, 0, 0, 160));
			Renderer::AddRectangle(rectSfondo, IM_COL32(0, 0, 0, 160));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.x + 10.0f, rect.Position.y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
			ImGui::SliderInt(this->DisplayName, &this->Value, this->MinimumValue, this->MaximumValue, ImVec2(box.Position.x, box.Position.y));

			//TODO
			if (this->Tooltip[0] != 0)
			{
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


		void Slider::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos) {
			if (!this->Visible || !this->Parent->IsVisible()) {
				return;
			}

			auto position = this->GetPosition();
			auto textWidth = 10.0f + render::imFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->DisplayName).x;

			auto rect = Rect(position.x + textWidth, position.y, this->GetWidth(), MenuComponent::Height);

			if ((msg == WM_MOUSEMOVE && this->Interacting) || (msg == WM_LBUTTONDOWN && !this->Interacting && rect.Contains(cursorPos))) {
				this->Value = min(this->MaximumValue, max(this->MinimumValue, (int)((((cursorPos.x - rect.Position.x) / rect.Width) * (this->MaximumValue - this->MinimumValue) + this->MinimumValue) / this->Step) * this->Step));
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}

			if (msg == WM_LBUTTONDOWN && rect.Contains(cursorPos) || msg == WM_LBUTTONUP) {
				this->Interacting = msg == WM_LBUTTONDOWN;
			}
		}
	}
}