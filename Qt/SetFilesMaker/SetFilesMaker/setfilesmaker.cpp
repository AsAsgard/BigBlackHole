#include "setfilesmaker.h"

SetFilesMaker::SetFilesMaker(int LanguageKey,int SetDebug)
{
    setFixedSize(600, 161);

	SetLanguage=LanguageKey;
	Set_Debug=SetDebug;


    FileFound = new QPushButton(QString::fromWCharArray(L"Открыть файл"), this);
	if (LanguageKey==1)
	{
		FileFound->setText(QString::fromWCharArray(L"Open file"));
	}
    FileFound->setGeometry(20, 10, 135, 25);
    FileFound->setFont(QFont("Times", 12));
	FileFound->setStyleSheet(
        "QPushButton{"
		"background-color: rgb(220,220,220);"
		"border-style: outset;"
		"border-width: 2px;"
		"border-radius: 10px;"
		"border-color: beige;"
		"min-width: 10em;}");

	OpenDirect = new QPushButton(QString::fromWCharArray(L"Открыть папку"), this);
	if (LanguageKey==1)
	{
		OpenDirect->setText(QString::fromWCharArray(L"Open directory"));
	}
    OpenDirect->setGeometry(20, 125, 135, 25);
    OpenDirect->setFont(QFont("Times", 11));
	OpenDirect->setStyleSheet(
        "QPushButton{"
		"background-color: rgb(220,220,220);"
		"border-style: outset;"
		"border-width: 2px;"
		"border-radius: 10px;"
		"border-color: beige;"
		"min-width: 10em;}");
	OpenDirect->setEnabled(false);
	//  
	QGroupBox *Shadow = new QGroupBox(this);
	Shadow->setGeometry(20, 45, 135, 70);

	TextBrowser = new QTextBrowser (this);
	TextBrowser->setGeometry(190, 10, 400, 140);

	connect(FileFound, SIGNAL(clicked()), this, SLOT(Open_Finder()));
	connect(OpenDirect, SIGNAL(clicked()), this, SLOT(Open_Direct()));
}

SetFilesMaker::~SetFilesMaker() {}

void SetFilesMaker::Open_Direct()
{
	printf(DirToSave_Bippar);
	int z=strlen(DirToSave_Bippar);
	TWIC(DirToSave_Bippar,z);
}

void SetFilesMaker::Open_Finder()
{
	QFileDialog *FileFinder = new QFileDialog();
	QString fileName = FileFinder->getExistingDirectory();
	if (fileName!=NULL){
	QByteArray ba = fileName.toLocal8Bit();
	File_Name_C = ba.data();
	int len_dir = strlen(File_Name_C);
	memset(DirToSave_Bippar, 0, sizeof(DirToSave_Bippar));
	strcat(DirToSave_Bippar,File_Name_C);
	int ExitCode = 0;
	DATASIMMETRYARRAY(Set_Debug);
	if (setFilesMaker->isVisible()) FUELSORTSLIBRARY(ExitCode,File_Name_C,len_dir,Set_Debug);
	if (setFilesMaker->isVisible()) GENERWRITEPROGRAM(Set_Debug);
	OpenDirect->setEnabled(true);
	FileFound->setEnabled(false);}
}


extern "C" void CALL error_C(char *text)
{		
	QMessageBox msgBox;
	msgBox.setText(text);
	msgBox.exec();
	setFilesMaker->close();
}

extern "C" void CALL error_C_bippar(char *text)
{		
    QMessageBox msgBox;
	if (setFilesMaker->SetLanguage==1){msgBox.setText(text);}
	if (setFilesMaker->SetLanguage==0){msgBox.setText(QString::fromWCharArray(L"Ошибка! Не найден файл Bippar в данной директории"));}
	msgBox.exec();
	setFilesMaker->close();
}

extern "C" void CALL PRINT(char *text)
{		
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text, "File bippar is successfully read.")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл bippar считан успешно"));}
		if(strcmp(text, "File Master.set open for writing data.")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл Master.set открыт для записи"));}
		if(strcmp(text, "File Master.set successfully written.")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл Master.set записан успешно"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->TextBrowser->append(text);
	}
	setFilesMaker->TextBrowser->repaint();
}

extern "C" void CALL PRINT_INT(char *text, int& num)
{		
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text, "File burboc is successfully read in company")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл burboc считан успешно для компании:"));}
		if(strcmp(text, "File bippar is successfully read in company")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл bippar считан успешно для компании:"));}
		if(strcmp(text, "File Masteri.set open for writing data in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл Master.set открыт для записи для компании:"));}
		if(strcmp(text, "File Masteri.set successfully written in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл Master.set успашно записан для компании:"));}
		if(strcmp(text, "File bippar exists in the campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл bippar существует для кампании:"));}
		if(strcmp(text, "File UserKi.set open for writing data in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл UserKi.set открыт для записи для компании:"));}
		if(strcmp(text, "File UserKi.set NOT written in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"НЕ УДАЛОСЬ записать файл UserKi.set для компании:"));}
		if(strcmp(text, "File UserKi.set successfully written in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл UserKi.set успашно записан для компании:"));}
		if(strcmp(text, "File shlKinew open for writing data in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл shlKinew открыт для записи для компании:"));}
		if(strcmp(text, "File shlKinew successfully written in campaign")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Файл shlKinew успашно записан для компании:"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->TextBrowser->append(text);
	}
	setFilesMaker->TextBrowser->insertPlainText(QString::fromWCharArray(L" "));
	wchar_t m_reportFileName[256];
	swprintf_s(m_reportFileName, L"%d", num);
	setFilesMaker->TextBrowser->insertPlainText(QString::fromWCharArray(m_reportFileName));
	setFilesMaker->TextBrowser->repaint();
}

extern "C" void CALL PRINT_INTUSER(char *text1, int& num , char *text2)
{		
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text1, "There is")==0){setFilesMaker->TextBrowser->append(QString::fromWCharArray(L"Есть"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->TextBrowser->append(text1);
	} 
	setFilesMaker->TextBrowser->insertPlainText(QString::fromWCharArray(L" "));
	wchar_t m_reportFileName[256];
	swprintf_s(m_reportFileName, L"%d", num);
	setFilesMaker->TextBrowser->insertPlainText(QString::fromWCharArray(m_reportFileName));
	setFilesMaker->TextBrowser->insertPlainText(QString::fromWCharArray(L" "));
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text2, "varieties of TVS.")==0){setFilesMaker->TextBrowser->insertPlainText(QString::fromWCharArray(L"разновидности ТВС"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->TextBrowser->insertPlainText(text2);
	} 
	   
	setFilesMaker->TextBrowser->repaint();
}

extern "C" void CALL PRINT_NULL()
{		
	setFilesMaker->TextBrowser->append(" ");
	setFilesMaker->TextBrowser->repaint();
}

extern "C" void CALL ClearDispley()
{		
	setFilesMaker->TextBrowser->clear();
	setFilesMaker->TextBrowser->repaint();
}