#include <QColorDialog>
#include "extfunctions.h"
#include "cf_colorchanger.h"
#include "fa_box.h"
#include "filebrowser.h"

// �����������
CF_ColorChanger::CF_ColorChanger(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// ������ ���� ��������� � �������������� �������
	CF_ColorChanger::setFixedSize(CF_ColorChanger::size());
	CF_ColorChanger::setWindowModality(Qt::ApplicationModal);

	// ��������� ����� ������ � ��������������� ����
	ui.textBrowser_Max->setText(QString::fromWCharArray(L"�������� �������� � ") + State1FileName() +
								QString::fromWCharArray(L" ������, ��� � ") + State2FileName());
	ui.textBrowser_Min->setText(QString::fromWCharArray(L"�������� �������� � ") + State1FileName() +
								QString::fromWCharArray(L" ������, ��� � ") + State2FileName());
	// �������� ��������� ������ �� ������� �������� �����������
	newLimitColors[Parameters::Kq].maxColor = cFA_Box::GetMaxColor(Parameters::Kq);
	newLimitColors[Parameters::Kq].minColor = cFA_Box::GetMinColor(Parameters::Kq);
	newLimitColors[Parameters::Burn].maxColor = cFA_Box::GetMaxColor(Parameters::Burn);
	newLimitColors[Parameters::Burn].minColor = cFA_Box::GetMinColor(Parameters::Burn);
	newLimitColors[Parameters::Kv].maxColor = cFA_Box::GetMaxColor(Parameters::Kv);
	newLimitColors[Parameters::Kv].minColor = cFA_Box::GetMinColor(Parameters::Kv);

	// ������ ���������������� �������� (����� � ������ ������)
	double demoValue1 = 1.433;
	double demoValue2 = 1.254;

	// ������ ��������� ������� ����������������� ���� - ����, �������������� � ������������ ������ �������� ���������� ���������
	ui.ColorField_Max->setBackgroundRole(QPalette::Base);
	ui.ColorField_Max->setAutoFillBackground(true);
	connect(ui.ColorField_Max, &demoAreaCF::getColor, [this] () { return newLimitColors.at(cFA_Box::ActiveMode()).maxColor;});
	// ���������� ������ �������� ���������������� ���������
	connect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Max, SLOT(getStates(const QPair<double, double>&)));
	// �������� ���������������� �������� 
	emit sendDemoStates(qMakePair( demoValue1, demoValue2));
	// ��������� ������ �������� ���������������� ���������
	disconnect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Max, SLOT(getStates(const QPair<double, double>&)));

	// ������ ��������� ������� ����������������� ���� - ����, �������������� � ������������ ������ �������� ���������� ���������
	ui.ColorField_Min->setBackgroundRole(QPalette::Base);
	ui.ColorField_Min->setAutoFillBackground(true);
	connect(ui.ColorField_Min, &demoAreaCF::getColor, [this] () { return newLimitColors.at(cFA_Box::ActiveMode()).minColor;});
	// ���������� ������ �������� ���������������� ���������
	connect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Min, SLOT(getStates(const QPair<double, double>&)));
	// �������� ���������������� �������� 
	emit sendDemoStates(qMakePair( demoValue2, demoValue1));
	// ��������� ������ �������� ���������������� ���������
	disconnect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Min, SLOT(getStates(const QPair<double, double>&)));

	// � ����������� �� �������� ���������� ��������� �������� ������ ������ � �������������� ��������������� ����
	switch (cFA_Box::ActiveMode()) {
		case (Parameters::Kq) : 
			ui.KqButton->click();
			break;
		case (Parameters::Burn) : 
			ui.BurnButton->click();
			break;
		case (Parameters::Kv) : 
			ui.KvButton->click();
			break;
	}

	// ��������� ������� ��� ������������� � high dpi
	QFont font(ui.groupBox->font());
	font.setPixelSize(13);
	ui.groupBox->setFont(font);
}

// ����������
CF_ColorChanger::~CF_ColorChanger() {}


// ������ ����� Kq
void CF_ColorChanger::on_KqButton_clicked()
{
	// ������ Kq �������� ������� - ��������� ���
	ui.KqButton->setChecked(true);
	ui.BurnButton->setChecked(false);
	ui.KvButton->setChecked(false);
	// �������� ����� - Kq
	activeMode = Parameters::Kq;
	// ��������� ��������� ������ �����������
	emit changeActiveMode(activeMode);
	// ��������� ������ ���������������� �����
	changeColorField();
}

// ������ ����� Burn
void CF_ColorChanger::on_BurnButton_clicked()
{
	// ������ Burn �������� ������� - ��������� ���
	ui.KqButton->setChecked(false);
	ui.BurnButton->setChecked(true);
	ui.KvButton->setChecked(false);
	// �������� ����� - Burn
	activeMode = Parameters::Burn;
	// ��������� ��������� ������ �����������
	emit changeActiveMode(activeMode);
	// ��������� ������ ���������������� �����
	changeColorField();
}

// ������ ����� Kv
void CF_ColorChanger::on_KvButton_clicked()
{
	// ������ Kv �������� ������� - ��������� ���
	ui.KqButton->setChecked(false);
	ui.BurnButton->setChecked(false);
	ui.KvButton->setChecked(true);
	// �������� ����� - Kv
	activeMode = Parameters::Kv;
	// ��������� ��������� ������ �����������
	emit changeActiveMode(activeMode);
	// ��������� ������ ���������������� �����
	changeColorField();
}

