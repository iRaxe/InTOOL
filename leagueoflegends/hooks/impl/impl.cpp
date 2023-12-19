#include "../../ListManager.h"
#include "../../stdafx.h"
#include "../../NewMenu.h"
#include "../../zoom.h"
#include "../../Orbwalker.h"
#include "../../imgui_freetype.h"
#include "../../imgui_notify.h"
#include "../../OnProcessSpellCast.h"

namespace hooks
{
	namespace impl
	{
		typedef unsigned int UINT;

		bool _init = false;
		void InitMainFunctions() {
			TryCatch(Skillshot::PopulateSpellsDB(), "Error in populating spells database");
			TryCatch(Engine::Init(), "Error in initializing functions");
			TryCatch(UPasta::SDK::Menu::Initialize(), "Error in initializing menu");
			TryCatch(render::Init(), "Error in initializing render features");
			TryCatch(Modules::Init(), "Error in initializing scripts");
			TryCatch(UPasta::SDK::ListManager::Functions::Initialize(), "Error in initializing sdk functions");
		}

		void OnGameUpdate() {
			TryCatch(UPasta::SDK::ListManager::Functions::Refresh(), "Error in listmanager update");
			TryCatch(Modules::Update();, "Error in scripts update");
			TryCatch(render::Update(), "Error in render update");
			TryCatch(UPasta::SDK::Menu::OnDraw(), "Error in menu update");
		}

		void OnWndProc(UINT msg, WPARAM param) {
			//globals::scripts::orbwalker::orbwalkState = OrbwalkState::Off
			if (param == UPasta::SDK::OrbwalkerConfig::comboKey->Key && UPasta::SDK::OrbwalkerConfig::statusComboMode->Value) {
				switch (msg) {
					case WM_KEYDOWN: globals::scripts::orbwalker::orbwalkState = OrbwalkState::Attack; break;
					case WM_KEYUP: break;
				}
			}

			if (param == UPasta::SDK::OrbwalkerConfig::harassKey->Key && UPasta::SDK::OrbwalkerConfig::statusLaneClearMode->Value) {
				switch (msg) {
				case WM_KEYDOWN: globals::scripts::orbwalker::orbwalkState = OrbwalkState::Clear; break;
				case WM_KEYUP: break;
				}
			}

			if (param == UPasta::SDK::OrbwalkerConfig::laneClearKey->Key && UPasta::SDK::OrbwalkerConfig::statusFastClearMode->Value) {
				switch (msg) {
					case WM_KEYDOWN: globals::scripts::orbwalker::orbwalkState = OrbwalkState::FastClear; break;
					case WM_KEYUP: break;
				}
			}

			if (param == UPasta::SDK::OrbwalkerConfig::fastClearKey->Key && UPasta::SDK::OrbwalkerConfig::statusHarassMode->Value) {
				switch (msg) {
					case WM_KEYDOWN: globals::scripts::orbwalker::orbwalkState = OrbwalkState::Harass; break;
					case WM_KEYUP: break;
				}
			}

			if (param == UPasta::SDK::OrbwalkerConfig::fastClearKey->Key && UPasta::SDK::OrbwalkerConfig::statusLastHitMode->Value) {
				switch (msg) {
					case WM_KEYDOWN: globals::scripts::orbwalker::orbwalkState = OrbwalkState::Lasthit; break;
					case WM_KEYUP: break;
				}
			}

			if (param == UPasta::SDK::OrbwalkerConfig::fastClearKey->Key && UPasta::SDK::OrbwalkerConfig::statusFleeMode->Value) {
				switch (msg) {
					case WM_KEYDOWN:globals::scripts::orbwalker::orbwalkState = OrbwalkState::Flee; break;
					case WM_KEYUP: break;
				}
			}

			if (param == VK_SHIFT) {
				switch (msg) {
					case WM_KEYDOWN: globals::menuOpen = true; break;
					case WM_KEYUP: globals::menuOpen = false; break;
				}
			}

			if (param == VK_DELETE && Engine::IsGameFocused()) {
				switch (msg) {
					case WM_KEYDOWN: Dispose(); globals::eject = true; break;
					case WM_KEYUP: break;
				}
			}
		}

