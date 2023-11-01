#include "../Awareness.h"
#include "../stdafx.h"
namespace render
{
	float Distance(Vector2 vec1, Vector2 vec2)
	{
		float x_d = (vec2.x - vec1.x);
		float y_d = (vec2.y - vec1.y);
		return sqrt((x_d * x_d) + (y_d * y_d));
	}

	float Distance(Vector3 vec1, Vector3 vec2)
	{
		float x_d = (vec2.x - vec1.x);
		float y_d = (vec2.y - vec1.y);
		float z_d = (vec2.z - vec1.z);
		return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
	}

	Vector3 CalcAngle(Vector3 src, Vector3 dst)
	{
		Vector3 angles;
		angles.x = (-(float)atan2(dst.x - src.x, dst.y - src.y)) / PI * 180.0f;
		angles.y = (atan2(dst.z - src.z, Distance(src, dst))) * 180.0f / PI;
		angles.z = 0.0f;
		return angles;
	}

	Vector3 Add(Vector3 src, Vector3 dst)
	{
		Vector3 sum;
		sum.x = src.x + dst.x;
		sum.y = src.y + dst.y;
		sum.z = src.z + dst.z;
		return sum;
	}

	Vector3 Subtract(Vector3 src, Vector3 dst)
	{
		Vector3 diff;
		diff.x = src.x - dst.x;
		diff.y = src.y - dst.y;
		diff.z = src.z - dst.z;
		return diff;
	}

	Vector3 Divide(Vector3 src, float num)
	{
		Vector3 vec;
		vec.x = src.x / num;
		vec.y = src.y / num;
		vec.z = src.z / num;

		return vec;
	}

	Vector3 Multiply(Vector3 src, float num)
	{
		Vector3 vec;
		vec.x = src.x * num;
		vec.y = src.y * num;
		vec.z = src.z * num;

		return vec;
	}

	float Magnitude(Vector3 vec)
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	Vector3 Normalize(Vector3 src)
	{
		Vector3 vec = Divide(src, Magnitude(src));
		return vec;
	}

	Vector3 GetLine(Vector2 point1, Vector2 point2)
	{
		return Vector3(point1.y - point2.y, point2.x - point1.x, point1.x * point2.y - point2.x * point1.y);
	}

	Vector3 GetVector(Vector3 point1, Vector3 point2)
	{
		return Vector3(point2.x - point1.x, point2.y - point1.y, point2.z - point1.z);
	}

	float GetDistPointToLine(Vector2 point, Vector2 pointLine1, Vector2 pointLine2)
	{
		Vector3 line = GetLine(pointLine1, pointLine2);
		return abs(line.x * point.x + line.y * point.y + line.z) / sqrt(line.x * line.x + line.y * line.y);
	}

	bool IsVectorEqual(Vector3 vec1, Vector3 vec2)
	{
		return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z;
	}

	bool IsVector2Null(Vector2 vec)
	{
		return vec.x == 0.0f && vec.y == 0.0f;
	}

	bool IsVector3Null(Vector3 vec)
	{
		return vec.x == 0.0f && vec.y == 0.0f && vec.z == 0.0f;
	}

	bool IsOnScreen(Vector2 pos)
	{
		return pos.x >= 0.0f && pos.y >= 0.0f && pos.x <= globals::windowWidth && pos.y <= globals::windowHeight;
	}

