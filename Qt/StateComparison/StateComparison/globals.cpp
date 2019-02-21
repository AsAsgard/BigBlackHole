#include "globals.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>


void initLangSuffixes()
{
    langData.LangSuffixes[Lang::EN] = "en_EN";
    langData.LangSuffixes[Lang::RU] = "ru_RU";
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
                _defaultPath = Value;
            }
        } else if (Var == "lang") {
            if (Value.toLower() == "en") {
                langData.Lang = Lang::EN;
            } else if (Value.toLower() == "ru") {
                langData.Lang = Lang::RU;
            }
        }
    }
}

QString LangToQString(const Lang::LangEnum& lang)
{
    switch (lang) {
    // English
    case(Lang::EN) :
        return QCoreApplication::tr("English");
        break;
    // Russian
    case(Lang::RU) :
        return QCoreApplication::tr("Russian");
        break;
    // Unexpected
    default:
        return "";
        break;
    }
}

void changeAppLanguage(Lang::LangEnum lang)
{
    if (lang != langData.Lang) {
        langData.Lang = lang;
        translator.load("StateComparisonFullTranslations_"
                        + ( (!langData.LangSuffixes[lang].isEmpty()) ? langData.LangSuffixes[lang] : langData.LangSuffixes[Lang::EN] )
                        ,":/translations/translations");
        qApp->installTranslator(&translator);
    }
}


// реализация стандартного пути - его получение и задание
QString _defaultPath = "./";
QString defaultPath() {return _defaultPath;}
void setDefaultPath(const QString &NewDefaultPath) { _defaultPath = NewDefaultPath;}
void setDefaultPath(QString &&NewDefaultPath) { if (&_defaultPath != &NewDefaultPath) _defaultPath = NewDefaultPath;}

// реализация имен файлов - их получение и задание
QString _State1FileName = "", _State2FileName = "";
QString State1FileName() { return _State1FileName;}
void setState1FileName(const QString &NewState1FileName) { _State1FileName = NewState1FileName;}
void setState1FileName(QString &&NewState1FileName) { if (&_State1FileName != &NewState1FileName) _State1FileName = NewState1FileName;}
QString State2FileName() { return _State2FileName;}
void setState2FileName(const QString &NewState2FileName) { _State2FileName = NewState2FileName;}
void setState2FileName(QString &&NewState2FileName) { if (&_State2FileName != &NewState2FileName) _State2FileName = NewState2FileName;}
