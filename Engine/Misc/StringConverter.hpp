#ifndef STRING_CONVERTER_HPP
#define STRING_CONVERTER_HPP
#include <string>
#include <optional>

namespace StringConverter
{
	std::optional<std::wstring> ToWideString(const std::string& str);
	std::optional<std::string> ToString(const std::wstring& wstr);
}

#endif