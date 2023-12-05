#pragma once
#include "stdafx.h"

class Syscaller
{
    static QWORD GetModuleBase(std::string module_name);
    static QWORD GetExportAddress(QWORD module_base, std::string export_name);

    static inline BYTE _shellcode[]
    {
        0x49, 0x89, 0xCA,			                          // mov r10,rcx
        0xB8, 0x00, 0x00, 0x00, 0x00,                         // mov eax, syscall_index 0x50 = ntprotectvirtualmemory
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00        // jmp to ntvirtualprotect + 12 to dodge stub hook			                              // ret
    };

    static inline void* _allocation = nullptr;
    static inline HMODULE _ntdll_module = NULL;
    static inline bool _init = false;
public:

    static bool Init();

    static BOOL VirtualProtect(LPVOID address, DWORD size, DWORD protection, PDWORD old_protection);
    static SIZE_T VirtualQuery(LPVOID address, PMEMORY_BASIC_INFORMATION buffer, SIZE_T size = sizeof(MEMORY_BASIC_INFORMATION));
};

