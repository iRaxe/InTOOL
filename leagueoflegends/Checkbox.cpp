#include "stdafx.h"
#include "CheckBox.h"
#include "NewMenu.h"
#include "MenuSettings.h"
#include "MenuRenderer.h"
#include "notification.h"
#include "Vayne.h"

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
			Checkbox(SP_STRING(this->DisplayName), &this->Value);
			
		}

		void CheckBox::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos)
		{
			if (msg != WM_LBUTTONUP || !this->Visible || !this->Parent->IsVisible())
			{
				return;
			}

			auto position = this->GetPosition();
			auto box = Rect(position.x + this->GetWidth() - MenuComponent::Height, position.y, MenuComponent::Height, MenuComponent::Height);

			if (box.Contains(cursorPos)) 
			{
				this->Value = !this->Value;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}
		}
	}
}