		void HookFunctions() {
			TryCatch(HookOnProcessSpellCast(), " Error in hooking OnProcessSpellSound");
		}

		//TODO: REMOVE THE SUBTOMAINEVENTS (PLACEHOLDER FOR OLD CHAMP TEMPLATE)
		void SubscribeToMainEvents() {
			TryCatch(Event::Subscribe(Event::OnGameTick, OnGameUpdate), "Error in champions drawings");

			TryCatch(Event::Subscribe(Event::OnDraw, Modules::Champions::RenderUpdate), "Error in champions drawings");
			TryCatch(Event::Subscribe(Event::OnDraw, UPasta::SDK::Awareness::Functions::Update), "Error in awareness drawings");

			TryCatch(Event::Subscribe(Event::OnBeforeAttack, Modules::Champions::DoBeforeAttack), "Error in DoBeforeAttack");
			TryCatch(Event::Subscribe(Event::OnAfterAttack, Modules::Champions::DoAfterAttack), "Error in DoAfterAttack");

			TryCatch(Event::Subscribe(Event::OnCastSpell, Modules::Champions::DoCreateMissile), "Error in DoCreateMissile");
			TryCatch(Event::Subscribe(Event::OnFinishCast, Modules::Champions::DoDeleteMissile), "Error in DoDeleteMissile");

			TryCatch(Event::Subscribe(Event::OnWndProc, &OnWndProc), "Error subscribing to OnWndProc event");
		}

		void UnSubscribeFromMainEvents() {
			TryCatch(Event::UnSubscribe(Event::OnGameTick, OnGameUpdate), "Error in unsubscribing from champions drawings");
			TryCatch(Event::UnSubscribe(Event::OnDraw, Modules::Champions::RenderUpdate), "Error in unsubscribing from champions drawings");
			TryCatch(Event::UnSubscribe(Event::OnWndProc, &OnWndProc), "Error unsubscribing from OnWndProc event");
		}

		void Dispose() {
			TryCatch(UPasta::SDK::Menu::Dispose(), " Error in saving menu configs");
			TryCatch(UnHookOnProcessSpellCast(), " Error in unhooking OnProcessSpellSound");
			TryCatch(UnSubscribeFromMainEvents(), " Error in unsubscribing from events");
		}

		void Inits()
		{
			InitMainFunctions();
			HookFunctions();
			SubscribeToMainEvents();

			RECT windowRect;
			if (GetWindowRect(windowDX, &windowRect)) {
				globals::windowWidth = (float)abs(windowRect.right - windowRect.left);
				globals::windowHeight = (float)abs(windowRect.bottom - windowRect.top);
			}

			Engine::PrintChat(CHAT_COLOR("#72ff72", "Loooool now u are going to get rekt"));
			globals::hookResponse = true;
		}

		
		bool KeyChecks()
		{
			return false;
		}

		//DX
		typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		static Present o_presentDX;

		typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

		HWND windowDX = nullptr;
		WNDPROC o_wndProcDX;


		LRESULT __stdcall wndProcDX(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			Event::Publish(Event::OnWndProc, uMsg, wParam);

			if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
				return true;

			return CallWindowProc(o_wndProcDX, hWnd, uMsg, wParam, lParam);
		}

		typedef long(__stdcall* ResetFn)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
		static ResetFn o_resetDX9 = NULL;

		typedef long(__stdcall* EndSceneFn)(LPDIRECT3DDEVICE9);
		static EndSceneFn o_endSceneDX9 = NULL;

		HRESULT __stdcall Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			HRESULT result = o_resetDX9(pDevice, pPresentationParameters);
			ImGui_ImplDX9_CreateDeviceObjects();

			return result;
		}

