#include "initialwindow.h"
#include "processid.h"
#include "inidata.h"
#include <QApplication>

QMap<Programs::ProgramsEnum, ProcessInfo> ProcessID;
InitialData IniData;
QString IniFilename("IRtools.ini");

int main(int argc, char *argv[])
{
    InitProcessID();
    checkIni();

    QApplication a(argc, argv);
    InitialWindow * w = new InitialWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    return a.exec();
}
