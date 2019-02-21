#include <QColorDialog>
#include "kv_settings.h"
#include "filebrowser.h"
#include "globals.h"

// конструктор
// задаем параметры отрисовки и автоматической настройки оси X
KV_Settings::KV_Settings(const QPen& State1Pen, const QPen& State2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting, QWidget *parent)
	: QWidget(parent), newState1Pen(State1Pen), newState2Pen(State2Pen), newAutoAxisSetting(AutoAxisSetting)
{
	ui.setupUi(this);
	// делаем окно модальным и фиксированного размера
	KV_Settings::setFixedSize(KV_Settings::size());
	KV_Settings::setWindowModality(Qt::ApplicationModal);
	
	// задаем параметры демонстрационных полей - цвет, автозаполнение и присоединяем сигнал передачи параметров отрисовки
	ui.State1demoArea->setBackgroundRole(QPalette::Base);
	ui.State1demoArea->setAutoFillBackground(true);
	connect(ui.State1demoArea, &demoAreaKv::getPen, [this] () { return newState1Pen;});
	ui.State2demoArea->setBackgroundRole(QPalette::Base);
	ui.State2demoArea->setAutoFillBackground(true);
	connect(ui.State2demoArea, &demoAreaKv::getPen, [this] () { return newState2Pen;});

	// вставляем имена файлов в соответствующие поля
	ui.State1Filename->setText(State1FileName() + ":");
	ui.State2Filename->setText(State2FileName() + ":");
	
	// задаем 
	showParameters(State1Pen, State2Pen, AutoAxisSetting);

	// параметры шрифтов для совместимости с high dpi
	QFont font(ui.buttonBox->font());
	font.setPixelSize(11);
	ui.buttonBox->setFont(font);
}

// деструктор
KV_Settings::~KV_Settings() {}

// отпраляем сигнал о закрытии окна
void KV_Settings::closeEvent(QCloseEvent *)
{
	emit closing();
}


void KV_Settings::on_buttonBox_clicked(QAbstractButton * button)
{
	// OK
	if (button == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		// применяем настройки и закрываем окно
		applySettings();
		KV_Settings::close();
	// CANCEL
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Cancel)) {
		// закрываем окно
		KV_Settings::close();
	// APPLY
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) {
		// применяем настройки
		applySettings();
	// DISCARD
	} else if (button == ui.buttonBox->button(QDialogButtonBox::Discard)) {
		// установка текущих параметров гистограммы
		auto PairPens = emit getCurrentPens();
		// установка новых параметров
		showParameters(PairPens.first, PairPens.second, emit getCurrentAutoAxisSetting());
	// RESTORE DEFAULTS
	} else if (button == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		// установка стандартных параметров гистограммы
		auto PairPens = emit getDefaultPens();
		// установка новых параметров
		showParameters(PairPens.first, PairPens.second, AutoAxis::NoAuto);
	}
}


//устанавливаем параметры аргументов на виджеты 
void KV_Settings::showParameters(const QPen& State1Pen, const QPen& State2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting)
{
	// значение автонастройки оси в RadioButton-ах
	switch (AutoAxisSetting) {
	case AutoAxis::NoAuto:			ui.NoAuto->setChecked(true);
									break;
	case AutoAxis::HighBorderAuto:  ui.HighBorderAuto->setChecked(true);
									break;
	case AutoAxis::AllAuto:			ui.AllAuto->setChecked(true);
									break;
	}

	// смена отрисовок демонстрационных полей и их перерисовка
	newState1Pen.setColor(State1Pen.color());
	ui.State1demoArea->repaint();
	newState2Pen.setColor(State2Pen.color());
	ui.State2demoArea->repaint();

	// установка стиля линии в ComboBox-ах
	ui.State1lineStyleBox->setCurrentIndex(State1Pen.style());
	ui.State2lineStyleBox->setCurrentIndex(State2Pen.style());
	
	// установка значений ширины линии в SpinBox-ах
	ui.State1WidthBox->setValue(State1Pen.widthF());
	ui.State2WidthBox->setValue(State2Pen.widthF());
}

// применить настройки к гистограмме
void KV_Settings::applySettings()
{
	emit sendNewSettings(newState1Pen, newState2Pen, newAutoAxisSetting);
}

// новый стиль линии первого состояния
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
	// перерисовываем первую демонстрационную область
	ui.State1demoArea->repaint();
}


// новый стиль линии второго состояния
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
	// перерисовываем вторую демонстрационную область
	ui.State2demoArea->repaint();
}


// изменение значения ширины первой линии
void KV_Settings::on_State1WidthBox_valueChanged(double value)
{
	// задаем новую ширину первой линии
	newState1Pen.setWidthF(value);
	// перерисовываем первую демонстрационную область
	ui.State1demoArea->repaint();
}

// изменение значения ширины второй линии
void KV_Settings::on_State2WidthBox_valueChanged(double value)
{
	// задаем новую ширину второй линии
	newState2Pen.setWidthF(value);
	// перерисовываем вторую демонстрационную область
	ui.State2demoArea->repaint();
}


// изменение значения цвета первой линии
void KV_Settings::on_State1ColorChange_clicked()
{
	// вызываем ColorDialog
	QColor newState1Color = QColorDialog::getColor( newState1Pen.color(),
													this,
                                                    tr("Select color"));
	// если цвет выбран
	if (newState1Color.isValid())
	{
		// задаем новый цвет отрисовки
		newState1Pen.setColor(newState1Color);
		// перерисовываем первую демонстрационную область
		ui.State1demoArea->repaint();
	}
}


// изменение значения цвета второй линии
void KV_Settings::on_State2ColorChange_clicked()
{
	// вызываем ColorDialog
	QColor newState2Color = QColorDialog::getColor( newState2Pen.color(),
													this,
                                                    tr("Select color"));
	// если цвет выбран
	if (newState2Color.isValid())
	{
		// задаем новый цвет отрисовки
		newState2Pen.setColor(newState2Color);
		// перерисовываем вторую демонстрационную область
		ui.State2demoArea->repaint();
	}
}


// если выбран режим без настройки оси - фиксируем это в переменной
void KV_Settings::on_NoAuto_toggled(bool checked)
{
	if (checked) newAutoAxisSetting = AutoAxis::NoAuto;
}


// если выбран режим с частичной настройкой оси - фиксируем это в переменной
void KV_Settings::on_HighBorderAuto_toggled(bool checked)
{
	if (checked) newAutoAxisSetting = AutoAxis::HighBorderAuto;
}


// если выбран режим с полной настройкой - фиксируем это в переменной
void KV_Settings::on_AllAuto_toggled(bool checked)
{
	if (checked) newAutoAxisSetting = AutoAxis::AllAuto;
}
