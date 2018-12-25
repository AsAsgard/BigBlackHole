#include <QDesktopWidget>
#include <QFileDialog>
#include <QString>
#include <QFont>
#include "filebrowser.h"
#include "comparisonfield.h"

using namespace std;

// ���������� ������������ ���� - ��� ��������� � �������
static QString _defaultPath = "./";
QString defaultPath() {return _defaultPath;}
void setDefaultPath(const QString &NewDefaultPath) { _defaultPath = NewDefaultPath;}
void setDefaultPath(QString &&NewDefaultPath) { if (&_defaultPath != &NewDefaultPath) _defaultPath = NewDefaultPath;}

// ���������� ���� ������ - �� ��������� � �������
static QString _State1FileName = "", _State2FileName = "";
QString State1FileName() { return _State1FileName;}
void setState1FileName(const QString &NewState1FileName) { _State1FileName = NewState1FileName;}
void setState1FileName(QString &&NewState1FileName) { if (&_State1FileName != &NewState1FileName) _State1FileName = NewState1FileName;}
QString State2FileName() { return _State2FileName;}
void setState2FileName(const QString &NewState2FileName) { _State2FileName = NewState2FileName;}
void setState2FileName(QString &&NewState2FileName) { if (&_State2FileName != &NewState2FileName) _State2FileName = NewState2FileName;}


FileBrowser::FileBrowser(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// ������ ���� ��������� � �������������� �������
	FileBrowser::setFixedSize(FileBrowser::size());
	FileBrowser::setWindowModality(Qt::ApplicationModal);

	// �������� ������� ������-����, ������� �� ��� Label, ������ ��� ��������� � ������������ � ��������� �����
	ui.statusBar->resize(this->width(), 20);
	statusLabel.reset(new QLabel(ui.statusBar));
	statusLabel->setGeometry(4, 0, this->width(), ui.statusBar->height());
	statusLabel->setAlignment(Qt::AlignVCenter);
	statusLabel->setText(QString::fromStdWString(L"������� �������� ������ � �����������, ������� ���������� ��������."));
	
	// ��������� ������� ��� ������������� � high dpi
	QFont font(ui.File1Label->font());
	font.setPixelSize(15);
	ui.File1Label->setFont(font);
	ui.File2Label->setFont(font);
	font = ui.CommonLabel->font();
	font.setPixelSize(16);
	ui.CommonLabel->setFont(font);
	font = ui.File1lineEdit->font();
	font.setPixelSize(16);
	ui.File1lineEdit->setFont(font);
	ui.File2lineEdit->setFont(font);
}

// ����������
FileBrowser::~FileBrowser() 
{
	// ������������� - ������������ ������ (��-�� ����, ��� ���� �������� � ������� ��� ���������� ����������)
	statusLabel.reset(nullptr);
}

// ������������ ������� ���������
void FileBrowser::on_File1Button_clicked() 
{
	// ��������� ������-���
	statusLabel->setText(QString::fromStdWString(L"�������� ���� � ������ ����������."));
	// �������� ������ �������� �����
	QString FileName = QFileDialog::getOpenFileName(
		this,
		QString::fromStdWString(L"�������� ���� �� ������ ����������."),
		defaultPath(),
		"All files (*.*);;Dat file (*.dat);;Text file (*.txt);;Exp files (*.exp)"
		);
	// ���� ���� ������
	if (!FileName.isEmpty()) 
	{
		// ������ ����� ����������� ����
		setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
		// ������� ��� ����� � LineEdit
		ui.File1lineEdit->setText(FileName);
		// ������ ������ � TextBrowser-�
		ui.textBrowser->append(QString::fromStdWString(L"� ����� \"������ ���������\" ������� ���� "+ FileName.toStdWString() + L"."));
	}
	// ��������� ������-���
	CheckingFileNames();
}

// ������������ ������� ���������
void FileBrowser::on_File2Button_clicked() 
{
	// ��������� ������-���
	statusLabel->setText(QString::fromStdWString(L"�������� ���� �� ������ ����������."));
	// �������� ������ �������� �����
	QString FileName = QFileDialog::getOpenFileName(
		this,
		QString::fromStdWString(L"�������� ���� �� ������ ����������."),
		defaultPath(),
		"All files (*.*);;Dat file (*.dat);;Text file (*.txt);;Exp files (*.exp)"
		);
	// ���� ���� ������
	if (!FileName.isEmpty()) 
	{
		// ������ ����� ����������� ����
		setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
		// ������� ��� ����� � LineEdit
		ui.File2lineEdit->setText(FileName);
		// ������ ������ � TextBrowser-�
		ui.textBrowser->append(QString::fromStdWString(L"� ����� \"������ ���������\" ������� ���� "+ FileName.toStdWString() + L"."));
	}
	// ��������� ������-���
	CheckingFileNames();
}

