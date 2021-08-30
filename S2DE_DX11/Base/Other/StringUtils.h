#pragma once
#include <string>
#include <locale>
#include <codecvt>

namespace S2DE
{
	inline static bool isStringEmpty(std::string str)
	{
		return str == "" || str.empty();
	}

	inline static bool isWStringEmpty(std::wstring str)
	{
		return str == L"" || str.empty();
	}

	inline static std::string ConvertWStringToString(std::wstring str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		return converter.to_bytes(str);
	}

	inline static std::wstring ConvertStringToWString(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		return converter.from_bytes(str);
	}
}