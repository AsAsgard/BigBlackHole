#ifndef KV_SETTINGS_H
#define KV_SETTINGS_H

#include <QPair>
#include <QWidget>
#include <QPen>
#include "ui_kv_settings.h"

// перечисление вариантов автоматической настройки оси X
namespace AutoAxis {
	enum AutoAxisEnum {
		NoAuto,			// без автонастройки
		HighBorderAuto,	// автонастройка только по верхней границе
		AllAuto			// автонастройка по верхней и нижней границам
	};
}

// класс формы - настройка параметров гистограммы Kv
class KV_Settings : public QWidget
{
	Q_OBJECT

public:
	// конструктор и деструктор
	KV_Settings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting, QWidget *parent = 0);
	~KV_Settings();

signals:
	// закрытие окна
	void closing();
	// получить текущие параметры отрисовки
	QPair<QPen, QPen> getCurrentPens();
	// получить параметры отрисовки по умолчанию
	QPair<QPen, QPen> getDefaultPens();
	// получить текущие режим автонастройки оси
	AutoAxis::AutoAxisEnum getCurrentAutoAxisSetting();
	// отправить новые параметры отрисовки
	void sendNewSettings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum newAutoAxisSetting);

protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private slots:
	// нажатие на кнопки снизу формы
	void on_buttonBox_clicked(QAbstractButton * button);
	// выбор стилей линий
	void on_State1lineStyleBox_currentIndexChanged(int index);
	void on_State2lineStyleBox_currentIndexChanged(int index);
	// выбор толщины линий
	void on_State1WidthBox_valueChanged(double value);
	void on_State2WidthBox_valueChanged(double value);
	// выбор цвета линий
	void on_State1ColorChange_clicked();
	void on_State2ColorChange_clicked();
	// выбор режима автонастройки оси X гистограммы
	void on_NoAuto_toggled(bool checked);
	void on_HighBorderAuto_toggled(bool checked);
	void on_AllAuto_toggled(bool checked);

private:
	// ПОЛЯ
	Ui::KV_Settings ui;
	// параметры настройки гистограммы
	QPen newState1Pen;
	QPen newState2Pen;
	AutoAxis::AutoAxisEnum newAutoAxisSetting;

	// МЕТОДЫ
	// применить настройки
	void applySettings();
	// установить нужные параметры на виджетах
	void showParameters(const QPen& State1Pen, const QPen& State2Pen, AutoAxis::AutoAxisEnum AutoAxisSetting);
};

#endif // KV_SETTINGS_H
