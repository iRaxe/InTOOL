#pragma once
typedef unsigned long long QWORD;

class Memory
{
	static inline QWORD _main;
	static inline QWORD _hidden;
	static inline QWORD _stub;

public:

	static void Init();
	static inline QWORD GetMainModule() { return _main; }
	static inline QWORD GetHiddenModule() { return _hidden; }
	static inline QWORD GetStubModule() { return _stub; }
};

