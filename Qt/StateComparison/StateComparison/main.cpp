// плагин Windows
#ifdef WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "filebrowser.h"

int main(int argc, char *argv[])
{
	// создаем приложение
	QApplication a(argc, argv);
	// подключаем файлы с переводами и загружаем их в программу
	QTranslator qtTranslator;
    qtTranslator.load("qt_ru",":/translations/translations");
    a.installTranslator(&qtTranslator);
	// создание нового окна 
	FileBrowser *w = new FileBrowser;
	// аттрибут для удаления памяти после закрытия окна
	w->setAttribute(Qt::WA_DeleteOnClose);
	// показываем окно
	w->show();
	// запуск QApplication
	return a.exec();
}
