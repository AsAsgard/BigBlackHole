#ifndef EXTFUNCTIONS
#define EXTFUNCTIONS

#include <QString>
#include <sstream>
#include <string>

// шаблонные функции перевода в строки (отсутствуют в компиляторе msvc-2010)
namespace ExtFunctions {
	template<typename T> std::string to_string(const T &inparam)
	{
		std::string str = "";
		std::stringstream ss;
		ss << inparam;
		std::getline(ss,str);
		return str;
	}

	template<typename T> std::wstring to_wstring(const T &inparam)
	{
		std::wstring wstr = L"";
		std::wstringstream wss;
		wss << inparam;
		std::getline(wss,wstr);
		return wstr;
	}

	template<typename T> QString to_QString(const T &inparam)
	{
		std::wstring wstr = L"";
		std::wstringstream wss;
		wss << inparam;
		std::getline(wss,wstr);
		return QString::fromStdWString(wstr);
	}
}

#endif  // EXTFUNCTIONS