#include "StringConverter.hpp"
#include <Windows.h>

std::optional<std::wstring> StringConverter::ToWideString(const std::string& str)
{
	std::wstring wstr;
	
	int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
	
	wstr.resize(size);
	
	int result = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), const_cast<wchar_t*>(wstr.data()), size);
	
	if (result == 0)
	{
		return std::optional<std::wstring>();
	}

	return wstr;
}

std::optional<std::string> StringConverter::ToString(const std::wstring & wstr)
{
	std::string str;
	char buffer[1024];

	int size1 = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), buffer, sizeof(buffer), NULL, NULL);

	str.resize(size1);

	int size2 = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(str.size()), str.data(), size1, NULL, NULL);

	if (size2 == 0)
	{
		return std::optional<std::string>();
	}
	return str;
}
