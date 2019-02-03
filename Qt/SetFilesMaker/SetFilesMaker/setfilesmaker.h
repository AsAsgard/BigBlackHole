#ifndef SETFILESMAKER
#define SETFILESMAKER

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>

#define CALL __declspec(dllexport)

extern "C"  int TWIC(const char* x, int& y);

extern "C" void DATASIMMETRYARRAY(int& Debug);

extern "C" void FUELSORTSLIBRARY(int& ExitCode,const char* adress, int& len_dir, int& Debug);

extern "C" void GENERWRITEPROGRAM(int& Debug);


class SetFilesMaker : public QWidget
{
	Q_OBJECT

public:
	SetFilesMaker(int LanguageKey,int SetDebug = 0);
	~SetFilesMaker();

	QPushButton *FileFound;
	QPushButton *OpenDirect;
	QTextBrowser *TextBrowser;
	int SetLanguage;
	int Set_Debug;
	char* File_Name_C;
	char DirToSave_Bippar[127];

private slots:
	void Open_Finder();
	void Open_Direct();
};

extern SetFilesMaker * setFilesMaker;

#endif // SETFILESMAKER