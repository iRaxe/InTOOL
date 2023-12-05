#pragma once

#include "../stdafx.h"

namespace render
{
	extern ImFont* imFont;

	float Distance(Vector2 vec1, Vector2 vec2);
	float Distance(Vector3 vec1, Vector3 vec2);
	Vector3 CalcAngle(Vector3 src, Vector3 dst);
	Vector3 Add(Vector3 src, Vector3 dst);
	Vector3 Subtract(Vector3 src, Vector3 dst);
	Vector3 Divide(Vector3 src, float num);
	Vector3 Multiply(Vector3 src, float num);
	float Magnitude(Vector3 vec);
	Vector3 Normalize(Vector3 src);
	Vector3 GetLine(Vector2 point1, Vector2 point2);
	Vector3 GetVector(Vector3 point1, Vector3 point2);
	float GetDistPointToLine(Vector2 point, Vector2 pointLine1, Vector2 pointLine2);
	bool IsVectorEqual(Vector3 vec1, Vector3 vec2);
	bool IsVector2Null(Vector2 vec);
	bool IsVector3Null(Vector3 vec);
	bool IsOnScreen(Vector2 pos);

	void* CreateTexture(void* data, int size, bool compressed);
	void FreeTexture(void* data);

	void RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center);
	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness);
	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments, bool filled = false);
	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness, bool filled = false);
	void RenderImage(ImTextureID pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, bool flipped = false, bool rounded = false, float rounding = 0.0f, ImDrawFlags_ roundingCornersFlags = ImDrawFlags_RoundCornersAll);

	void RenderTextWorld(const std::string& text, const Vector3& position, float size, uint32_t color, bool center);
	void RenderLineWorld(const Vector3& from, const Vector3& to, uint32_t color, float thickness);
	void RenderCircleWorld(const Vector3& worldPos, int numPoints, float radius, uint32_t color, float thickness, bool height = false, bool glow = false, bool filled = false, uint32_t fillColor = 0, float transparency = 1);
	void RenderPolygonWorld(const Geometry::Polygon& poly, uint32_t color, float thickness, bool filled = false, uint32_t fillColor = 0);
	void RenderArcWorld(const Vector3& worldPos, int numPoints, float radius, uint32_t color, float thickness, float arcSize, const Vector3& directionPos, bool dontDrawWalls);

	void RenderWardRange(const Vector3& position, uint32_t color, const float& range = 1100.f, bool brushes = true);

	void Texture2D(void* texture, const ImVec2& from, const ImVec2& to, bool rounded = false, float rounding = 4.0f);
	float AngleBetween3point(const Vector3& vec_3d, const Vector3& pos_ext, const Vector3& current_pos);
	void DrawArc(const Vector3& worldPos, float radius, float thickness, const uintptr_t color, int numSegments, float startAngle, float endAngle);
	void initAngles();
	void setQuality();

	void Init();
	void Update();

}