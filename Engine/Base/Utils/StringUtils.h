#pragma once
#include <string>
#include "Libs/utfcpp/utf8.h"
#include <boost/algorithm/string/find.hpp>

namespace S2DE::Core::Utils
{
	[[nodiscard]] inline bool isStringEmpty(std::string str)
	{
		return str == "" || str.empty();
	}

	[[nodiscard]] inline bool isWStringEmpty(std::wstring str)
	{
		return str == L"" || str.empty();
	}

	[[nodiscard]] inline bool StringAllReplace(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return false;
		}

		std::size_t start_pos = 0;

		while ((start_pos = str.find(from, start_pos)) != std::string::npos) 
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); 
		}

		return true;
	}

	[[nodiscard]] inline bool StringReplace(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return false;
		}

		std::size_t start_pos = str.find(from);

		if (start_pos == std::string::npos)
		{
			return false;
		}

		str.replace(start_pos, from.length(), to);
		return true;
	}

	template<typename T>
	[[nodiscard]] inline std::string GetClassNameInString(T obj)
	{
		//Get class name by typeid function
		std::string class_name = typeid(obj).name();

		//Remove all namespaces
		std::uint64_t pos = 0;
		while (pos != std::string::npos)
		{
			class_name = class_name.substr(pos + 1);
			pos = class_name.find(":", pos);
		}

		//Remove other garbage 
		pos = 0;
		pos = class_name.find(" ", pos);
		class_name.erase(pos, class_name.length() - 1);

		return class_name;
	}

	[[nodiscard]] inline std::string GetFileExtension(const std::string& filename)
	{
		size_t off = filename.find_last_of('.');

		if (off == std::string::npos)
			return std::string();
		
		return std::string(filename.substr(off + 1));
	}

	[[nodiscard]] inline std::wstring StringToWString(std::string str)
	{
		std::wstring wstr;
#ifdef _MSC_VER
		utf8::utf8to16(str.begin(), str.end(), std::back_inserter(wstr));
#else
		utf8::utf8to32(str.begin(), str.end(), std::back_inserter(wstr));
#endif

		return wstr;
	}

	[[nodiscard]] inline std::string WStringToString(std::wstring wstr)
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