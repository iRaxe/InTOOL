#pragma warning(disable : 6387)

#include <filesystem>

#include "../stdafx.h"
#include "../zoom.h"
#include "../MenuSettings.h"
#include "../skStr.h"
#include "../auth.hpp"

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

uintptr_t GetTextSectionChecksum() {
	// Header data.
	PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)GetModuleHandle(nullptr) + ((PIMAGE_DOS_HEADER)GetModuleHandle(nullptr))->e_lfanew);
	PIMAGE_SECTION_HEADER SectionHeader = IMAGE_FIRST_SECTION(NtHeaders);

	// Initialize the checksum to 0.
	uintptr_t TextChecksum = 0;
	bool foundTextSection = false;

	// Iterate through all sections.
	for (int i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++, SectionHeader++) {
		// We found the .text section.
		if (!strcmp((const char*)SectionHeader->Name, ".text")) {
			foundTextSection = true;
			// Get the base address and size of the .text section.
			uintptr_t TextSectionBase = (uintptr_t)GetModuleHandle(nullptr) + SectionHeader->VirtualAddress;
			uintptr_t TextSectionSize = SectionHeader->Misc.VirtualSize;

			// Iterate over each uintptr_t in the .text section and add it to the checksum.
			for (uintptr_t i = 0; i < TextSectionSize; i += sizeof(uintptr_t)) {
				TextChecksum += *(uint64_t*)(TextSectionBase + i);
			}
			break;
		}
	}

	if (!foundTextSection)
	{
		// Handle the case where the .text section was not found.
		// You could return a special value, throw an exception, or do something else here.
	}
	return TextChecksum;
}

uintptr_t GetChecksum() {
	// Get the base address of the module and its NT headers.
	auto module_base = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(
		module_base +
		reinterpret_cast<PIMAGE_DOS_HEADER>(module_base)->e_lfanew
		);

	// Initialize the checksum to 0.
	uintptr_t checksum = 0;

	// Iterate through all sections.
	for (auto section_header = IMAGE_FIRST_SECTION(nt_headers);
		section_header < IMAGE_FIRST_SECTION(nt_headers) + nt_headers->FileHeader.NumberOfSections;
		++section_header) {
		// Check if the section is executable or read-only.
		if ((section_header->Characteristics & IMAGE_SCN_MEM_EXECUTE) ||
			(section_header->Characteristics & IMAGE_SCN_MEM_READ)) {
			// Get the base address and size of the section.
			uintptr_t section_base = module_base + section_header->VirtualAddress;
			uintptr_t section_size = section_header->Misc.VirtualSize;

			// Iterate over each uintptr_t in the section and add it to the checksum.
			for (uintptr_t i = 0; i < section_size; i += sizeof(uintptr_t)) {
				checksum += *reinterpret_cast<uintptr_t*>(section_base + i);
			}
		}
	}

	return checksum;
}

std::string ReadFromJson(std::string path, std::string section)
{
	if (!std::filesystem::exists(path))
		return skCrypt("File Not Found").decrypt();
	std::ifstream file(path);
	json data = json::parse(file);
	return data[section];
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
	/*static const uintptr_t InitialChecksum = GetChecksum();
	LOG("Initial checksum: %p", InitialChecksum);
	RestoreVMHook();
	41 0f b6 32 33 f1 e9 ? ? ? ? 8b 54 25 <= stub check
	8b 01 4c 8d 04 02 <= primo check league  + 0x4000
	0f b6 34 0b f9 <= primo check
	0f b6 04 03 d2 f7 <= secondo check
	0f b6 1c 1e <= terzo check
	0f b6 04 06 <= quarto check
	13.23 = 0x2D1410*/

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
		float* gameTimePtr = (float*)(globals::moduleBase + UPasta::Offsets::Instance::Game::GameTime);
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
		if (!globals::hookResponse && GetAsyncKeyState(VK_DELETE)) //|| Engine::GetGameState() == Paused)
			globals::eject = true;
	}

#ifdef CONSOLE_ENABLED
	fclose(f);
	FreeConsole();
#endif
	CreateThread(nullptr, 0, EjectThread, nullptr, 0, nullptr);

	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
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