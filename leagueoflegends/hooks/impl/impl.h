#pragma once

namespace hooks
{
	namespace impl
	{
		extern HWND windowDX;
		void AssignGlobalLists();
		void InitMainFunctions();
		void OnGameUpdate();
		void OnWndProc(UINT msg, WPARAM param);
		void HookFunctions();
		void SubscribeToMainEvents();
		void UnSubscribeFromMainEvents();
		void Dispose();
		void Init();
			
	}
}



IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);