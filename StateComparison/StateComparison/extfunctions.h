﻿#ifndef EXTFUNCTIONS
#define EXTFUNCTIONS

/*
 * Вспомогательные шаблоные функции для переводов
 *
 * Version 1.5
 *
 * Writed by Brylkin Dmitry. 28.11.2018
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QString>
#include <sstream>
#include <string>

// для сравнения double с 0
#ifndef DBL_CALIBRATION
#define DBL_CALIBRATION 1.e-07
#endif // DBL_CALIBRATION

/*
 *  Шаблонные функции перевода в строки (отсутствуют в компиляторе msvc-2010)
 */
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
