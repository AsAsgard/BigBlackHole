#include "converterslags.h"

int nLayersNew;
int LastPoint1;
int LastPoint2;
char AdressCh[127];
char OldName[127];
char NewAdress[127];
char nLayersNewCh[2];

ConverterSlags::ConverterSlags(int LanguageKey,int SetDebug)
{
	 setFixedSize(600, 161);

	 SetLanguage=LanguageKey;
	 Set_Debug=SetDebug;

     FileFound = new QPushButton(QString::fromWCharArray(L"Открыть файл"),this);
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
	 FileFound->setEnabled(true);

	 OpenDirect = new QPushButton(QString::fromWCharArray(L"Открыть папку"),this);
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

	 QGroupBox *Shadow = new QGroupBox(this);
	 Shadow->setGeometry(20, 45, 135, 70);

	 OldRead = new QSpinBox(this);
	 OldRead->setGeometry(30, 70, 45, 25);
	 OldRead->setMinimum(1);
	 OldRead->setMaximum(99);
     OldRead->setFont(QFont("Times", 12, QFont::Bold));
	 OldRead->setEnabled(false);

	 TransformSlags = new QPushButton(QString::fromWCharArray(L"OK"),this);
     TransformSlags->setGeometry(85, 70, 60, 25);
     TransformSlags->setFont(QFont("Times", 12));
	 TransformSlags->setStyleSheet(
            "QPushButton{"
			"background-color: rgb(220,220,220);"
			"border-style: outset;"
			"border-width: 2px;"
			"border-radius: 10px;"
			"border-color: beige;}");
	 TransformSlags->setEnabled(false);

	 TextBrowser = new QTextBrowser (this);
	 TextBrowser->setGeometry(190, 10, 400, 140);

	 connect(FileFound, SIGNAL(clicked()), this, SLOT(File_Founds()));
	 connect(TransformSlags, SIGNAL(clicked()), this, SLOT(ReadOld_Function()));
	 connect(OpenDirect, SIGNAL(clicked()), this, SLOT(Open_Direct()));
}

ConverterSlags::~ConverterSlags() {}

void ConverterSlags::Open_Direct()
{
	int z=strlen(DirToSave);
	TWIC(DirToSave,z);
}

void ConverterSlags::ChangeSpiner(int NZSHL_C) 
{
	OldRead->setValue(NZSHL_C);
}

void ConverterSlags::AddTextDisplay(char* text) 
{
	TextBrowser->append(text);
}

 void ConverterSlags::File_Founds()
{
	QFileDialog *FileFinder = new QFileDialog();
	QString fileName = FileFinder->getOpenFileName();
	if (fileName!=NULL){
	QByteArray ba = fileName.toLocal8Bit();
	File_Name_C = ba.data();
	SHLAKIOLD(Set_Debug);	
	this->FileFound->setEnabled(false);
	this->TransformSlags->setEnabled(true);
	this->OldRead->setEnabled(true);}
}

void ConverterSlags::ReadOld_Function()
{
	nLayersNew = this->OldRead->value();
	LISTCONV(Set_Debug);
	DATAREWRITE(Set_Debug);
}


 extern "C" void CALL ReadAdress(char* AdressOld, int& SizeAdressOld,int& Dealindex)
   {
	   size_t Size = strlen(Converter->File_Name_C);
	   strncpy(AdressOld,Converter->File_Name_C, Size);
	   SizeAdressOld=Size;
	   Dealindex=0;
   }

extern "C" void CALL NameList()
	{
		memset(OldName, 0, sizeof(OldName));
		memset(Converter->DirToSave, 0, sizeof(Converter->DirToSave));
		strcpy (AdressCh,Converter->File_Name_C);
		for (int i=0; i<=strlen(AdressCh); i++)
		{
			if (AdressCh[i]=='/'){
			LastPoint1=i;
			}
		}
		LastPoint2=0;
		for (int i=0; i<=strlen(AdressCh); i++)
		{
			if (AdressCh[i]=='.'){
			LastPoint2=i;
			}
		}
        if (LastPoint2==0||LastPoint2<LastPoint1) {LastPoint2=strlen(AdressCh);}
		for (int i=LastPoint1+1; i<LastPoint2; i++)
		{
			OldName[i-LastPoint1-1]=AdressCh[i];
		}
		for (int i=0; i<=LastPoint1; i++)
		{
			Converter->DirToSave[i]=AdressCh[i];
		}
	}

extern "C" void CALL Param(int& NZSHL)
	{
	   if (Converter) 
	   {
		   Converter->ChangeSpiner(NZSHL);
		   if (Converter->SetLanguage==1){Converter->TextBrowser->append(QString::fromWCharArray(L"Source file address: "));}
		   if (Converter->SetLanguage==0){Converter->TextBrowser->append(QString::fromWCharArray(L"Адрес исходного файла: "));}
		   Converter->AddTextDisplay(const_cast<char*>(Converter->File_Name_C));
		   if (Converter->SetLanguage==1){Converter->TextBrowser->append(QString::fromWCharArray(L"Number of layers in source file: "));}
		   if (Converter->SetLanguage==0){Converter->TextBrowser->append(QString::fromWCharArray(L"Число слоев исходного файла: "));}
		   wchar_t m_reportFileName[256];
		   swprintf_s(m_reportFileName, L"%d", NZSHL);
		   Converter->TextBrowser->insertPlainText(QString::fromWCharArray(m_reportFileName));
	   }
   }

extern "C" void CALL ReadOld_C(int& nLayers,char* NewAdress2)
{
	memset(NewAdress, 0, sizeof(NewAdress));
	nLayers=nLayersNew;
	itoa(nLayersNew,nLayersNewCh,10);
	strcat(OldName,"_");
	strcat(OldName,nLayersNewCh);
	strcat(OldName,"Layers.shl");
	strcpy(NewAdress,Converter->DirToSave);
	strcat(NewAdress,OldName);
	strncpy(NewAdress2,NewAdress, strlen(NewAdress));
}

extern "C" void CALL Run_C()
   {
	   if (Converter->SetLanguage==1){Converter->TextBrowser->append(QString::fromWCharArray(L"Done "));}
	   if (Converter->SetLanguage==0){Converter->TextBrowser->append(QString::fromWCharArray(L"Преобразование файла шлаков выполнено "));}

	   if (Converter->SetLanguage==1){Converter->TextBrowser->append(QString::fromWCharArray(L"Address of the converted file: "));}
	   if (Converter->SetLanguage==0){Converter->TextBrowser->append(QString::fromWCharArray(L"Адрес преобразованного файла: "));}
	   
	   Converter->AddTextDisplay(const_cast<char*>(NewAdress));

	   if (Converter->SetLanguage==1){Converter->TextBrowser->append(QString::fromWCharArray(L"Number of layers in converted file: "));}
	   if (Converter->SetLanguage==0){Converter->TextBrowser->append(QString::fromWCharArray(L"Число слоев преобразованного файла: "));}
	   
	   wchar_t m_reportFileName[256];
	   swprintf_s(m_reportFileName, L"%d", nLayersNew);
	   Converter->TextBrowser->insertPlainText(QString::fromWCharArray(m_reportFileName));
	   Converter->TransformSlags->setEnabled(false);
	   Converter->OldRead->setEnabled(false);
	   Converter->OpenDirect->setEnabled(true);
   }
