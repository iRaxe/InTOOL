#pragma once
#include <memory>

class VMTHook
{
private:
	uintptr_t** originalVMT = nullptr;
	uintptr_t* copiedVMT = nullptr;
	std::unique_ptr<std::uintptr_t[]> ourVMT = nullptr;
	size_t iMethods = 0;
	bool bEnabled = false;

public:

	VMTHook(void* original = nullptr)
	{
		if (original)
			Init(original);
	}

	~VMTHook()
	{
		if (bEnabled)
			UnHook();

		originalVMT = nullptr;
		copiedVMT = nullptr;
		ourVMT.reset();

		iMethods = 0;
	}

	bool Init(void* original);

	uintptr_t Hook(void* original, const size_t& index, const uintptr_t& function);

	bool UnHook();

	uintptr_t HookIndex(const size_t& index, const uintptr_t& function);
};