// ��������� ��������������� ������ � ������ LineEdit - ������������ ������ ����� �����
void FileBrowser::on_File1lineEdit_editingFinished()
{
	// ���� ��� ����� ���� ��������������
	if (ui.File1lineEdit->isModified())
	{
		// ��������� ��� �����
		ui.File1lineEdit->setModified(false);
		QString FileName = ui.File1lineEdit->text();
		// ���� ��� ����� �� ������
		if (!FileName.isEmpty()) 
		{
			// ������ ����� ����������� ����
			setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
			// ������ ������ � TextBrowser-�
			ui.textBrowser->append(QString::fromStdWString(L"� ����� \"������ ���������\" ������� ���� "+ FileName.toStdWString() + L"."));
		}
		// ��������� ������-���
		CheckingFileNames();
	}
}

// ��� �������������� ���� ������ - ���������� �������������� ���������� ������-����
void FileBrowser::on_File1lineEdit_textEdited(QString) { CheckingFileNames();}

// ��������� ��������������� ������ �� ������ LineEdit - ������������ ������ ����� �����
void FileBrowser::on_File2lineEdit_editingFinished()
{
	// ���� ��� ����� ���� ��������������
	if (ui.File2lineEdit->isModified()) 
	{
		// ��������� ��� �����
		ui.File2lineEdit->setModified(false);
		QString FileName = ui.File2lineEdit->text();
		// ���� ��� ����� �� ������
		if (!FileName.isEmpty()) 
		{
			// ������ ����� ����������� ����
			setDefaultPath(FileName.left(FileName.lastIndexOf('/')+1));
			// ������ ������ � TextBrowser-�
			ui.textBrowser->append(QString::fromStdWString(L"� ����� \"������ ���������\" ������� ���� "+ FileName.toStdWString() + L"."));
		}
		// ��������� ������-���
		CheckingFileNames();
	}
}

// ��� �������������� ���� ������ - ���������� �������������� ���������� ������-����
void FileBrowser::on_File2lineEdit_textEdited(QString) { CheckingFileNames();}


