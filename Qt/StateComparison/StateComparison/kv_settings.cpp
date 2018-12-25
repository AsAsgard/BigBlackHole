#include <QColorDialog>
#include "kv_settings.h"
#include "filebrowser.h"

// �����������
// ������ ��������� ��������� � �������������� ��������� ��� X
KV_Settings::KV_Settings(const QPen& State1Pen, const QPen& State2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting, QWidget *parent)
	: QWidget(parent), newState1Pen(State1Pen), newState2Pen(State2Pen), newAutoAxisSetting(AutoAxisSetting)
{
	ui.setupUi(this);
	// ������ ���� ��������� � �������������� �������
	KV_Settings::setFixedSize(KV_Settings::size());
	KV_Settings::setWindowModality(Qt::ApplicationModal);
	
	// ������ ��������� ���������������� ����� - ����, �������������� � ������������ ������ �������� ���������� ���������
	ui.State1demoArea->setBackgroundRole(QPalette::Base);
	ui.State1demoArea->setAutoFillBackground(true);
	connect(ui.State1demoArea, &demoAreaKv::getPen, [this] () { return newState1Pen;});
	ui.State2demoArea->setBackgroundRole(QPalette::Base);
	ui.State2demoArea->setAutoFillBackground(true);
	connect(ui.State2demoArea, &demoAreaKv::getPen, [this] () { return newState2Pen;});

	// ��������� ����� ������ � ��������������� ����
	ui.State1Filename->setText(State1FileName() + ":");
	ui.State2Filename->setText(State2FileName() + ":");
	
	// ������ 
	showParameters(State1Pen, State2Pen, AutoAxisSetting);

	// ��������� ������� ��� ������������� � high dpi
	QFont font(ui.buttonBox->font());
	font.setPixelSize(11);
	ui.buttonBox->setFont(font);
}

// ����������
KV_Settings::~KV_Settings() {}

// ��������� ������ � �������� ����
void KV_Settings::closeEvent(QCloseEvent *)
{
	emit closing();
}


void KV_Settings::on_buttonBox_clicked(QAbstractButton * button)
{
	// OK
	if (button == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		// ��������� ��������� � ��������� ����
		applySettings();
		KV_Settings::close();
	// CANCEL
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Cancel)) {
		// ��������� ����
		KV_Settings::close();
	// APPLY
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) {
		// ��������� ���������
		applySettings();
	// DISCARD
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Discard)) {
		// ��������� ������� ���������� �����������
		auto PairPens = emit getCurrentPens();
		// ��������� ����� ����������
		showParameters(PairPens.first, PairPens.second, emit getCurrentAutoAxisSetting());
	// RESTORE DEFAULTS
	} else if (button == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		// ��������� ����������� ���������� �����������
		auto PairPens = emit getDefaultPens();
		// ��������� ����� ����������
		showParameters(PairPens.first, PairPens.second, AutoAxis::NoAuto);
	}
}


//������������� ��������� ���������� �� ������� 
void KV_Settings::showParameters(const QPen& State1Pen, const QPen& State2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting)
{
	// �������� ������������� ��� � RadioButton-��
	switch (AutoAxisSetting) {
	case AutoAxis::NoAuto:			ui.NoAuto->setChecked(true);
									break;
	case AutoAxis::HighBorderAuto:  ui.HighBorderAuto->setChecked(true);
									break;
	case AutoAxis::AllAuto:			ui.AllAuto->setChecked(true);
									break;
	}

	// ����� ��������� ���������������� ����� � �� �����������
	newState1Pen.setColor(State1Pen.color());
	ui.State1demoArea->repaint();
	newState2Pen.setColor(State2Pen.color());
	ui.State2demoArea->repaint();

	// ��������� ����� ����� � ComboBox-��
	ui.State1lineStyleBox->setCurrentIndex(State1Pen.style());
	ui.State2lineStyleBox->setCurrentIndex(State2Pen.style());
	
	// ��������� �������� ������ ����� � SpinBox-��
	ui.State1WidthBox->setValue(State1Pen.widthF());
	ui.State2WidthBox->setValue(State2Pen.widthF());
}

// ��������� ��������� � �����������
void KV_Settings::applySettings()
{
	emit sendNewSettings(newState1Pen, newState2Pen, newAutoAxisSetting);
}

