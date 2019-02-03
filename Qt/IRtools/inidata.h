#ifndef INIDATA_H
#define INIDATA_H

#include <QMessageBox>
#include <QDesktopWidget>
#include <QString>
#include <QPair>

namespace Lang {
    enum LangEnum {
        EN,
        RU
    };
}

struct InitialData {
    bool TrueLang = false;
    Lang::LangEnum Lang = Lang::EN;
    bool TruePath = false;
    QString Path = "./";
};

extern InitialData IniData;
extern QString IniFilename;

void checkIni(void);
bool writeIni(void);
QPair<bool, QString> checkPathIR(void);
QString LangToQString(const Lang::LangEnum& lang);


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