// ����������� ���������������� �����
void CF_ColorChanger::changeColorField()
{
	ui.ColorField_Max->repaint();
	ui.ColorField_Min->repaint();
}

// ��������� �������� ����� ������ ���
void CF_ColorChanger::on_ChangeMaxColor_clicked()
{
	// �������� ColorDialog
	QColor newMaxColor = QColorDialog::getColor( newLimitColors.at(activeMode).maxColor,
												 this,
											 	 QString::fromWCharArray(L"�������� ����"));
	// ���� ���� ������
	if (newMaxColor.isValid())
	{
		// ������ ����� ���� ���������
		newLimitColors[activeMode].maxColor = newMaxColor;
		// �������������� ���������������� �������
		changeColorField();
	}
}

// ��������� �������� ����� ������ ���
void CF_ColorChanger::on_ChangeMinColor_clicked()
{
	// �������� ColorDialog
	QColor newMinColor = QColorDialog::getColor( newLimitColors.at(activeMode).minColor,
												 this,
												 QString::fromWCharArray(L"�������� ����"));
	// ���� ���� ������
	if (newMinColor.isValid())
	{
		// ������ ����� ���� ���������
		newLimitColors[activeMode].minColor = newMinColor;
		// �������������� ���������������� �������
		changeColorField();
	}
}


// ���������� ����������� � �������� ���� ���������
void CF_ColorChanger::closeEvent(QCloseEvent *)
{
	emit closing();
}


void CF_ColorChanger::on_buttonBox_clicked(QAbstractButton * button)
{
	// OK
	if (button == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		// ��������� ��������� � ��������� ����
		applyColors();
		CF_ColorChanger::close();
	// CANCEL
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Cancel)) {
		// ��������� ����
		CF_ColorChanger::close();
	// APPLY
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) {
		// ��������� ���������
		applyColors();
	// DISCARD
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Discard)) {
		// �������� ��������� ������ �� ������� �������� �����������
		newLimitColors[Parameters::Kq].maxColor = cFA_Box::GetMaxColor(Parameters::Kq);
		newLimitColors[Parameters::Kq].minColor = cFA_Box::GetMinColor(Parameters::Kq);
		newLimitColors[Parameters::Burn].maxColor = cFA_Box::GetMaxColor(Parameters::Burn);
		newLimitColors[Parameters::Burn].minColor = cFA_Box::GetMinColor(Parameters::Burn);
		newLimitColors[Parameters::Kv].maxColor = cFA_Box::GetMaxColor(Parameters::Kv);
		newLimitColors[Parameters::Kv].minColor = cFA_Box::GetMinColor(Parameters::Kv);
		// �������������� ����
		if (ui.KqButton->isChecked()) {
			ui.KqButton->click();
		} else if (ui.BurnButton->isChecked()) {
			ui.BurnButton->click();
		} else if (ui.KvButton->isChecked()) {
			ui.KvButton->click();
		} else {
			ui.KqButton->click();
		}
	// RESTORE DEFAULTS
	} else if (button == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		// �������� ��������� ������ �� ����������� �������� �����������
		newLimitColors[Parameters::Kq].maxColor = cFA_Box::GetDefaultMaxColor(Parameters::Kq);
		newLimitColors[Parameters::Kq].minColor = cFA_Box::GetDefaultMinColor(Parameters::Kq);
		newLimitColors[Parameters::Burn].maxColor = cFA_Box::GetDefaultMaxColor(Parameters::Burn);
		newLimitColors[Parameters::Burn].minColor = cFA_Box::GetDefaultMinColor(Parameters::Burn);
		newLimitColors[Parameters::Kv].maxColor = cFA_Box::GetDefaultMaxColor(Parameters::Kv);
		newLimitColors[Parameters::Kv].minColor = cFA_Box::GetDefaultMinColor(Parameters::Kv);
		// �������������� ����
		if (ui.KqButton->isChecked()) {
			ui.KqButton->click();
		} else if (ui.BurnButton->isChecked()) {
			ui.BurnButton->click();
		} else if (ui.KvButton->isChecked()) {
			ui.KvButton->click();
		} else {
			ui.KqButton->click();
		}
	}
}

// ��������� �����
void CF_ColorChanger::applyColors()
{
	// ������� ���������� ������ �����������
	cFA_Box::SetMaxColor(Parameters::Kq, newLimitColors.at(Parameters::Kq).maxColor);
	cFA_Box::SetMinColor(Parameters::Kq, newLimitColors.at(Parameters::Kq).minColor);
	cFA_Box::SetMaxColor(Parameters::Burn, newLimitColors.at(Parameters::Burn).maxColor);
	cFA_Box::SetMinColor(Parameters::Burn, newLimitColors.at(Parameters::Burn).minColor);
	cFA_Box::SetMaxColor(Parameters::Kv, newLimitColors.at(Parameters::Kv).maxColor);
	cFA_Box::SetMinColor(Parameters::Kv, newLimitColors.at(Parameters::Kv).minColor);
	// ����������� �����������
	emit RecolorComparisonField();
}