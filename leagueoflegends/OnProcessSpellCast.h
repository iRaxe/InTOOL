#pragma once
#include "Awareness.h"
#include "ListManager.h"
#include "../stdafx.h"

#include "VMTHook.h"
#include "selfprotection/pipe/namedpipeclient.h"

inline uintptr_t pOnProcessSpellCast;

inline VMTHook VMTHOnProcessSpellCast[10];

__int64 __fastcall hkOnProcessSpellCast(__int64 spell_book, int state, SpellCast* spellCastInfo, __int64 a6) noexcept
{
	//STATE 12: ONCREATE | 13: ONSPELLIMPACT | 17: ONDELETE
	auto res = reinterpret_cast<__int64(*)(__int64, int, SpellCast*, __int64)>(pOnProcessSpellCast)(spell_book, state, spellCastInfo, a6);

	Event::Publish(Event::OnProcessSpell, state, spellCastInfo);
	if (spellCastInfo->IsAutoAttack())
	{
		switch (state) {
		case 12:
			Event::Publish(Event::OnBeforeAttack, spellCastInfo);
			break;

		case 17:
			Event::Publish(Event::OnAfterAttack, spellCastInfo);
			break;
		}
	}
	else
	{
		switch (state) {
		case 12:
			Event::Publish(Event::OnCastSpell, spellCastInfo);
			break;

		case 13:
			Event::Publish(Event::OnImpactSpell, spellCastInfo);
			break;

		case 17:
			Event::Publish(Event::OnFinishCast, spellCastInfo);
			break;
		}
	}

	return res;
}

inline void HookOnProcessSpellCast()
{
	for (int i = 0; i < globals::heroManager->GetListSize(); i++)
	{
		auto obj = globals::heroManager->GetIndex(i);

		for (Object* obj : *globals::heroManager)
		{
			uintptr_t dwOnProcessSpell = (uintptr_t)obj + UPasta::Offsets::Events::Spellcast::ProcessIndex;
			pOnProcessSpellCast = VMTHOnProcessSpellCast[i].Hook((void*)dwOnProcessSpell, 30, (uintptr_t)&hkOnProcessSpellCast);
			i++;

		}
	}
	
}

inline void UnHookOnProcessSpellCast()
{
	for (VMTHook& vmt : VMTHOnProcessSpellCast)
		vmt.UnHook();
}