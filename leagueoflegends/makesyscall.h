#pragma once
#include <Windows.h>
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

template<typename Ret = NTSTATUS>
class DirectSysCall
{
public:
	explicit DirectSysCall(const char* func_name) :
		func_addr_((uint8_t*)GetProcAddress(GetModuleHandle(L"ntdll.dll"), func_name)),
		shell_code_(nullptr)
	{
		create_shell_code();
	}

	~DirectSysCall()
	{
		if (shell_code_)
			VirtualFree(shell_code_, 0, MEM_RELEASE);
	}

	template<typename ...Args>
	Ret operator()(Args... args)
	{
		if (!shell_code_)
			return Ret();

		typedef Ret(__stdcall* func_t)(Args...);
		func_t func = (func_t)shell_code_;

		return func(args...);
	}
private:
	void create_shell_code()
	{
		uint32_t sys_index = 0;
		if (!func_addr_)
			return;

		for (int8_t i = 0; i < 0xF; ++i)
		{
			if (*(func_addr_ + i) != 0xB8)
				continue;
			sys_index = *(uint32_t*)(func_addr_ + i + 1);
		}

		if (!sys_index)
			return;

#ifdef _WIN64
		uint8_t shell_code[]
		{
			0x4C, 0x8B, 0xD1,					// mov r10, rcx 
			0xB8, 0x00, 0x00, 0x00, 0x00,		// mov eax, SysCallIndex
			0x0F, 0x05,					        // syscall
			0xC3								// ret				
		};
#elif _WIN32
		uint8_t shell_code[]
		{
			0x90, 0x90, 0x90,					// nop, nop, nop
			0xB8, 0x00, 0x00, 0x00, 0x00,		// mov eax, SysCallIndex
			0xBA, 0x00, 0x00, 0x00, 0x00,		// mov edx, [Wow64Transition]
			0xFF, 0xD2,							// call edx
			0xC3								// ret
		};

		*(uintptr_t*)(shell_code + 9) = *(uintptr_t*)GetProcAddress(GetModuleHandle("ntdll.dll"), "Wow64Transition");
#else
#error NOT SUPPORTED
#endif

		* (uint32_t*)(shell_code + 4) = sys_index;

		shell_code_ = (uint8_t*)VirtualAlloc(nullptr, sizeof shell_code, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		if (!shell_code_)
			return;

		memcpy(shell_code_, shell_code, sizeof shell_code);
	}

	uint8_t* func_addr_;
	uint8_t* shell_code_;
};