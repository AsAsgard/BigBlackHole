/*
 * Глобальные данные для работы с параметрами приложения (реализация)
 *
 * Version 1.17
 *
 * Writed by Brylkin Dmitry. 01.12.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include "globals.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>

// инициализация суффиксов файлов переводов для различных языков
void initLangSuffixes()
{
    langData.LangSuffixes[Lang::EN] = "en_EN";
    langData.LangSuffixes[Lang::RU] = "ru_RU";
}

// считывание файла начальных данных
void checkIni(void)
{
    // ЛОКАЛЬНЫЕ ПЕРЕМЕННЫЕ
    // сюда считываем строки из файла
    QString QStr("");
    // название переменной
    QString Var("");
    // значение в строке
    QString Value("");

    // открываем файл для записи
    // если неудачно - возвращаемся
    QFile IniFile(IniFilename);
    if(!IniFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    // подключаем текстовый поток для чтения из файла
    QTextStream IniStream(&IniFile);

    // цикл до конца файла
    while (!IniStream.atEnd()) {
        // считываем строку
        QStr = IniStream.readLine();
        // находим '='
        int EqIndex = QStr.indexOf('=');
        // если его нет - к следующей строке
        if (EqIndex <= 0 || EqIndex >= QStr.size() - 1) continue;
        // считываем имя переменной (слева от '=')
        Var = QStr.left(EqIndex);
        // удаляем пробелы и переводим название в нижний регистр
        Var.remove(' ');
        Var = Var.toLower();
        // если не path или lang - к следующей строке
        if (Var != "path" && Var != "lang") continue;
        // значение получаем справа от '=' и удаляем первичные и завершающие пробелы
        Value = QStr.mid(EqIndex + 1);
        Value = Value.trimmed();
        // удаляем кавычки при наличии
        if (Value.endsWith("\"") || Value.endsWith("\'")) Value.chop(1);
        if (Value.startsWith("\"") || Value.startsWith("\'")) Value.remove(0,1);
        // если путь
        if (Var == "path") {
            // проверяем, что поле не пустое и что путь существует
            if (!Value.trimmed().isEmpty() && QDir(Value).exists()) {
                // задаем путь и ставим флаг верной загрузки
                _defaultPath = Value;
            }
        // если язык
        } else if (Var == "lang") {
            if (Value.toLower() == "en") {
                // английский
                langData.Lang = Lang::EN;
            } else if (Value.toLower() == "ru") {
                // русский
                langData.Lang = Lang::RU;
            }
        }
    }
}

// функция перевода перечисления языка в его аббревиатуру
QString langToQString(Lang::LangEnum lang)
{
    // в зависимости от значения перечисления языка выводим его аббревиатуру
    switch (lang) {
    // English
    case(Lang::EN) :
        return "EN";
        break;
    // Russian
    case(Lang::RU) :
        return "RU";
        break;
    // Unexpected
    default:
        return "";
        break;
    }
}

// изменение языка приложения
void changeAppLanguage(Lang::LangEnum lang)
{
    // перевод осуществляем, если язык другой
    if (lang != langData.Lang) {
        // задаем значение языка
        langData.Lang = lang;
        // загружаем перевод в переводчик
        translator.load("StateComparisonFullTranslations_"
                        + ( (!langData.LangSuffixes[lang].isEmpty()) ? langData.LangSuffixes[lang] : langData.LangSuffixes[Lang::EN] )
                        ,":/translations/translations");
        // устанавливаем переводчик в приложение
        qApp->installTranslator(&translator);
    }
}


// реализация стандартного пути - его получение и задание
QString _defaultPath = "./";
QString defaultPath() {return _defaultPath;}
void setDefaultPath(const QString &NewDefaultPath) { _defaultPath = NewDefaultPath;}
void setDefaultPath(QString &&NewDefaultPath) { if (&_defaultPath != &NewDefaultPath) _defaultPath = NewDefaultPath;}

// реализация имен файлов - их получение и задание
QString _State1FileName = "", _State2FileName = "";
QString State1FileName() { return _State1FileName;}
void setState1FileName(const QString &NewState1FileName) { _State1FileName = NewState1FileName;}
void setState1FileName(QString &&NewState1FileName) { if (&_State1FileName != &NewState1FileName) _State1FileName = NewState1FileName;}
QString State2FileName() { return _State2FileName;}
void setState2FileName(const QString &NewState2FileName) { _State2FileName = NewState2FileName;}
void setState2FileName(QString &&NewState2FileName) { if (&_State2FileName != &NewState2FileName) _State2FileName = NewState2FileName;}
