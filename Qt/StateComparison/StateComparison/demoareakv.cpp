#include "demoareakv.h"

// ����������� - ������ ������ ��������� ���������
demoAreaKv::demoAreaKv(QWidget *parent)
	: QFrame(parent), pen() {}

// ����������
demoAreaKv::~demoAreaKv() {}

// ����������� �������
void demoAreaKv::paintEvent(QPaintEvent *)
{
	// ����� ����� �������� ���������� ���������
	pen = emit getPen();
	// ������� ������ ��� ���������
	QPainter painter(this);
	// ������ � ��� ��������� ���������
	painter.setPen(pen);
	// ������ �����
	painter.drawLine(0, this->height() / 2, this->width(), this->height() / 2);
}
