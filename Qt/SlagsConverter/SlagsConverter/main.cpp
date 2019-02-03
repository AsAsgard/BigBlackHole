#ifdef _WIN32
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

#include "converterslags.h"
#include <QApplication>

ConverterSlags * Converter = nullptr;

int main(int argc, char *argv[])
{
 QApplication app(argc, argv);
 Converter = new ConverterSlags(0);
 Converter->setAttribute(Qt::WA_DeleteOnClose);
 Converter->show();
 return app.exec();
}
