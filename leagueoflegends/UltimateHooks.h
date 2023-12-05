#pragma once
#include <Windows.h>
#include <vector>
#include <list>
#include <string>
#include <regex>

#pragma warning(4:4596)

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
BOOL sys_VirtualProtect(LPVOID lpAddress, SIZE_T* dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

class UltimateHooks
{
public:
	static DWORD64 RestoreZwProtectVirtualMemory();
	static DWORD64 RestoreZwQueryVirtualMemory();
};
extern UltimateHooks UltHook;