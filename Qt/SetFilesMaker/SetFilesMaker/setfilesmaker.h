#ifndef SETFILESMAKER_H
#define SETFILESMAKER_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_setfilesmaker.h"

#define CALL __declspec(dllexport)

extern "C"  int TWIC(const char* x, int& y);

extern "C" void DATASIMMETRYARRAY(int& Debug);

extern "C" void FUELSORTSLIBRARY(int& ExitCode,const char* adress, int& len_dir, int& Debug);

extern "C" void GENERWRITEPROGRAM(int& Debug);

class SetFilesMaker : public QWidget
{
	Q_OBJECT

public:
	explicit SetFilesMaker(int LanguageKey,int SetDebug = 0, QWidget *parent = 0);
	~SetFilesMaker();

	void AddTextDisplay(const QString& text);
	void addPlainText(const QString& text);
	void clearText();

	void setEnabledOpenDirect(bool enabled) { ui.OpenDirect->setEnabled(enabled);}
	void setEnabledFileFound(bool enabled) { ui.FileFound->setEnabled(enabled);}

	int SetLanguage;
	int Set_Debug;
	char* File_Name_C;
	char DirToSave_Bippar[127];

private slots:
	void on_OpenDirect_clicked();
	void on_FileFound_clicked();

private:
	Ui::SetFilesMaker ui;
};

extern SetFilesMaker * setFilesMaker;

#endif // SETFILESMAKER_H
