/*
 * class demoAreaCF (implementation)
 *
 * Version 1.1
 *
 * Writed by Brylkin Dmitry. 15.12.2018
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include "demoareacf.h"

// конструктор - цвет не задаем, задаем основные параметры демонстрационной ТВС
demoAreaCF::demoAreaCF(QWidget *parent)
	: QFrame(parent), color(), demoFA() 
{
	demoFA.SetParent(this);
	demoFA.SetVisible(true);
	demoFA.SetFA_Number(135);
}

// деструктор
demoAreaCF::~demoAreaCF() {}

// получаем новые значения
void demoAreaCF::getStates(const QPair<double, double>& States)
{
	// задаем значение 1
	demoFA.SetState1(States.first);
	// задаем значение 2
	demoFA.SetState2(States.second);
	// задаем разницу значений
	demoFA.SetDelta(States.first - States.second);
}

// перерисовываем область
void demoAreaCF::paintEvent(QPaintEvent *)
{
	// задаем геометрию ТВС
	demoFA.SetGeometry(this->width() / 2, this->height() / 2, 9 * this->height() / 10, 9 * this->height() / 10);
	// получаем цвет отрисовки
	color = emit getColor();
	// создааем объект рисования
	QPainter painter(this);
	// задаем стандартные его параметры
	painter.setPen(QPen(Qt::black, 1));
	// подключаем полученный цвет
	painter.setBrush(color);
	// рисуем ТВС
	painter.drawPath(demoFA.GetPainterPath());
}
