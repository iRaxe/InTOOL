#pragma once
#include "UltimateHooks.h"
#include "makesyscall.h"

BOOL sys_VirtualProtect(LPVOID lpAddress, SIZE_T* dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {
	return NT_SUCCESS(DirectSysCall<NTSTATUS>("ZwProtectVirtualMemory")(GetCurrentProcess(), lpAddress, dwSize, flNewProtect, lpflOldProtect));
}

DWORD64 UltimateHooks::RestoreZwProtectVirtualMemory() {
	HMODULE ntdll = (HMODULE)GetModuleHandle(L"ntdll.dll");
	uintptr_t ZwProtectVirtualMemoryAddr = reinterpret_cast<uintptr_t>(GetProcAddress(ntdll, "ZwProtectVirtualMemory"));
	if (!ZwProtectVirtualMemoryAddr)
		return 0;
	unsigned char ZwPVM[] = { 0x4C, 0x8B, 0xD1, 0xB8, 0x50, 0x00, 0x00, 0x00, 0xF6, 0x04, 0x25, 0x08, 0x03, 0xFE, 0x7F, 0x01, 0x75, 0x03, 0x0F, 0x05, 0xC3 };
	DWORD currentProtect;
	auto size = sizeof(ZwPVM);
	sys_VirtualProtect((char*)ZwProtectVirtualMemoryAddr, &size, PAGE_EXECUTE_READWRITE, &currentProtect);
	for (int i = 0; i < size; i++) {
		*(BYTE*)(ZwProtectVirtualMemoryAddr + i) = ZwPVM[i];
	}
	VirtualProtect((LPVOID)ZwProtectVirtualMemoryAddr, size, currentProtect, 0);
	return ZwProtectVirtualMemoryAddr;
}

DWORD64 UltimateHooks::RestoreZwQueryVirtualMemory() {
	HMODULE ntdll = (HMODULE)GetModuleHandle(L"ntdll.dll");
	uintptr_t ZwQueryVirtualMemoryAddr = reinterpret_cast<uintptr_t>(GetProcAddress(ntdll, "ZwQueryVirtualMemory"));
	if (!ZwQueryVirtualMemoryAddr)
		return 0;
	unsigned char ZwQVM[] = { 0x4C, 0x8B, 0xD1, 0xB8, 0x23, 0x00, 0x00, 0x00, 0xF6, 0x04, 0x25, 0x08, 0x03, 0xFE, 0x7F, 0x01, 0x75, 0x03, 0x0F, 0x05, 0xC3 };
	DWORD currentProtect;
	auto size = sizeof(ZwQVM);
	VirtualProtect((char*)ZwQueryVirtualMemoryAddr, size, PAGE_EXECUTE_READWRITE, &currentProtect);
	for (int i = 0; i < size; i++) {
		*(BYTE*)(ZwQueryVirtualMemoryAddr + i) = ZwQVM[i];
	}
	VirtualProtect((LPVOID)ZwQueryVirtualMemoryAddr, size, currentProtect, 0);
	return ZwQueryVirtualMemoryAddr;
}