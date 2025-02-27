#pragma once

#include "stdafx.h"
#include "Rect.h"


namespace UPasta {
	namespace Renderer {
		extern ImDrawList* DrawList;
		extern ImFont* DefaultFont;

		void Initialize();
		void Initialize(IDirect3DDevice9* pdevice);
		void Dispose();
		void PreReset();
		void PostReset();
		void NewFrame();
		void RenderFrame();

		void AddCircle(Vector2 position, float radius, float thickness = 1.0f, uintptr_t color = 0xFFFFFFFF);
		void AddCircle(Vector3 position, float radius, float thickness = 1.0f, uintptr_t color = 0xFFFFFFFF);
		void AddCircleFilled(Vector2 position, float radius, uintptr_t color = 0xFFFFFFFF);
		void AddCircleFilled(Vector3 position, float radius, uintptr_t color = 0xFFFFFFFF);
		//AddImage
		void AddLine(Vector2 line1, Vector2 line2, float thickness = 1.0f, uintptr_t color = 0xFFFFFFFF);
		void AddRectangle(Vector2 position, float width, float height, uintptr_t color = 0xFFFFFFFF);
		void AddRectangle(Rect rectangle, uintptr_t color = 0xFFFFFFFF, float thickness = 1);
		void AddRoundedRectangle(Rect rectangle, uintptr_t color = 0xFFFFFFFF, float thickness = 1, int rounding = 0, int roundSettings = ImDrawFlags_RoundCornersAll);
		void AddRectangleFilled(Vector2 position, float width, float height, uintptr_t color = 0xFFFFFFFF);
		void AddRectangleFilled(Rect rectangle, uintptr_t color = 0xFFFFFFFF);
		void AddRoundedRectangleFilled(Rect rectangle, uintptr_t color = 0xFFFFFFFF, int rounding = 0, int roundSettings = ImDrawFlags_RoundCornersAll);
		void AddText(const char* text, float size, Vector2 position, uintptr_t color);
		void AddText(float size, Vector2 position, uintptr_t color, const char* format, ...);
		void AddText(const char* text, float size, Rect rectangle, uint32_t flags, uintptr_t color);
		void AddText(float size, Rect rectangle, uint32_t flags, uintptr_t color, const char* format, ...);
		void AddTriangle(Vector2 point1, Vector2 point2, Vector2 point3, float thickness = 1.0f, uintptr_t color = 0xFFFFFFFF);
		void AddTriangleFilled(Vector2 point1, Vector2 point2, Vector2 point3, uintptr_t color = 0xFFFFFFFF);
	}
}