#pragma once
#include <string>
#include <locale>
#include <codecvt>

namespace S2DE::Core::Other
{
	inline static bool isStringEmpty(std::string str)
	{
		return str == "" || str.empty();
	}

	inline static bool isWStringEmpty(std::wstring str)
	{
		return str == L"" || str.empty();
	}
}