#include "Syscaller.h"

#include <cstring>
#include <winternl.h>

/*

Anticheat placed jumps (hooked) these two functions so it can protect the game.
VirtualProtect makes sure executable pages cannot be changed, ViritualQuery hides the hidden module.

Instead of re-patching the function, we allocate executable memory and execute the syscall instruction with the relative ID.

*/

BOOL Syscaller::VirtualProtect(LPVOID address, DWORD size, DWORD protection, PDWORD old_protection) {

	if (!_init) return false;

	// CHANGE SYSCALL ID
	*reinterpret_cast<DWORD*>(reinterpret_cast<QWORD>(_allocation) + 4) = 0x50;


	// GET ADDRESS OF SYSCALL + 12
	auto nt_original = (QWORD)GetProcAddress(_ntdll_module, "NtProtectVirtualMemory") + 0x12;
	*reinterpret_cast<QWORD*>(reinterpret_cast<QWORD>(_allocation) + 14) = nt_original;


	ULONG(*func)(HANDLE, PVOID*, PULONG, ULONG, PULONG);
	memcpy(&func, &_allocation, sizeof(_allocation));

	auto result = func(INVALID_HANDLE_VALUE, &address, (PULONG)&size, protection, old_protection);
	LOG("address: 0x%p - result: %lu", address, result);

	return result == 0;
}


SIZE_T Syscaller::VirtualQuery(LPVOID address, PMEMORY_BASIC_INFORMATION buffer, SIZE_T size) {
	if (!_init) return 0;

	// CHANGE SYSCALL ID
	*reinterpret_cast<DWORD*>(reinterpret_cast<QWORD>(_allocation) + 4) = 0x23;

	// GET ADDRESS OF SYSCALL + 12
	auto nt_original = (QWORD)GetProcAddress(_ntdll_module, "NtQueryVirtualMemory") + 0x12;
	*reinterpret_cast<QWORD*>(reinterpret_cast<QWORD>(_allocation) + 14) = nt_original;

	ULONG(*func)(HANDLE, PVOID, DWORD, PVOID, SIZE_T, PSIZE_T);
	memcpy(&func, &_allocation, sizeof(_allocation));

	SIZE_T lenght = 0;
	auto result = func(GetCurrentProcess(), address, 0, buffer, size, &lenght);

	if (result)
		LOG("ERROR: 0x%x", result);
	return lenght;

}


bool Syscaller::Init() {

	_ntdll_module = GetModuleHandleA("ntdll.dll");

	if (!_ntdll_module) {
		LoadLibraryA("ntdll.dll"), _ntdll_module = GetModuleHandleA("ntdll.dll");
		if (!_ntdll_module) return _init;
	}

	_allocation = VirtualAlloc(nullptr, sizeof(_shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!_allocation) return _init;

	memcpy(_allocation, &_shellcode, sizeof(_shellcode));

	_init = true;
	return _init;
}
