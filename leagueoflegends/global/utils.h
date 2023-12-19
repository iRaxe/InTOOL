#pragma once

typedef unsigned long long QWORD;
#define IsValidPtr(addr) ((QWORD)(addr) > 0x100 && (QWORD)(addr) < 0x00007fffffffffff && !IsBadReadPtr(addr, sizeof(PVOID)))
#define IsNotZeroPtr(addr) ((QWORD)(addr) > 0000000000000000)

#define NOP 0x90

#define PI 3.1415927f

#define CHAT_COLOR(color, text) std::string(SP_STRING("<font color='") + std::string(SP_STRING(color)) + SP_STRING("'>") + std::string(SP_STRING(text)) + SP_STRING("</font>"))
#define CHAT_COLOR_DT(color, text) std::string(SP_STRING("<font color='") + std::string(SP_STRING(color)) + SP_STRING("'>") + std::string(text) + SP_STRING("</font>"))

#define DEFINE_PADDING 0
#define STR_MERGE_IMPL(x, y)                x##y
#define STR_MERGE(x,y)                        STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)                        BYTE STR_MERGE(pad_, __COUNTER__) [ size ]
#define DEFINE_MEMBER_0(x)                    x;
#define DEFINE_MEMBER_N(x,offset)            struct { MAKE_PAD((QWORD)offset - DEFINE_PADDING); x; };
#define ReadQWORD2(type,base, addr) *reinterpret_cast<type*>((QWORD)base + (QWORD)addr)
#define ReadQWORD(base, addr) *reinterpret_cast<QWORD*>((QWORD)base + (QWORD)addr)
#define ReadQWORD2(type,base, addr) *reinterpret_cast<type*>((QWORD)base + (QWORD)addr)

#define ReadFLOAT(base, addr) *reinterpret_cast<float*>((QWORD)base + addr)
#define ReadINT(base, addr) *reinterpret_cast<int*>((QWORD)base + addr)
#define ReadDWORD(base, addr) *reinterpret_cast<DWORD*>((QWORD)base + addr)
#define ReadBOOL(base, addr) *reinterpret_cast<bool*>((QWORD)base + addr)

#define ReadVTable(addr, idx) ((QWORD*)ReadQWORD(addr, 0))[(int)idx]
#define STB_IMAGE_IMPLEMENTATION
#define TryCatch(function, catchError) __try {function;} __except (1){	LOG(catchError); }
#define Compare(x, y, z) StringCompare(x, y, z)


#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)
#define PAD(size) \
private: \
    std::byte PAD_NAME(__LINE__)[size]; \
public:

#define CLASS_GETTER(returnType, name, offset) \
[[nodiscard]] inline returnType name() const noexcept \
{ \
	return *reinterpret_cast<returnType*>((QWORD)this + offset); \
}

#define CLASS_GETTER_P(returnType, name, offset) \
[[nodiscard]] inline returnType* name() const noexcept \
{ \
	return reinterpret_cast<returnType*>((QWORD)this + offset); \
}

#define STRING_GETTER(returnType, name, offset) \
[[nodiscard]] inline std::string name() const noexcept \
{ \
	return *reinterpret_cast<returnType*>((QWORD)this + offset); \
}

bool StringContains(std::string strA, std::string strB, bool ignore_case = false);
bool StringContains(std::wstring strA, std::wstring strB, bool ignore_case = false);
bool StringCompare(std::string strA, std::string strB, bool ignore_case = false);
bool StringCompare(std::wstring strA, std::wstring strB, bool ignore_case = false);
inline std::string ToLower(std::string str)
{
	std::string strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::tolower);

	return strLower;
	return str;
}

inline std::wstring ToLower(std::wstring str)
{
	std::wstring strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::tolower);

	return strLower;
	return str;
}

inline bool StringContains(std::string strA, std::string strB, bool ignore_case)
{
	if (strA.empty() || strB.empty())
		return true;

	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	if (strA.find(strB) != std::string::npos)
		return true;

	return false;
}

inline bool StringContains(std::wstring strA, std::wstring strB, bool ignore_case)
{
	if (strA.empty() || strB.empty())
		return true;

	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	if (strA.find(strB) != std::wstring::npos)
		return true;

	return false;
}

inline bool StringCompare(std::string strA, std::string strB, bool ignore_case) {
	if (strA.empty() && strB.empty())
		return false;
	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	return strA.compare(strB) == 0;
}

inline bool StringCompare(std::wstring strA, std::wstring strB, bool ignore_case) {
	if (strA.empty() && strB.empty())
		return false;
	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	return strA.compare(strB) == 0;
}

