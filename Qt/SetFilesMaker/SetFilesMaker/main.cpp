#ifdef _WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

#include "setfilesmaker.h"
#include <QApplication>

SetFilesMaker * setFilesMaker = nullptr;

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
	 setFilesMaker = new SetFilesMaker(0);
	 setFilesMaker->setAttribute(Qt::WA_DeleteOnClose);
     setFilesMaker->show();
     return app.exec();
 }