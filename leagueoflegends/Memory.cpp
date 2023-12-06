#include "Memory.h"
#include "FindHiddenModule.h"

void Memory::Init() {
	_main = (QWORD)GetModuleHandle(NULL);
	if (!_main) LOG("WARNING: NO MAIN MODULE FOUND");

	_hidden = GetShadowHandle();
	if (!_main) LOG("WARNING: NO SHADOW MODULE FOUND");

	_stub = (QWORD)GetModuleHandleA("stub.dll");
	if (!_main) LOG("WARNING: NO STUB MODULE FOUND");

	LOG("FOUND MAIN MODULE AT: 0x%p", _main);
	LOG("FOUND SHADOW MODULE AT: 0x%p", _hidden);
	LOG("FOUND STUB MODULE AT: 0x%p", _stub);

}