// ������ ������ "��������"
void FileBrowser::on_Compare_clicked()
{
	// ��������� �������� - ��� ��� ���������� ������-����, ������ � ���������� ������ ����� � � TextBrowser
	if (ui.File1lineEdit->text().isEmpty() && ui.File2lineEdit->text().isEmpty())
	{
		ui.textBrowser->append(QString::fromStdWString(L"������� �������� ������ � ����������� � ����� \"������ ���������\" � \"������ ���������\"."));
		statusLabel->setText(QString::fromStdWString(L"������� �������� ������ � �����������, ������� ���������� ��������."));
		return;
	}
	if (ui.File1lineEdit->text().isEmpty())
	{
		ui.textBrowser->append(QString::fromStdWString(L"������� �������� ����� � ������ ���������� � ����� \"������ ���������\"."));
		statusLabel->setText(QString::fromStdWString(L"������� �������� ����� � ������ ���������� � ����� \"������ ���������\"."));
		return;
	}
	if (ui.File2lineEdit->text().isEmpty())
	{
		ui.textBrowser->append(QString::fromStdWString(L"������� �������� ����� �� ������ ���������� � ����� \"������ ���������\"."));
		statusLabel->setText(QString::fromStdWString(L"������� �������� ����� �� ������ ���������� � ����� \"������ ���������\"."));
		return;
	}
	if (ui.File1lineEdit->text() == ui.File2lineEdit->text())
	{
		ui.textBrowser->append(QString::fromStdWString(L"�������� ��� ��������� ��������� �����."));
		statusLabel->setText(QString::fromStdWString(L"���������� ������� ��������� ����� ��� ���������."));
		return;
	}
	// ������� ���������, � ������� ����� ���������, � ����� ���������� � ������� ������
	cDataState State1, State2;
	QString FileName = "", FileName1 = "", FileName2 = "";
	try {
		// ���������� ������� ����� �������� �����
		cDataState::rememberNumLayers();
		// ������ ����� ����� = 1
		cDataState::reduceNumLayers();
		// ��������� ������ ��� �����
		FileName = FileName1 = ui.File1lineEdit->text();
		// �������� ������� ������ ���������
		State1.ReadDataFromFile(FileName.toStdWString());
		// ��������� ������ ��� �����
		FileName = FileName2 = ui.File2lineEdit->text();
		// �������� ������� ������ ���������
		State2.ReadDataFromFile(FileName.toStdWString());
	} catch(const ios_base::failure &ex) {
		// ������� ��������� �� ������
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"������!"));
		ui.textBrowser->append(QString(ex.what()));
		ui.textBrowser->append(QString::fromWCharArray(L"�������� � ������ ")+FileName+QString::fromWCharArray(L". ��������� ������������ ���������� ����."));
		statusLabel->setText(QString::fromWCharArray(L"��������� ������������ ���� ����� � ���������."));
		// ���������� ���������� ����� ����� � ������� �� �������
		cDataState::setRememberedNumLayers();
		return;
	} catch(const invalid_argument &ex) {
		// ������� ��������� �� ������
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"������!"));
		ui.textBrowser->append(QString(ex.what()));
		ui.textBrowser->append(QString::fromWCharArray(L"�������� � ������ ")+FileName+QString::fromWCharArray(L". ��������� ������������ ������� �����."));
		statusLabel->setText(QString::fromWCharArray(L"��������� ������������ ������� ����� � ���������."));
		// ���������� ���������� ����� ����� � ������� �� �������
		cDataState::setRememberedNumLayers();
		return;
	} catch(const exception &ex) {
		// ������� ��������� �� ������
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"�������������� ������!"));
		ui.textBrowser->append(QString(ex.what()));
		ui.textBrowser->append(QString::fromWCharArray(L"�������� � ������ ")+FileName);
		ui.textBrowser->append(QString::fromWCharArray(L"���������� �������� ���� � ������������� ���������. ���� �������� ���������, ��������� � �������������."));
		statusLabel->setText(QString::fromWCharArray(L"���������� �������� ���� � ��������� � ������������� ���������."));
		// ���������� ���������� ����� ����� � ������� �� �������
		cDataState::setRememberedNumLayers();
		return;
	} catch(...) {
		// ������� ��������� �� ������
		ui.textBrowser->append("");
		ui.textBrowser->append(QString::fromWCharArray(L"�������������� ������!"));
		ui.textBrowser->append(QString::fromWCharArray(L"�������� � ������ ")+FileName);
		ui.textBrowser->append(QString::fromWCharArray(L"���������� �������� ���� � ������������� ���������. ���� �������� ���������, ��������� � �������������."));
		statusLabel->setText(QString::fromWCharArray(L"���������� �������� ���� � ��������� � ������������� ���������."));
		// ���������� ���������� ����� ����� � ������� �� �������
		cDataState::setRememberedNumLayers();
		return;
	}
	// ���� ��� ������ - ���������� ����� ������ �� ���������� �����������
	setState1FileName(FileName1.mid(FileName.lastIndexOf('/')+1));
	setState2FileName(FileName2.mid(FileName.lastIndexOf('/')+1));
	/*
		���� ����������� ������� - ������ ���������
		���� ����������� �� ������� - ������� ��, ������ �������� - ������� ��� �������� - � ������� �� �����
		��� �������� �����������, �������� � � ����������� ��������� ������
	*/
	if (emit isComparisonFieldActive())
	{
		emit ChangeStatesFB(State1, State2);
	} else {
		QDesktopWidget Desktop;
		ComparisonField *w = new ComparisonField(0,State1,State2,Desktop.screenGeometry(this));
		w->setAttribute(Qt::WA_DeleteOnClose);	
		w->show();
	}
	// ��������� ������������
	FileBrowser::close();
}

// ���������� ������-����
void FileBrowser::CheckingFileNames(void) 
{
	// ���� ��� LineEdit-� �����
	if (ui.File1lineEdit->text().isEmpty() && ui.File2lineEdit->text().isEmpty())
	{
		statusLabel->setText(QString::fromStdWString(L"������� �������� ������ � �����������, ������� ���������� ��������."));
		return;
	}
	// ���� ���� ������ ������ LineEdit
	if (ui.File1lineEdit->text().isEmpty())
	{
		statusLabel->setText(QString::fromStdWString(L"������� �������� ����� � ������ ���������� � ����� \"������ ���������\"."));
		return;
	}
	// ���� ���� ������ ������ LineEdit
	if (ui.File2lineEdit->text().isEmpty())
	{
		statusLabel->setText(QString::fromStdWString(L"������� �������� ����� �� ������ ���������� � ����� \"������ ���������\"."));
		return;
	}
	// ���� � LineEdit-�� ���������� �������� ������
	if (ui.File1lineEdit->text() == ui.File2lineEdit->text())
	{
		statusLabel->setText(QString::fromStdWString(L"���������� ������� ��������� ����� ��� ���������."));
		return;
	}
	// ������ �� ������������������
	statusLabel->setText(QString::fromStdWString(L"������� ������ \"��������\" ��� ��������� ���������."));
}

// ���������� ����������� � �������� ������������ ������
void FileBrowser::closeEvent(QCloseEvent *)
{
	emit closing();
}