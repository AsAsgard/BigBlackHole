#include "setfilesmaker.h"

SetFilesMaker::SetFilesMaker(int LanguageKey,int SetDebug, QWidget * parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    this->setFixedSize(this->size());

	SetLanguage=LanguageKey;
	Set_Debug=SetDebug;
}

SetFilesMaker::~SetFilesMaker() {}

void SetFilesMaker::AddTextDisplay(const QString& text) 
{
	ui.TextBrowser->append(text);
}

void SetFilesMaker::addPlainText(const QString& text) 
{
	ui.TextBrowser->insertPlainText(text);
}

void SetFilesMaker::clearText() 
{
	ui.TextBrowser->clear();
}

void SetFilesMaker::on_OpenDirect_clicked()
{
	printf(DirToSave_Bippar);
	int z=strlen(DirToSave_Bippar);
	TWIC(DirToSave_Bippar,z);
}

void SetFilesMaker::on_FileFound_clicked()
{
	QString fileName = QFileDialog::getExistingDirectory(this,
														 tr("Open directory with file bippar"),
														 "./");
	if (!fileName.isNull())
	{
		QByteArray ba = fileName.toLocal8Bit();
		File_Name_C = ba.data();
		int len_dir = strlen(File_Name_C);
		memset(DirToSave_Bippar, 0, sizeof(DirToSave_Bippar));
		strcat(DirToSave_Bippar,File_Name_C);
		int ExitCode = 0;
		DATASIMMETRYARRAY(Set_Debug);
		if (setFilesMaker->isVisible()) FUELSORTSLIBRARY(ExitCode,File_Name_C,len_dir,Set_Debug);
		if (setFilesMaker->isVisible()) GENERWRITEPROGRAM(Set_Debug);
		ui.OpenDirect->setEnabled(true);
		ui.FileFound->setEnabled(false);
	}
}


extern "C" void CALL error_C(char *text)
{		
	QMessageBox msgBox;
	msgBox.setText(text);
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle(QMessageBox::tr("Error!"));
	msgBox.exec();
	setFilesMaker->close();
}

extern "C" void CALL error_C_bippar()
{		
    QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle(QMessageBox::tr("Error!"));
	msgBox.setText(QMessageBox::tr("File bippar do not find in current folder! Program exit."));
	msgBox.exec();
	setFilesMaker->close();
}

extern "C" void CALL PRINT(char *text)
{		
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text, "File bippar is successfully read.")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл bippar считан успешно"));}
		if(strcmp(text, "File Master.set open for writing data.")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл Master.set открыт для записи"));}
		if(strcmp(text, "File Master.set successfully written.")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл Master.set записан успешно"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->AddTextDisplay(text);
	}
}

extern "C" void CALL PRINT_INT(char *text, int& num)
{		
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text, "File burboc is successfully read in company")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл burboc считан успешно для компании:"));}
		if(strcmp(text, "File bippar is successfully read in company")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл bippar считан успешно для компании:"));}
		if(strcmp(text, "File Masteri.set open for writing data in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл Master.set открыт для записи для компании:"));}
		if(strcmp(text, "File Masteri.set successfully written in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл Master.set успашно записан для компании:"));}
		if(strcmp(text, "File bippar exists in the campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл bippar существует для кампании:"));}
		if(strcmp(text, "File UserKi.set open for writing data in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл UserKi.set открыт для записи для компании:"));}
		if(strcmp(text, "File UserKi.set NOT written in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"НЕ УДАЛОСЬ записать файл UserKi.set для компании:"));}
		if(strcmp(text, "File UserKi.set successfully written in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл UserKi.set успашно записан для компании:"));}
		if(strcmp(text, "File shlKinew open for writing data in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл shlKinew открыт для записи для компании:"));}
		if(strcmp(text, "File shlKinew successfully written in campaign")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Файл shlKinew успашно записан для компании:"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->AddTextDisplay(text);
	}
	setFilesMaker->addPlainText(QString::fromWCharArray(L" "));
	wchar_t m_reportFileName[256];
	swprintf_s(m_reportFileName, L"%d", num);
	setFilesMaker->addPlainText(QString::fromWCharArray(m_reportFileName));
}

extern "C" void CALL PRINT_INTUSER(char *text1, int& num , char *text2)
{		
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text1, "There is")==0){setFilesMaker->AddTextDisplay(QString::fromWCharArray(L"Есть"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->AddTextDisplay(text1);
	} 
	setFilesMaker->addPlainText(QString::fromWCharArray(L" "));
	wchar_t m_reportFileName[256];
	swprintf_s(m_reportFileName, L"%d", num);
	setFilesMaker->addPlainText(QString::fromWCharArray(m_reportFileName));
	setFilesMaker->addPlainText(QString::fromWCharArray(L" "));
	if (setFilesMaker->SetLanguage==0)
	{
		if(strcmp(text2, "varieties of TVS.")==0){setFilesMaker->addPlainText(QString::fromWCharArray(L"разновидности ТВС"));}
	}
	if (setFilesMaker->SetLanguage==1)
	{
		setFilesMaker->addPlainText(text2);
	} 
}

extern "C" void CALL PRINT_NULL()
{		
	setFilesMaker->AddTextDisplay(" ");
}

extern "C" void CALL ClearDispley()
{		
	setFilesMaker->clearText();
}