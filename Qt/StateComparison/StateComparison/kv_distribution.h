#ifndef KV_DISTRIBUTION_H
#define KV_DISTRIBUTION_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPair>
#include <qcustomplot.h>
#include "extfunctions.h"
#include "ui_kv_distribution.h"
#include "kv_settings.h"
#include "renderarea.h"

//класс формы - гистограмма распределения Kv
class Kv_Distribution : public QMainWindow
{
	Q_OBJECT


public:
	// конструктор и деструктор
	Kv_Distribution(const QPen& oldState1Pen, const QPen& oldState2Pen, const AutoAxis::AutoAxisEnum& oldAutoAxisSetting, QWidget *parent = 0);
	~Kv_Distribution();

	// отправка текущего номера ТВС, открытого на гистограмме
	int currentFA() const { return CurrentFA;}

signals:
	// закрытие гистограммы с отправкой текущих параметров отрисовки
	void closing(const QPen& State1Pen, const QPen& State2Pen, const AutoAxis::AutoAxisEnum& AutoAxisSetting);\
	// получить значение текущего высотного слоя
	int getSpinBoxValue();

protected:
	void closeEvent(QCloseEvent *) override;
	void resizeEvent(QResizeEvent * res) override;

private slots:
	//определение положения мыши на графике - отрисовка доп линий
	void MouseOnPlot(QMouseEvent * mouse);
	// сделать скриншот гистограмы
	void on_Screenshot_triggered();
	// окно настроек гистограммы
	void on_Settings_triggered();
	// окно настроек гистограммы было закрыто 
	void SetterClosed();
	// отправить текущие настройки отрисовки
	QPair<QPen, QPen> sendCurrentPens();
	// отправить настройки отрисовки по умолчанию
	QPair<QPen, QPen> sendDefaultPens();
	// отправить текущюю автонастройку оси
	AutoAxis::AutoAxisEnum sendCurrentAutoAxisSetting();
	// получение новых параметров отрисовки и автонастройки
	void getNewSettings(const QPen& newState1Pen, const QPen& newState2Pen, AutoAxis::AutoAxisEnum newAutoAxisSetting);
	// новая ТВС выбрана в поле картограммы
	void FA_selected(int FA_Number);
	// необходимо скрыть линию слоя
	void LayerLineHiding() {ui.plotArea->graph(2)->data()->clear(); ui.plotArea->replot();}
	// необходимо пересчитать параметры оси
	void ResetAxisNeededTrue() { AxisSettingNeeded = true;}
	// получить указатель на поле рисования
	void getRenderAreaPointer(RenderArea *);

private:
	// ПОЛЯ
	Ui::Kv_DistributionClass ui;
	// текущая ТВС
	int CurrentFA;
	// Layout легенды на графике
	QScopedPointer<QCPLayoutGrid> LegendLayout;
	// Текстовый элемент на графике
	QScopedPointer<QCPTextElement> FA_Num;
	// окно параметров отрисовки гистограммы
	QScopedPointer<KV_Settings> Setter;
	// указатель на поле рисования
	RenderArea * renderArea;
	// параметры отрисовки по умолчанию
	QPen defaultState1Pen;
	QPen defaultState2Pen;
	// текущие параметры отрисовки
	QPen State1Pen;
	QPen State2Pen;
	// текущее значение автонастройки осей
	AutoAxis::AutoAxisEnum AutoAxisSetting;
	// необходимость перерисовки
	bool AxisSettingNeeded;
	// МЕТОДЫ
	// установить новое значение номера ТВС
	void SetFAnum(const int& newFA_num) { CurrentFA = newFA_num; FA_Num->setText(QString::fromStdWString(L"ТВС " + ExtFunctions::to_wstring(newFA_num)));}
	// задание параметров оси X
	void XAxisSetting(void);
};

#endif // KV_DISTRIBUTION_H
