#ifndef DEMOAREACF_H
#define DEMOAREACF_H

/*
 * class demoAreaCF
 *
 * Version 1.1
 *
 * Writed by Brylkin Dmitry. 15.12.2018
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QPair>
#include "fa_box.h"

/*
 * Класс demoAreaCF
 *
 * Демонстрационное поле для настройки картограммы ТВС
 *
 * Показывает ТВС с некими значениями (для примера) в цвете,
 * который выбрал пользователь
 *
 * Используется окном настройки цветов картограммы
 * для демонстрации измененного цвета
 */
class demoAreaCF : public QFrame
{
	Q_OBJECT

	
public:
	// конструкторы
	demoAreaCF(QWidget *parent);
	~demoAreaCF();

signals:
	// получить цвет, в который покрасить ТВС
	QColor getColor();

protected:
	// перерисовать поле
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private slots:
	// получить значения
	void getStates(const QPair<double, double>& States);

private:
	// текущий цвет
	QColor color;
	// демонстрационная ТВС
	cFA_Box demoFA;
};

#endif // DEMOAREACF_H
