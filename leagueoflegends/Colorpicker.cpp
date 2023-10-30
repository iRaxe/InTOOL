#include "Awareness.h"
#include "stdafx.h"
#include "ColorPicker.h"
#include "NewMenu.h"
#include "MenuSettings.h"
#include "MenuRenderer.h"
#include "notification.h"
#include "Vayne.h"

namespace UPasta
{
    namespace SDK
    {
        ColorPicker::ColorPicker(const char* name, const char* displayName, float col[4], std::function<void(ColorPicker*, bool)> callback)
            : Callback(callback)
        {
            strncpy(this->Name, name, sizeof(this->Name));
            strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
            this->R = col[0];
            this->G = col[1];
            this->B = col[2];
            this->A = col[3];
            this->RGBA = col; // Ho rimosso RGBA e ho usato col direttamente
            this->Tooltip[0] = '\0'; // Inizializzazione della stringa vuota
        }

        void ColorPicker::AddTooltip(const char* tooltip)
        {
            strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
            this->Tooltip[sizeof(this->Tooltip) - 1] = '\0'; // Assicurati che la stringa sia terminata correttamente
        }

        Vector2 ColorPicker::GetPosition()
        {
            auto& components = this->Parent->Components;
            for (size_t i = 0; i < components.size(); i++)
            {
                if (components[i] == this)
                {
                    return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
                }
            }
            return Vector2(); // Restituisci un valore di default se il componente non è stato trovato
        }

        float ColorPicker::GetWidth()
        {
            float value = MenuComponent::Width;

            for (auto child : this->Parent->Children)
            {
                value = max(value, child->NeededWidth());
            }
            for (auto component : this->Parent->Components)
            {
                value = max(value, component->NeededWidth());
            }

            return value;
        }

        float ColorPicker::NeededWidth()
        {
            return 10.0f + render::imFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + MenuComponent::Height;
        }

        void ColorPicker::GetSave(json& j)
        {
            j[this->Name] = {
                {"R", this->RGBA[0]},
                {"G", this->RGBA[1]},
                {"B", this->RGBA[2]},
                {"A", this->RGBA[3]}
            };
        }

        bool multibox = false;
        void ColorPicker::Draw()
        {
            CheckPicker(this->DisplayName,this->Name, &multibox, this->RGBA);
        }

        void ColorPicker::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos)
        {
            if (msg != WM_LBUTTONUP || !this->Visible || !this->Parent->IsVisible())
            {
                return;
            }

            auto position = this->GetPosition();
            auto box = Rect(position.x + this->GetWidth() - MenuComponent::Height, position.y, MenuComponent::Height, MenuComponent::Height);

            if (box.Contains(cursorPos))
            {
                this->R = !this->R;
                if (this->Callback)
                    this->Callback(this, this->RGBA[0]);
                this->G = !this->G;
                if (this->Callback)
                    this->Callback(this, this->RGBA[1]);
                this->B = !this->B;
                if (this->Callback)
                    this->Callback(this, this->RGBA[2]);
                this->A = !this->A;
                if (this->Callback)
                    this->Callback(this, this->RGBA[3]);
            }
        }


    }
}
