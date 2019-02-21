#ifndef DEMOAREACF_H
#define DEMOAREACF_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QPair>
#include "fa_box.h"

// демонстрационное поле для настройки картограммы ТВС
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
