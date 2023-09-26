#pragma warning(disable : 6387)

#include "../stdafx.h"
#include "../zoom.h"
#include "../MenuSettings.h"

//#define CONSOLE_ENABLED

HMODULE hLocalModule;

bool WINAPI HideThread(const HANDLE hThread) noexcept
{
	__try {
		using FnSetInformationThread = NTSTATUS(NTAPI*)(HANDLE ThreadHandle, UINT ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength);
		const auto NtSetInformationThread{ reinterpret_cast<FnSetInformationThread>(::GetProcAddress(::GetModuleHandle(L"ntdll.dll"), "NtSetInformationThread")) };

		if (!NtSetInformationThread)
			return false;

		if (const auto status{ NtSetInformationThread(hThread, 0x11u, nullptr, 0ul) }; status == 0x00000000)
			return true;
	}
	__except (TRUE) {
		return false;
	}
}

bool RestoreVMHook() noexcept
{
	__try {
		HMODULE NTDLL = GetModuleHandle(L"ntdll.dll");
		if (!NTDLL)
			return false;

		DWORD64 ZwProtectVirtualMemoryAddr = reinterpret_cast<DWORD64>(
			GetProcAddress(NTDLL, "ZwProtectVirtualMemory"));

		BYTE ZwPVM[] = {
		0x4C, 0x8B, 0xD1, 0xB8, 0x50, 0x00, 0x00, 0x00, 0xF6, 0x04, 0x25, 0x08, 0x03, 0xFE, 0x7F, 0x01
		};

		int i = 0;
		for (BYTE _byte : ZwPVM) {
			*(BYTE*)(ZwProtectVirtualMemoryAddr + i) = _byte;
			i++;
		}
	}
	__except (TRUE) {
		return false;
	}
}


DWORD __stdcall EjectThread(LPVOID lpParameter)
{
	Sleep(100);
	FreeLibraryAndExitThread(hLocalModule, 0);

	return 0;
}


DWORD __stdcall OnInject(LPVOID lpReserved)
{
#ifdef CONSOLE_ENABLED
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif

	LOG("Injected");
	RestoreVMHook();
	std::thread(startZoom).detach();
	Sleep(100);

	if (HideThread(::GetCurrentThread()))
		LOG("Thread hidden");

	globals::moduleBase = (uintptr_t)GetModuleHandle(nullptr);
	if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)globals::moduleBase, &globals::moduleInfo, sizeof(MODULEINFO)))
	{
		Sleep(100);
		LOG("Failed to get module information.");

		return 0;
	}

	while (true)
	{
		float* gameTimePtr = (float*)(globals::moduleBase + oGameTime);
		if (IsValidPtr(gameTimePtr) && *gameTimePtr > 3.0f) break;
		Sleep(300);
	}
	
	int hooked = 2;
	for (int i = 0; i < hooks::renderTypeNames.size(); i++)
	{
		LOG("Testing %s", hooks::renderTypeNames[i]);
		if (hooked = hooks::Init(i))
		{
			Sleep(400);
			if (globals::hookResponse)
				break;
			kiero::shutdown();
		}
		Sleep(100);
	}

	Sleep(100);

	if (!globals::hookResponse && hooked == 1)
		LOG("Hook function not called by process (press detach key)");

	while (!globals::eject)
	{
		Sleep(5);
		if (!globals::hookResponse && GetAsyncKeyState(VK_DELETE))
			globals::eject = true;
	}

#ifdef CONSOLE_ENABLED
	fclose(f);
	FreeConsole();
#endif
	CreateThread(nullptr, 0, EjectThread, nullptr, 0, nullptr);

	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,	DWORD  ul_reason_for_call,	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hLocalModule = hModule;
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, OnInject, hModule, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}