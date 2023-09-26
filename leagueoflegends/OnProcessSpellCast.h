#pragma once
#include "../stdafx.h"

#include "VMTHook.h"
#include "selfprotection/pipe/namedpipeclient.h"

inline uintptr_t pOnProcessSpellCast;

inline VMTHook VMTHOnProcessSpellCast[10];
class LolString
{
	char content[0x10]; // 0x0
	int len = 0; // 0x10
	int max = 0; // 0x14

public:

	operator const char* (void)
	{
		return c_str();
	}
	operator std::string(void)
	{
		return std::string(c_str());
	}

	std::string str()
	{
		return std::string(c_str());
	}

private:
	char* c_str(void)
	{
		if (DWORD(this) <= 0x1000)
			return (char*)"";

		return max >= 0x10
			? *reinterpret_cast<char**>(content)
			: content;
	}
};

class SpellCastInfo2 {
public:
	std::string GetCasterName()
	{
		auto r = (LolString*)((QWORD)this + 0x28);
		return r->str();
	}

	SpellInfo* GetSpellInfo() {
		return *(SpellInfo**)((QWORD)this);
	}

	Vector3 GetStartPos() {
		return *(Vector3*)((QWORD)this + 0x80);
	}

	Vector3 GetEndPos() {
		return *(Vector3*)((QWORD)this + 0x24);
	}

	int GetSlot() {
		return *(int*)((QWORD)this + 0xf4);
	}

	int GetSenderIndex() {
		return *(int*)((QWORD)this + 0x64); //unsure

	}

	int GetSpellslotId()
	{
		return *(int*)((QWORD)this + 0x8);
	}

	int GetMissileIndex()
	{
		return *(int*)((QWORD)this + 0x14);
	}

	float GetDashSpeed()
	{
		return *(float*)((QWORD)this + 0x4C0);
	}

	Vector3 GetTargetPosition()
	{
		return functions::ReadVector3((QWORD)this + 0x80);
	}
	std::string GetName()
	{
		return (char*)*(QWORD*)((QWORD)this + 0x0) + 0x28;
	}
};

/*void __fastcall on_cast(void* thisptr, int state, void* spell_cast_info, __int64 a6) noexcept {
 
	static auto fn = reinterpret_cast<void(__fastcall*)(void*, int, void*, __int64)>(original_on_cast);
	if (!spell_cast_info)
		return fn(thisptr, state, spell_cast_info, a6);
 
 
 
	Event::Publish(Event::OnCast, state, spell_cast_info);
 
	return fn(thisptr, state, spell_cast_info, a6);
 
}*/

int __fastcall hkOnProcessSpellCast(void* spell_book, void* edx, SpellCastInfo2* spellCastInfo) noexcept
{
	static auto fn = reinterpret_cast<int(__fastcall*)(void*, void*, SpellCastInfo2*)>(pOnProcessSpellCast);

	if (spellCastInfo != nullptr)
	{
		LOG("PTR %p", edx);
		return fn(spell_book, edx, spellCastInfo);

	}
	//Spell* spell = cheatManager.memory->localPlayer->GetSpellBySlotId(SpellIndex::Q);
	//cheatManager.memory->PrintChat(tfm::format("spell %x", spell));

	return fn(spell_book, edx, spellCastInfo);
}


inline void HookOnProcessSpellCast()
{

	for (int i = 0; i < globals::heroManager->GetListSize(); i++)
	{
		auto obj = globals::heroManager->GetIndex(i);
		for (Object* obj : *globals::heroManager)
		{
			uintptr_t dwOnProcessSpell = (uintptr_t)obj + 0x1188;// 0xE6C;
			pOnProcessSpellCast = VMTHOnProcessSpellCast[i].Hook((void*)dwOnProcessSpell, 29, (uintptr_t)&hkOnProcessSpellCast);
			LOG("Success hook");
			i++;
			
		}
	}
    
}

inline void UnHookOnProcessSpellCast()
{
	for (VMTHook& vmt : VMTHOnProcessSpellCast)
		vmt.UnHook();

	
}