#include "demoareakv.h"

// конструктор - задаем пустые параметры отрисовки
demoAreaKv::demoAreaKv(QWidget *parent)
	: QFrame(parent), pen() {}

// деструктор
demoAreaKv::~demoAreaKv() {}

// перерисовка области
void demoAreaKv::paintEvent(QPaintEvent *)
{
	// берем новое значение параметров отрисовки
	pen = emit getPen();
	// создаем объект для рисования
	QPainter painter(this);
	// задаем в нем параметры отрисовки
	painter.setPen(pen);
	// рисуем линию
	painter.drawLine(0, this->height() / 2, this->width(), this->height() / 2);
}
