#pragma once
#include <string>
#include "Libs/utfcpp/utf8.h"
#include <boost/algorithm/string/find.hpp>

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

	template<typename T>
	inline std::string GetClassNameInString(T obj)
	{
		std::string class_name = typeid(obj).name();
		std::uint32_t pos = 0;

		while (pos != std::string::npos)
		{
			pos = class_name.find("::");
			class_name = class_name.substr(pos + 1);
		}

		class_name.erase(std::remove(class_name.begin(), class_name.end(), ':'), class_name.end());
		return class_name;
	}

	inline std::string GetFileExtension(const std::string& filename)
	{
		size_t off = filename.find_last_of('.');

		if (off == std::string::npos)
			return std::string();
		
		return std::string(filename.substr(off + 1));
	}

	inline std::wstring StringToWString(std::string str)
	{
		std::wstring wstr;
#ifdef _MSC_VER
		utf8::utf8to16(str.begin(), str.end(), std::back_inserter(wstr));
#else
		utf8::utf8to32(str.begin(), str.end(), std::back_inserter(wstr));
#endif

		return wstr;
	}

	inline std::string WStringToString(std::wstring wstr)
	{
		std::string str;

#ifdef _MSC_VER
		utf8::utf16to8(wstr.begin(), wstr.end(), std::back_inserter(str));
#else
		utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(str));
#endif
		return str;
	}
}