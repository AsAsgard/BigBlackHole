#ifndef DEMOAREAKV_H
#define DEMOAREAKV_H

/*
 * class demoAreaKv
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

/*
 * Класс demoAreaKv
 *
 * Демонстрационное поле для настройки графика высотного распределения Kv
 *
 * Показывает линии графика, цвет, размер и тип которой выбрал пользователь
 *
 * Используется окном настройки графика высотного распределения Kv
 * для демонстрации изменений линий графика
 */
class demoAreaKv : public QFrame
{
	Q_OBJECT

public:
	// конструкторы
	demoAreaKv(QWidget *parent);
	~demoAreaKv();

signals:
	// получить параметры рисования
	QPen getPen();

protected:
	// перерисовать поле
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
	// параметры рисования
	QPen pen;
};

#endif // DEMOAREAKV_H
