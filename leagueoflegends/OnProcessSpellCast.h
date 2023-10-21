#pragma once
#include "Awareness.h"
#include "../stdafx.h"

#include "VMTHook.h"
#include "selfprotection/pipe/namedpipeclient.h"

inline uintptr_t pOnProcessSpellCast;

inline VMTHook VMTHOnProcessSpellCast[10];

int __fastcall hkOnProcessSpellCast(void* spell_book, void* edx, SpellCast* spellCastInfo) noexcept
{
	static auto fn = reinterpret_cast<int(__fastcall*)(void*, void*, SpellCast*)>(pOnProcessSpellCast);

	if (spellCastInfo != nullptr)
	{
		LOG("PTR %p", spellCastInfo);
		SpellInfo* test = spellCastInfo->GetProcessSpellInfo();
		LOG("PTR2 %p", test);
		LOG("SPELLNAME %s", test->GetSpellData()->GetName().c_str());
		LOG("ENDPOS %p", test->GetSpellData()->GetSpellEndPos());
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
			uintptr_t dwOnProcessSpell = (uintptr_t)obj + 0x1198;
			pOnProcessSpellCast = VMTHOnProcessSpellCast[i].Hook((void*)dwOnProcessSpell, 29, (uintptr_t)&hkOnProcessSpellCast);
			LOG("\nSuccessfully hooked OnProcessSpellCast of: %s", obj->GetName().c_str());
			i++;
			
		}
	}
    
}

inline void UnHookOnProcessSpellCast()
{
	for (VMTHook& vmt : VMTHOnProcessSpellCast)
		vmt.UnHook();
}