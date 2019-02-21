#include "initialwindow.h"
#include "processid.h"
#include "inidata.h"
#include <QApplication>

QMap<Programs::ProgramsEnum, ProcessInfo> ProcessID;
QMap<Lang::LangEnum, QString> LangSuffixes;
InitialData IniData;
QString IniFilename("IRtools.ini");
QString thisProgramName("");
QTranslator translator;

int main(int argc, char *argv[])
{
    initProcessID();
    initLangSuffixes();
    checkIni();

    QApplication a(argc, argv);

    translator.load("IRtoolsTranslations_"
                    + ( (!LangSuffixes[IniData.Lang].isEmpty()) ? LangSuffixes[IniData.Lang] : LangSuffixes[Lang::EN] )
                    ,":/translations");
    qApp->installTranslator(&translator);

    thisProgramName = QApplication::applicationName();
    InitialWindow * w = new InitialWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    return a.exec();
}
