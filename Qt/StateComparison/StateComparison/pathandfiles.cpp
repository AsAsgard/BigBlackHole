#include "pathandfiles.h"

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