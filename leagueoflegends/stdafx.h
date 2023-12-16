#pragma once

#include <algorithm>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <Psapi.h>

#include <vector>
#include <map>
#include <variant>
#include <TlHelp32.h>
#include <directxmath.h>
#include <winternl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <format>
#include <functional>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>

#include <dxgi.h>
#include <d3d11.h>
#include <d3d10.h>
#include <d3d9.h>

#include "selfprotection/utils.h"
#include "selfprotection/pipe/namedpipeclient.h"

#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "mem/mem.h"
#include "mem/spoof_call.h"
#include "global/strings.h"
#include "global/colors.h"
#include "global/offsets.h"
#include "global/structs.h"
#include "Geometry.h"
#include "global/utils.h"
#include "global/leagueobfuscation.h"
#include "classes/classes.h"
#include "global/Engine.h"
#include "render/render.h"
#include "global/globals.h"
#include "global/targetver.h"
#include "scripts/championmodule.h"
#include "scripts/skillshotdatabase.h"
#include "scripts/Modules.h"

#include "hooks/hooks.h"
#include "hooks/impl/impl.h"
#include "Eventmanager.h"

#include "json.hpp"
using json = nlohmann::json;

#define RVA(address) (uintptr_t)(globals::moduleBase + address)
#define RVA2(address) (globals::moduleBase + address)

#define DEFINE_RVA(address) ((uintptr_t)((uintptr_t)globals::moduleBase + (uintptr_t)address)
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#define M_PI_F (float)M_PI
#endif

inline SkillShotDB database;
