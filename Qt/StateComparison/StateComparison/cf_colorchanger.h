#ifndef CF_COLORCHANGER_H
#define CF_COLORCHANGER_H

#include <QWidget>
#include "fa_box.h"
#include "ui_cf_colorchanger.h"

// класс формы - меню настройки окна картограммы
class CF_ColorChanger : public QWidget
{
	Q_OBJECT

public:
	// конструктор и деструктор
	CF_ColorChanger(QWidget *parent = 0);
	~CF_ColorChanger();

signals:
	// закрытие окна
	void closing();
	// изменить цвета окна картограммы
	void RecolorComparisonField();
	// изменить просматриваемый параметр
	void changeActiveMode(const Parameters::ParametersEnum &);
	// перерисовать демонстрационные ТВС
	void sendDemoStates(const QPair<double,double>& demoStates);

protected:
	void closeEvent(QCloseEvent *) override;

private slots:
	// вызов ColorDialog-ов и считывание с них цветов
	void on_ChangeMaxColor_clicked();
	void on_ChangeMinColor_clicked();

	// нажатие на кнопки снизу
	void on_buttonBox_clicked(QAbstractButton * button);
	
	// выбор режима
	void on_KqButton_clicked();
	void on_BurnButton_clicked();
	void on_KvButton_clicked();

private:
	// ПОЛЯ
	Ui::CF_ColorChanger ui;
	// активный параметр
	Parameters::ParametersEnum activeMode;

	// вспомогательная структура
	struct LimColors {
		QColor maxColor;
		QColor minColor;
	};

	// новые цвета
	std::map<Parameters::ParametersEnum, LimColors> newLimitColors;

	// МЕТОДЫ
	// применить цвета к картограмме
	void applyColors();
	// применить цвета к демонстрационному полю
	void changeColorField();
};

#endif // CF_COLORCHANGER_H
