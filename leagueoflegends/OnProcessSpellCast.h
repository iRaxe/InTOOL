#pragma once
#include "Awareness.h"
#include "ListManager.h"
#include "../stdafx.h"

#include "VMTHook.h"
#include "selfprotection/pipe/namedpipeclient.h"

inline uintptr_t pOnProcessSpellCast;

inline VMTHook VMTHOnProcessSpellCast[10];

//int __fastcall hkOnProcessSpellCast(void* spell_book, void* edx, SpellCast* spellCastInfo) noexcept
int __fastcall hkOnProcessSpellCast(void* spell_book, void* edx, SpellCast* spellCastInfo) noexcept
{
	static auto fn = reinterpret_cast<int(__fastcall*)(void*, void*, SpellCast*)>(pOnProcessSpellCast);

	if (spellCastInfo != nullptr)
	{
		// Estrai il nome del campione da spell_book
		const auto charScriptPtr = *(uintptr_t*)((uintptr_t)spell_book + 0x90);
		const auto charScriptName = *(char**)(charScriptPtr + 0x8);

		const std::string fullNameStr(charScriptName); // Converte char* in std::string
		const std::string charName = fullNameStr.substr(10); // Rimuovi i primi 10 caratteri (la lunghezza di "CharScript")

		// Estrai le informazioni sul cast dell'abilità
		SpellInfo* spellInfo = spellCastInfo->GetProcessSpellInfo();
		const auto spellData = spellInfo->GetSpellData();
		const auto spellName = spellData->GetName();

		// Ottieni il numero dello slot dell'abilità
		const int spellSlot = functions::getCooldownData<int>(charName, spellName);
		if (spellSlot >= 0)
		{
			Object* spellCaster = functions::GetPlayerPointer(charName);
			Spell* spell = spellCaster->GetSpellBySlotId(spellSlot);
			const int spellLevel = spell->GetLevel();
			const auto cooldownValue = spell->GetSpellInfo()->GetSpellData()->GetCooldownArray()->GetArrayIndex(spellLevel)->GetBaseCooldown();

			// Calcola il tempo di prontezza
			const auto reduction = 100 / (100 + spellCaster->GetAbilityHaste());
			const auto readyAt = functions::GetGameTime() + cooldownValue * reduction;
			UPasta::SDK::ListManager::Functions::InsertCooldown(spellCaster, spellSlot, readyAt);
		}
		return fn(spell_book, edx, spellCastInfo);
	}

	return fn(spell_book, edx, spellCastInfo);
}


inline void HookOnProcessSpellCast()
{
	for (int i = 0; i < globals::heroManager->GetListSize(); i++)
	{
		auto obj = globals::heroManager->GetIndex(i);

		for (Object* obj : *globals::heroManager)
		{
			uintptr_t dwOnProcessSpell = (uintptr_t)obj + UPasta::Offsets::Events::Spellcast::ProcessIndex;
			pOnProcessSpellCast = VMTHOnProcessSpellCast[i].Hook((void*)dwOnProcessSpell, 29, (uintptr_t)&hkOnProcessSpellCast);
			//LOG("\nSuccessfully hooked OnProcessSpellCast of: %s", obj->GetName().c_str());
			i++;

		}
	}
	
}

inline void UnHookOnProcessSpellCast()
{
	for (VMTHook& vmt : VMTHOnProcessSpellCast)
		vmt.UnHook();
}