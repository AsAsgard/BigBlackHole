#include "converterslags.h"

int nLayersNew;
int LastPoint1;
int LastPoint2;
char AdressCh[127];
char OldName[127];
char NewAdress[127];
char nLayersNewCh[2];

ConverterSlags::ConverterSlags(int LanguageKey,int SetDebug, QWidget * parent)
	: QWidget(parent)
{
	 ui.setupUi(this);
	 this->setFixedSize(this->size());

	 SetLanguage=LanguageKey;
	 Set_Debug=SetDebug;
}

ConverterSlags::~ConverterSlags() {}

void ConverterSlags::on_OpenDirect_clicked()
{
	int z=strlen(DirToSave);
	TWIC(DirToSave,z);
}

void ConverterSlags::ChangeSpiner(int NZSHL_C) 
{
	ui.OldRead->setValue(NZSHL_C);
}

void ConverterSlags::AddTextDisplay(const QString& text) 
{
	ui.TextBrowser->append(text);
}

void ConverterSlags::addPlainText(const QString& text) 
{
	ui.TextBrowser->insertPlainText(text);
}

 void ConverterSlags::on_FileFound_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
													tr("Choose slags file."),
													"./");
	if (!fileName.isNull())
	{
		QByteArray ba = fileName.toLocal8Bit();
		File_Name_C = ba.data();
		SHLAKIOLD(Set_Debug);	
		ui.FileFound->setEnabled(false);
		ui.TransformSlags->setEnabled(true);
		ui.OldRead->setEnabled(true);
	}
}

void ConverterSlags::on_TransformSlags_clicked()
{
	nLayersNew = ui.OldRead->value();
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
		   if (Converter->SetLanguage==1){Converter->AddTextDisplay(QString::fromWCharArray(L"Source file address: "));}
		   if (Converter->SetLanguage==0){Converter->AddTextDisplay(QString::fromWCharArray(L"Адрес исходного файла: "));}
		   Converter->AddTextDisplay(QString(Converter->File_Name_C));
		   if (Converter->SetLanguage==1){Converter->AddTextDisplay(QString::fromWCharArray(L"Number of layers in source file: "));}
		   if (Converter->SetLanguage==0){Converter->AddTextDisplay(QString::fromWCharArray(L"Число слоев исходного файла: "));}
		   wchar_t m_reportFileName[256];
		   swprintf_s(m_reportFileName, L"%d", NZSHL);
		   Converter->addPlainText(QString::fromWCharArray(m_reportFileName));
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
	   if (Converter->SetLanguage==1){Converter->AddTextDisplay(QString::fromWCharArray(L"Done "));}
	   if (Converter->SetLanguage==0){Converter->AddTextDisplay(QString::fromWCharArray(L"Преобразование файла шлаков выполнено "));}

	   if (Converter->SetLanguage==1){Converter->AddTextDisplay(QString::fromWCharArray(L"Address of the converted file: "));}
	   if (Converter->SetLanguage==0){Converter->AddTextDisplay(QString::fromWCharArray(L"Адрес преобразованного файла: "));}
	   
	   Converter->AddTextDisplay(NewAdress);

	   if (Converter->SetLanguage==1){Converter->AddTextDisplay(QString::fromWCharArray(L"Number of layers in converted file: "));}
	   if (Converter->SetLanguage==0){Converter->AddTextDisplay(QString::fromWCharArray(L"Число слоев преобразованного файла: "));}
	   
	   wchar_t m_reportFileName[256];
	   swprintf_s(m_reportFileName, L"%d", nLayersNew);
	   Converter->addPlainText(QString::fromWCharArray(m_reportFileName));
	   Converter->setEnabledTransformSlags(false);
	   Converter->setEnabledSpinner(false);
	   Converter->setEnabledOpenDirect(true);
   }