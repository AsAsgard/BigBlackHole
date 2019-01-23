#include <QString>

extern QString _defaultPath;
extern QString _State1FileName, _State2FileName;

// ��������/���������� ����� �� ��������� ��� �������� FileDialog
QString defaultPath();
void setDefaultPath(const QString &NewDefaultPath);
void setDefaultPath(QString &&NewDefaultPath);

// ��������/���������� ��� ����� � ������ ����������
QString State1FileName();
void setState1FileName(const QString &NewState1FileName);
void setState1FileName(QString &&NewState1FileName);

// ��������/���������� ��� ����� �� ������ ����������
QString State2FileName();
void setState2FileName(const QString &NewState2FileName);
void setState2FileName(QString &&NewState2FileName);