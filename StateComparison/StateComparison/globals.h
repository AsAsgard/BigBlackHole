#ifndef GLOBALS_H
#define GLOBALS_H

/*
 * Глобальные данные для работы с параметрами приложения
 *
 * Version 1.12
 *
 * Writed by Brylkin Dmitry. 01.12.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QString>
#include <QMap>
#include <QTranslator>

/*
 *  ЯЗЫКОВОЙ БЛОК
 */

/*
 * Перечисление языков, поддерживаемых программой
 */
namespace Lang {
    enum LangEnum {
        EN,
        RU
    };
}

/*
 * Структура LanguageData
 *
 * Языковые данные приложения:
 *  1) Язык
 *  2) Суффиксы файлов переводов для различных языков
 */
struct LanguageData {
	explicit LanguageData() : Lang(Lang::EN) {} 

    Lang::LangEnum Lang;
    QMap<Lang::LangEnum, QString> LangSuffixes;
};

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// объект структуры языковых данных
extern LanguageData langData;
// название .ini файла
extern QString IniFilename;
// переводчик (подключение файлов перевода)
extern QTranslator translator;

// ФУНКЦИИ
// считывание файла начальных данных
void checkIni(void);
// инициализация суффиксов файлов переводов для различных языков
void initLangSuffixes(void);
// функция перевода перечисления языка в его аббревиатуру
QString langToQString(Lang::LangEnum lang);
// изменение языка приложения
void changeAppLanguage(Lang::LangEnum);

/*
 * БЛОК ПУТЕЙ К ФАЙЛАМ И ДИРЕКТОРИЯМ
 */

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// путь, по которому изначально открываются все обозреватели файлов
extern QString _defaultPath;
// имена файлов с состояниями
extern QString _State1FileName, _State2FileName;

// получить/установить папку по умолчанию для открытия FileDialog
QString defaultPath();
void setDefaultPath(const QString &NewDefaultPath);
void setDefaultPath(QString &&NewDefaultPath);

// получить/установить имя файла с первым состоянием
QString State1FileName();
void setState1FileName(const QString &NewState1FileName);
void setState1FileName(QString &&NewState1FileName);

// получить/установить имя файла со вторым состоянием
QString State2FileName();
void setState2FileName(const QString &NewState2FileName);
void setState2FileName(QString &&NewState2FileName);

#endif // GLOBALS_H