// ����� ����� ����� ������� ���������
void KV_Settings::on_State1lineStyleBox_currentIndexChanged(int index)
{
	switch(index) {
	case Qt::NoPen : 
		newState1Pen.setStyle(Qt::NoPen);
		break;
	case Qt::SolidLine : 
		newState1Pen.setStyle(Qt::SolidLine);
		break;
	case Qt::DashLine : 
		newState1Pen.setStyle(Qt::DashLine);
		break;
	case Qt::DotLine : 
		newState1Pen.setStyle(Qt::DotLine);
		break;
	case Qt::DashDotLine : 
		newState1Pen.setStyle(Qt::DashDotLine);
		break;
	case Qt::DashDotDotLine : 
		newState1Pen.setStyle(Qt::DashDotDotLine);
		break;
	}
	// �������������� ������ ���������������� �������
	ui.State1demoArea->repaint();
}


// ����� ����� ����� ������� ���������
void KV_Settings::on_State2lineStyleBox_currentIndexChanged(int index)
{
	switch(index) {
	case Qt::NoPen : 
		newState2Pen.setStyle(Qt::NoPen);
		break;
	case Qt::SolidLine : 
		newState2Pen.setStyle(Qt::SolidLine);
		break;
	case Qt::DashLine : 
		newState2Pen.setStyle(Qt::DashLine);
		break;
	case Qt::DotLine : 
		newState2Pen.setStyle(Qt::DotLine);
		break;
	case Qt::DashDotLine : 
		newState2Pen.setStyle(Qt::DashDotLine);
		break;
	case Qt::DashDotDotLine : 
		newState2Pen.setStyle(Qt::DashDotDotLine);
		break;
	}
	// �������������� ������ ���������������� �������
	ui.State2demoArea->repaint();
}


// ��������� �������� ������ ������ �����
void KV_Settings::on_State1WidthBox_valueChanged(double value)
{
	// ������ ����� ������ ������ �����
	newState1Pen.setWidthF(value);
	// �������������� ������ ���������������� �������
	ui.State1demoArea->repaint();
}

// ��������� �������� ������ ������ �����
void KV_Settings::on_State2WidthBox_valueChanged(double value)
{
	// ������ ����� ������ ������ �����
	newState2Pen.setWidthF(value);
	// �������������� ������ ���������������� �������
	ui.State2demoArea->repaint();
}


// ��������� �������� ����� ������ �����
void KV_Settings::on_State1ColorChange_clicked()
{
	// �������� ColorDialog
	QColor newState1Color = QColorDialog::getColor( newState1Pen.color(),
													this,
													QString::fromWCharArray(L"�������� ����"));
	// ���� ���� ������
	if (newState1Color.isValid())
	{
		// ������ ����� ���� ���������
		newState1Pen.setColor(newState1Color);
		// �������������� ������ ���������������� �������
		ui.State1demoArea->repaint();
	}
}


// ��������� �������� ����� ������ �����
void KV_Settings::on_State2ColorChange_clicked()
{
	// �������� ColorDialog
	QColor newState2Color = QColorDialog::getColor( newState2Pen.color(),
													this,
													QString::fromWCharArray(L"�������� ����"));
	// ���� ���� ������
	if (newState2Color.isValid())
	{
		// ������ ����� ���� ���������
		newState2Pen.setColor(newState2Color);
		// �������������� ������ ���������������� �������
		ui.State2demoArea->repaint();
	}
}


// ���� ������ ����� ��� ��������� ��� - ��������� ��� � ����������
void KV_Settings::on_NoAuto_toggled(bool checked)
{
	if (checked) newAutoAxisSetting = AutoAxis::NoAuto;
}


// ���� ������ ����� � ��������� ���������� ��� - ��������� ��� � ����������
void KV_Settings::on_HighBorderAuto_toggled(bool checked)
{
	if (checked) newAutoAxisSetting = AutoAxis::HighBorderAuto;
}


// ���� ������ ����� � ������ ���������� - ��������� ��� � ����������
void KV_Settings::on_AllAuto_toggled(bool checked)
{
	if (checked) newAutoAxisSetting = AutoAxis::AllAuto;
}