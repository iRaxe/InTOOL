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

	void setQuality()
	{
		if (const uint32_t last_quality = k_angles_amount_2; last_quality != UPasta::SDK::Awareness::Configs::Radius::qualityDraw->Value)
		{
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

	void CalculateSinesAndCosines(int numPoints, std::vector<float>& sines, std::vector<float>& cosines)
	{
		float step = 6.28318530718f / numPoints;
		sines.resize(numPoints);
		cosines.resize(numPoints);
		for (int i = 0; i < numPoints; ++i) 
		{
			float theta = step * i;
			sines[i] = std::sin(theta);
			cosines[i] = std::cos(theta);
		}
	}

	void RenderCircleWorld(const Vector3& worldPos, int numPoints, float radius, uintptr_t color, float thickness, bool height, bool glow) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window == nullptr || window->SkipItems)
			return;

		constexpr int MAX_POINTS = 100;
		ImVec2 points[MAX_POINTS];

		numPoints = min(numPoints, MAX_POINTS - 1);

		// Utilizza la funzione di cache per i seni e i coseni
		static std::vector<float> sines, cosines;
		CalculateSinesAndCosines(numPoints, sines, cosines);

		float currentTime = ImGui::GetTime(); // Ottieni il tempo corrente di ImGui
		float rainbowSpeed = 0.5f; // Velocità di cambiamento dei colori, regolabile

		for (int i = 0; i <= numPoints; ++i) {
			// Usa i valori memorizzati nella cache
			float cosTheta = cosines[i % numPoints];
			float sinTheta = sines[i % numPoints];

			Vector3 worldSpace = {
				worldPos.x + radius * cosTheta,
				worldPos.y, // adjust for height if necessary
				worldPos.z - radius * sinTheta
			};
			ImVec2 screenSpace = functions::WorldToScreen(worldSpace).ToImVec();

			points[i] = ImVec2(screenSpace.x, screenSpace.y);
		}
		/*
		for (int i = 0; i < numPoints; ++i) {
			float hue = fmodf(currentTime * rainbowSpeed + (float)i / numPoints, 1.0f);
			ImVec4 rainbowColor = ImColor::HSV(hue, 1.0f, 1.0f); // Genera il colore HSV
			ImU32 lineColor = ImGui::ColorConvertFloat4ToU32(rainbowColor);

			// Disegna ogni segmento con un colore univoco
			window->DrawList->AddLine(points[i], points[(i + 1) % numPoints], lineColor, thickness);
		}*/
		window->DrawList->AddPolyline(points, numPoints + 1, color, true, thickness);

		if (glow) {
			for (int i = 0; i < numPoints; ++i) {
				ImVec2 p1 = points[i];
				ImVec2 p2 = points[i + 1];
				for (int j = 1; j <= 1; ++j) {
					float hue = fmodf(currentTime * rainbowSpeed + (float)i / numPoints, 1.0f);
					ImVec4 rainbowColor = ImColor::HSV(hue, 1.0f, 1.0f); // Genera il colore HSV
					ImU32 glowColorRainbow = ImGui::ColorConvertFloat4ToU32(rainbowColor);
					window->DrawList->AddLine(p1, p2, glowColorRainbow, thickness + j * 1);
				}
			}
		}
	}

	void RenderWardRange(const Vector3& position, const ImColor& color, const float& range, bool brushes) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window == nullptr || window->SkipItems)
			return;

		constexpr int numPoints = 120;
		ImVec2 points[numPoints];

		// Utilizza la funzione di cache per i seni e i coseni
		static std::vector<float> sines, cosines;
		CalculateSinesAndCosines(numPoints, sines, cosines);

		for (int i = 0; i < numPoints; ++i) {
			float cosTheta = cosines[i];
			float sinTheta = sines[i];
			Vector3 p = position;

			for (float step2 = 20.f; step2 <= range; step2 += 20.f) {
				p.x = position.x + (step2 * cosTheta);
				p.z = position.z - (step2 * sinTheta);

				if (functions::IsWall(p) || step2 == range || (brushes && functions::IsBrush(p))) {
					break;
				}
			}

			points[i] = functions::WorldToScreen(p).ToImVec();
		}

		for (int i = 0; i < numPoints; i += 3) {
			const int next = (i + 3) % numPoints;
			window->DrawList->AddBezierCubic(points[i], points[(i + 1) % numPoints], points[(i + 2) % numPoints], points[next], color, 1.0f);
		}
	}
	
	void RenderPolygon(const Geometry::Polygon& poly, uintptr_t color, float thickness)
	{
		ImVec2 points[100]; // Assuming 100 is the upper limit of points we want to draw
		const int numPoints = min(static_cast<int>(poly.Points.size()), 100);

		for (int i = 0; i < numPoints; ++i) {
			points[i] = functions::WorldToScreen(poly.Points[i]).ToImVec();
		}

		ImGui::GetBackgroundDrawList()->AddPolyline(points, numPoints, color, true, thickness);
	}

	void RenderFilledPolygon(const Geometry::Polygon& poly, uintptr_t color) {
		ImVec2 points[100]; // Assuming 100 is the upper limit of points we want to draw
		const int numPoints = min(static_cast<int>(poly.Points.size()), 100);

		for (int i = 0; i < numPoints; ++i) {
			points[i] = functions::WorldToScreen(poly.Points[i]).ToImVec();
		}

		ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(points, numPoints, color);
	}

	void RenderArcWorld(const Vector3& worldPos, int numPoints, float radius, uintptr_t color, float thickness, float arcSize, const Vector3& directionPos, bool dontDrawWalls) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window == nullptr || window->SkipItems)
			return;

		Vector3 dir = directionPos - worldPos;
		float angle = atan2(dir.z, dir.x) * -1;
		float startTheta = angle - arcSize / 2.0f;
		float endTheta = angle + arcSize / 2.0f;

		numPoints = min(numPoints, 49);  // Limit the number of points to a fixed max value

		// Pre-calculate sine and cosine values for the arc
		static std::vector<float> sines, cosines;
		CalculateSinesAndCosines(numPoints, sines, cosines);

		float thetaStep = (endTheta - startTheta) / numPoints;
		float currentTheta = startTheta;

		ImVec2 lastValidPoint;
		bool lastPointValid = false;

		// Loop through each point in the arc
		for (int i = 0; i <= numPoints; ++i) {
			// Find the sine and cosine based on the current angle in the loop
			float sinTheta = sin(currentTheta);
			float cosTheta = cos(currentTheta);
			Vector3 worldSpace = { worldPos.x + radius * cosTheta, worldPos.y, worldPos.z - radius * sinTheta };

			// Check for walls if necessary
			if (dontDrawWalls && (functions::IsWall(worldSpace) || functions::IsBrush(worldSpace) )) {
				lastPointValid = false;  // Invalidate this point if it's a wall
			}
			else {
				ImVec2 screenSpace = functions::WorldToScreen(worldSpace).ToImVec();

				// If we have a valid 'last point', and the current point is valid, draw the line segment
				if (lastPointValid) {
					window->DrawList->AddLine(lastValidPoint, screenSpace, color, thickness);
				}

				lastValidPoint = screenSpace;  // Update the last valid point
				lastPointValid = true;         // Mark the current point as valid
			}

			currentTheta += thetaStep;  // Increment the angle
		}
	}
}