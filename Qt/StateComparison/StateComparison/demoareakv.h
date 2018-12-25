#ifndef DEMOAREAKV_H
#define DEMOAREAKV_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>

// демонстрационное поле для настройки гистограммы Kv
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
	void paintEvent(QPaintEvent *) override;

private:
	// параметры рисования
	QPen pen;
};

#endif // DEMOAREAKV_H
