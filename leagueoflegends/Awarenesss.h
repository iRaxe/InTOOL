#pragma once
#include "Awareness.h"
using namespace UPasta::SDK::Awareness;
class Awarenesss
{
	static inline ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize;

	static void InitializeWardPosDrawingsMenu();

public:
	class HPBar;
	class PathTracker;
	class LastPosition;
	class LastHitDamage;
	class Sidebar;
	static void DrawChampIcon(Object* obj, const ImVec2& fromPos, const ImVec2& toPos);

	static void Initialize();
};

