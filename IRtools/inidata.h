#ifndef INIDATA_H
#define INIDATA_H

/*
 * Вспомогательные структуры для работы с параметрами приложения
 *
 * Version 1.5
 *
 * Writed by Brylkin Dmitry. 02.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QString>
#include <QPair>
#include <QTranslator>

/*
 * Перечисление категорий возможных отсутствующих
 * файлов или директорий при запуске программы
 * "Имитатор Реактора"
 */
namespace DoesntExist {
    enum DoesntExistEnum {
        ALL_EXIST,
        DLL,
        SET
    };
}

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
 * Структура InitialData
 *
 * Начальные данные приложения:
 *  1) Язык
 *  2) Путь - входная точка запускаемых программ
 *  3) Корректность пути
 */
struct InitialData {
    Lang::LangEnum Lang = Lang::EN;
    bool TruePath = false;
    QString Path = "./";
};

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// объект структуры начальных данных
extern InitialData IniData;
// суффиксы файлов переводов для различных языков
extern QMap<Lang::LangEnum, QString> LangSuffixes;
// название .ini файла
extern QString IniFilename;
// переводчик (подключение файлов перевода)
extern QTranslator translator;

// ФУНКЦИИ
// считывание файла начальных данных
void checkIni(void);
// запись файла начальных данных
bool writeIni(void);
// проверка необходимых файлов для запуска программы ИР
QPair<DoesntExist::DoesntExistEnum, QString> checkPathIR(void);
// функция перевода перечисления языка в его аббревиатуру
QString langToQString(Lang::LangEnum lang);
// инициализация суффиксов файлов переводов для различных языков
void initLangSuffixes(void);
// изменение языка приложения
void changeAppLanguage(Lang::LangEnum);

/*
 * Макрос ALIGNMENT
 *
 * Используется для выравнивания окна - отодвигает окно от краев экрана
 *
 * Работает с геометрией экрана родителя и перемещает текущее окно на
 * некоторое расстояние от границы этого экрана
 */
#ifndef ALIGNMENT
#define ALIGNMENT(window, parent) { \
    QDesktopWidget Desktop; \
    QPoint parentCenter = parent->frameGeometry().center(); \
    window.move(parentCenter.x() - window.width() / 2, parentCenter.y() - window.height() / 2);  \
    if (window.x() <= Desktop.screenGeometry(parent).x() + 0.030 * Desktop.screenGeometry(parent).width())  \
        window.move(Desktop.screenGeometry(parent).x() + 0.030 * Desktop.screenGeometry(parent).width(),window.y());   \
    if (window.y() <= Desktop.screenGeometry(parent).y() + 0.050 * Desktop.screenGeometry(parent).height())  \
        window.move(window.x(),Desktop.screenGeometry(parent).y() + 0.050 * Desktop.screenGeometry(parent).height());   \
    if (window.x() >= Desktop.screenGeometry(parent).x() + Desktop.screenGeometry(parent).width() - 0.030 * Desktop.screenGeometry(parent).width() - window.width())  \
        window.move(Desktop.screenGeometry(parent).x() + Desktop.screenGeometry(parent).width() - 0.030 * Desktop.screenGeometry(parent).width() - window.width(),window.y());   \
    if (window.y() >= Desktop.screenGeometry(parent).y() + Desktop.screenGeometry(parent).height() - 0.095 * Desktop.screenGeometry(parent).height() - window.height())  \
        window.move(window.x(),Desktop.screenGeometry(parent).y() + Desktop.screenGeometry(parent).height() - 0.095 * Desktop.screenGeometry(parent).height() - window.height());   \
}
#endif // ALIGNMENT


#endif // INIDATA_H
