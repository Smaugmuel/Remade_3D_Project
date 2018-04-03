#include "StringConverter.hpp"
#include <locale>
#include <codecvt>

std::wstring StringConverter::ToWideString(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

std::string StringConverter::ToString(const std::wstring & wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}
