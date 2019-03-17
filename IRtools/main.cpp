/*
 * Функция main()
 *
 * Version 1.5
 *
 * Writed by Brylkin Dmitry. 30.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QApplication>
#include "initialwindow.h"
#include "processid.h"
#include "inidata.h"

// создание основных глобальных переменных
QMap<Programs::ProgramsEnum, ProcessInfo> ProcessID;
QMap<Lang::LangEnum, QString> LangSuffixes;
InitialData IniData;
QString IniFilename("IRtools.ini");
QTranslator translator;

int main(int argc, char *argv[])
{
    // сопоставляем программам их названия
    initProcessID();
    // инициализируем словарь суффиксов файлов с переводами
    initLangSuffixes();
    // считываем IRtools.ini (при наличии)
    checkIni();

    // создаем приложение Qt
    QApplication a(argc, argv);

    // подгружаем в приложение первый переводчик (на основе данных, считанных в IRtools.ini)
    translator.load("IRtoolsTranslations_"
                    + ( (!LangSuffixes[IniData.Lang].isEmpty()) ? LangSuffixes[IniData.Lang] : LangSuffixes[Lang::EN] )
                    ,":/translations");
    qApp->installTranslator(&translator);

    // создаем начальное окно
    InitialWindow * w = new InitialWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    // запускаем приложение Qt
    return a.exec();
}
