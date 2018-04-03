#ifndef STRING_CONVERTER_HPP
#define STRING_CONVERTER_HPP
#include <string>

namespace StringConverter
{
	std::wstring ToWideString(const std::string& str);
	std::string ToString(const std::wstring& wstr);
}

#endif