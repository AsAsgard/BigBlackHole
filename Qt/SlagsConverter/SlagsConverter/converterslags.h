#ifndef SECOND_WINDOW
#define SECOND_WINDOW

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QTextBrowser>
#include <QSpinBox>
#include <QFileDialog>

#define CALL __declspec(dllexport)

extern "C"  int TWIC(const char* x, int& y);

extern "C" void SHLAKIOLD(int& Set_Debug);

extern "C" void LISTCONV(int& Set_Debug);

extern "C" void DATAREWRITE(int& Set_Debug);

class ConverterSlags : public QWidget
{
	Q_OBJECT

public:
	explicit ConverterSlags(int LanguageKey,int SetDebug = 0);
	~ConverterSlags();
	
	void ChangeSpiner(int NZSHL_C);
	void AddTextDisplay(char* text);

	QSpinBox *OldRead;
	QTextBrowser *TextBrowser;
	QPushButton *FileFound;
	QPushButton *OpenDirect;
	QPushButton *TransformSlags;
	int SetLanguage;
	int Set_Debug;
	char* File_Name_C;
	char DirToSave[127];

private slots:
	void File_Founds();
	void ReadOld_Function();
	void Open_Direct();
};


extern int nLayersNew;
extern int LastPoint1;
extern int LastPoint2;
extern char AdressCh[127];
extern char OldName[127];
extern char NewAdress[127];
extern char nLayersNewCh[2];

extern ConverterSlags * Converter;

#endif