// ������ Windows
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
	// ������� ����������
	QApplication a(argc, argv);
	// ���������� ����� � ���������� � ��������� �� � ���������
	QTranslator qtTranslator;
    qtTranslator.load("qt_ru",":/translations/translations");
    a.installTranslator(&qtTranslator);
	// �������� ������ ���� 
	FileBrowser *w = new FileBrowser;
	// �������� ��� �������� ������ ����� �������� ����
	w->setAttribute(Qt::WA_DeleteOnClose);
	// ���������� ����
	w->show();
	// ������ QApplication
	return a.exec();
}
