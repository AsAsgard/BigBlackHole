#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QMap>
#include <QTranslator>

namespace Lang {
    enum LangEnum {
        EN,
        RU
    };
}

struct LanguageData {
	explicit LanguageData() : Lang(Lang::EN) {} 

    Lang::LangEnum Lang;
    QMap<Lang::LangEnum, QString> LangSuffixes;
};

extern LanguageData langData;
extern QString IniFilename;
extern QTranslator translator;

void checkIni(void);
void initLangSuffixes(void);
QString LangToQString(const Lang::LangEnum& lang);
void changeAppLanguage(Lang::LangEnum);


extern QString _defaultPath;
extern QString _State1FileName, _State2FileName;

// получить/установить папку по умолчанию для открытия FileDialog
QString defaultPath();
void setDefaultPath(const QString &NewDefaultPath);
void setDefaultPath(QString &&NewDefaultPath);

// получить/установить имя файла с первым состоянием
QString State1FileName();
void setState1FileName(const QString &NewState1FileName);
void setState1FileName(QString &&NewState1FileName);

// получить/установить имя файла со вторым состоянием
QString State2FileName();
void setState2FileName(const QString &NewState2FileName);
void setState2FileName(QString &&NewState2FileName);

#endif // GLOBALS_H
