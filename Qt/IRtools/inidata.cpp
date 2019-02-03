#include <QDir>
#include <QFile>
#include <QTextStream>
#include "inidata.h"

void checkIni(void)
{
    QFile IniFile(IniFilename);
    if(!IniFile.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream IniStream(&IniFile);
    QString QStr("");
    QString Var("");
    QString Value("");
    while (!IniStream.atEnd()) {
        QStr = IniStream.readLine();
        int EqIndex = QStr.indexOf('=');
        if (EqIndex <= 0 || EqIndex >= QStr.size() - 1) continue;
        Var = QStr.left(EqIndex);
        Var.remove(' '); Var = Var.toLower();
        if (Var != "path" && Var != "lang") continue;
        Value = QStr.mid(EqIndex + 1);
        Value = Value.trimmed();
        if (Value.endsWith("\"") || Value.endsWith("\'")) Value.chop(1);
        if (Value.startsWith("\"") || Value.startsWith("\'")) Value.remove(0,1);
        if (Var == "path") {
            if (!Value.trimmed().isEmpty() && QDir(Value).exists()) {
                IniData.TruePath = true;
                IniData.Path = Value;
            }
        } else if (Var == "lang") {
            if (Value.toLower() == "en") {
                IniData.TrueLang = true;
                IniData.Lang = Lang::EN;
            } else if (Value.toLower() == "ru") {
                IniData.TrueLang = true;
                IniData.Lang = Lang::RU;
            }
        }
    }
}


bool writeIni(void)
{
    QFile IniFile(IniFilename);
    if(!IniFile.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream IniStream(&IniFile);
    IniStream << QString("PATH = \"%1\"\n").arg(IniData.TruePath ? IniData.Path : "./");
    IniStream << QString("LANG = \"%1\"\n").arg(IniData.TrueLang ? LangToQString(IniData.Lang) : LangToQString(Lang::EN));
    return true;
}

#define CHECK_FILE(filename) {  \
    if (!UirDir.exists(filename)) return QPair<bool, QString>(false, filename); \
}

QPair<bool, QString> checkPathIR(void)
{
    QDir UirDir("./");
#ifdef _WIN32
    CHECK_FILE("archlib.dll");
    CHECK_FILE("libicis.dll");
    CHECK_FILE("sqlite3.dll");
#endif
    if (!UirDir.cd(IniData.TruePath ? IniData.Path : "./")) return QPair<bool, QString>(false, "Fuel Load directory");
    if (!UirDir.cd("SET")) return QPair<bool, QString>(false, "directory SET");
    CHECK_FILE("master.set");
    CHECK_FILE("user.set");
    return QPair<bool, QString>(true, "");
}


QString LangToQString(const Lang::LangEnum& lang)
{
    switch (lang) {
    // English
    case(Lang::EN) :
        return "EN";
        break;
    // Russian
    case(Lang::RU) :
        return "RU";
        break;
    // Unexpected
    default:
        return "";
        break;
    }
}
