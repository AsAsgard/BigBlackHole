/*
 * class CF_ColorChanger (implementation)
 *
 * Version 1.7
 *
 * Writed by Brylkin Dmitry. 11.12.2018
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QColorDialog>
#include "extfunctions.h"
#include "cf_colorchanger.h"
#include "fa_box.h"
#include "filebrowser.h"
#include "globals.h"

// конструктор
CF_ColorChanger::CF_ColorChanger(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// делаем окно модальным и фиксированного размера
	CF_ColorChanger::setFixedSize(CF_ColorChanger::size());
	CF_ColorChanger::setWindowModality(Qt::ApplicationModal);

	// вставляем имена файлов в соответствующие поля
    ui.textBrowser_Max->setText(tr("Value in %1 is more than in %2")
                                .arg(State1FileName())
                                .arg(State2FileName()));
    ui.textBrowser_Min->setText(tr("Value in %1 is less than in %2")
                                .arg(State1FileName())
                                .arg(State2FileName()));

	// копируем настройки цветов из текущих значений картограммы
	newLimitColors[Parameters::Kq].maxColor = cFA_Box::GetMaxColor(Parameters::Kq);
	newLimitColors[Parameters::Kq].minColor = cFA_Box::GetMinColor(Parameters::Kq);
	newLimitColors[Parameters::Burn].maxColor = cFA_Box::GetMaxColor(Parameters::Burn);
	newLimitColors[Parameters::Burn].minColor = cFA_Box::GetMinColor(Parameters::Burn);
	newLimitColors[Parameters::Kv].maxColor = cFA_Box::GetMaxColor(Parameters::Kv);
	newLimitColors[Parameters::Kv].minColor = cFA_Box::GetMinColor(Parameters::Kv);

	// задаем демонстрационные значения (какие в голову придут)
	double demoValue1 = 1.433;
	double demoValue2 = 1.254;

	// задаем параметры первого демонстрационного поля - цвет, автозаполнение и присоединяем сигнал передачи параметров отрисовки
	ui.ColorField_Max->setBackgroundRole(QPalette::Base);
	ui.ColorField_Max->setAutoFillBackground(true);
	connect(ui.ColorField_Max, &demoAreaCF::getColor, [this] () { return newLimitColors.at(cFA_Box::ActiveMode()).maxColor;});
	// подключаем сигнал передачи демонстрационных значениий
	connect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Max, SLOT(getStates(const QPair<double, double>&)));
	// передаем демонстрационные значения 
	emit sendDemoStates(qMakePair( demoValue1, demoValue2));
	// отключаем сигнал передачи демонстрационных значениий
	disconnect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Max, SLOT(getStates(const QPair<double, double>&)));

	// задаем параметры второго демонстрационного поля - цвет, автозаполнение и присоединяем сигнал передачи параметров отрисовки
	ui.ColorField_Min->setBackgroundRole(QPalette::Base);
	ui.ColorField_Min->setAutoFillBackground(true);
	connect(ui.ColorField_Min, &demoAreaCF::getColor, [this] () { return newLimitColors.at(cFA_Box::ActiveMode()).minColor;});
	// подключаем сигнал передачи демонстрационных значениий
	connect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Min, SLOT(getStates(const QPair<double, double>&)));
	// передаем демонстрационные значения 
	emit sendDemoStates(qMakePair( demoValue2, demoValue1));
	// отключаем сигнал передачи демонстрационных значениий
	disconnect(this, SIGNAL(sendDemoStates(const QPair<double, double>&)), ui.ColorField_Min, SLOT(getStates(const QPair<double, double>&)));

	// в зависимости от текущего выбранного параметра нажимаем нужную кнопку и перерисовываем демонтрационные поля
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

	// параметры шрифтов для совместимости с high dpi
	QFont font(ui.groupBox->font());
	font.setPixelSize(13);
	ui.groupBox->setFont(font);
}

// деструктор
CF_ColorChanger::~CF_ColorChanger() {}


// выбран режим Kq
void CF_ColorChanger::on_KqButton_clicked()
{
	// кнопка Kq является нажатой - остальные нет
	ui.KqButton->setChecked(true);
	ui.BurnButton->setChecked(false);
	ui.KvButton->setChecked(false);
	// активный режим - Kq
	activeMode = Parameters::Kq;
	// изменение активного режима картограммы
	emit changeActiveMode(activeMode);
	// изменение цветов демонстрационных полей
	changeColorField();
}

// выбран режим Burn
void CF_ColorChanger::on_BurnButton_clicked()
{
	// кнопка Burn является нажатой - остальные нет
	ui.KqButton->setChecked(false);
	ui.BurnButton->setChecked(true);
	ui.KvButton->setChecked(false);
	// активный режим - Burn
	activeMode = Parameters::Burn;
	// изменение активного режима картограммы
	emit changeActiveMode(activeMode);
	// изменение цветов демонстрационных полей
	changeColorField();
}

// выбран режим Kv
void CF_ColorChanger::on_KvButton_clicked()
{
	// кнопка Kv является нажатой - остальные нет
	ui.KqButton->setChecked(false);
	ui.BurnButton->setChecked(false);
	ui.KvButton->setChecked(true);
	// активный режим - Kv
	activeMode = Parameters::Kv;
	// изменение активного режима картограммы
	emit changeActiveMode(activeMode);
	// изменение цветов демонстрационных полей
	changeColorField();
}

// перерисовка демонстрационных полей
void CF_ColorChanger::changeColorField()
{
	ui.ColorField_Max->repaint();
	ui.ColorField_Min->repaint();
}

// изменение значения цвета первой ТВС
void CF_ColorChanger::on_ChangeMaxColor_clicked()
{
	// вызываем ColorDialog
	QColor newMaxColor = QColorDialog::getColor( newLimitColors.at(activeMode).maxColor,
												 this,
                                                 tr("Select color"));
	// если цвет выбран
	if (newMaxColor.isValid())
	{
		// задаем новый цвет отрисовки
		newLimitColors[activeMode].maxColor = newMaxColor;
		// перерисовываем демонстрационные области
		changeColorField();
	}
}

// изменение значения цвета второй ТВС
void CF_ColorChanger::on_ChangeMinColor_clicked()
{
	// вызываем ColorDialog
	QColor newMinColor = QColorDialog::getColor( newLimitColors.at(activeMode).minColor,
												 this,
                                                 tr("Select color"));
	// если цвет выбран
	if (newMinColor.isValid())
	{
		// задаем новый цвет отрисовки
		newLimitColors[activeMode].minColor = newMinColor;
		// перерисовываем демонстрационные области
		changeColorField();
	}
}


// оповещение картограммы о закрытии окна настройки
void CF_ColorChanger::closeEvent(QCloseEvent *)
{
	emit closing();
}


void CF_ColorChanger::on_buttonBox_clicked(QAbstractButton * button)
{
	// OK
	if (button == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		// применяем настройки и закрываем окно
		applyColors();
		CF_ColorChanger::close();
	// CANCEL
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Cancel)) {
		// закрываем окно
		CF_ColorChanger::close();
	// APPLY
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) {
		// применяем настройки
		applyColors();
	// DISCARD
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Discard)) {
		// копируем настройки цветов из текущих значений картограммы
		newLimitColors[Parameters::Kq].maxColor = cFA_Box::GetMaxColor(Parameters::Kq);
		newLimitColors[Parameters::Kq].minColor = cFA_Box::GetMinColor(Parameters::Kq);
		newLimitColors[Parameters::Burn].maxColor = cFA_Box::GetMaxColor(Parameters::Burn);
		newLimitColors[Parameters::Burn].minColor = cFA_Box::GetMinColor(Parameters::Burn);
		newLimitColors[Parameters::Kv].maxColor = cFA_Box::GetMaxColor(Parameters::Kv);
		newLimitColors[Parameters::Kv].minColor = cFA_Box::GetMinColor(Parameters::Kv);
		// перерисовываем поля
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
		// копируем настройки цветов из стандартных значений картограммы
		newLimitColors[Parameters::Kq].maxColor = cFA_Box::GetDefaultMaxColor(Parameters::Kq);
		newLimitColors[Parameters::Kq].minColor = cFA_Box::GetDefaultMinColor(Parameters::Kq);
		newLimitColors[Parameters::Burn].maxColor = cFA_Box::GetDefaultMaxColor(Parameters::Burn);
		newLimitColors[Parameters::Burn].minColor = cFA_Box::GetDefaultMinColor(Parameters::Burn);
		newLimitColors[Parameters::Kv].maxColor = cFA_Box::GetDefaultMaxColor(Parameters::Kv);
		newLimitColors[Parameters::Kv].minColor = cFA_Box::GetDefaultMinColor(Parameters::Kv);
		// перерисовываем поля
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

// применить цвета
void CF_ColorChanger::applyColors()
{
	// задание предельных цветов картограммы
	cFA_Box::SetMaxColor(Parameters::Kq, newLimitColors.at(Parameters::Kq).maxColor);
	cFA_Box::SetMinColor(Parameters::Kq, newLimitColors.at(Parameters::Kq).minColor);
	cFA_Box::SetMaxColor(Parameters::Burn, newLimitColors.at(Parameters::Burn).maxColor);
	cFA_Box::SetMinColor(Parameters::Burn, newLimitColors.at(Parameters::Burn).minColor);
	cFA_Box::SetMaxColor(Parameters::Kv, newLimitColors.at(Parameters::Kv).maxColor);
	cFA_Box::SetMinColor(Parameters::Kv, newLimitColors.at(Parameters::Kv).minColor);
	// перекрасить картограмму
	emit RecolorComparisonField();
}
