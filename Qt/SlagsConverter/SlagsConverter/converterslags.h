#ifndef CONVERTERSLAGS_H
#define CONVERTERSLAGS_H

#include <QWidget>
#include <QFileDialog>
#include "ui_converterslags.h"

#define CALL __declspec(dllexport)

extern "C"  int TWIC(const char* x, int& y);

extern "C" void SHLAKIOLD(int& Set_Debug);

extern "C" void LISTCONV(int& Set_Debug);

extern "C" void DATAREWRITE(int& Set_Debug);

class ConverterSlags : public QWidget
{
	Q_OBJECT

public:
	explicit ConverterSlags(int LanguageKey, int SetDebug = 0, QWidget *parent = 0);
	~ConverterSlags();

	void ChangeSpiner(int NZSHL_C);
	void AddTextDisplay(const QString& text);
	void addPlainText(const QString& text);

	void setEnabledOpenDirect(bool enabled) { ui.OpenDirect->setEnabled(enabled);}
	void setEnabledTransformSlags(bool enabled) { ui.TransformSlags->setEnabled(enabled);}
	void setEnabledFileFound(bool enabled) { ui.FileFound->setEnabled(enabled);}
	void setEnabledSpinner(bool enabled) { ui.OldRead->setEnabled(enabled);}

	int SetLanguage;
	int Set_Debug;
	char* File_Name_C;
	char DirToSave[127];

private slots:
	void on_FileFound_clicked();
	void on_TransformSlags_clicked();
	void on_OpenDirect_clicked();

private:
	Ui::ConverterSlags ui;
};

extern int nLayersNew;
extern int LastPoint1;
extern int LastPoint2;
extern char AdressCh[127];
extern char OldName[127];
extern char NewAdress[127];
extern char nLayersNewCh[2];

extern ConverterSlags * Converter;


#endif // CONVERTERSLAGS_H
