#pragma once
#include "stdafx.h"
#include "Syscaller.h"

/*

Find Copy of the main module (same size) that has been removed from the PE header list.

Source: https://www.unknowncheats.me/forum/league-of-legends/520496-league-stub-text-section-hook-bypass.html

*/

static QWORD GetShadowHandle()
{
    /* The `shadow module` has the same size as the `league` module, but has a different base */
    /* Get base address of `League of Legends.exe` */
    std::uintptr_t league_base = (std::uintptr_t)GetModuleHandleA("League of Legends.exe");

    /* Check if `League of Legends.exe` is loaded */
    if (!league_base)
        return 0;

    /* Get Module information */
    MODULEINFO module_info = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)league_base, &module_info, sizeof(MODULEINFO)))
        return 0;

    /* Get module size */
    std::uintptr_t module_size = (std::uintptr_t)module_info.SizeOfImage;

    MEMORY_BASIC_INFORMATION memory_info = { 0 };
    /* Start at end of league base */
    std::uintptr_t current_address = 0;


    /* Loop until we find the shadow module */
    while (Syscaller::VirtualQuery((LPVOID)current_address, &memory_info, sizeof(MEMORY_BASIC_INFORMATION)))
    {

        /* Check if the size is the same as the league module */
        if (memory_info.RegionSize == module_size && memory_info.BaseAddress != (LPVOID)league_base)
        {
            /* Set the base address */
            return current_address;
        }

        /* Move to the next region */
        current_address += memory_info.RegionSize;
    }
}

bool get_shadow_module(std::uintptr_t& base_address)
{
    /* The `shadow module` has the same size as the `league` module, but has a different base */
    /* Get base address of `League of Legends.exe` */
    std::uintptr_t league_base = (std::uintptr_t)GetModuleHandleA("League of Legends.exe");

    /* Check if `League of Legends.exe` is loaded */
    if (!league_base)
        return false;

    /* Get Module information */
    MODULEINFO module_info = { 0 };
    if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)league_base, &module_info, sizeof(MODULEINFO)))
        return false;

    /* Get module size */
    std::uintptr_t module_size = (std::uintptr_t)module_info.SizeOfImage;

    MEMORY_BASIC_INFORMATION memory_info = { 0 };
    /* Start at end of league base */
    std::uintptr_t current_address = 0;


    /* Loop until we find the shadow module */
    while (Syscaller::VirtualQuery((LPVOID)current_address, &memory_info, sizeof(MEMORY_BASIC_INFORMATION)))
    {

        /* Check if the size is the same as the league module */
        if (memory_info.RegionSize == module_size && memory_info.BaseAddress != (LPVOID)league_base)
        {
            /* Set the base address */
            base_address = current_address;
            return true;
        }

        /* Move to the next region */
        current_address += memory_info.RegionSize;
    }
}