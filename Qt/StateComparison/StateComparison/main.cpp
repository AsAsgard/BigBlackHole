// плагин Windows
#ifdef _WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "filebrowser.h"
#include "globals.h"

LanguageData langData;
QString IniFilename("IRtools.ini");
QTranslator translator;

int main(int argc, char *argv[])
{
    initLangSuffixes();
    checkIni();

    // создаем приложение
	QApplication a(argc, argv);

    // установка изначального значения языка приложения
    translator.load("StateComparisonFullTranslations_"
                    + ( (!langData.LangSuffixes[langData.Lang].isEmpty()) ? langData.LangSuffixes[langData.Lang] : langData.LangSuffixes[Lang::EN] )
                    ,":/translations/translations");
    qApp->installTranslator(&translator);

	// создание нового окна 
	FileBrowser *w = new FileBrowser;
	// аттрибут для удаления памяти после закрытия окна
	w->setAttribute(Qt::WA_DeleteOnClose);
	// показываем окно
	w->show();
	// запуск QApplication
	return a.exec();
}
