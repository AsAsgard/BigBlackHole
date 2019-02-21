#include <QDir>
#include <QFile>
#include <QTextStream>
#include "inidata.h"

void initLangSuffixes()
{
    LangSuffixes[Lang::EN] = "en_EN";
    LangSuffixes[Lang::RU] = "ru_RU";
}

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
                IniData.Lang = Lang::EN;
            } else if (Value.toLower() == "ru") {
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
    IniStream << QString("LANG = \"%1\"\n").arg(LangToQString(IniData.Lang));
    return true;
}

#define CHECK_FILE(filename, type) {  \
    if (!UirDir.exists(filename)) return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::type, filename); \
}

QPair<DoesntExist::DoesntExistEnum, QString> checkPathIR(void)
{
    QDir UirDir("./");
#ifdef _WIN32
    CHECK_FILE("archlib.dll", DLL);
    CHECK_FILE("libicis.dll", DLL);
    CHECK_FILE("sqlite3.dll", DLL);
#endif
    if (!UirDir.cd(IniData.TruePath ? IniData.Path : "./")) return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::SET, "Fuel Load directory");
    if (!UirDir.cd("SET")) return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::SET, "directory SET");
    CHECK_FILE("master.set", SET);
    CHECK_FILE("user.set", SET);
    return QPair<DoesntExist::DoesntExistEnum, QString>(DoesntExist::ALL_EXIST, "");
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

void changeAppLanguage(Lang::LangEnum lang)
{
    if (lang != IniData.Lang) {
        IniData.Lang = lang;
        translator.load("IRtoolsTranslations_"
                        + ( (!LangSuffixes[lang].isEmpty()) ? LangSuffixes[lang] : LangSuffixes[Lang::EN] )
                        ,":/translations");
        qApp->installTranslator(&translator);
    }
}