		//DX9
		HRESULT __stdcall EndScene(LPDIRECT3DDEVICE9 pDevice)
		{
			if (!_init)
			{
				D3DDEVICE_CREATION_PARAMETERS params;
				pDevice->GetCreationParameters(&params);
				windowDX = params.hFocusWindow;
				o_wndProcDX = (WNDPROC)SetWindowLongPtr(windowDX, GWLP_WNDPROC, (LONG_PTR)wndProcDX);

				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				io.IniFilename = "window.ini";
				io.Fonts->AddFontFromFileTTF(SP_STRING("C:\\Windows\\Fonts\\Arial.ttf"), 14);
				globals::pDeviceDX9var = pDevice;

				ImGui_ImplWin32_Init(windowDX);
				ImGui_ImplDX9_Init(pDevice);
				Inits();

				_init = true;
			}

			if (globals::eject)
			{
				SetWindowLongPtr(windowDX, GWLP_WNDPROC, (LONG_PTR)o_wndProcDX);

				ImGui_ImplDX9_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();

				HRESULT result = o_endSceneDX9(pDevice);

				kiero::shutdown();
				pDevice->Release();

				return result;
			}

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			Event::Publish(Event::OnGameTick);


			//ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

			return o_endSceneDX9(pDevice);
		}

		//DX11
		ID3D11Device* pDeviceDX11 = nullptr;
		ID3D11DeviceContext* pContextDX11 = nullptr;
		ID3D11RenderTargetView* mainRenderTargetViewDX11;

		HRESULT __stdcall presentDX11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
		{
			if (!_init)
			{
				if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDeviceDX11)))
				{
					pDeviceDX11->GetImmediateContext(&pContextDX11);
					DXGI_SWAP_CHAIN_DESC sd;
					pSwapChain->GetDesc(&sd);
					windowDX = sd.OutputWindow;
					ID3D11Texture2D* pBackBuffer;
					pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
					if (!pBackBuffer) return o_presentDX(pSwapChain, SyncInterval, Flags);
					pDeviceDX11->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetViewDX11);
					pBackBuffer->Release();
					o_wndProcDX = (WNDPROC)SetWindowLongPtr(windowDX, GWLP_WNDPROC, (LONG_PTR)wndProcDX);

					IMGUI_CHECKVERSION();
					ImGui::CreateContext();
					ImGuiIO& io = ImGui::GetIO(); (void)io;
					io.IniFilename = "window.ini";
					io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
					io.Fonts->AddFontFromFileTTF(SP_STRING("C:\\Windows\\Fonts\\Arial.ttf"), 14);

					DXGI_SWAP_CHAIN_DESC desc;
					pSwapChain->GetDesc(&desc);

					ID3D11Device* device;
					pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

					ID3D11DeviceContext* context;
					device->GetImmediateContext(&context);

					globals::pDeviceDX11var = device;

					ImGui_ImplWin32_Init(windowDX);
					ImGui_ImplDX11_Init(device, context);

					Inits();

					_init = true;

				}
				else
					return o_presentDX(pSwapChain, SyncInterval, Flags);
			}

			if (globals::eject)
			{
				SetWindowLongPtr(windowDX, GWLP_WNDPROC, (LONG_PTR)o_wndProcDX);

				ImGui_ImplDX11_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();

				HRESULT result = o_presentDX(pSwapChain, SyncInterval, Flags);

				kiero::shutdown();
				pDeviceDX11->Release();
				globals::eject = true;

				return result;
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			Event::Publish(Event::OnGameTick);

			ImGui::EndFrame();
			ImGui::Render();

			pContextDX11->OMSetRenderTargets(1, &mainRenderTargetViewDX11, NULL);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			return o_presentDX(pSwapChain, SyncInterval, Flags);
		}

		void Init()
		{
			if (globals::renderType == "D3D9")
			{
				kiero::bind(16, (void**)&o_resetDX9, Reset);
				kiero::bind(42, (void**)&o_endSceneDX9, EndScene);
			}
			else if (globals::renderType == "D3D11")
			{
				kiero::bind(8, (void**)&o_presentDX, presentDX11);
			}
		}
	}
}
