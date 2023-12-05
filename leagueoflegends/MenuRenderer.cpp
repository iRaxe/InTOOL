#include "stdafx.h"
#include "MenuRenderer.h"

namespace UPasta
{
	ImDrawList* Renderer::DrawList;
	ImFont* Renderer::DefaultFont;

	void Renderer::Initialize()
	{
		ImGui::CreateContext();
		DrawList = ImGui::GetForegroundDrawList();
		DefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 15.0f);
		ImGui_ImplWin32_Init(hooks::impl::windowDX);
		ImGui_ImplDX9_Init(globals::pDeviceDX9var);
	}

	void Renderer::Initialize(IDirect3DDevice9* pdevice) {
		ImGui::CreateContext();
		DrawList = ImGui::GetForegroundDrawList();
		DefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 15.0f);
		ImGui_ImplWin32_Init(hooks::impl::windowDX);
		ImGui_ImplDX9_Init(pdevice);
	}

	void Renderer::Dispose() {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::PreReset() {
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void Renderer::PostReset() {
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	void Renderer::NewFrame() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void Renderer::RenderFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}


	void Renderer::AddCircle(Vector2 position, float radius, float thickness, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		window->DrawList->AddCircle(*(ImVec2*)&position, radius, color, 100, thickness);
	}

	void Renderer::AddCircle(Vector3 position, float radius, float thickness, uintptr_t color)
	{
		if ((color & IM_COL32_A_MASK) == 0)
			return;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->_Path.reserve(window->DrawList->_Path.Size + 100);

		Vector2 pos;
		for (auto i = 0; i <= 99; i++) 
		{
			auto angle = (float)i * IM_PI * 1.98f / 99.0f;
			
			position.x = position.x + ImCos(angle) * radius;
			position.z = position.z + ImSin(angle) * radius;
			
			Engine::WorldToScreen(position);

			window->DrawList->_Path.push_back(*(ImVec2*)&pos);
		}
		window->DrawList->PathStroke(color, true, thickness);
	}

	void Renderer::AddCircleFilled(Vector2 position, float radius, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddCircleFilled(*(ImVec2*)&position, radius, color, 100);
	}

	void Renderer::AddCircleFilled(Vector3 position, float radius, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddCircleFilled(Engine::WorldToScreen(position).ToImVec(), radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), 100);
	}

	void Renderer::AddLine(Vector2 line1, Vector2 line2, float thickness, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddLine(*(ImVec2*)&line1, *(ImVec2*)&line2, color, thickness);
	}

	void Renderer::AddRectangle(Vector2 position, float width, float height, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddRect(*(ImVec2*)&position, ImVec2(position.x + width, position.y + height), color, 4.f);
	}

	void Renderer::AddRectangle(Rect rectangle, uintptr_t color, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddRect(ImVec2(rectangle.Position.x, rectangle.Position.y), ImVec2(rectangle.Position.x + rectangle.Width, rectangle.Position.y + rectangle.Height), color, 0, ImDrawFlags_RoundCornersAll, thickness);
	}

	void Renderer::AddRoundedRectangle(Rect rectangle, uintptr_t color, float thickness, int rounding,
		int roundSettings)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddRect(ImVec2(rectangle.Position.x, rectangle.Position.y), ImVec2(rectangle.Position.x + rectangle.Width, rectangle.Position.y + rectangle.Height), color, rounding, roundSettings, thickness);
	}

	void Renderer::AddRectangleFilled(Vector2 position, float width, float height, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddRectFilled(*(ImVec2*)&position, ImVec2(position.x + width, position.y + height), color, 4.f);
	}

	void Renderer::AddRectangleFilled(Rect rectangle, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddRectFilled(ImVec2(rectangle.Position.x, rectangle.Position.y), ImVec2(rectangle.Position.x + rectangle.Width, rectangle.Position.y + rectangle.Height), color);
	}

	void Renderer::AddRoundedRectangleFilled(Rect rectangle, uintptr_t color, int rounding,
		int roundSettings)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddRectFilled(ImVec2(rectangle.Position.x, rectangle.Position.y), ImVec2(rectangle.Position.x + rectangle.Width, rectangle.Position.y + rectangle.Height), color, rounding, roundSettings);
	}

	void Renderer::AddText(const char* text, float size, Vector2 position, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddText(render::imFont, size, *(ImVec2*)&position, color, text);
	}

	void Renderer::AddText(float size, Vector2 position, uintptr_t color, const char* format, ...)
	{
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 256, format, args);
		va_end(args);
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddText(render::imFont, size, *(ImVec2*)&position, color, buffer);
	}

	void Renderer::AddText(const char* text, float size, Rect rectangle, uint32_t flags, uintptr_t color) {
		auto textSize = render::imFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text);
		auto position = ImVec2(rectangle.Position.x, rectangle.Position.y);

		if (flags & DT_CENTER) {
			position.x = rectangle.Position.x + (rectangle.Width - textSize.x) * 0.5f;
		}
		else if (flags & DT_RIGHT) {
			position.x = rectangle.Position.x + rectangle.Width - textSize.x;
		}

		if (flags & DT_VCENTER) {
			position.y = rectangle.Position.y + (rectangle.Height - textSize.y) * 0.5f;
		}
		else if (flags & DT_BOTTOM) {
			position.y = rectangle.Position.y + rectangle.Height - textSize.y;
		}
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddText(render::imFont, size, position, color, text);
	}

	void Renderer::AddText(float size, Rect rectangle, uint32_t flags, uintptr_t color, const char* format, ...) {
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 256, format, args);
		va_end(args);

		auto textSize = render::imFont->CalcTextSizeA(size, FLT_MAX, 0.0f, buffer);
		auto position = ImVec2(rectangle.Position.x, rectangle.Position.y);

		if (flags & DT_CENTER) {
			position.x = rectangle.Position.x + (rectangle.Width - textSize.x) * 0.5f;
		}
		else if (flags & DT_RIGHT) {
			position.x = rectangle.Position.x + rectangle.Width - textSize.x;
		}

		if (flags & DT_VCENTER) {
			position.y = rectangle.Position.y + (rectangle.Height - textSize.y) * 0.5f;
		}
		else if (flags & DT_BOTTOM) {
			position.y = rectangle.Position.y + rectangle.Height - textSize.y;
		}
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddText(render::imFont, size, position, color, buffer);
	}

	void Renderer::AddTriangle(Vector2 point1, Vector2 point2, Vector2 point3, float thickness, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddTriangle(*(ImVec2*)&point1, *(ImVec2*)&point2, *(ImVec2*)&point3, color, thickness);
	}

	void Renderer::AddTriangleFilled(Vector2 point1, Vector2 point2, Vector2 point3, uintptr_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->AddTriangleFilled(*(ImVec2*)&point1, *(ImVec2*)&point2, *(ImVec2*)&point3, color);
	}
}