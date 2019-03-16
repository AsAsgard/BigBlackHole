/*
 * Вспомогательные структуры для работы с параметрами приложения (реализация)
 *
 * Version 1.6
 *
 * Writed by Brylkin Dmitry. 02.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QDir>
#include <QFile>
#include <QTextStream>
#include "inidata.h"

// инициализация суффиксов файлов переводов для различных языков
void initLangSuffixes()
{
    LangSuffixes[Lang::EN] = "en_EN";
    LangSuffixes[Lang::RU] = "ru_RU";
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
                IniData.TruePath = true;
                IniData.Path = Value;
            }
        // если язык
        } else if (Var == "lang") {
            if (Value.toLower() == "en") {
                // английский
                IniData.Lang = Lang::EN;
            } else if (Value.toLower() == "ru") {
                // русский
                IniData.Lang = Lang::RU;
            }
        }
    }
}

// запись файла начальных данных
bool writeIni(void)
{
    // открываем файл для записи
    // если неудачно - возвращаем false
    QFile IniFile(IniFilename);
    if(!IniFile.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    // подключаем текстовый поток для записи в файл
    QTextStream IniStream(&IniFile);
    // записываем в файл путь и язык
    IniStream << QString("PATH = \"%1\"\n").arg(IniData.TruePath ? IniData.Path : "./");
    IniStream << QString("LANG = \"%1\"\n").arg(langToQString(IniData.Lang));
    return true;
}

// Макрос CHECK_FILE - проверяет наличие файла(директории), а в случае отсутствия - возвращает что отсутствует
#define CHECK_FILE(filename, type) {  \
    if (!UirDir.exists(filename)) return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::type, filename); \
}

// проверка необходимых файлов для запуска программы ИР
QPair<DoesntExist::DoesntExistEnum, QString> checkPathIR(void)
{
    QDir UirDir("./");
// проверка только на Windows - dll, необходимые для работы ИР
#ifdef _WIN32
    CHECK_FILE("archlib.dll", DLL);
    CHECK_FILE("libicis.dll", DLL);
    CHECK_FILE("sqlite3.dll", DLL);
#endif
    // Проверяем наличие директории загрузки, папки SET, master.set и user.set
    if (!UirDir.cd(IniData.TruePath ? IniData.Path : "./")) return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::SET, "Fuel Load directory");
    if (!UirDir.cd("SET")) return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::SET, "directory SET");
    CHECK_FILE("master.set", SET);
    CHECK_FILE("user.set", SET);
    // если все на месте - возвращаем, что ничего не отсутствует
    return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::ALL_EXIST, "");
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
    if (lang != IniData.Lang) {
        // задаем значение языка
        IniData.Lang = lang;
        // загружаем перевод в переводчик
        translator.load("IRtoolsTranslations_"
                        + ( (!LangSuffixes[lang].isEmpty()) ? LangSuffixes[lang] : LangSuffixes[Lang::EN] )
                        ,":/translations");
        // устанавливаем переводчик в приложение
        qApp->installTranslator(&translator);
    }
}
