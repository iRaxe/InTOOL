#include "../stdafx.h"
#include "../NewMenu.h"
#include "../TargetSelector.h"
#include "../Awareness.h"

namespace render
{
	ImFont* imFont = NULL;
	void Init()
	{
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontDefault();
		imFont = io.Fonts->AddFontDefault();
		initAngles();
	}

	void Update()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });

		ImGui::Begin(SP_STRING("transparentwindow"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

		Event::Publish(Event::OnDraw); 
		

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->PushClipRectFullScreen();

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
	}

	void test()
	{
		QWORD tempAllObject = globals::moduleBase + UPasta::Offsets::Instance::Lists::ObjManager;
		QWORD dw0 = *(QWORD*)tempAllObject;
		if (!dw0)
			return;

		QWORD dw1 = *(QWORD*)(dw0 + 0x18);
		if (!dw1)
			return;

		QWORD dw2 = *(QWORD*)(dw0 + 0x20);
		if (!dw2)
			return;

		int countAllObj = (dw2 - dw1) / 8;
		if (!countAllObj)
			return;

		int jCount = 0;
		for (int k = 0; k < countAllObj; ++k)
		{
			QWORD dw3 = *(QWORD*)(dw1 + 8 * k);
			if (dw3 == 0)
				continue;

			if ((BYTE)dw3 & 1)
				continue;

			Object* object = (Object*)dw3;
			if (IsValidPtr(object) && IsNotZeroPtr(object) && object)
			{
				char* name = *reinterpret_cast<char**>(reinterpret_cast<QWORD>(object) + 0x60);
				if (IsValidPtr(name) && IsNotZeroPtr(name) && name)
					if (strstr(name, ".troy") != nullptr)
					{
						RenderCircleWorld(object->GetPosition(), 30, 200, COLOR_WHITE, 1.0f, false, false);
						RenderText(name, Engine::WorldToScreen(object->GetPosition()).ToImVec(), 24, COLOR_WHITE, true);
					}

			}
		}
	}
}