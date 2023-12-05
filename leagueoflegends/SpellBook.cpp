#include "SpellBook.h"

SpellSlot2* SpellBook2::GetSpellSlot(SLOT s) {

	return *(SpellSlot2**)((QWORD)this + UPasta::Offsets::SpellBook::SpellSlotInstance + (0x8 * (int)s));
}

SpellBook2::STATE SpellBook2::GetSpellSlotState(SLOT s) {


	typedef STATE(*fnNewIssueOrder)(SpellBook2*, SLOT, uintptr_t);
	fnNewIssueOrder fun = (fnNewIssueOrder)(RVA(UPasta::Offsets::SpellBook::FnGetSpellState)); // 0x8396B0

	//return spoof_call((void*)RVA(Offsets::SpoofGadget), *fun, this, s, (uintptr_t)NULL);

	return reinterpret_cast<STATE(*)(void*, const SLOT, const uintptr_t&)> (RVA(UPasta::Offsets::SpellBook::FnGetSpellState)) (this, s, NULL);
}

SpellCast2* SpellBook2::GetActiveSpellCast() {

	auto p = *(uintptr_t*)((uintptr_t)this + 0x38);
	if (!p) return nullptr;
	return (SpellCast2*)(p + 0x8);

}