	void RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		ImVec2 textSize = imFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());

		float xOffset = (center) ? textSize.x / 2.0f : 0.0f;

		window->DrawList->AddText(imFont, size, { (position.x - xOffset) + 1.0f, (position.y + textSize.y) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());
		window->DrawList->AddText(imFont, size, { (position.x - xOffset) - 1.0f, (position.y + textSize.y) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());
		window->DrawList->AddText(imFont, size, { (position.x - xOffset) + 1.0f, (position.y + textSize.y) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());
		window->DrawList->AddText(imFont, size, { (position.x - xOffset) - 1.0f, (position.y + textSize.y) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());

		window->DrawList->AddText(imFont, size, { position.x - xOffset, position.y + textSize.y }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), text.c_str());
	}

	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
	}

	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments, thickness);
	}

	void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color, uint32_t segments)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), segments);
	}

	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
	}

	void RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
	}

	void RenderImage(ImTextureID pTexture, const ImVec2& from, const ImVec2& to, uint32_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddImage(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
	}

	void RenderImageHFlip(ImTextureID pTexture, const ImVec2& from, const ImVec2& to, uint32_t color)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddImage(pTexture, from, to, { 1.0f, 0.0f }, { 0.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
	}

	void RenderImageRounded(ImTextureID pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		window->DrawList->AddImageRounded(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
	}

	uint32_t k_angles_amount_2 = 150;
	std::vector<float> sine_table_2 = {};
	std::vector<float> cosine_table_2 = {};
	void initAngles()
	{
		cosine_table_2.resize(k_angles_amount_2);
		sine_table_2.resize(k_angles_amount_2);

		for (uint32_t i = 0; i < k_angles_amount_2; ++i)
		{
			cosine_table_2[i] = cosf(static_cast<float>(i) * 6.28318530718f / static_cast<float>(k_angles_amount_2 - 1));
			sine_table_2[i] = sinf(static_cast<float>(i) * 6.28318530718f / static_cast<float>(k_angles_amount_2 - 1));
		}
	}

	void setQuality(const uint32_t quality)
	{
		if (const uint32_t last_quality = k_angles_amount_2; last_quality != quality)
		{
			k_angles_amount_2 = quality;
			initAngles();
		}
	}

	float AngleBetween3point(const Vector3& vec_3d, const Vector3& pos_ext, const Vector3& current_pos)
	{
		float vec1_x = vec_3d.x - pos_ext.x;
		float vec1_y = vec_3d.y - pos_ext.y;
		float vec2_x = current_pos.x - pos_ext.x;
		float vec2_y = current_pos.y - pos_ext.y;

		float dot_product = vec1_x * vec2_x + vec1_y * vec2_y;
		float magnitude_vec1 = std::sqrt(vec1_x * vec1_x + vec1_y * vec1_y);
		float magnitude_vec2 = std::sqrt(vec2_x * vec2_x + vec2_y * vec2_y);

		float cos_angle = dot_product / (magnitude_vec1 * magnitude_vec2);
		float angle_radians = std::acos(cos_angle);

		return angle_radians;
	}

	void test(ImVec2 lineStart, ImVec2 lineEnd)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		const int NUM_LINES = 10;
		const float ALPHA_VALUE = 0.030f;
		const float BASE_LINE_WIDTH = 2.f;
		const float EXTRA_LINE_WIDTH = 5;

		for (int j = 1; j <= NUM_LINES; ++j)
		{
			ImVec2 lineStartOffset = { lineStart.x + j, lineStart.y + j };
			ImVec2 lineEndOffset = { lineEnd.x + j, lineEnd.y + j };

			ImColor lineColor = ImVec4(1.0f, 1.0f, 0.4f, ALPHA_VALUE);
			float lineWidth = BASE_LINE_WIDTH + j + EXTRA_LINE_WIDTH;

			window->DrawList->AddLine(lineStartOffset, lineEndOffset, lineColor, lineWidth);

			lineStartOffset = { lineStart.x - j, lineStart.y - j };
			lineEndOffset = { lineEnd.x - j, lineEnd.y - j };

			window->DrawList->AddLine(lineStartOffset, lineEndOffset, lineColor, lineWidth);
		}
	}

	void RenderCircleWorld(const Vector3& worldPos, int numPoints, float radius, uintptr_t color, float thickness, bool height, bool glow)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		const int NUM_LINES = 3;
		const float BASE_LINE_WIDTH = 1.f;
		const float EXTRA_LINE_WIDTH = 3;

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		numPoints = min(numPoints, 49);
		ImVec2 points[50];

		float step = 6.2831f / numPoints;
		float theta = 0.f;

		for (int i = 0; i < numPoints; i++, theta += step)
		{
			const Vector3 worldSpace = { worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) };
			const ImVec2 screenSpace = functions::WorldToScreen(worldSpace).ToImVec();
			points[i].x = screenSpace.x;

			if (height)
				points[i].y = screenSpace.y - min(max(-UPasta::SDK::Awareness::Configs::Radius::heightTollerance->Value, functions::GetHeightAtPosition(worldSpace)), UPasta::SDK::Awareness::Configs::Radius::heightTollerance->Value);
			else
				points[i].y = screenSpace.y;
		}

		points[numPoints] = points[0];
		window->DrawList->AddPolyline(points, numPoints, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), true, 1.0f);
		if (glow)
		{
			for (int j = 1; j <= NUM_LINES; ++j)
			{
				for (int i = 0; i < numPoints; i++)
				{
					ImVec2 lineStartOffset = { points[i].x + j, points[i].y + j };
					ImVec2 lineEndOffset = { points[(i + 1) % numPoints].x + j, points[(i + 1) % numPoints].y + j };

					window->DrawList->AddLine(lineStartOffset, lineEndOffset, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, 0.030f }), BASE_LINE_WIDTH + j + EXTRA_LINE_WIDTH);
				}
			}
		}
	}

	void RenderWardRange(const Vector3& position, const ImColor& color, const float& range, bool brushes)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		// todo, improve this so it doesnt count only own brush
		if (brushes)
		{
			if (functions::IsBrush(position))
				brushes = false;
		}

		int numPoints = 30;
		static ImVec2 points[30];

		const float step = 6.2831f / numPoints;
		float theta = 0.f;

		for (int i = 0; i < numPoints; i++, theta += step)
		{
			Vector3 p = Vector3(0.f, 0.f, 0.f);

			for (float step2 = 20.f; step2 <= range; step2 += 20.f)
			{
				Vector3 p2 = Vector3(position.x + (step2 * cos(theta)), position.y, position.z - (step2 * sin(theta)));
				if (functions::IsWall(p2) || step2 == range || (brushes && functions::IsBrush(p2)))
				{
					p = p2;
					break;
				}
			}
			const ImVec2 screenSpace = functions::WorldToScreen(p).ToImVec();
			points[i].x = screenSpace.x;
			points[i].y = screenSpace.y;
		}

		points[numPoints] = points[0];
		window->DrawList->AddPolyline(points, numPoints, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), true, 1.0f);
	}

	/*void RenderCircleWorld(const Vector3& worldPos, int numPoints, float radius, uintptr_t color, float thickness, bool height)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		//window->DrawList->_FringeScale = 2.0f;

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		numPoints = min(numPoints, 49);
		ImVec2 points[50];

		float step = 6.2831f / numPoints;
		float theta = 0.f;
		for (int i = 0; i < numPoints; i++, theta += step)
		{
			const Vector3 worldSpace = { worldPos.x + radius * cos(theta), worldPos.y , worldPos.z - radius * sin(theta) };
			const ImVec2 screenSpace = functions::WorldToScreen(worldSpace).ToImVec();
			points[i].x = screenSpace.x;

			if (height)
				points[i].y = screenSpace.y - min(max(-UPasta::SDK::Awareness::Configs::Radius::heightTollerance->Value, functions::GetHeightAtPosition(worldSpace)), UPasta::SDK::Awareness::Configs::Radius::heightTollerance->Value);
			else
				points[i].y = screenSpace.y;
		}

		points[numPoints] = points[0];

		window->DrawList->AddPolyline(points, numPoints, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), true, 1.0f);

	}*/

	void RenderPolygon(const Geometry::Polygon poly, uintptr_t color, float thickness)
	{
		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);


		static ImVec2 points[200];
		int i = 0;
		for (const auto& point : poly.Points)
		{
			ImVec2 pos = functions::WorldToScreen(point).ToImVec();
			points[i].x = pos.x;
			points[i].y = pos.y;
			i++;
		}

		ImGui::GetBackgroundDrawList()->AddPolyline(points, i, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), true, thickness);

	}

	void RenderFilledPolygon(const Geometry::Polygon poly, uintptr_t color)
	{
		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);


		static ImVec2 points[200];
		int i = 0;
		for (const auto& point : poly.Points)
		{
			ImVec2 pos = functions::WorldToScreen(point).ToImVec();
			points[i].x = pos.x;
			points[i].y = pos.y;
			i++;
		}
		ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(points, i, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }));
	}

	void RenderArcWorld(const Vector3& worldPos, int numPoints, float radius, uintptr_t color, float thickness, float arcSize, const Vector3& directionPos, bool dontDrawWalls)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		float a = (float)((color >> 24) & 0xff);
		float r = (float)((color >> 16) & 0xff);
		float g = (float)((color >> 8) & 0xff);
		float b = (float)((color) & 0xff);

		Vector3 dir = directionPos - worldPos;

		float angle = atan2(dir.z, dir.x) * -1;
		float startTheta = angle - arcSize / 2.0f;
		float endTheta = angle + arcSize / 2.0f;

		numPoints = min(numPoints, 49);
		std::vector<Vector2> points = {};

		float thetaStep = (endTheta - startTheta) / numPoints;
		float theta = startTheta;
		for (int i = 0; i <= numPoints; i++, theta += thetaStep)
		{
			Vector3 worldSpace = { worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) };
			Vector2 screenSpace = (dontDrawWalls && functions::IsWall(worldSpace)) ? Vector2() : functions::WorldToScreen(worldSpace);

			points.push_back(screenSpace);
		}

		for (int i = 0; i < points.size() - 1; i++)
		{
			if (!points[i].IsValid() || !points[i + 1].IsValid()) continue;

			auto from = points[i].ToImVec();
			auto to = points[i + 1].ToImVec();
			window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
		}
	}
}