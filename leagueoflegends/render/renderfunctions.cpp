#include "../Awareness.h"
#include "../stdafx.h"
#include "../image.h"
#include "../decompress.h"

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

	void* CreateTexture(void* data, int size, bool compressed) {

		ID3D11ShaderResourceView* pShaderResource = nullptr;

		int image_width = 0;
		int image_height = 0;

		const unsigned int buf_decompressed_size = stb_decompress_length(static_cast<unsigned char*>(data));
		auto buf_decompressed_data = static_cast<unsigned char*>(malloc(buf_decompressed_size));
		stb_decompress(buf_decompressed_data, static_cast<unsigned char*>(data), size);

		unsigned char* image_data = stbi_load_from_memory(compressed ? buf_decompressed_data : static_cast<unsigned char*>(data),
			compressed ? buf_decompressed_size : size,
			&image_width,
			&image_height,
			nullptr,
			4);

		// Create texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		globals::pDeviceDX11var->CreateTexture2D(&desc, &subResource, &pTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		globals::pDeviceDX11var->CreateShaderResourceView(pTexture, &srvDesc, &pShaderResource);
		pTexture->Release();

		stbi_image_free(image_data);

		return (void*)pShaderResource;
		return nullptr;
	}

	void FreeTexture(void* data) {

		auto p = static_cast<ID3D11ShaderResourceView*>(data);
		p->Release();

	}

	ImVec4 ConvertColor(uint32_t color) {
		const float a = static_cast<float>((color >> 24) & 0xff) / 255.0f;
		const float r = static_cast<float>((color >> 16) & 0xff) / 255.0f;
		const float g = static_cast<float>((color >> 8) & 0xff) / 255.0f;
		const float b = static_cast<float>((color) & 0xff) / 255.0f;
		return ImVec4(r, g, b, a);
	}

	void RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
	{
		const ImGuiWindow* window = ImGui::GetCurrentWindow();
		const float a = static_cast<float>((color >> 24) & 0xff) / 255.0f;

		const ImVec2 textSize = imFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());

		const float xOffset = (center) ? textSize.x / 2.0f : 0.0f;

		window->DrawList->AddText(imFont, size, { (position.x - xOffset) + 1.0f, (position.y + textSize.y) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());
		window->DrawList->AddText(imFont, size, { (position.x - xOffset) - 1.0f, (position.y + textSize.y) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());
		window->DrawList->AddText(imFont, size, { (position.x - xOffset) + 1.0f, (position.y + textSize.y) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());
		window->DrawList->AddText(imFont, size, { (position.x - xOffset) - 1.0f, (position.y + textSize.y) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), text.c_str());

		window->DrawList->AddText(imFont, size, { position.x - xOffset, position.y + textSize.y }, ImGui::GetColorU32(ConvertColor(color)), text.c_str());
	}

	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
	{
		const ImGuiWindow* window = ImGui::GetCurrentWindow();

		window->DrawList->AddLine(from, to, ImGui::GetColorU32(ConvertColor(color)), thickness);
	}

	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness, uint32_t segments, bool filled)
	{
		const ImGuiWindow* window = ImGui::GetCurrentWindow();

		if (filled)
			window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32(ConvertColor(color)), segments);
		else
			window->DrawList->AddCircle(position, radius, ImGui::GetColorU32(ConvertColor(color)), segments, thickness);
	}

	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness, bool filled)
	{
		const ImGuiWindow* window = ImGui::GetCurrentWindow();

		if (filled)
			window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32(ConvertColor(color)), rounding, roundingCornersFlags);
		else
			window->DrawList->AddRect(from, to, ImGui::GetColorU32(ConvertColor(color)), rounding, roundingCornersFlags, thickness);
	}

	void RenderImage(ImTextureID pTexture, const ImVec2& from, const ImVec2& to, uint32_t color, bool flipped, bool rounded, float rounding, ImDrawFlags_ roundingCornersFlags)
	{
		const ImGuiWindow* window = ImGui::GetCurrentWindow();

		if (flipped)
		{
			if (rounded)
				window->DrawList->AddImageRounded(pTexture, from, to, { 1.0f, 0.0f }, { 0.0f, 1.0f }, ImGui::GetColorU32(ConvertColor(color)), rounding, roundingCornersFlags);
			else
				window->DrawList->AddImage(pTexture, from, to, { 1.0f, 0.0f }, { 0.0f, 1.0f }, ImGui::GetColorU32(ConvertColor(color)));
		}

		if (rounded)
			window->DrawList->AddImageRounded(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32(ConvertColor(color)), rounding, roundingCornersFlags);
		else
			window->DrawList->AddImage(pTexture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImGui::GetColorU32(ConvertColor(color)));
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

	void RenderTextWorld(const std::string& text, const Vector3& position, float size, uint32_t color, bool center)
	{
		const auto worldToScreen = Engine::WorldToScreen(position).ToImVec();
		RenderText(text, worldToScreen, size, color, center);
	}

	void RenderLineWorld(const Vector3& from, const Vector3& to, uint32_t color, float thickness)
	{
		const ImGuiWindow* window = ImGui::GetCurrentWindow();
		const auto fromWorldToScreen = Engine::WorldToScreen(from).ToImVec();
		const auto toWorldToScreen = Engine::WorldToScreen(to).ToImVec();

		RenderLine(fromWorldToScreen, toWorldToScreen, color, thickness);
	}

	void RenderCircleWorld(const Vector3& worldPos, int numPoints, float radius, uint32_t color, float thickness, bool height, bool glow, bool filled, uint32_t fillColor, float transparency) {
		const ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window == nullptr || window->SkipItems)
			return;

		constexpr int MAX_POINTS = 100;
		ImVec2 points[MAX_POINTS];

		numPoints = min(numPoints, MAX_POINTS - 1);

		static std::vector<float> sines, cosines;
		CalculateSinesAndCosines(numPoints, sines, cosines);

		const float currentTime = Engine::GetGameTime(); // Ottieni il tempo corrente di ImGui

		for (int i = 0; i <= numPoints; ++i) {
			const float cosTheta = cosines[i % numPoints];
			const float sinTheta = sines[i % numPoints];

			const Vector3 worldSpace = {worldPos.x + radius * cosTheta, worldPos.y, worldPos.z - radius * sinTheta};
			const ImVec2 screenSpace = Engine::WorldToScreen(worldSpace).ToImVec();

			points[i] = ImVec2(screenSpace.x, screenSpace.y);
		}

		if (filled)
			window->DrawList->AddConvexPolyFilled(points, numPoints, ImGui::GetColorU32(ConvertColor(fillColor)) * transparency);
		else
			window->DrawList->AddPolyline(points, numPoints + 1, ImGui::GetColorU32(ConvertColor(color)), true, thickness);

		if (glow) {
			for (int i = 0; i < numPoints; ++i) {
				ImVec2 p1 = points[i];
				ImVec2 p2 = points[i + 1];
				for (int j = 1; j <= 1; ++j) {
					constexpr float rainbowSpeed = 0.5f;
					float hue = fmodf(currentTime * rainbowSpeed + static_cast<float>(i) / numPoints, 1.0f);
					ImVec4 rainbowColor = ImColor::HSV(hue, 1.0f, 1.0f); // Genera il colore HSV
					const ImU32 glowColorRainbow = ImGui::ColorConvertFloat4ToU32(rainbowColor);
					window->DrawList->AddLine(p1, p2, glowColorRainbow, thickness + j * 1);
				}
			}
		}
	}

	void RenderPolygonWorld(const Geometry::Polygon& poly, uint32_t color, float thickness, bool filled, uint32_t fillColor)
	{
		ImVec2 points[100];
		const int numOfPoints = min(static_cast<int>(poly.Points.size()), 100);

		for (int i = 0; i < numOfPoints; ++i) {
			points[i] = Engine::WorldToScreen(poly.Points[i]).ToImVec();
		}

		if (filled)
			ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(points, numOfPoints, ImGui::GetColorU32(ConvertColor(fillColor)));
		else
			ImGui::GetBackgroundDrawList()->AddPolyline(points, numOfPoints, ImGui::GetColorU32(ConvertColor(color)), true, thickness);
	}

	void RenderArcWorld(const Vector3& worldPos, int numPoints, float radius, uint32_t color, float thickness, float arcSize, const Vector3& directionPos, bool dontDrawWalls) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window == nullptr || window->SkipItems)
			return;

		Vector3 dir = directionPos - worldPos;
		float angle = atan2(dir.z, dir.x) * -1;
		float startTheta = angle - arcSize / 2.0f;
		float endTheta = angle + arcSize / 2.0f;

		numPoints = min(numPoints, 49);

		static std::vector<float> sines, cosines;
		CalculateSinesAndCosines(numPoints, sines, cosines);

		const float thetaStep = (endTheta - startTheta) / numPoints;
		float currentTheta = startTheta;

		ImVec2 lastValidPoint;
		bool lastPointValid = false;

		for (int i = 0; i <= numPoints; ++i) {
			const float sinTheta = sin(currentTheta);
			const float cosTheta = cos(currentTheta);
			const Vector3 worldSpace = { worldPos.x + radius * cosTheta, worldPos.y, worldPos.z - radius * sinTheta };

			if (dontDrawWalls && (Engine::IsWall(worldSpace) || Engine::IsBrush(worldSpace))) {
				lastPointValid = false;
			}
			else {
				ImVec2 screenSpace = Engine::WorldToScreen(worldSpace).ToImVec();

				if (lastPointValid) {
					window->DrawList->AddLine(lastValidPoint, screenSpace, ImGui::GetColorU32(ConvertColor(color)), thickness);
				}

				lastValidPoint = screenSpace;
				lastPointValid = true;
			}

			currentTheta += thetaStep;
		}
	}

	void RenderWardRange(const Vector3& position, uint32_t color, const float& range, bool brushes) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window == nullptr || window->SkipItems)
			return;

		constexpr int numPoints = 120;
		ImVec2 points[numPoints];

		static std::vector<float> sines, cosines;
		CalculateSinesAndCosines(numPoints, sines, cosines);

		for (int i = 0; i < numPoints; ++i) {
			float cosTheta = cosines[i];
			float sinTheta = sines[i];
			Vector3 p = position;

			for (float step2 = 20.f; step2 <= range; step2 += 20.f) {
				p.x = position.x + (step2 * cosTheta);
				p.z = position.z - (step2 * sinTheta);

				if (Engine::IsWall(p) || step2 == range || (brushes && Engine::IsBrush(p))) {
					break;
				}
			}

			points[i] = Engine::WorldToScreen(p).ToImVec();
		}

		for (int i = 0; i < numPoints; i += 3) {
			const int next = (i + 3) % numPoints;
			window->DrawList->AddBezierCubic(points[i], points[(i + 1) % numPoints], points[(i + 2) % numPoints], points[next], ImGui::GetColorU32(ConvertColor(color)), 1.0f);
		}
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

	void Texture2D(void* texture, const ImVec2& from, const ImVec2& to, bool rounded, float rounding) {

		if (rounded)
			ImGui::GetCurrentWindow()->DrawList->AddImageRounded(texture, from, to, { 0.0f, 0.0f }, { 1.0f, 1.0f }, COLOR_WHITE, rounding, ImDrawFlags_RoundCornersAll);
		else
			ImGui::GetCurrentWindow()->DrawList->AddImage(texture, from, to);
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
}